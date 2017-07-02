rm snesc.smc
rm snesc.map
rm main.obj
rm startupSnes.obj

wdc816cc -wl -PX -sop main.c
wdc816as -V startupSnes.asm
wdcln -HB -MN -V -T -P00 -C0000,8000 startupSnes.obj main.obj -L cs -O snesc.smc