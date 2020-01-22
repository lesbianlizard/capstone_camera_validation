#!/usr/bin/env zsh

filename=Big*.mp4

ffmpeg -c:v h264_mmal -i $filename -c:v h264_omx -c:a copy -b:v 1000k -f webm - | ffplay -fflags nobuffer -
