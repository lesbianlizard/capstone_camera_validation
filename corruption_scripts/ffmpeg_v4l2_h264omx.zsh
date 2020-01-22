#!/usr/bin/env zsh

ffmpeg -f v4l2 -i /dev/video0 -input_format yuv420p -c:v h264_omx -b:v 10000k  -f h264 - | ffplay -fflags nobuffer -
