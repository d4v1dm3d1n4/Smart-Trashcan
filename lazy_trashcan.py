
from ultralytics import YOLO
import cv2
import serial
import time

arduino = serial.Serial(port='COM6', baudrate=9600, timeout=.1)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))

def detect_bottle_or_can(frame_in, results_in):
    # Capture the desired elements from frame
    frame_names = results_in[0].names # Names list
    frame_boxes = results_in[0].boxes  # Boxes list

    for i in range(len(frame_boxes)):
        current_frame_classification = frame_names[frame_boxes.cls[i].item()]
        print(current_frame_classification)

        # 0 -> background
        # 1 -> can
        # 2 -> bottle
        if current_frame_classification == 'can':
            write_read('1')
        elif current_frame_classification == 'bottle':
            write_read('2')

    frame_in = results_in[0].plot()
    cv2.imshow('YOLOv8 inference', frame_in)

# main() function #######################################################

# Load a pre-trained YOLOv8n model
model = YOLO(model='best_2.pt')

# Open the webcam
cap = cv2.VideoCapture(0)

# Variables for controlling the inference rate
start_time = time.time()
inference_interval = 1  # Set the desired interval in seconds

# Loop through the video frames
while cap.isOpened():
    # Read a frame from the video
    success, frame = cap.read()

    if success:
        # Resize the input image to 640x640
        resized_frame = cv2.resize(frame, (640, 640))

        # Check if the time for the next inference has passed
        if time.time() - start_time >= inference_interval:
            # Run YOLOv8 inference on the frame
            results = model(resized_frame, verbose=False, conf=0.5)

            # Detect and classify
            detect_bottle_or_can(resized_frame, results)

            # Reset the start time for the next inference
            start_time = time.time()
        else:
            cv2.imshow('YOLOv8 inference', resized_frame)

        # Break the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        # Break the loop if the end of the video is reached
        break

# Release the webcam & close the display window
cap.release()
cv2.destroyAllWindows()

