#!/usr/bin/env zsh

raspivid --nopreview -o - -b 10000000 -fps 30 | ffmpeg -c:v h264_mmal -i - -c:v h264_omx -b:v 10000k -f flv - | ffplay -fflags nobuffer - 
