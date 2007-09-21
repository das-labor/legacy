#!/bin/bash
ffmpeg -i $1 -f flv -s 320x240 -vcodec flv -acodec mp3 -ab 56 -ar 22050 -ac 2 -aspect 4:3 -b 350 $2
