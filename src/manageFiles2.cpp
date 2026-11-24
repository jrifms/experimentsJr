#include "manageFiles2.h"


FileOperation::FileOperation()
{
    // Let's configurate the dat from connection
    setData("ConfigureDeviceJR", "12345678", "velhojack.local", CONFIGURE);

    // Let's catch the name of file
    nameDefaultFile = "/conf.txt";
}

bool FileOperation::fileWrite()
{
    // Check if file is mounted!
    if (isFileMounted())
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - The file system is mounted!");
        }
    }
    //-----------------------------------------------

    File arquivo = SPIFFS.open(this->nameDefaultFile.c_str(), FILE_WRITE);
    if (!arquivo)
    {
        if (DEBUG_MSG)
        {
            Serial.println("Erro ao abrir arquivo!");
        }
        return false;
    }
    arquivo.write((uint8_t *)&dat, sizeof(data));
    arquivo.close();
    return true;
}

bool FileOperation::fileRead()
{
    // Check if the system file is mounted
    if (isFileMounted())
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - The file system is mounted!");
        }
    }
    //-----------------------------------------------------------------
    File arquivo = SPIFFS.open(this->nameDefaultFile.c_str(), FILE_READ);
    if (!arquivo)
    {
        if (DEBUG_MSG)
        {
            Serial.println("Arquivo nao encontrado!");
        }
        return false;
    }

    if (arquivo.size() != sizeof(data))
    {
        if (DEBUG_MSG)
        {
            Serial.println("|   - The file not have the data storing!");
        }
        arquivo.close();
        return false;
    }
    size_t bytes = arquivo.read((uint8_t *)&dat, sizeof(data));
    arquivo.close();

    return bytes == sizeof(data);
}

bool FileOperation::fileExists()
{
    if (isFileMounted())
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - The file system is mounted!");
        }
    }
    if (SPIFFS.exists(this->nameDefaultFile.c_str()))
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - The file [" + this->nameDefaultFile + "] exist");
        }
        return true;
    }
    else
    {
        if (DEBUG_MSG)
        {
            Serial.println("|   - The file [" + this->nameDefaultFile + "] not exist! ");
        }
        return false;
    }
}

bool FileOperation::fileRemove()
{
    if (isFileMounted())
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - The file system is mounted!");
        }
    }
    if (this->fileExists())
    {
        if (SPIFFS.remove(this->nameDefaultFile.c_str()))
        {
            if (DEBUG_MSG)
            {
                Serial.println("|   - The file[" + this->nameDefaultFile + "] is removed !");
            }
        }
        else
        {
            if (DEBUG_MSG)
            {
                Serial.println("|   - The file[" + this->nameDefaultFile + "] is not removed!");
            }
            return false;
        }
    }
    else
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - The file [" + this->nameDefaultFile + "] not exists! ");
        }
        return false;
    }
    return true;
}

data FileOperation::getData()
{
    return this->dat;
}

void FileOperation::setData(String ssid, String password, String dns, networktype type)
{
    dat.typeNet = type;
    strcpy(dat.ssid, ssid.c_str());
    strcpy(dat.password, password.c_str());
    strcpy(dat.dns, dns.c_str());
}

bool FileOperation::isFileMounted()
{
    if (this->fileMounted)
    {
        return true;
    }
#if defined(ESP32)
    if (!SPIFFS.begin(false))
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - Erro ao montar SPIFFS!");
        }
        this->fileMounted = false;
        return false;
    }
#elif defined(ESP8266)
    if (!SPIFFS.begin())
    {
        if (DEBUG_MSG)
        {
            Serial.println("|  - Erro ao montar SPIFFS!");
        }
        this->fileMounted = false;
        return false;
    }
#endif

    this->fileMounted = true;
    return true;
}

