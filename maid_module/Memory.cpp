#include "tools.h"

extern int __bss_end;
extern void *__brkval;

bool Memory::InitROM(Sd2Card &card, SdVolume &volume, SdFile &root)
{ 
    if (!card.init(SPI_HALF_SPEED, ROM_PIN))
    {
        Serial.println("Not found!");
        return false;
    }

	  else if (!volume.init(card))
    {
        Serial.println("Failed to init volume");
        return false;
    }

    root.openRoot(volume);

    root.ls(LS_R);
  
    return true;
}

uint Memory::GetFreeRAM()
{
    int free_memory;
    
    if ((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    
    else
        free_memory = ((int)&free_memory) - ((int)__brkval);

    return free_memory;
}

uint Memory::GetFreeROM(SdVolume &volume)
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
    
    return ((volume.blocksPerCluster() * volume.clusterCount() * 512) - used_space) / 1024 / 1024;
}