#client.py
"""client asks the user the desired number of data points N, receives (number of iterations, pi value) from tally"""

import zmq, threading, time
bitsrcpull='tcp://127.0.0.1:6710' #for PUSH_PULL with bitsource 
clienturl='tcp://127.0.0.1:6720'
pushpull='tcp://127.0.0.1:6702'

print("client activated")
context = zmq.Context()
osock = context.socket(zmq.PUSH) #socket types for communication between "client->bitsource" : PUSH-PULL
osock.connect(bitsrcpull) #send N value to bitsource

osock2 = context.socket(zmq.PUSH) #socket types for communication between "client->tally" : PUSH-PULL
osock2.connect(pushpull) #send N value to tally

isock = context.socket(zmq.PULL) #socket types for communication between "client<-tally" : PUSH-PULL
isock.bind(clienturl) #receive data from tally

N=input("insert number of data points N : ") #asks the user the desired number of data points N
if N.isdigit()==False: #exception for unexpected input
	N=input("insert correct number of data points N : ")
	
osock.send_string(N) #send N to bitsource
osock2.send_string(N) #send N to tally

print("<iter>  <PI>")
cnt=0
while cnt<int(N):
	numbers = isock.recv_json() #receives (number of iterations, pi value) from tally
	print('{0:<7}'.format(numbers[0]), numbers[1]) #print the changes in pi value in real-time
	cnt=cnt+1
print("client end")

