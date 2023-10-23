#ifndef Abstract_H
#define Abstract_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <HighPowerStepperDriver.h>
#include "CNCIndexerVariables.h"

class Abstract {
  public:
    // byte lineIndex = 0;
    // byte cursorIndex = 0;
    // byte staticCursorIndex = 0;
    // String staticScreenInput = "";
    // bool cursorMode = true;
    // byte numberOfLines = 6;
    String lines[6];
    // enum StringLines {
    //   Jog, 
    //   Division, 
    //   Degree, 
    //   Continuous, 
    //   Program, 
    //   Setup
    // };
    // virtual byte getLineIndex();
    // virtual void setLineIndex(byte lineIndex);
    // virtual byte getCursorIndex();
    // virtual void setCursorIndex(byte cursorIndex);
    // virtual byte getStaticCursorIndex();
    // virtual void setStaticCursorIndex(byte staticCursorIndex);
    // virtual String getStaticScreenInput();
    // virtual void setStaticScreenInput(String staticScreenInput);
    // virtual bool getCursorMode();
    // virtual void setCursorMode(bool cursorMode);
    // virtual byte getNumberOfLines();
    // virtual void setNumberOfLines(byte numberOfLines);
    
    virtual void processButtonPressed(char keyPressed) = 0;
    virtual void processCursorMode(char keyPressed) = 0;
    virtual void execute(byte lineSelection) = 0;
    // virtual void execute(byte lineSelection, byte LCD_COLS,LiquidCrystal_I2C lcd, Abstract *currentMenu) = 0;
    virtual void createScrollScreen(byte moveLines) = 0;
    virtual void drawScrollCursor() = 0;
    virtual void createStaticScreen(String staticScreenInput, LiquidCrystal_I2C lcd) = 0;
    virtual void drawStaticCursor(byte x, byte y, LiquidCrystal_I2C lcd) = 0;
    virtual void staticUpdate(char keyPressed) = 0;
    
    byte determineLineSelection();
    byte checkIndexInBounds(byte index, byte numberOfLines);
    void goBackOneScreen();
    void goToMainMenu();
    void step();
    void setDirection(bool dir);

    Abstract() {
      Serial.println("constructor Abstract");
    }
    ~Abstract() {
      Serial.println("destructor Abstract");
    }
  private:

  protected:
    byte lineIndex = 0;
    byte cursorIndex = 0;
    byte staticCursorIndex = 0;
    String staticScreenInput = "";
    bool cursorMode = true;
    byte numberOfLines = 6;
};
#endif