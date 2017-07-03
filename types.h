#ifndef TYPES_H
#define TYPES_H

typedef     unsigned short  word;
typedef     unsigned char   byte;
typedef     unsigned long   dword;

#define TILE(v, h, o, p, c) (unsigned short) \
    (v & 0x1) << 15 | \
    (h & 0x1) << 14 | \
    (o & 0x1) << 13 | \
    (p & 0x7) << 10 | \
    (c & 0x3ff)

#define RGB24toBGR555(v) (unsigned short) \
    (((v & 0x0000FF) & 0x1f) << 10 | \
    (((v & 0x00FF00) >> 8) & 0x1f) << 5) | \
    (((v & 0xFF0000) >> 16) & 0x1f) & 0x7FFF

#endif // TYPES_H
