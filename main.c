
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

void DoDMA(
    unsigned short source_addr, 
    unsigned short dest_addr, 
    unsigned short size,
    unsigned short channel)
{
    
}

void ClearVRam()
{
    unsigned char value = 0;
    
    *(unsigned short*)VRAMADDRLOW = 0x0000; // set vram dest address
    *(unsigned short*)DMASOURCELOADDR = ((unsigned short)&value); // ram source address

    *(unsigned short*)DMASIZELO = 0x10000 - 1;
    *(unsigned char*)PPUENABLE = 0x80;

    *(unsigned char*)DMACONTROL = 0x09;

    *(unsigned char*)DMADESTADDR = 0x18;

    *(unsigned char*)DMASOURCEREG1 = 0x7E;

    *(unsigned char*)DMAENABLE = 0x01;
}

void waitVSync()
{

}

int main()
{
    unsigned char brightness = 0x0f;
    ClearVRam();

    *(unsigned char*)SCREENDISPLAYREG = 0x80;
    *(unsigned char*)CGRAMWRITEREG = 0xff;
    *(unsigned char*)CGRAMWRITEREG = 0xff;
    *(unsigned char*)SCREENDISPLAYREG = Poop.data;

    while(1)
    {
    }

    return 0;
}

void IRQHandler()
{
    
}