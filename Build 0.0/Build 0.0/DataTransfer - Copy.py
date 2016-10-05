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
mac2 = '00170d000058755b'   

#======================= Split String to Blocks of Four Hex ==================

def split_every(n, s):  
    return (','.join([s[i:i+n] for i in xrange(0, len(s), n)] ))


#============================ Handling Data ====================

def handle_data(notifName, notifParams):

    print '   Received from mote {0}: "{1}"'.format(
        '-'.join(['%02x'%b for b in notifParams.macAddress]),
        ''.join(['%02x' %a for a in notifParams.data])
    )

    #print (notifParams.macAddress) 
    # ============== Breakding Down the Packet  ================
    d = datetime.datetime.now() 

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
                 
             text_file1 = open("EMG_Datafile.txt", "a")  
             print 'EMG File Opened'  
                     
             # timeh = str(datetime.datetime.now().hour)
             # timem = str(datetime.datetime.now().minute)
             # times = str(datetime.datetime.now().second)
            
             timems = str(datetime.datetime.now().microsecond)
             first_timems = str(datetime.datetime.now().microsecond - 10000) 
             second_timems = str(datetime.datetime.now().microsecond - 9000) 
             third_timems = str(datetime.datetime.now().microsecond - 8000) 
             fourth_timems = str(datetime.datetime.now().microsecond - 7000) 
             fifth_timems = str(datetime.datetime.now().microsecond - 6000) 
             sixth_timems = str(datetime.datetime.now().microsecond - 5000) 
             seventh_timems = str(datetime.datetime.now().microsecond - 4000) 
             eigth_timems = str(datetime.datetime.now().microsecond - 3000) 
             ninth_timems = str(datetime.datetime.now().microsecond - 2000) 
             tenth_timems = str(datetime.datetime.now().microsecond - 1000) 
             #datetime.datetime.now().ctime()
             for i in range(0,10):
                if i == 0:
                    text_file1.write(d.strftime("%H%M%S") + first_timems + "," + queue.popleft() + "\n") 
                elif i == 1: 
                    text_file1.write(d.strftime("%H%M%S") + second_timems + "," + queue.popleft() + "\n")
                elif i == 2: 
                    text_file1.write(d.strftime("%H%M%S") + third_timems + "," + queue.popleft() + "\n")
                elif i == 3: 
                    text_file1.write(d.strftime("%H%M%S") + fourth_timems + "," + queue.popleft() + "\n")
                elif i == 4:
                    text_file1.write(d.strftime("%H%M%S") + fifth_timems + "," + queue.popleft() + "\n")
                elif i == 5:
                    text_file1.write(d.strftime("%H%M%S") + sixth_timems + "," + queue.popleft() + "\n")
                elif i == 6: 
                    text_file1.write(d.strftime("%H%M%S") + seventh_timems + "," + queue.popleft() + "\n")
                elif i == 7:
                    text_file1.write(d.strftime("%H%M%S") + eigth_timems + "," + queue.popleft() + "\n")
                elif i == 8: 
                    text_file1.write(d.strftime("%H%M%S") + ninth_timems + "," + queue.popleft() + "\n")
                elif i == 9: 
                    text_file1.write(d.strftime("%H%M%S") + tenth_timems + "," + queue.popleft() + "\n")
                else:
                    print ("Done")
                
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
             
             for p in y.split(','):
                 queue2.append(p)

             print queue2
                 
             text_file2 = open("Force_Datafile.txt", "a")  
             print 'File Opened'  
                     
             # timeh = str(datetime.datetime.now().hour)
             # timem = str(datetime.datetime.now().minute)
             # times = str(datetime.datetime.now().second)
            
             timems = str(datetime.datetime.now().microsecond)
             first_timems = str(datetime.datetime.now().microsecond - 10000) 
             second_timems = str(datetime.datetime.now().microsecond - 9000) 
             third_timems = str(datetime.datetime.now().microsecond - 8000) 
             fourth_timems = str(datetime.datetime.now().microsecond - 7000) 
             fifth_timems = str(datetime.datetime.now().microsecond - 6000) 
             sixth_timems = str(datetime.datetime.now().microsecond - 5000) 
             seventh_timems = str(datetime.datetime.now().microsecond - 4000) 
             eigth_timems = str(datetime.datetime.now().microsecond - 3000) 
             ninth_timems = str(datetime.datetime.now().microsecond - 2000) 
             tenth_timems = str(datetime.datetime.now().microsecond - 1000) 
             #datetime.datetime.now().ctime()
             
             # Force sensor has a left foot data and a right foot data
             # To distinguish between the two, the line of data associated with left foot has a delimiter '.' at the beginning 
             # While the line associated with the right foot does not. 
             for i in range(0,10):
                if i == 0:
                    text_file2.write("." + d.strftime("%H%M%S") + first_timems + "," + queue2.popleft() + "\n") 
                elif i == 1: 
                    text_file2.write(d.strftime("%H%M%S") + second_timems + "," + queue2.popleft() + "\n")
                elif i == 2: 
                    text_file2.write("." + d.strftime("%H%M%S") + third_timems + "," + queue2.popleft() + "\n")
                elif i == 3: 
                    text_file2.write( d.strftime("%H%M%S") + fourth_timems + "," + queue2.popleft() + "\n")
                elif i == 4:
                    text_file2.write( "." + d.strftime("%H%M%S") + fifth_timems + "," + queue2.popleft() + "\n")
                elif i == 5:
                    text_file2.write( d.strftime("%H%M%S") + sixth_timems + "," + queue2.popleft() + "\n")
                elif i == 6: 
                    text_file2.write( "." + d.strftime("%H%M%S") + seventh_timems + "," + queue2.popleft() + "\n")
                elif i == 7:
                    text_file2.write( d.strftime("%H%M%S") + eigth_timems + "," + queue2.popleft() + "\n")
                elif i == 8: 
                    text_file2.write("." + d.strftime("%H%M%S") + ninth_timems + "," + queue2.popleft() + "\n")
                elif i == 9: 
                    text_file2.write( d.strftime("%H%M%S") + tenth_timems + "," + queue2.popleft() + "\n")
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

