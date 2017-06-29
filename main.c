
#define SCREENDISPLAYREG        0x2100

#define CGRAMWRITEREG           0x2122

#define PPUENABLE               0x2115
#define VRAMADDR                0x2116

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

#define WAITVBLANK()    asm wai;

#define WRITEPALCOL(i, v) \
    WRITEBYTE(0x2121, i); \
    WRITEWORD(0x2122, (v & 0xFF00) >> 8); \
    WRITEWORD(0x2122, (v & 0x00FF));

#define SETBG1ADDR(v)    WRITEBYTE(0x2107, v)
#define SETBG2ADDR(v)    WRITEBYTE(0x2108, v)
#define SETBG3ADDR(v)    WRITEBYTE(0x2109, v)
#define SETBG4ADDR(v)    WRITEBYTE(0x210A, v)

void DisableNMI()
{
    WRITEBYTE(0x4200, 0x00);
}

void EnableNMI()
{
    WRITEBYTE(0x4200, 0x80);
}

void ClearVRam()
{
    unsigned short null = 0;
    WRITEWORD(VRAMADDR, 0x00);
    WRITEWORD(DMASOURCE, (unsigned short)&null);

    WRITEWORD(DMASIZE, 0x10000 - 1);

    WRITEBYTE(PPUENABLE, 0x80);
    WRITEBYTE(DMACONTROL, 0x09);
    WRITEBYTE(DMADESTADDR, 0x18);
    WRITEBYTE(DMASOURCEREG1, 0x7E);
    WRITEBYTE(DMAENABLE, 0x01);
}

void WriteVRAM(
        const unsigned short dst,
        const unsigned short src,
        const unsigned short size)
{
    WRITEWORD(VRAMADDR, dst);
    WRITEWORD(DMASOURCE, (unsigned short)src);

    WRITEWORD(DMASIZE, size);

    WRITEBYTE(PPUENABLE, 0x80);
    WRITEBYTE(DMACONTROL, 0x09);
    WRITEBYTE(DMADESTADDR, 0x18);
    WRITEBYTE(DMASOURCEREG1, 0x7E);
    WRITEBYTE(DMAENABLE, 0x01);
}

static const unsigned long tile[8] =
{
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111
};

int main()
{
    DisableNMI();
    ClearVRam();

    WRITEBYTE(SCREENDISPLAYREG, 0x00);

    WriteVRAM(0x0000, (unsigned short)tile, sizeof(tile) * 2);

    WRITEPALCOL(0, 0x0000);
    WRITEPALCOL(1, 0x7fff);
    WRITEBYTE(SCREENDISPLAYREG, 0x0f);

    EnableNMI();

    while(1)
    {
        WAITVBLANK();
    }

    return 0;
}

void IRQHandler(unsigned short y, unsigned short x, unsigned short a)
{
}
