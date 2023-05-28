#include "tools.h"

extern int __bss_end;
extern void *__brkval;

// * Check the availability of the drive
bool Memory::InitializeRom(Sd2Card &card, SdVolume &volume, SdFile &root)
{ 
    if (!card.init(SPI_HALF_SPEED, ROM_DATA_PIN) || !volume.init(card))
        return false;
    else
        return true;
}

// * Get the amount of free dynamic memory
uint Memory::GetFreeRam()
{
    int free_memory;
    
    if ((int)__brkval == 0)
        return ((int)&free_memory) - ((int)&__bss_end);
    
    else
        return ((int)&free_memory) - ((int)__brkval);
}

// * Get the amount of free static memory
uint Memory::GetFreeRom(SdVolume &volume)
{
    File root = SD.open("/");

    ulong used_space = 0;
    while (true)
    {
        File entry = root.openNextFile();
        
        if (!entry)
        {
            entry.close();
            break;
        }

        else
        {
            used_space += entry.size();
            entry.close();
        }
    }
    root.close();
    
    const ulong left_space = ((volume.blocksPerCluster() * volume.clusterCount() * 512) - used_space) / pow(1024, 2);

    return left_space;
}