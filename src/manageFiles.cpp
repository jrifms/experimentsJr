#include "manageFiles.h"


FilesJr::FilesJr()
{
#if defined(ESP8266)

    this->filesystem = SPIFFS.begin();

#elif defined(ESP32)

    this->filesystem = SPIFFS.begin(true);

#endif

}


bool FilesJr::deleteFile(const char* path)
{
    if (!SPIFFS.exists(path)) {
        return false;
    }

    return SPIFFS.remove(path);
}


bool FilesJr::createFile(
    const char* path,
    const char* content
)
{
#if defined(ESP8266)

    File file = SPIFFS.open(path, "w");

#elif defined(ESP32)

    File file = SPIFFS.open(path, FILE_WRITE);

#endif

    if (!file) {

        //Serial.println( "Failed to create file");
        return false;
    }

    file.print(content);
    file.close();

    return true;
}


bool FilesJr::readFile(
    const char* path,
    String& content
)
{
#if defined(ESP8266)

    File file = SPIFFS.open(path, "r");

#elif defined(ESP32)

    File file = SPIFFS.open(path, FILE_READ);

#endif

    if (!file) {

        //Serial.println( "Failed to open file for reading");

        return false;
    }

    content = file.readString();

    file.close();

    return true;
}


void FilesJr::listFiles()
{
    //Serial.println("Listing files:");

#if defined(ESP8266)

    File root = SPIFFS.open("/", "r");

#elif defined(ESP32)

    File root = SPIFFS.open("/");

#endif

    if (!root) {

        //Serial.println("Failed to open root directory");

        return;
    }

    File file = root.openNextFile();

    while (file) {

        Serial.print("FILE: ");
        Serial.println(file.name());

        file = root.openNextFile();
    }
}


bool FilesJr::isFileSystemMounted()
{
    return this->filesystem;
}


bool FilesJr::fileExists(const char* path)
{
    return SPIFFS.exists(path);
}