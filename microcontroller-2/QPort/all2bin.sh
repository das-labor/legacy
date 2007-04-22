for i in `ls -1 *.o`; do avr-objcopy -j .text -j .data -O binary $i $i.bin; done; 
