#ifndef SIMPLE_SD_H
#define SIMPLE_SD_H

// #include "FS.h"
#include "SD.h"
// #include "SPI.h"

class SimpleSD {
private:
  File current_file;
public:
  void begin() {
    if (!SD.begin(SS, SPI, 4000000)) {
      Serial.println("[ERROR] Card Mount Failed");
      return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
      Serial.println("[ERROR] No SD card attached");
      return;
    }

    Serial.print("[INFO] SD Card Type: ");
    if (cardType == CARD_MMC) {
      Serial.println("MMC");
    } else if (cardType == CARD_SD) {
      Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
      Serial.println("SDHC");
    } else {
      Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("[INFO] SD Card Size: %lluMB\n", cardSize);
  }

  File open(const char *filename, const char *flags = FILE_READ) {
    current_file = SD.open(filename, flags);
    return current_file;
  }

  void close(){
    current_file.close();
  };

  void ls(const char *dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);

    File root = SD.open(dirname);
    if (!root) {
      Serial.println("Failed to open directory");
      return;
    }
    if (!root.isDirectory()) {
      Serial.println("Not a directory");
      return;
    }

    File file = root.openNextFile();
    while (file) {
      if (file.isDirectory()) {
        Serial.print("  DIR : ");
        Serial.println(file.name());
        if (levels) {
          ls(file.name(), levels - 1);
        }
      } else {
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("  SIZE: ");
        Serial.println(file.size());
      }
      file = root.openNextFile();
    }
  }

  void mkdir(const char *path) {
    if (!SD.mkdir(path)) {
      Serial.println("mkdir failed");
    }
  }

  void rmdir(const char *path) {
    if (!SD.rmdir(path)) {
      Serial.println("rmdir failed");
    }
  }

  int32_t readFile(char *buffer, uint32_t size, const char *path) {
    Serial.printf("Reading file: %s\n", path);

    File file = SD.open(path);
    if (!file) {
      Serial.println("Failed to open file for reading");
      return -1;
    }
    int32_t bytes_read = 0;
    bytes_read = file.readBytes(buffer, size);
    file.close();
    buffer[bytes_read] = '\0';
    return bytes_read;
  }

  void writeFile(const char *path, const char *message) {
    Serial.printf("Writing file: %s\n", path);

    File file = SD.open(path, FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }
    if (file.print(message)) {
      Serial.println("File written");
    } else {
      Serial.println("Write failed");
    }
    file.close();
  }

  void appendFile(const char *path, const char *message) {
    Serial.printf("Appending to file: %s\n", path);

    File file = SD.open(path, FILE_APPEND);
    if (!file) {
      Serial.println("Failed to open file for appending");
      return;
    }
    if (file.print(message)) {
      Serial.println("Message appended");
    } else {
      Serial.println("Append failed");
    }
    file.close();
  }

  void mv(const char *source, const char *destination){
    if (!SD.rename(source, destination)){
      Serial.println("[ERROR] mv failed");
    }
  }

  void rm(const char *path) {
    if (!SD.remove(path)) {
      Serial.println("[ERROR] rm failed");
    }
  }

  void testFileIO(const char *path) {
    File file = SD.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file) {
      len = file.size();
      size_t flen = len;
      start = millis();
      while (len) {
        size_t toRead = len;
        if (toRead > 512) {
          toRead = 512;
        }
        file.read(buf, toRead);
        len -= toRead;
      }
      end = millis() - start;
      Serial.printf("%u bytes read for %u ms\n", flen, end);
      file.close();
    } else {
      Serial.println("Failed to open file for reading");
    }


    file = SD.open(path, FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++) {
      file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
  }
};

#endif