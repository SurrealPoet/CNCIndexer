#ifndef Menu_H
#define Menu_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Abstract.h"
#include "Division.h"

class Menu: public Abstract {
  public:
  
    byte lineIndex = 0;
    byte cursorIndex = 0;
    bool cursorMode = true;
    byte numberOfLines = 6;
    String lines[6] = {{"Jog"}, {"Division"}, {"Degree"}, {"Continuous"}, {"Program"}, {"Setup"}};
    void processButtonPressed(char keyPressed);
    void processCursorMode(char keyPressed);
    byte determineLineSelection();
    void execute(byte lineSelection);
    void createScrollScreen(byte moveLines);
    void drawScrollCursor();
    void createStaticScreen(String staticScreenInput);
    void drawStaticCursor(byte x, byte y);
    void staticUpdate(char keyPressed);
    Menu (byte rows, byte cols, byte lcdRows, byte lcdCols, LiquidCrystal_I2C &lcd);
    ~Menu ();
  private:  
    byte rows;
    byte cols;
    byte lcdRows;
    byte lcdCols;
    LiquidCrystal_I2C lcd;
};
#endif