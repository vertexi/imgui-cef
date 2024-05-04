import zmq
import multiprocessing
from multiprocessing import Process
import sys
import time

import os
current_dir = os.path.dirname(os.path.abspath(__file__))
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'
from PIL import Image
from transformers import TrOCRProcessor
from optimum.onnxruntime import ORTModelForVision2Seq
from io import BytesIO

context = zmq.Context()

processor = TrOCRProcessor.from_pretrained(os.path.join(current_dir, 'onnx'), local_files_only=True)
model = ORTModelForVision2Seq.from_pretrained(os.path.join(current_dir, 'onnx'), use_cache=False, local_files_only=True)

def infer(imageData):
    images = Image.open(BytesIO(imageData)).convert('RGB')
    pixel_values = processor(images=images, return_tensors="pt").pixel_values
    generated_ids = model.generate(pixel_values)
    generated_text = processor.batch_decode(generated_ids, skip_special_tokens=True)
    return generated_text[0]

def data_loop():
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:8848")
    while True:
        print("waiting...")
        #  Wait for next request from client
        image = socket.recv()
        print("Received request!")
        res = infer(image)
        socket.send_string(res)
        print(res)

def control_loop():
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:8849")
    time.sleep(0.5)
    socket.recv()
    socket.send_string("Ready")
    while True:
        socket.recv()
        print("Received control!")
        sys.exit(0)

if __name__ == "__main__":
    multiprocessing.freeze_support()

    p1 = Process(target=data_loop)
    p1.start()
    p2 = Process(target=control_loop)
    p2.start()
    p2.join()
    p1.kill()
