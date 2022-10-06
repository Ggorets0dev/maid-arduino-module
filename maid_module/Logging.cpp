#include "devices.h"

Logging::Logging(String logs_filename, String blocks_filename)
{
    this->logs_filename = logs_filename;
    this->blocks_filename = blocks_filename;
}

ulong Logging::GetFreeSpaceSize()
{
    File root = SD.open("/");
    ulong free_space_left_bytes = 0;

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
            free_space_left_bytes += entry.size();
            entry.close();
        }
    }

    root.close();
    return volume_size_mb - (free_space_left_bytes / 1024 / 1024);
}

bool Logging::MemoryInit(Sd2Card &card, SdVolume &volume, SdFile &root)
{ 
    if (!card.init(SPI_HALF_SPEED, MEMORY_PIN))
    {
        Serial.println("Not found!");
        return false;
    }

	  else if (!volume.init(card))
    {
        Serial.println("Failed to init volume");
        return false;
    }

    this->volume_size_mb = (volume.blocksPerCluster() * volume.clusterCount() * 512) / 1024 / 1024;
    Serial.println(volume_size_mb);
    root.openRoot(volume);

    root.ls(LS_R);

    File logs_file = SD.open(logs_filename);
    File blocks_file = SD.open(blocks_filename);
    
  	if (!logs_file || !blocks_file)
    {
    		Serial.println("Failed to find all files needed");
    		return false;
  	}

   logs_file.close();
   blocks_file.close();
  
   return true;
}

void Logging::WriteBlocks(Node* head)
{

}

void Logging::Log(LogType type, String msg)
{
	
}
