#!/usr/bin/env zsh

inname=$1
outname=$2
mode=$3
noise_str=$4
noise_type=$5
is_on_pi=$6
is_raw_input=$7

if [[ ! $is_on_pi -eq 0 ]]
then
  ffmpeg_encode=(\
    -c:v h264_omx \
      -b:v 10000k \
  )
else
  ffmpeg_encode=(\
    -c:v libx264 \
      -preset ultrafast \
      -tune zerolatency \
      -b:v 50000k \
  )
fi

if [[ ! $is_raw_input -eq 0 ]]
then
  raw_decode_args=(
    -f rawvideo \
    -pix_fmt yuv420p \
    -s:v 1920x1088 \
    -r 30 \
  )
fi

function ffmpeg_exec()
{
  ffmpeg \
    -re \
    -y \
    $raw_decode_args \
    -i $inname \
    $ffmpeg_encode \
    -c:a copy \
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
