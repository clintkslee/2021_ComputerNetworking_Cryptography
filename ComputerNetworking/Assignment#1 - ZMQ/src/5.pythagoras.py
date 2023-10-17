#pythagoras.py
"""Return the sum-of-squares of number sequences"""

import zmq
reqrep='tcp://127.0.0.1:6701'
print("pythagoras activated")
context = zmq.Context()
zsock=context.socket(zmq.REP) #socket types for communication between "judge<->pythagoras" : REQ-REP
zsock.bind(reqrep)
while True:
	numbers = zsock.recv_json()
	zsock.send_json(sum(n*n for n in numbers))









