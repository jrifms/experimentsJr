#include "manageFiles.h"


FilesJr::FilesJr()
{
}

bool FilesJr::begin(){

    #if defined(ESP8266)

    this->filesystem = SPIFFS.begin();

    #elif defined(ESP32)

    this->filesystem = SPIFFS.begin(true);

    #endif
    return this->filesystem;
}


bool FilesJr::deleteFile(const char* path)
{
    if (!SPIFFS.exists(path)) {
        return false;
    }
    
    #if defined(ESP32)
        String newPath = String("/") + String(path);
        return SPIFFS.remove(newPath.c_str());
    #endif  

    return SPIFFS.remove(path);
}


bool FilesJr::createFile(const char* path, const char* content)
{
#if defined(ESP8266)

    File file = SPIFFS.open(path, "w");

#elif defined(ESP32)

    String newPath = String("/") + String(path);
    File file = SPIFFS.open(newPath.c_str(), FILE_WRITE);

#endif

    if (!file) {
        //Serial.println( "Failed to create file");
        return false;
    }

    file.print(content);
    file.close();
    return true;
}


String FilesJr::readFile(const char* path)
{
String content = "";
#if defined(ESP8266)

    File file = SPIFFS.open(path, "r");

#elif defined(ESP32)

 String newPath = String("/") + String(path);
    File file = SPIFFS.open(newPath.c_str(), FILE_READ);

#endif

    if (!file) {
        return "";
    }
    content = file.readString();
    file.close();
    return content;
}


std::vector<String>  FilesJr::listFiles()
{
    std::vector<String> arquivos;

#if defined(ESP8266)

    File root = SPIFFS.open("/", "r");

#elif defined(ESP32)

    File root = SPIFFS.open("/");

#endif

    if (!root) {
        //Serial.println("Failed to open root directory");
        return arquivos;
    }

    File file = root.openNextFile();
    while (file) {
        arquivos.push_back(file.name());
        file = root.openNextFile();
    }
    return arquivos;
}


bool FilesJr::isFileSystemMounted()
{
    return this->filesystem;
}


bool FilesJr::fileExists(const char* path)
{
    #if defined(ESP8266)
        return SPIFFS.exists(path);
    #elif defined(ESP32)
        String newPath = String("/") + String(path);
        return SPIFFS.exists(newPath.c_str());
    #endif
}