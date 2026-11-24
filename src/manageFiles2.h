#ifndef FILES2_JR_H
#define FILES2_JR_H

#include <vector>
#include "constants.h"

#if defined(ESP8266)

    #include <FS.h>

#elif defined(ESP32)

    #include <FS.h>
    #include <SPIFFS.h>

#else

    #error "Plataforma não suportada"

#endif

/*This enumeration define the type of network*/
typedef enum networktype
{
    CONFIGURE,
    POINTACCESS,
    NETWORK
};

/* This struct define and keep the data that is save in file*/
typedef struct data
{
    networktype typeNet;
    bool isConfigured;
    char ssid[500];
    char password[32];
    char dns[500];
};

class FileOperation
{

public:
    String nameDefaultFile;

    FileOperation();
    bool fileWrite();
    bool fileRead();
    bool fileExists();
    bool fileRemove();
    data getData();
    void setData(String ssid, String password, String dns, bool isConfigured, networktype type);
    bool isFileMounted();
    
private:
    data dat;
    bool fileMounted = false;
};

#endif