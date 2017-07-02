
#include "types.h"
#include "registers.h"

void memcpy(void* dst, void* src, word size);
void memset(void* dst, byte val, word size);

static PadStatus Pad1;

void SetInterruptRegister(const byte val)
{
    WRITEBYTE(0x4200, val);
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

static UpdatePad(byte pad_num)
{
    word test;

    pad_num <<= 1;

    test = READWORD(0x4218 + pad_num);

    //test = ((word)*(byte*)0x4218 + pad_num) << 8;
    //test |= ((word)*(byte*)0x4219 + pad_num);

    memcpy(&Pad1, &test, 2);
}

static const long empty_tile[8] =
{
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

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

static const word poop[] =
{
    0xffff,
};

void preInit(void) {
    // For testing purpose ...
    // Insert code here to be executed before register init
}

int main()
{
    word paltest = 0xffff;
    word i = 0;

    ClearVRam();

    WRITEBYTE(SCREENDISPLAYREG, 0x00);

    WRITEBYTE(0x210b, (0x2000 >> 12) & 0xfc); // tile data address 0x2000
    SETBG1TILEMAPADDR(0x1000); // set BG1 tilemap address

    // load tiles
    WriteVRam((word)empty_tile, 0x2000, 32);
    WriteVRam((word)tile, 0x2000 + 32, 32);

    // load tilemap
    for (i = 0; i < 1; i++)
        WriteVRam((word)poop, 0x1000 + (i * 32), sizeof(poop) * 2);

    // load palette
    WRITEPALCOL(0, paltest);
    WRITEPALCOL(1, 0x0000);
    //WRITEPALCOL(2, 0x00ff);
    //WRITEPALCOL(3, 0x70ff);
    //WRITEPALCOL(4, 0x7f00);
    //WRITEPALCOL(5, 0xe111);

    WRITEBYTE(0x2105, 0x02); // mode 1
    WRITEBYTE(0x212c, 0x01); // enable plane 0
    //WRITEBYTE(0x212d, 0x00); // disable all subplanes

    WRITEBYTE(SCREENDISPLAYREG, 0x0f);

    SetInterruptRegister(INT_NMI | INT_CTRL); // enable NMI + CTRL pad reading

    while(1)
    {
        waitForVBlank();
    }

    return 0;
}

void IRQHandler()
{
}

void NMIHandler()
{
    UpdatePad(0);
}
