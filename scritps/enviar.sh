/home/jr/.arduino15/packages/esp8266/hardware/esp8266/3.1.2/tools/upload.py \
--chip esp8266 \
--port /dev/ttyUSB0 \
--baud 115200 \
write_flash \
0x200000 \
spiffs.bin
