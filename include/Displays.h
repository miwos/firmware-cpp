#ifndef Displays_h
#define Displays_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LuaOnArduino.h>
#include <fonts/Vevey_Positive20pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET 4

#define WHITE SSD1306_WHITE
#define BLACK SSD1306_BLACK

class Displays {
public:
  typedef Adafruit_SSD1306 Display;
  static const byte maxDisplays = 2;

private:
  LuaOnArduino *loa;
  Display displays[maxDisplays] = {
      Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET)};

  void initializeDisplay(Display *display) {
    display->clearDisplay();
    display->setFont(&Vevey_Positive20pt7b);
    display->setCursor(0, 10);
    display->setTextColor(WHITE);
    // !For some reasons when using `display->setTextColor()` have to print
    // print something afterwards to the serial, otherwise we cant sync files
    // with loa anymore... really strange.
    // TODO: Investigate what is causing the problem. Maybe somehow the loa
    // bridge switches it's read mode?
    Serial.print(" ");
  }

public:
  Displays(LuaOnArduino *loa) { this->loa = loa; }

  void begin() {
    for (byte i = 0; i < maxDisplays; i++) {
      if (displays[i].begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        initializeDisplay(&displays[i]);
      } else {
        loa->logger.errorBegin();
        // Increase the index to be consistent with lua's index.
        Serial.printf(F("Display #%d couldn't be initialized."), i + 1);
        loa->logger.logEnd();
      }
    }
  }

  Display *getDisplay(byte index) {
    if (index >= maxDisplays) {
      loa->logger.errorBegin();
      // Increase the index to be consistent with lua's index.
      Serial.printf(F("Display #%d doesn't exist."), index + 1);
      loa->logger.logEnd();
      return NULL;
    }
    return &(displays[index]);
  }
};

#endif