cd Objects
del *.bin
del bootrom.txt
C:\Tools\hex2bin  bootrom.hex 
rename bootrom.bin bootrom.bin 
C:\Keil_v5\ARM\ARMCC\bin\fromelf  --text -a -c --output bootrom.txt  bootrom.axf
del 1.log
