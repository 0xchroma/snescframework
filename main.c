
#define SCREENDISPLAYREG        0x2100

#define CGRAMWRITEREG           0x2122

#define PPUENABLE               0x2115
#define VRAMADDRLOW             0x2116

#define DMAENABLE               0x420b

#define DMACONTROL              0x4300
#define DMADESTADDR             0x4301
#define DMASOURCELOADDR         0x4302
#define DMASOURCEHIADDR         0x4303
#define DMASOURCEREG1           0x4304
#define DMASIZELO               0x4305
#define DMASIZEHI               0x4306

#define WRITEBYTE(v, d) *(unsigned char*)v = d
#define WRITEWORD(v, d) *(unsigned short*)v = d

#define READBYTE(v) *(unsigned char*)v
#define READWORD(v) *(unsigned short*)v

unsigned short RGB24toBGR555(unsigned char r, unsigned char g, unsigned char b)
{

}

void DisableNMI()
{
    WRITEBYTE(0x4200, 0x80);
}

void EnableNMI()
{
    WRITEBYTE(0x4200, 0x00);
}

void WaitVSync()
{
    unsigned char flag = READBYTE(0x4212) & 0xE0;
    //flag = flag >> 7;

    asm {
        lda %%flag;
        lsr 7;
        sta %%flag
    };
    
    while (flag != 0x00)
    {
    }
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
    unsigned char brightness = 0x00;

    DisableNMI();
    ClearVRam();

    WRITEBYTE(SCREENDISPLAYREG, 0x00);
    WRITEBYTE(0x2106, 0xff);
    WRITEBYTE(CGRAMWRITEREG, 0xe0);
    WRITEBYTE(CGRAMWRITEREG, 0x00);
    WRITEBYTE(SCREENDISPLAYREG, 0x00);

    EnableNMI();

    while(1)
    {
        if (brightness == 0)
            brightness = 0x0f;

        if (brightness > 0)
            brightness--;

        WRITEBYTE(SCREENDISPLAYREG, brightness);
        WaitVSync();
    }

    return 0;
}

void IRQHandler(unsigned short y, unsigned short x, unsigned short a)
{
}