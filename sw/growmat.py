#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import csv
from datetime import datetime
import serial
import os.path

class growmat:
    
    # configure the serial connections (the parameters differs on the device you are connecting to)
    port='/dev/ttyUSB1'
    port='COM3'

    ser = None
    
    data = ''
    
    def connect(self, port):
        '''
        self.ser = serial.Serial(
            port=port,
            baudrate=9600,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            rtscts=0
        )
        '''
        self.ser = serial.Serial()
        self.ser.port = port
        self.ser.baudrate = 9600
        self.ser.parity=serial.PARITY_NONE
        self.ser.stopbits=serial.STOPBITS_ONE
        #self.ser.timeout = 1
        self.ser.setDTR(False)
        self.ser.open()
        #self.ser.setDTR(False)
        print('Connected to: ' + self.ser.portstr)
        
    def disconnect(self):
        if self.ser != None:
            self.ser.close()
            self.ser = None
        print('Disconnected')
  

    #########################
    
    outputInstruments = []
    inputInstruments = []
    
    def __init__(self):
        self.outputLight = self.createOutputInstrument('L:', 'Light')
        self.outputHeater = self.createOutputInstrument('H:', 'Heater')
        self.outputFan = self.createOutputInstrument('F:', 'Fan')
        self.outputCycler = self.createOutputInstrument('C:', 'Cycler')
        
        self.inputTemperature = self.createInputInstrument('TA=', 'Temperature', 'C')
        self.inputHumidity = self.createInputInstrument('HA=', 'Humidity', '%')
        self.inputLight = self.createInputInstrument('LA=', 'Light', '%')
        self.inputPower = self.createInputInstrument('PW=', 'Power', '%')
        self.inputExternal = self.createInputInstrument('EX=', 'External', '%')
        self.inputPH = self.createInputInstrument('PH=', 'pH', 'pH')
        self.inputEC = self.createInputInstrument('EC=', 'EC', 'mS/m')
        self.inputTemperature2 = self.createInputInstrument('TM=', 'Temperature 2', 'C')
        self.inputLevel = self.createInputInstrument('LV=', 'Level', 'cm')
        
        self.modem = modem(self)
    
    def send2(self, data):
        print('Send: {}'.format(data))
        self.ser.write(data + '\r\n') 
        
    def read2(self):
        #self.ser.flushInput() #flush input buffer, discarding all its contents
        #self.ser.flushOutput()#flush output buffer, aborting current output 
        
        self.send2('#?')
        while self.ser.inWaiting() == 0:
            pass
                
        data = ''
        while self.ser.inWaiting() > 0:
            line = self.ser.readline()
            data = data + line
            if line == '':
                line = self.ser.readline()
                data = data + line
                if line == '':
                    break 
        print('Read: {}'.format(data))
        return data
    
    def update(self):
        #self.send2('#?')
        self.data = self.read2()        
        
    def createOutputInstrument(self, code, name):
        instrument = outputInstrument(self, code, name)
        self.outputInstruments.append(instrument)
        return instrument
    
    def createInputInstrument(self, code, name, unit):
        instrument = inputInstrument(self, code, name, unit)
        self.inputInstruments.append(instrument)
        return instrument
            
    #data = 'L:0M C:1M F:1A TM=25.3 HA=54 PH=15 H:0M'
    


class modem:
    
    def __init__(self, device):
        self.device = device
    
    def sendSMS(self, number, text):
        pass
      
    def readSMS(self):
        pass
    
    def call(self, number, timeout=30):
        pass  
        
    def hangUp(self):
        pass
 
class controlMode:
    UNKNOWN = 0
    AUTO = 1
    MANUAL = 2
    DISABLE = 3

#class controlMode:
#    UNKNOWN, AUTO, MANUAL, DISABLE = range(4)    

class outputInstrument:
    
    def __init__(self, device, code, name):
        self.code = code
        self.name = name
        self.device = device
        
    def __repr__(self):
        return 'OutputInstrument\t{}\t{}\t{}'.format(self.name, self.getState(), self.getMode())
        
    def getArray(self):
        return [self.name, self.getState()]

    def setState(self, state):
        data = '{}{}'.format(self.code, str(state))
        self.device.send2(data)
    def setOn(self):
        self.setState(1)
    def setOff(self):
        self.setState(0)
    
    def getState(self):
        p = self.device.data.find(self.code)
        if p > -1:
            p = p + len(self.code)
            try:
                state = int(self.device.data[p: p + 1])
            except Exception as e:
                print(e)
                return -1
            return state
        return -1
     
    def getMode(self):
        p = self.device.data.find(self.code)
        if p > -1:
            p = p + len(self.code)
            m = self.device.data[p + 1: p + 2]
            if m == 'A':
                return controlMode.AUTO
            if m == 'M':
                return controlMode.MANUAL
            if m == 'D':
                return controlMode.DISABLE
        return controlMode.UNKNOWN
        
