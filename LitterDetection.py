# Project: Autonomous Litter Detection and Collection Robot
# Author: Razal Fayis V. S.
# Date: 8 April 2024
# Note: Update and Changes to the libraries may be required in the future to properly run the program

import serial
import time
from picamera2 import Picamera2, Preview
import cv2
import numpy as np
import tflite_runtime.interpreter as tflite

# UART setup
ser = serial.Serial('/dev/ttyUSB0', 115200)  # Replace with your serial port and baud rate
time.sleep(2)  # Wait for the serial connection to initialize

normalSize = (640, 480)
lowresSize = (320, 240)
rectangles = []

def read_label_file(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
    ret = {}
    for line in lines:
        pair = line.strip().split(maxsplit=1)
        ret[int(pair[0])] = pair[1].strip()
    return ret

def draw_rectangles(request):
    stream = request.picam2.stream_map["main"]
    fb = request.request.buffers[stream]
    with fb.mmap(0) as b:
        im = np.array(b, copy=False, dtype=np.uint8).reshape((normalSize[1], normalSize[0], 4))

        for rect in rectangles:
            rect_start = (int(rect[0] * 2) - 5, int(rect[1] * 2) - 5)
            rect_end = (int(rect[2] * 2) + 5, int(rect[3] * 2) + 5)
            cv2.rectangle(im, rect_start, rect_end, (0, 255, 0, 0))
            if len(rect) == 5:
                text = rect[4]
                font = cv2.FONT_HERSHEY_SIMPLEX
                cv2.putText(im, text, (int(rect[0] * 2) + 10, int(rect[1] * 2) + 10), font, 1, (255, 255, 255), 2,
                            cv2.LINE_AA)
        del im

def inference_tensorflow(image, model, output, label=None):
    global rectangles

    if label:
        labels = read_label_file(label)
    else:
        labels = None

    interpreter = tflite.Interpreter(model_path=model, num_threads=4)
    interpreter.allocate_tensors()

    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    height = input_details[0]['shape'][1]
    width = input_details[0]['shape'][2]
    floating_model = False
    if input_details[0]['dtype'] == np.float32:
        floating_model = True

    rgb = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)
    initial_h, initial_w, channels = rgb.shape

    picture = cv2.resize(rgb, (width, height))

    input_data = np.expand_dims(picture, axis=0)
    if floating_model:
        input_data = (np.float32(input_data) - 127.5) / 127.5

    interpreter.set_tensor(input_details[0]['index'], input_data)

    interpreter.invoke()

    detected_boxes = interpreter.get_tensor(output_details[0]['index'])
    detected_classes = interpreter.get_tensor(output_details[1]['index'])
    detected_scores = interpreter.get_tensor(output_details[2]['index'])
    num_boxes = interpreter.get_tensor(output_details[3]['index'])

    rectangles = []
    for i in range(int(num_boxes)):
        top, left, bottom, right = detected_boxes[0][i]
        classId = int(detected_classes[0][i])
        score = detected_scores[0][i]
        if score > 0.5:
            xmin = left * initial_w
            ymin = bottom * initial_h
            xmax = right * initial_w
            ymax = top * initial_h
            box = [xmin, ymin, xmax, ymax]
            rectangles.append(box)
            if labels:
                print(labels[classId], 'score = ', score)
                rectangles[-1].append(labels[classId])
            else:
                print('score = ', score)

def main():
    model_path = "litdet.tflite"
    label_path = "labelmap.txt"

    picam2 = Picamera2()
    picam2.start_preview()

    while True:
        img = picam2.capture_array()
        inference_tensorflow(img, model_path, output=None, label=label_path)
        
        if rectangles:
            # Send 'P' command to NodeMCU to pick litter
            ser.write(b'P')
            
            # Read ultrasonic distance from NodeMCU
            ser.write(b'D')
            distance = float(ser.readline().decode().strip())
            
            if distance <= 15:
                # Send stop command to NodeMCU
                ser.write(b'S')
                # Send pick command to NodeMCU
                ser.write(b'P')
                time.sleep(3)
                # Send release command to NodeMCU
                ser.write(b'R')
                time.sleep(3)
            
        cv2.imshow("preview", img)
        if cv2.waitKey(1) == ord('q'):
            break

    picam2.stop()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
