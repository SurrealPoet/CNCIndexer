#ifndef Menu_H
#define Menu_H
#include "Abstract.h"
#include "Division.h"

class Menu: public Abstract {
  public:
    // LiquidCrystal_I2C lcd;
    // byte lcdCols;
    // byte lineIndex = 0;
    // byte cursorIndex = 0;
    // bool cursorMode = true;
    // byte numberOfLines = 6;
    String lines[6] = {{"Jog"}, {"Division"}, {"Degree"}, {"Continuous"}, {"Program"}, {"Setup"}};
    void processButtonPressed(char keyPressed);
    void processCursorMode(char keyPressed);
    // byte determineLineSelection();
    void execute(byte lineSelection);
    // void execute(byte lineSelection, byte LCD_COLS,LiquidCrystal_I2C lcd, Abstract *currentMenu);
    void createScrollScreen(byte moveLines);
    void drawScrollCursor();
    void createStaticScreen(String staticScreenInput, LiquidCrystal_I2C lcd);
    void drawStaticCursor(byte x, byte y, LiquidCrystal_I2C lcd);
    void staticUpdate(char keyPressed);
    Menu (byte _lcdCols, byte _lcdRows, LiquidCrystal_I2C _lcd);
    ~Menu ();
  private:  
    byte lcdCols;
    byte lcdRows;
    LiquidCrystal_I2C lcd;
    // Abstract currentMenu;
};
#endif