class inputInstrument:
    
    def __init__(self, device, code, name, unit):
        self.code = code
        self.name = name
        self.device = device
        self.unit = unit
        
    def __repr__(self):
        return 'InputInstrument\t{}\t{}\t{}'.format(self.name, self.getValue(), self.unit)
    
    def getArray(self):
        return [self.name, self.getValue(), self.unit]
        
    def getValue(self):
        p = self.device.data.find(self.code)
        if p > -1:
            p = p + len(self.code)
            try:
                data = self.device.data[p : ]
                #print(data)
                value = float(data.split()[0])
            except Exception as e:
                print(e)
                return float('nan')
            return value
        return float('nan')
        
            
class alarm:
    
    activationTime = 0
    deactivationTime = 0
    active = False
    unack = False
        
    def __init__(self, name, activationDelay=0, deactivationDelay=0):
        self.name = name
        self.activationDelay = activationDelay
        self.deactivationDelay = deactivationDelay
        
    def activate(self, state):
        ticks = time.time()
        if state:
            if not self.active:
                if self.activationTime == 0:
                    self.activationTime = ticks
                if self.activationTime + self.activationDelay < ticks:
                    self.alarm = True
                    self.unack = True
                    activeTime = 0
                    return True
        else:
            activationTime = 0    
        return False
        
    def deactivate(self, state):
        ticks = time.time()
        if state:
            if self.active:
                if self.deactivationTime == 0:
                    self.deactivationTime = ticks
                if self.deactivationTime + self.deactivationDelay < ticks:
                    self.alarm = False
                    deactiveTime = 0
                    return True
        else:
            deactivationTime = 0 
        return False
        
class csvWriter:
    
    def create(self, path, filename, data):
        if not os.path.isfile(os.path.join(path, filename)):
            with open(os.path.join(path, filename), 'w') as f:
                w = csv.writer(f, delimiter=';', lineterminator='\n')
                w.writerow(data)
    
    def append(self, path, filename, data): 
        with open(os.path.join(path, filename), 'a') as f:
            w = csv.writer(f, delimiter=';', lineterminator='\n')
            w.writerow(data)        

if __name__ == "__main__":
    
    # configure the serial connections (the parameters differs on the device you are connecting to)
    port='/dev/ttyUSB1'
    port='COM16'
    
    g = growmat()
    g.connect(port)

    g.outputLight.setOff()
    g.outputCycler.setOn()
    
    print(g.outputCycler.getState())
    print(g.outputCycler.getMode())
  
 
    if g.outputCycler.getMode() == controlMode.MANUAL:
        print('manual')
  
    archive = csvWriter()
   
    while True:   
        
        g.update()
        
        print(g.modem.readSMS())
    
        print(g.inputTemperature.getValue())
    
        print('\n\nInputs:')
        for instrument in g.inputInstruments:
            print(instrument.getArray())
        
        print('\n\nOutputs:')
        for instrument in g.outputInstruments:
            print(instrument.getArray())
        
        now = datetime.now()
        d = now.strftime('%Y-%m-%d')  
        t = now.strftime('%H:%M:%S')
        
        filename = 'g1-' + d +'.csv'
        if not os.path.isfile(filename):
            data = ['Date', 'Time']
            for instrument in g.inputInstruments:
                data.append('{} [{}]'.format(instrument.getArray()[0], instrument.getArray()[2]))
            for instrument in g.outputInstruments:
                data.append('{}'.format(instrument.getArray()[0]))
            archive.create('', filename, data)    
         
        data = [d, t]
        for instrument in g.inputInstruments:
            data.append('{}'.format(instrument.getArray()[1]))
        for instrument in g.outputInstruments:
            data.append('{}'.format(instrument.getArray()[1]))
        archive.append('', filename, data)
                        
        time.sleep(5)
  
    exit()
 
    '''
    
    now = datetime.now()
    d = now.strftime('%Y-%m-%d')  
    t = now.strftime('%H:%M:%S')  
    row = [d, t,  m0] #time.time(), 
    rowHead = ['date', 'time', 'temp[C]']
    
     
    fname = 'g1-' + d +'.csv'
    if not os.path.isfile(fname):
        with open(fname, 'w') as f:
            w = csv.writer(f, delimiter=';', lineterminator='\r\n')
            w.writerow(rowHead)    
        
    with open(fname, 'a') as f:
        w = csv.writer(f, delimiter=';', lineterminator='\r\n')
        w.writerow(row)

    time.sleep(1)
        ''' 
