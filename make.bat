wdc816cc -wl -PX -sop main.c

wdc816as -V startupSnes.asm
wdcln -HB -MN -V -T -P00 -C0000,8000 startupSnes.obj main.obj -L cc -O snesc.smc

rm main.obj
rm startupSnes.obj

c:\emulator\higan\higan.exe snesc.smc