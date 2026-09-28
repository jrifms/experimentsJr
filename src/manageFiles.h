/*
  FilesJr.h - Library for managing files on SPIFFS.
  Created by Junior Silva Souza, Setember 9, 2026.
  Released into the public domain.
*/

/**
 * @brief A simple file management class for Arduino.
 * This class allows you to manage files on SPIFFS.
 * @example
 *
 * #include "FilesJr.h"
 *
 * FilesJr files();
 *
 * void setup() {
 *   if(files.isFileSystemMounted()) {
 *     Serial.println("File system mounted successfully");
 *   } else {
 *     Serial.println("Failed to mount file system");
 * }
 *
 */
#ifndef FILES_JR_H
#define FILES_JR_H
#include <FS.h> 

class FilesJr {
  public:
    FilesJr();
    void listFiles();
    bool createFile(char* content);
    bool readFile(String& content);
    bool deleteFile(char* path);
    bool fileExists(char* path);
    bool isFileSystemMounted();

    private:
        char* path;
        bool filesytem;
};
#endif