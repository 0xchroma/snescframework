#ifndef REGISTERS_H
#define REGISTERS_H

#include "types.h"

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

#define SETBG1TILEMAPADDR(v)        WRITEBYTE(0x2107, (v >> 10) & 0xf0)
#define SETBG2TILEMAPADDR(v)        WRITEBYTE(0x2108, (v >> 10) & 0xf0)
#define SETBG3TILEMAPADDR(v)        WRITEBYTE(0x2109, (v >> 10) & 0xf0)
#define SETBG4TILEMAPADDR(v)        WRITEBYTE(0x210A, (v >> 10) & 0xf0)

#define INT_NMI     0x80
#define INT_CTRL    0x1

typedef struct
{
    byte right:1;
    byte left:1;
    byte down:1;
    byte up:1;
    byte start:1;	// Enter
    byte select:1;	// Space
    byte Y:1;	// X
    byte B:1;	// C
    //--------------------------------
    byte Dummy:4;
    byte R:1;	// Z
    byte L:1;	// A
    byte X:1; 	// S
    byte A:1;	// D
} PadStatus;

#endif // REGISTERS_H
