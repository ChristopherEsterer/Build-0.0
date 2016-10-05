#!/usr/bin/python

#============================ adjust path =====================================

import sys
import os
import csv 
import traceback
import unicodedata
import datetime
import subprocess
from collections import deque
from subprocess import check_output, CREATE_NEW_CONSOLE

if __name__ == "__main__":
    here = sys.path[0]
    sys.path.insert(0, os.path.join(here, '..', '..','libs'))
    sys.path.insert(0, os.path.join(here, '..', '..','external_libs'))

#============================ SDK imports =========================================
from subprocess                         import check_output
from SmartMeshSDK                       import sdk_version
from SmartMeshSDK.IpMgrConnectorMux     import IpMgrConnectorMux
#from SmartMeshSDK.IpMgrConnectorSerial  import IpMgrConnectorSerial
from SmartMeshSDK.IpMgrConnectorMux     import IpMgrSubscribe

#============================ helper functions ================================
def timeStamped(fname, fmt='%H%M%sdk_version{fname}'):
    return datetime.datetime.now().strftime(fmt).format(fname=fname) 

#=========================== Mac Addresses for each Sensor ====================
mac1 = '00170d0000587c22'   
mac2 = '00170d000058755a'   

#======================= Split String to Blocks of Four Hex ==================

def split_every(n, s):  
    return (','.join([s[i:i+n] for i in xrange(0, len(s), n)] ))


#============================ Handling Data ====================

def handle_data(notifName, notifParams):

    print '   Received from mote {0}: "{1}"'.format(
        '-'.join(['%02x'%b for b in notifParams.macAddress]),
        ''.join(['%02x' %a for a in notifParams.data])
    )

    # ============== Timing Definitions  ================
    d = datetime.datetime.now() 
    ms = datetime.datetime.now().microsecond 
   # delta = datetime.datetime.now() - datetime.timedelta(milliseconds=9) 
   # delta1 = datetime.datetime.now() - datetime.timedelta(milliseconds=8)
   # delta2 = datetime.datetime.now() - datetime.timedelta(milliseconds=7) 
   # delta3 = datetime.datetime.now() - datetime.timedelta(milliseconds=6)
   # delta4 = datetime.datetime.now() - datetime.timedelta(milliseconds=5)
   # delta5 = datetime.datetime.now() - datetime.timedelta(milliseconds=4)
   # delta6 = datetime.datetime.now() - datetime.timedelta(milliseconds=3)
   # delta7 = datetime.datetime.now() - datetime.timedelta(milliseconds=2)
   # delta8 = datetime.datetime.now() - datetime.timedelta(milliseconds=1)
   # delta9 = datetime.datetime.now() - datetime.timedelta(milliseconds=0)
    
    timems = str(datetime.datetime.now().microsecond)

    #=============== Datetime Variables 
  
    first_timems =  delta.strftime("%H%M%S%f")
   # second_timems = delta1.strftime("%H%M%S%f")
   # third_timems =  delta2.strftime("%H%M%S%f")
   # fourth_timems = delta3.strftime("%H%M%S%f")
   # fifth_timems =  delta4.strftime("%H%M%S%f")
   # sixth_timems =  delta5.strftime("%H%M%S%f")
   # seventh_timems =delta6.strftime("%H%M%S%f")
   # eigth_timems =  delta7.strftime("%H%M%S%f")
   # ninth_timems =  delta8.strftime("%H%M%S%f")
   # tenth_timems =  delta9.strftime("%H%M%S%f")

    #============== Handling EMG Sensor Data =====================
    if mac1 == format("".join(['%02x'%b for b in notifParams.macAddress])):
        try:  
            # Join the string all together without any delimiters 
             x = format(''.join(['%02x' %i for i in notifParams.data]))
            # Splitting the string into 10 blocks of 4 Hex values
             print (x)
             y = split_every(4, x) 
             print (y)

             #=== The queue will be used to write 10 values of 10 different times into the file 
             queue = deque([])
             
             # Turn 4 digits of hex into decimal
             for p in y.split(','):
                 ConvertedToInt = int(p, 16)
                 ConvertedToStr = str(ConvertedToInt)
                 queue.append(ConvertedToStr)

             print queue
             
             # Open to Write to File    
             text_file1 = open("EMG_Datafile.txt", "a")  
             print 'EMG File Opened'  
                    
             timems = str(datetime.datetime.now().microsecond)
             #print 'yo wtf'
             #first_timems = datetime.datetime.now().strftime("%f - 10000") 
             #print 'yo no way'
            
             #datetime.datetime.now().ctime()
             total = sum(queue[0,10])
             print total
             #for i in range(0,10):
                
             text_file1.write( first_timems + "," + (total/10) + "\n") 


                
             print 'File Written' 
             text_file1.close()
             print 'File Closed' 
             print queue 
        except IOError:
            print "Could not open, write and close file"
    #============ Handling Force Sensor Data ======================
    elif mac2 == format("".join(['%02x'%b for b in notifParams.macAddress])):
        try:  
             
             x = format(''.join(['%02x' %i for i in notifParams.data]))
             #Splitting the string into 10 blocks of 4 Hex values
             print (x)
             y = split_every(4, x) 
             print (y)

             #=== The queue will be used to write 10 values of 10 different times into the file 
             queue2 = deque([])
             
              # Turn 4 digits of hex into decimal
             for p in y.split(','):
                 ConvertedToInt = int(p, 16)
                 ConvertedToStr = str(ConvertedToInt)
                 queue2.append(ConvertedToStr)

             print queue2
                 
             text_file2 = open("Force_Datafile.txt", "a")  
             print 'File Opened'  
                     
             
             # Force sensor has a left foot data and a right foot data
             # To distinguish between the two, the line of data associated with left foot has a delimiter '.' at the beginning 
             # While the line associated with the right foot does not. 
             for i in range(0,10):
                if i == 0:
                    text_file2.write("." + first_timems + "," + queue2.popleft() + "\n")
                elif i == 1: 
                    text_file2.write(  second_timems + "," + queue2.popleft() + "\n")
                elif i == 2: 
                    text_file2.write("." + third_timems + "," + queue2.popleft() + "\n")
                elif i == 3: 
                    text_file2.write( fourth_timems + "," + queue2.popleft() + "\n")
                elif i == 4:
                    text_file2.write( "." + fifth_timems + "," + queue2.popleft() + "\n") 
                elif i == 5:
                    text_file2.write( sixth_timems + "," + queue2.popleft() + "\n")
                elif i == 6: 
                    text_file2.write( "."  + seventh_timems + "," + queue2.popleft() + "\n")
                elif i == 7:
                    text_file2.write( eigth_timems + "," + queue2.popleft() + "\n")
                elif i == 8: 
                    text_file2.write("."  + ninth_timems + "," + queue2.popleft() + "\n")
                elif i == 9: 
                    text_file2.write( tenth_timems + "," + queue2.popleft() + "\n")
                else:
                    print ("More than 10 blocks of data")
                
             print 'File Written' 
             text_file2.close()
             print 'File Closed' 
             print queue2 
        except IOError:
            print "Could not open, write and close file"
    else: 
        print("EXTRA MOTE IN THE NETWORK")
    
    #print (i for i in notifParams.data)
