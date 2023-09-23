#include <LiquidCrystal_I2C.h>
#include <Keypad.h> 
#include <SPI.h>
#include <HighPowerStepperDriver.h>

#include "Abstract.h"
#include "Menu.h"
#include "Division.h"


//Keypad
#define ROWS 4
#define COLS 4
#define LCD_ROWS 4
#define LCD_COLS 20
//Motor driver 
#define DIR_PIN 2
#define STEP_PIN 3
#define CS_PIN 4
#define SLEEP_PIN 5
#define STEP_PERIOD 2000
//Lcd
#define LCD_ADDRESS 0x27

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'S', '0', 'E', 'D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25}; 
byte colPins[COLS] = {26, 27, 28, 29}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//Timers
unsigned long startTime;
unsigned long currentTime;
const int buttonDelayTime = 250;
//Button flags and timers
bool buttonAvailable = true;
bool buttonHeld = false;
bool buttonCanRepeat = false;
const int buttonRepeatSpeed = 400;
unsigned long buttonRepeatTimer;
char currentKey;
char heldKey; 
//Stepper Motor and Rotary Table Ratio's
int stepperMotorStepsRatio = 200;
int rotaryTableRatio = 90;
float stepsForCompleteCircle = stepperMotorStepsRatio * rotaryTableRatio; //18,000
//Motor driver
HighPowerStepperDriver sd;

//Menu flags
bool mainMenu = false;
Abstract *currentMenu[5];
byte currentMenuIndex = 0;
//Keypad declarations
void keypadInput(Menu&);
void buttonPressed(char, Menu&);
void resetButtonAvailableFlagAndStartTimeTimer();
//Menu declarations
byte checkIndexInBounds(byte, byte);
void goBackOneScreen();
void goToMainMenu();
byte TEST = 4;
// //Start of program
void setup() {
  Serial.begin(9600);
  customKeypad.setHoldTime(500);

  lcd.begin(); 
  SPI.begin();
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
  sd.setCurrentMilliamps36v4(2800);
  sd.setStepMode(HPSDStepMode::MicroStep1);
  sd.enableDriver();
}

void loop() {
  Menu menu(ROWS, COLS, LCD_ROWS, LCD_COLS, lcd);
  menu.createScrollScreen(0);
  currentMenu[0] = &menu;
  Serial.println(sd.readStatus());
  while(!mainMenu){
    keypadInput(currentMenu[currentMenuIndex]);
  }
  mainMenu = false;
}
// //End of program
// //Keypad functions
void keypadInput(Menu *menu){
  currentTime = millis();
  if (currentTime - startTime >= buttonDelayTime){
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
  if (buttonHeld && currentTime - buttonRepeatTimer >= buttonRepeatSpeed){
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

void goBackOneScreen(){
  delete currentMenu[currentMenuIndex];
  currentMenuIndex--;
  if(currentMenu[currentMenuIndex]->cursorMode == true)
    currentMenu[currentMenuIndex]->createScrollScreen(0);
  else {
    currentMenu[currentMenuIndex]->createStaticScreen(currentMenu[currentMenuIndex]->staticScreenInput);
  }
}

void goToMainMenu(){
  lcd.cursor_off();
  for (byte i = 1; i <= currentMenuIndex; i++){
    delete currentMenu[i];
  }
  currentMenuIndex = 0;
  mainMenu = true;
}

// Sends a pulse on the STEP pin to tell the driver to take one step, and also
//delays to control the speed of the motor.
void step()
{
  // The STEP minimum high pulse width is 1.9 microseconds.
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(3); //3
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(3); //3
}

// Writes a high or low value to the direction pin to specify what direction to
// turn the motor.
void setDirection(bool dir)
{
  // The STEP pin must not change for at least 200 nanoseconds before and after
  // changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(DIR_PIN, dir);
  delayMicroseconds(1);
}




