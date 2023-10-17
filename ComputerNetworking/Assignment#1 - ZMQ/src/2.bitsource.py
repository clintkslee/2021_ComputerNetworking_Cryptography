#bitsource.py
"""Produce random points in the unit square."""

import random, time, zmq
B=32 # number of bits of precision in each random integer
bitsrcpull='tcp://127.0.0.1:6710' 
pubsub='tcp://127.0.0.1:6700'

def ones_and_zeros(digits):
	return bin(random.getrandbits(digits)).lstrip('0b').zfill(digits)

print("bitsource activated")
context = zmq.Context()
isock=context.socket(zmq.PULL) #socket types for communication between "client->bitsource" : PUSH-PULL
isock.bind(bitsrcpull)
N=int(isock.recv_string()) #receive N value from client

osock=context.socket(zmq.PUB) #socket types for communication between "bitsource->always_yes, judge" : PUB-SUB
osock.bind(pubsub)
cnt=0
while cnt<100000: #publish messages N times
	k=ones_and_zeros(B*2)
	osock.send_string(k)
	cnt+=1
	time.sleep(0.01)
print("bitsource end")













