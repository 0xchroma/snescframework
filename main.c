
#define SCREENDISPLAYREG        0x2100

#define CGRAMWRITEREG           0x2122

#define PPUENABLE               0x2115
#define VRAMADDRLOW             0x2116

#define DMAENABLE               0x420b

#define DMACONTROL              0x4300
#define DMADESTADDR             0x4301
#define DMASOURCE               0x4302
#define DMASOURCEREG1           0x4304
#define DMASIZE                 0x4305

#define WRITEBYTE(v, d) *(unsigned char*)v = d
#define WRITEWORD(v, d) *(unsigned short*)v = d

#define READBYTE(v)     *(unsigned char*)v
#define READWORD(v)     *(unsigned short*)v

void memcpy(void* dst, void* src, unsigned short length);

unsigned short RGBtoBGR555(unsigned char r, unsigned char g, unsigned char b)
{
    return ((b & 0x1f) << 5 | 
            (g & 0x1f) << 5 | 
            (r & 0x1f) << 5) & 0x7FFF;
}

void DisableNMI()
{
    WRITEBYTE(0x4200, 0x00);
}

void EnableNMI()
{
    WRITEBYTE(0x4200, 0x80);
}

void WaitVSync()
{
    while (((READBYTE(0x4212) & 0xE0) >> 7) != 0) {}
}

void ClearVRam()
{
    unsigned char value = 0;

    WRITEWORD(VRAMADDRLOW, 0x00);
    WRITEWORD(DMASOURCELOADDR, (unsigned short)&value);

    WRITEWORD(DMASIZELO, 0x10000 - 1);

    WRITEBYTE(PPUENABLE, 0x80);
    WRITEBYTE(DMACONTROL, 0x09);
    WRITEBYTE(DMADESTADDR, 0x18);
    WRITEBYTE(DMASOURCEREG1, 0x7E);
    WRITEBYTE(DMAENABLE, 0x01);
}

int main()
{
    DisableNMI();
    ClearVRam();

    WRITEBYTE(SCREENDISPLAYREG, 0x00);
    WRITEBYTE(0x2106, 0xff);
    WRITEBYTE(CGRAMWRITEREG, 0xef);
    WRITEBYTE(CGRAMWRITEREG, 0xff);
    WRITEBYTE(SCREENDISPLAYREG, 0x00);

    EnableNMI();

    while(1)
    {
        //WaitVSync();
    }

    return 0;
}

void IRQHandler(unsigned short y, unsigned short x, unsigned short a)
{
}
