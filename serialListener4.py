# -*- coding: utf-8 -*-

#Example calling using 
# (base) C:\...>python serialListener2.py "COM3" "box9"

import sys
import serial
import time
import csv
import datetime


def main(fileId, comPort, boxId):
    
    print("... Listening to "+comPort+" on "+boxId)
    now = datetime.datetime.now()
    # fileName=str(now.year)+"-"+str(now.month)+"-"+str(now.day)+"-"+str(now.hour)+"-"+str(now.minute)+"_"+boxId+".csv"
    fileName=fileId+".csv"
    
    ser = serial.Serial(comPort)
    ser.flushInput()
    
    auxLnCount=1
    auxLoop=True
    while auxLoop:
        try:
            #read the serial
            ser_bytes = ser.readline()
            ser_str=ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
            ser_str2=[int(s) for s in ser_str.split(',')]
            # write the CSV file
            with open(fileName,"a", newline='') as f:
                writer = csv.writer(f,delimiter=",")
                writer.writerow(ser_str2)
            
            print(str(auxLnCount)+","+ser_str)
            auxLnCount=auxLnCount+1
            
            #it is a START/STOP
            if ser_str2[0]>1000 & ser_str2[1]==1 & ser_str2[2]==1 & ser_str2[3]==1:
                auxLoop=False          
                
        except KeyboardInterrupt:
            print("Keyboard Interrupt")
            break
    
    ser.close()
    print("Exercise End")

if __name__ == '__main__':
    fileid = str(sys.argv[2])
    comPort = str(sys.argv[2])
    boxId = str(sys.argv[3])
    main(fileid , comPort, boxId)
      # main('COM4', 'box9')