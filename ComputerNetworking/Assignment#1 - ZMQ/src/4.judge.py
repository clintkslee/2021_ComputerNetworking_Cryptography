#judge.py
"""Determine whether each input coordinate is inside the unit circle"""

import zmq
B=32 # number of bits of precision in each random integer
pubsub='tcp://127.0.0.1:6700'
reqrep='tcp://127.0.0.1:6701'
pushpull='tcp://127.0.0.1:6702'
print("judge activated")

context = zmq.Context()
isock=context.socket(zmq.SUB) #socket types for communication between "bitsource->judge" : PUB-SUB
isock.connect(pubsub)

for prefix in b'01', b'10', b'11': #setsockopt to get messages start with b'01', b'10', b'11'
	isock.setsockopt(zmq.SUBSCRIBE, prefix)
psock=context.socket(zmq.REQ) #socket types for communication between "judge<->pythagoras" : REQ-REP
psock.connect(reqrep) #pythagoras url
osock=context.socket(zmq.PUSH) #socket types for communication between "judge->tally" : PUSH-PULL
osock.connect(pushpull)
unit=2**(B*2)

while True:
	bits=isock.recv_string() 
	n, m = int(bits[::2], 2), int(bits[1::2], 2)
	psock.send_json((n, m)) #send to pythagoras 
	sumsquares = psock.recv_json() #get sum of squares of the numbers
	osock.send_string('Y' if sumsquares<unit else 'N') #indicate Y or N and send to tally
