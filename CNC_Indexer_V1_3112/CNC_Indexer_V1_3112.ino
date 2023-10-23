#include <LiquidCrystal_I2C.h>
#include <Keypad.h> 
#include <SPI.h>
#include <HighPowerStepperDriver.h>
#include "Abstract.h"
#include "Menu.h"
#include "Division.h"
#include "CNCIndexerVariables.h"

//Keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'S', '0', 'E', 'D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25}; 
byte colPins[COLS] = {26, 27, 28, 29}; 
//Timers
unsigned long startTime;
unsigned long currentTime;
//Button flags
bool buttonAvailable = true;
bool buttonHeld = false;
bool buttonCanRepeat = false;
unsigned long buttonRepeatTimer;
char currentKey;
char heldKey; 
//Menu flag
bool mainMenu = false;
//Menu 
Abstract *currentMenu[5];
byte currentMenuIndex = 0;

//LCD initialization
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
//Keypad initialization
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//Motor driver initialization-must set milliamps for motor below
HighPowerStepperDriver sd;

// //Start of program
void setup() {
  Serial.begin(9600);
  // Serial.println("setup");
  SPI.begin();
  customKeypad.setHoldTime(500);
  lcd.init();
  lcd.backlight();
  sd.setChipSelectPin(CS_PIN);
   // Give the driver some time to power up
  delay(1);
  // Reset the driver to its default settings and clear latched status
  // conditions
  sd.resetSettings();
  sd.clearStatus();
  // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
  // for most applications
  sd.setDecayMode(HPSDDecayMode::AutoMixed);
  // Motor milliamps
  sd.setCurrentMilliamps36v4(2800);
  sd.setStepMode(HPSDStepMode::MicroStep1);
  sd.enableDriver();
}

void loop() {
  // Division *division = new Division(LCD_COLS, lcd);
  // Division division(LCD_COLS, lcd);
  // Test test;
  Menu menu(LCD_COLS, LCD_ROWS, lcd);
  menu.createScrollScreen(0);
  currentMenu[0] = &menu;
  Serial.println(sd.readStatus());
  while(!mainMenu){
    keypadInput(currentMenu[currentMenuIndex]);
    // menu.execute(1, LCD_COLS, lcd, currentMenu)
  }
  mainMenu = false;
}
// //End of program

//Keypad declarations
void keypadInput(Menu&);
void buttonPressed(char, Menu&);
void resetButtonAvailableFlagAndStartTimeTimer();

// //Keypad functions
void keypadInput(Menu *menu){
  currentTime = millis();
  if (currentTime - startTime >= BUTTON_DELAY_TIME){
    buttonAvailable = true;
  }
  if (buttonAvailable){ 
    char keypadKeyPressed = customKeypad.getKey();
    buttonPressed(keypadKeyPressed, menu);
  }
  if (customKeypad.getState() == HOLD){
    if (buttonHeld == false){
      buttonRepeatTimer = currentTime;
    }
    buttonHeld = true;
    heldKey = currentKey;
  }
  if (buttonHeld == true && customKeypad.getState() == RELEASED){
    buttonHeld = false;
  }
  if (buttonHeld && currentTime - buttonRepeatTimer >= BUTTON_REPEAT_SPEED){
    buttonCanRepeat = true;
  }
  if (buttonHeld && buttonCanRepeat){
    menu->processButtonPressed(heldKey);
    buttonCanRepeat = false;
    buttonRepeatTimer = currentTime;
  }
}

void buttonPressed(char keyPressed, Menu *menu){
  if(keyPressed){
    menu->processButtonPressed(keyPressed);
    currentKey = keyPressed;
  resetButtonAvailableFlagAndStartTimeTimer();
  }
}

void resetButtonAvailableFlagAndStartTimeTimer(){
  buttonAvailable = false;
  startTime = millis();  
}




