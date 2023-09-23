#ifndef Division_H
#define Division_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Abstract.h"

class Division: public Abstract {
  public:
    void processButtonPressed(char keyPressed);
    void processCursorMode(char keyPressed);
    void createScrollScreen(byte moveLines){

    }
    void drawScrollCursor(){

    }
    void createStaticScreen(String lineInput, LiquidCrystal_I2C lcd);
    void drawStaticCursor(byte x, byte y, LiquidCrystal_I2C lcd);
    void execute(byte lineSelection);
    void staticUpdate(char keyPressed);
    Division (byte rows, byte cols, byte lcdRows, byte lcdCols, LiquidCrystal_I2C &lcd);
    ~Division ();
  private:
    byte rows;
    byte cols;
    byte lcdRows;
    byte lcdCols;
    LiquidCrystal_I2C lcd;
};
#endif