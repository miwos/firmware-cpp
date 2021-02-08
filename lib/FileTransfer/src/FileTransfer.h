#ifndef FileTransfer_h
#define FileTransfer_h

#include <SD.h>

#define END_OF_TRANSMISSION 0x04

namespace fileTransfer {
  enum transferModes {
    TransferWrite,
    TransferRead
  };

  using WriteFinishedEvent = void (*)(char* fileName);

  File file;
  int transferMode = -1;
  uint8_t buffer = 0;

  WriteFinishedEvent handleWriteFinished;

  /**
   * Open a file and get ready for reading.
   */
  void beginRead(const char* fileName) {
    file = SD.open(fileName);
    transferMode = TransferRead;
  }

  /**
   * Open a file and get ready for writing.
   */
  void beginWrite(const char* fileName) {
    if (SD.exists(fileName)) SD.remove(fileName);
    file = SD.open(fileName, FILE_WRITE);
    transferMode = TransferWrite;
  }

  /**
   * Close the file and reset the transfer mode.
   */
  void close() {
    if (file) file.close();
    transferMode = -1;
  }

  /**
   * Read a byte from the file and write it to the serial.
   */
  void read() {
    if (file.available()) {
      Serial.write(file.read());
    } else {
      Serial.write(END_OF_TRANSMISSION);
      close();
    }
  }

  /**
   * Write a byte to the file.
   */
  void write() {
    if (buffer == END_OF_TRANSMISSION) {
      if (handleWriteFinished) {
        handleWriteFinished(file.name());
      }   
      close();
    } else {
      file.write(buffer);
    }
    buffer = 0;    
  }

  /**
   * Update the file transfer. This will either read or write to file or do
   * nothing depending on the transfer mode.
   */
  void update() {
    if (transferMode == TransferRead) {
      read();
    } else if (transferMode == TransferWrite && buffer != 0) {
      write();
    }
  }

  void onWriteFinished(WriteFinishedEvent handler) {
    handleWriteFinished = handler;
  }
}

#endif