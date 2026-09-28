#ifndef FILES_JR_H
#define FILES_JR_H

#include <vector>

#if defined(ESP8266)

    #include <FS.h>

#elif defined(ESP32)

    #include <FS.h>
    #include <SPIFFS.h>

#else

    #error "Plataforma não suportada"

#endif


class FilesJr {
public:

    FilesJr();

    std::vector<String>  listFiles();

    bool createFile(const char* path, const char* content);

    String readFile( const char* path);

    bool deleteFile(const char* path);

    bool fileExists(const char* path);

    bool isFileSystemMounted();

private:
    bool filesystem;
};

#endif