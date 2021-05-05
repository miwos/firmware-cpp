#ifndef Displays_h
#define Displays_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET 4

#define WHITE SSD1306_WHITE
#define BLACK SSD1306_BLACK

namespace Displays {
  typedef Adafruit_SSD1306 Display;

  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // const byte maxDisplays = 1;
  // Adafruit_SSD1306 displays[maxDisplays] = {
  //   Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
  // };

  // Display* getDisplay(byte index) {
    // return &displays[index];
  // }

  void begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("SSD1306 allocation failed");
      for(;;);
    }

    display.setTextSize(0);
    display.setTextColor(WHITE);
  }
}

#endif