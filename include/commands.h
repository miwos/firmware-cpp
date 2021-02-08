#ifndef commands_h
#define commands_h

File file;

namespace commands {
  class Command {
    public:
    virtual void begin(char* fileName) = 0;
    virtual void update(char c) = 0;
    virtual void end() = 0;
  };

  class WriteFile : public Command {
    public:
    void begin(char* fileName) {
      file = SD.open(fileName, FILE_WRITE);
    }
    void update(char c) {
      file.write((uint8_t) c);
    }
    void end() {
      file.close();
    }
  };

  class ReadFile : public Command {
    public:
    void begin(char* fileName) {
      file = SD.open(fileName, FILE_READ);
    }
    void update(char c) {
      if (file.available()) {
        Serial.write(file.read());
      } else {
        end();
      }
    }
    void end() {
      file.close();
    }
  };

  WriteFile writeFile;
  ReadFile readFile;
  Command* commands[] = {
    &writeFile,
    &readFile
  };

  enum commandTypes {
    TypeWriteFile,
    TypeReadFile
  };

  Command* getCommand(char* commandName) {
    if (strcmp(commandName, "write-file") == 0) {
      return commands[TypeWriteFile];
    } else if (strcmp(commandName, "read-file") == 0) {
      return commands[TypeReadFile];
    } else {
      return nullptr;
    }
    // } else if (strcmp(commandName, "read-file")) {
    //   return CommandReadFile;
    // } else if (strcmp(commandName, "list-files")) {
    //   return CommandListFiles;
    // }
  }
}

#endif