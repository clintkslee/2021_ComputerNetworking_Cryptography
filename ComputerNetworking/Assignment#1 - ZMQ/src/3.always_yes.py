#always_yes.py
"""Coordinates in the lower-left quadrant are inside the unit circle."""

import zmq
pubsub='tcp://127.0.0.1:6700'
pushpull='tcp://127.0.0.1:6702'

print("always_yes activated")

context = zmq.Context()
isock=context.socket(zmq.SUB) #socket types for communication between "bitsource->always_yes" : PUB-SUB
isock.connect(pubsub)
isock.setsockopt(zmq.SUBSCRIBE, b'00') #get messages start with b'00'

osock=context.socket(zmq.PUSH) #socket types for communication between "always_yes->tally" : PUSH-PULL
osock.connect(pushpull)

while True: #send 'Y' to tally if received a message from publisher 
	isock.recv_string()
	osock.send_string('Y')




