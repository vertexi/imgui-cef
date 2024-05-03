import zmq
from rapid_latex_ocr import LatexOCR
from multiprocessing import Process
import sys

context = zmq.Context()


model = LatexOCR()

def data_loop():
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:8848")
    while True:
        print("waiting...")
        #  Wait for next request from client
        image = socket.recv()
        print("Received request!")
        res, elapse = model(image)
        socket.send_string(res)
        print(res)
        print(elapse)

def control_loop():
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:8849")
    while True:
        socket.recv()
        print("Received control!")
        sys.exit(0)

if __name__ == "__main__":
    p1 = Process(target=data_loop)
    p1.start()
    p2 = Process(target=control_loop)
    p2.start()
    p2.join()
    p1.kill()
