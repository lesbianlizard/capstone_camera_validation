#!/usr/bin/env zsh

inname=$1
outname=$2
mode=$3
noise_str=$4
noise_type=$5

function ffmpeg_exec()
{
  ffmpeg \
    -re \
    -y \
    -i $inname \
    -c:a copy \
    -c:v libx264 \
      -preset ultrafast \
      -tune zerolatency \
      -b:v 50000k \
    -f flv \
    -vf "noise=alls=""$noise_str"":allf=""$noise_type" \
    $1
}

function ffplay_exec()
{
  ffplay -fflags nobuffer -
}

if [[ $mode == 0 ]]
then
  ffmpeg_exec - | ffplay_exec
elif [[ $mode == 1 ]]
then
  ffmpeg_exec $outname
else
  ffmpeg_exec -
fi
