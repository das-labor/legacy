#!/bin/bash

#PREFIX="/usr/local/arm-none-eabi/include/"
PREFIX="/usr/arm-elf/include/"
TARGET_DIR="crypto/"
SRC_FILE="main_headers"

OWNER="root"
GROUP="root"
MODE="644"

for i in `cat $SRC_FILE`; do
  x=`echo $i | sed 's#^.*/##g'`
  sed -f header_install_modifier.sed "${i}" > "header_tmp/${x}"
  /usr/bin/install -o $OWNER -g $GROUP -m $MODE -D "header_tmp/${x}" ${PREFIX}${TARGET_DIR}${x};
done;


