#include "manageFiles.h"

FilesJr files;
 
void setup() {
		Serial.begin(115200);
		delay(1000);

    if(files.isFileSystemMounted()) {
      
			Serial.println("File system mounted successfully");
			// Creating a new file
			if(files.createFile("conf.txt", "192.168.0.1, acesspoint")){
				Serial.println("File created successfully! ");
			}else{
				Serial.println("File not created!");
			}

			/* --------------------------------------------------------------------------  */

			// // Reading content from file
			// String line = files.readFile("conf.txt");
			// Serial.println(line);


			/* --------------------------------------------------------------------------  */

			// // Checking if the file exists!
			// if(files.fileExists("conf.txt")){
			// 	Serial.println("The file exists!");

			// 	files.deleteFile("conf.txt");
			// }else{
			// 	Serial.println("The file not exists!");
			// }

    } else {
      Serial.println("Failed to mount file system");
  	}
		delay(1000);
}

void loop(){

}