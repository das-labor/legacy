#!/bin/bash

for i in `ls example*.make`; do
  make $i.dot $i.png;
done


