#ifndef Division_H
#define Division_H
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
    // void execute(byte lineSelection, byte LCD_COLS,LiquidCrystal_I2C lcd, Abstract *currentMenu);
    void staticUpdate(char keyPressed);
    Division (byte _lcdCols, LiquidCrystal_I2C _lcd);
    ~Division ();
  private:
    byte lcdCols;
    LiquidCrystal_I2C lcd;
};
#endif