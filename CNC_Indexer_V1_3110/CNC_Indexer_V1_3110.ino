#include <LiquidCrystal_I2C.h>
#include <Keypad.h> 
#include <SPI.h>
#include <HighPowerStepperDriver.h>


//Keypad
const byte ROWS = 4; 
const byte COLS = 4;
byte lcdRow = 4;
byte lcdCol = 20;
LiquidCrystal_I2C lcd(0x27, lcdCol, lcdRow);
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
const byte DirPin = 2;
const byte StepPin = 3;
const byte CSPin = 4;
const byte sleepPin = 5;
const uint16_t StepPeriodUs = 2000;
HighPowerStepperDriver sd;

class Abstract{
  public:
      byte lineIndex = 0;
      byte cursorIndex = 0;
      byte staticCursorIndex = 0;
      String staticScreenInput = "";
      bool cursorMode = true;
      byte numberOfLines = 6;
      String lines[6];
      virtual void processButtonPressed(char keyPressed) = 0;
      virtual void processCursorMode(char keyPressed) = 0;
      byte determineLineSelection();
      virtual void execute(byte lineSelection) = 0;
      virtual void createScrollScreen(byte moveLines) = 0;
      virtual void drawScrollCursor() = 0;
      virtual void createStaticScreen(String staticScreenInput) = 0;
      virtual void drawStaticCursor(byte x, byte y) = 0;
      virtual void staticUpdate(char keyPressed) = 0;
      Abstract (){
        Serial.println("constructor Abstract");
      }
      virtual ~Abstract (){
        Serial.println("destructor Abstract");
      }
};
class Menu: public Abstract{
  public:
    byte lineIndex = 0;
    byte cursorIndex = 0;
    bool cursorMode = true;
    byte numberOfLines = 6;
    String lines[6] = {{"Jog"}, {"Division"}, {"Degree"}, {"Continuos"}, {"Program"}, {"Setup"}};
    void processButtonPressed(char keyPressed);
    void processCursorMode(char keyPressed);
    byte determineLineSelection();
    void execute(byte lineSelection);
    void createScrollScreen(byte moveLines);
    void drawScrollCursor();
    void createStaticScreen(String staticScreenInput);
    void drawStaticCursor(byte x, byte y);
    void staticUpdate(char keyPressed);
    Menu (){
      Serial.println("constructor Menu");
    }
    ~Menu (){
        Serial.println("destructor Menu");
    }
};

class Division: public Abstract{
  public:
    void processButtonPressed(char keyPressed);
    void processCursorMode(char keyPressed);
    void createScrollScreen(byte moveLines){

    }
    void drawScrollCursor(){

    }
    void createStaticScreen(String lineInput);
    void drawStaticCursor(byte x, byte y);
    void execute(byte lineSelection);
    void staticUpdate(char keyPressed);
    Division (){
      Serial.println("constructor Division");
      cursorMode = false;
    }
    ~Division (){
        Serial.println("destructor Division");
    }
};

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

