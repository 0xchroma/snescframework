
typedef     unsigned short  word;
typedef     unsigned char   byte;
typedef     unsigned long   dword;

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

#define WRITEBYTE(v, d)     *(byte*)(v) = d
#define WRITEWORD(v, d)     *(word*)(v) = d

#define READBYTE(v)         *(byte*)v
#define READWORD(v)         *(word*)v

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

void WaitVBlank()
{
    asm { wai; }
}

void waitForVBlank() {
    byte Status;
    do {
        Status = *(byte*)0x4210;
    } while (!(Status & 0x80));
}

void WriteVRam(
        word src,
        word dst,
        word size)
{
    WRITEBYTE(0x2115, 0x80);
    WRITEWORD(0x2116, dst);

    WRITEWORD(0x4300, 0x1801); // set DMA control reg (1 word increments)

    WRITEWORD(0x4302, src);

    WRITEBYTE(0x4304, 0x01); // source address bank (not sure about this yet)
    WRITEWORD(0x4305, size);

    // wait for vblank interrupt before starting DMA
    waitForVBlank();
    WRITEBYTE(0x2100, 0x80);
    WRITEBYTE(0x420b, 0x01);
    WRITEBYTE(0x2100, 0x00);
}
// 4bpp planar snes format
static const long tile[] =
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

static const word poop[2] =
{
    0x0000,
    0x0000
};

int main()
{
    ClearVRam();

    WRITEBYTE(SCREENDISPLAYREG, 0x00);

    WRITEBYTE(0x2107, (0x1000 >> 10) & 0xf0); // screen tilemap location 0x1000
    WRITEBYTE(0x210b, (0x2000 >> 12) & 0xfc); // tile data address 0x2000

    WriteVRam((word)tile, 0x2000, sizeof(tile) * sizeof(long));
    //WriteVRam((word)poop, 0x1000, 2 * 2);

    WRITEPALCOL(0, 0xffff);
    //WRITEPALCOL(1, 0x7fff);
    //WRITEPALCOL(2, 0x00ff);
    //WRITEPALCOL(3, 0x70ff);
    //WRITEPALCOL(4, 0x7f00);
    //WRITEPALCOL(5, 0xe111);

    WRITEBYTE(0x2105, 0x02); // mode 2
    WRITEBYTE(0x212c, 0x01); // enable plane 0

    WRITEBYTE(SCREENDISPLAYREG, 0x0f);

    EnableNMI();

    while(1)
    {
        WaitVBlank();
    }

    return 0;
}

void IRQHandler(unsigned short y, unsigned short x, unsigned short a)
{
}
