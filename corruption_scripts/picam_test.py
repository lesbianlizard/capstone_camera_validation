#!/usr/bin/env python3

import picamera

vid_fmt = "yuv"
#vid_filename = "picam_test." + vid_fmt
vid_filename = "a_pipe"
vid_bitrate = 17000000
x = 1920
y = 1088

cam = picamera.PiCamera()
cam.resolution = (x, y)
cam.start_recording(vid_filename, format=vid_fmt)
cam.wait_recording(1000)
cam.stop_recording()