#    try:  
#        text_file = open("Datafile.txt", "a")
#        print 'File Opened' 
#        print (notifParams.data)
#        timeh = str(datetime.datetime.now().hour)
#        timem = str(datetime.datetime.now().minute)
#        times = str(datetime.datetime.now().second)
#        timems = str(datetime.datetime.now().microsecond)
#       
#        #datetime.datetime.now().ctime()
#        text_file.write(timeh + timem + times + timems + "," + format(notifParams.data) + "\n")
#        #+ datetime.datetime.minute+datetime.datetime.second +datetime.datetime.microsecond +
#        print 'File Written' 
#        text_file.close()
#        print 'File Closed' 
#    except IOError:
#        print "Could not open, write and close file" 
    
# Writing to Stream to be read by C++ application
#    print 'Writing to STDIN'
#    proc.stdin.write(datetime.datetime.now().ctime()+ " : " + format(notifParams.data) + "\n")
#    print 'Done Writing and now flushing' 
#    proc.stdin.flush()
#    print 'Done flushing and passing the results'
#    result = proc.stdout.readline().strip()
#    print(result)

   
    
#============================== Test ==========================================

c = 'dir' #Windows

DEFAULT_MUXHOST = '127.0.0.1'
DEFAULT_MUXPORT = 9900
#============================ main ============================================

print 'SimpleIPUpstreamMgr (c) Dust Networks'
print 'SmartMesh SDK {0}\n'.format('.'.join([str(b) for b in sdk_version.VERSION]))
print 'Note: Use with SimpleIPUpstreamMote\n'
    
    #=====
muxHost = DEFAULT_MUXHOST
muxPort = DEFAULT_MUXPORT


print '\n=====\nCreating connector'
connector = IpMgrConnectorMux.IpMgrConnectorMux()
print 'done.'

print '\n=====\nConnecting to IP manager'
try:
    connector.connect({
                        'host': muxHost,
                        'port': muxPort,
                     })
except ConnectionError as err:
    print err
    raw_input('\nScript ended. Press Enter to exit.')
    sys.exit(1)
print 'done.'

#=== Initializing the Pipe to send Data to C++ 

#print 'Starting Process' 
#proc = subprocess.Popen('C:\Users\Henry Hein\Documents\Visual Studio 2015\Projects\ConsoleApplication4\Release\ConsoleApplication4',
#                        stdin = subprocess.PIPE, #stdout = subprocess.PIPE, stderr = subprocess.PIPE,
#                        creationflags = CREATE_NEW_CONSOLE) 

    #=====
try:     
    print "- subscribe to data notifications "
    
    subscriber = IpMgrSubscribe.IpMgrSubscribe(connector)
    subscriber.start()
    subscriber.subscribe(
        notifTypes =    [
                            IpMgrSubscribe.IpMgrSubscribe.NOTIFDATA,
                        ],
        fun =           handle_data,
        isRlbl =        False,
    )

    
    #===
    raw_input("Press any key to stop.")
    
    connector.disconnect()
    
    print 'Script ended normally.'

except:
    traceback.print_exc()
    print 'Script ended with an error.'
    
raw_input('Press Enter to close.')