//Start of program
void setup(){
  Serial.begin(9600);
  customKeypad.setHoldTime(500);
  // customKeypad.addEventListener(keypadEvent);

  lcd.begin(); 
  SPI.begin();
  sd.setChipSelectPin(CSPin);
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

void loop(){
  Menu menu;
  menu.createScrollScreen(0);
  currentMenu[0] = &menu;
  Serial.println(sd.readStatus());
  while(!mainMenu){
    keypadInput(currentMenu[currentMenuIndex]);
  }
  mainMenu = false;
}
//End of program
//Keypad functions
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

byte checkIndexInBounds(byte index, byte numberOfLines){
  byte tempIndex = index;
  if(tempIndex == 255){
    tempIndex = numberOfLines - 1;
  }
  if(tempIndex >= numberOfLines){
    tempIndex = 0;
  }
  return tempIndex;
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

void Menu::processButtonPressed(char keyPressed){
  static const char A[] = "F1-Main Menu";
  static const char B[] = "F2";
  static const char C[] = "F3";
  static const char D[] = "F4";
  static const char E[] = "END";
  static const char S[] = "START";
  if(cursorMode){
    processCursorMode(keyPressed);
  }
  else{
    switch (keyPressed){
      case '1':
        Serial.println(keyPressed);
      break;
      case '2':
        Serial.println(keyPressed);
        break;
      case '3':
        Serial.println(keyPressed);
        break;
      case '4':
        Serial.println(keyPressed);
        break;
      case '5':
        Serial.println(keyPressed);
        break;
      case '6':
        Serial.println(keyPressed);
        break;
      case '7':
        Serial.println(keyPressed);
        break;
      case '8':
        Serial.println(keyPressed);
        break;
      case '9':
        Serial.println(keyPressed);
        break;
      case '0':
        Serial.println(keyPressed);
        break;
      case 'A': //F1-Main Menu
        Serial.println(A);
        mainMenu = true;
        break;
      case 'B':
        Serial.println(B);
        break;
      case 'C':
        Serial.println(C);
        break;
      case 'D':
        Serial.println(D);
        break;
      case 'E':
        Serial.println(E);
        break;
      case 'S':
        Serial.println(S);
        break;
    }
  }    
}
void Menu::processCursorMode(char keyPressed){
  static const char A[] = "F1";  
  static const char S[] = "START";
  switch(keyPressed){
    case '2': //move up
      Serial.println(keyPressed);
      if (cursorMode){
        cursorIndex += -1;
      }
      if (cursorIndex == 255){
        cursorIndex = 0;
        createScrollScreen(-1);
      }
      else {
        createScrollScreen(0);
      }
      break;
    case '8': //move down
      Serial.println(keyPressed);
      if (cursorMode){
        cursorIndex += 1;
      }    
      if (cursorIndex == lcdRow){
        cursorIndex = lcdRow - 1;
        createScrollScreen(1);
      }
      else {
        createScrollScreen(0);
      }
      break;
    case 'A': //F1-Main Menu
        Serial.println(A);
        mainMenu = true;
        break;
    case 'S': //Start-Enter
      Serial.println(S);
      execute(determineLineSelection());
      break;
  }
}
byte Menu::determineLineSelection(){
  byte lineSelection = lineIndex + cursorIndex;
  if(lineSelection >= numberOfLines && lineSelection < 200){
    lineSelection -= numberOfLines;
  }
  return lineSelection;
}
void Menu::execute(byte lineSelection){
  switch(lineSelection){
    case 0:
      Serial.println("Jog");
      break;
    case 1:
      Serial.println("Division");
      {
        Division *division = new Division;
        currentMenuIndex++;
        currentMenu[currentMenuIndex] = division;
        division->createStaticScreen(staticScreenInput);
      }
      break;
    case 2:
      Serial.println("Degree");
      {

      }
      break;
    case 3:
      Serial.println("Continuos");
      break;
    case 4:
      Serial.println("Program");
      break;
    case 5:
      Serial.println("Setup");
      break;
  }
}
void Menu::createScrollScreen(byte moveLines){
  lcd.clear();
  lcd.setCursor(1, 0);
  byte x = 0;
  byte tempIndex = lineIndex + moveLines;
  lineIndex = checkIndexInBounds(tempIndex, numberOfLines);
  for(byte i = 0; i < 4; i++){
    tempIndex = checkIndexInBounds(tempIndex, numberOfLines); 
    lcd.print(lines[tempIndex]);
    tempIndex++;
    lcd.setCursor(1, x + 1);
    x++;
  }
  drawScrollCursor();
}
void Menu::drawScrollCursor(){
  lcd.setCursor(0, cursorIndex);
  lcd.print('>');
}
void Menu::createStaticScreen(String staticScreenInput){

}
void Menu::drawStaticCursor(byte x, byte y){

}
void Menu::staticUpdate(char keyPressed){

};

void Division::createStaticScreen(String lineInput){
  lcd.clear();
  lcd.home();
  lcd.print("Number of Divisions:");
  lcd.setCursor(0, 1);
  lcd.print(lineInput);
  lcd.setCursor(0, 2);
  lcd.print( "F1>Main Menu F2>Back");    
  lcd.setCursor(0, 3);
  lcd.print( "F3>Delete   F4>Enter");
  drawStaticCursor(staticCursorIndex, 1);
}
void Division::drawStaticCursor(byte x, byte y){
  lcd.setCursor(x, y);
  lcd.cursor_on();
}
void Division::execute(byte lineSelection){
  Serial.println("Division execute");
}
void Division::staticUpdate(char keyPressed){
  if (staticCursorIndex < lcdCol - 1){
    staticCursorIndex++;
    staticScreenInput += keyPressed;
  }
  createStaticScreen(staticScreenInput);
}
void Division::processButtonPressed(char keyPressed){
  static const char A[] = "F1-Main Menu";
  static const char B[] = "F2-Back";
  static const char C[] = "F3-Delete";
  static const char D[] = "F4-Enter";
  static const char S[] = "Start";
  static float temp = 0;
  static int counter = 0;
  if(cursorMode){
    processCursorMode(keyPressed);
  }
  else{
    switch (keyPressed){
      case '1':
        staticUpdate(keyPressed);
      break;
      case '2':
        staticUpdate(keyPressed);
        break;
      case '3':
        staticUpdate(keyPressed);
        break;
      case '4':
        staticUpdate(keyPressed);
        break;
      case '5':
        staticUpdate(keyPressed);
        break;
      case '6':
        staticUpdate(keyPressed);
        break;
      case '7':
        staticUpdate(keyPressed);
        break;
      case '8':
        staticUpdate(keyPressed);
        break;
      case '9':
        staticUpdate(keyPressed);
        break;
      case '0':
        staticUpdate(keyPressed);
        break;
      case 'A': //F1-Main Menu
        Serial.println(A);
        goToMainMenu();
        break;
      case 'B': //F2-Back
        Serial.println(B);
        lcd.cursor_off();
        goBackOneScreen();
        break;
      case 'C': //F3-Delete
        Serial.println(C);
        if (staticCursorIndex > 0){
          staticCursorIndex--;
          staticScreenInput.remove(staticCursorIndex);
        }
        createStaticScreen(staticScreenInput);
        break;
      case 'D': //F4-Enter
        Serial.println(D);
        Serial.println(stepsForCompleteCircle/staticScreenInput.toFloat());
        Serial.println(stepsForCompleteCircle/staticScreenInput.toInt());
        break;
      case 'S': //Start
      Serial.println(S);
      //Test at 11, 14 because int caste 17,999.9 to 17,999
      temp += stepsForCompleteCircle/staticScreenInput.toFloat();
      // Serial.println(temp);
      // Serial.println(int(temp));
      counter++;
      // Serial.println(counter); 

      digitalWrite(sleepPin, HIGH);
      setDirection(0);
      for(unsigned int x = 0; x < 1000; x++)
      {
        step();
        delayMicroseconds(StepPeriodUs);
      }
      digitalWrite(sleepPin, LOW);
    }
  }    
}
void Division::processCursorMode(char keyPressed){
  Serial.println("CursorMode");
}
// Sends a pulse on the STEP pin to tell the driver to take one step, and also
//delays to control the speed of the motor.
void step()
{
  // The STEP minimum high pulse width is 1.9 microseconds.
  digitalWrite(StepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(StepPin, LOW);
  delayMicroseconds(3);
}

// Writes a high or low value to the direction pin to specify what direction to
// turn the motor.
void setDirection(bool dir)
{
  // The STEP pin must not change for at least 200 nanoseconds before and after
  // changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(DirPin, dir);
  delayMicroseconds(1);
}

// void keypadEvent(KeypadEvent key){
//     switch (customKeypad.getState()){
//     case PRESSED:
//         Serial.println("Pressed");
//         break;

//     case RELEASED:
//         Serial.println("Released");
//         break;

//     case HOLD:
//         Serial.println("Hold");
//         break;
//     }
// }