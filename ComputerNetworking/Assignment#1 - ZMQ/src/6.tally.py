#tally.py
"""Tally how many points fall within the unit circle, print PI and send messages to client"""

import random, threading, time, zmq
pushpull='tcp://127.0.0.1:6702'
clienturl='tcp://127.0.0.1:6720'

print("tally activated")
context = zmq.Context()
isock=context.socket(zmq.PULL) #socket types for communication between "always_yes, judge->tally" : PUSH-PULL
isock.bind(pushpull)

osock=context.socket(zmq.PUSH) #socket types for communication between "tally->client" : PUSH-PULL
osock.connect(clienturl)

N=""
while N.isdigit()==False: #receive N from client
	N=isock.recv_string() 

p=q=0
cnt=0
pi=0
while cnt<int(N):
	decision = isock.recv_string()
	q+=1
	if decision == 'Y':
		p+=4
	print(cnt, decision, p, q, p/q)
	pi=p/q
	osock.send_json((cnt, pi)) #send (number of iterations, pi value) to client
	cnt=cnt+1

print("tally end!")






