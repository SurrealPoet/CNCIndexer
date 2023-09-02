#include <LiquidCrystal_I2C.h>
#include <Keypad.h> 

//Keypad Size
const byte ROWS = 4; 
const byte COLS = 4;
//Timers
unsigned long startTime;
unsigned long currentTime;
const int buttonDelayTime = 250;
//Button flags and timers
bool buttonAvailable = true;
bool buttonHeld = false;
bool buttonCanRepeat = false;
const int buttonRepeatSpeed = 500;
unsigned long buttonRepeatTimer;
char currentKey;
char heldKey;
//Menu flags
bool mainMenu = false;
bool cursorMode = true;
byte lcdRow[2];
byte lcdcol[16];
LiquidCrystal_I2C lcd(0x27, 20, 4);
//Map keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'S', '0', 'E', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6, 7, 8, 9}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

class Menu{
  public:
    byte lineIndex = 0;
    byte cursorIndex = 0;
    const byte numberOfLines = 6;
    String lines[6] = {{"Jog"}, {"Division"}, {"Degree"}, {"Continuos"}, {"Program"}, {"Setup"}};
    void execute(char buttonPressed);
    void createScreen(byte moveLines);
    void drawCursor();
    Menu (){
      Serial.println("constructor Menu");
    }
};

//Keypad declarations
void keypadInput(Menu&);
void processButtonPressed(char, Menu&);
void buttonActions(char, Menu&);
void resetButtonAvailableFlagAndStartTimeTimer();
//Menu declarations
byte checkIndexInBounds(byte, byte);

//Start of program
void setup(){
  Serial.begin(9600);
  customKeypad.setHoldTime(1000);
  lcd.begin(); 
}

void loop(){
  Menu menu;
  menu.createScreen(0);
  while(!mainMenu){
    keypadInput(menu);
  }
  mainMenu = false;
}
//End of program
//Keypad functions
void processButtonPressed(char buttonPressed, Menu &menu){
  if(buttonPressed){
    // buttonActions(buttonPressed, menu);
    menu.execute(buttonPressed);
    currentKey = buttonPressed;
  resetButtonAvailableFlagAndStartTimeTimer();
  }
}

void resetButtonAvailableFlagAndStartTimeTimer(){
  buttonAvailable = false;
  startTime = millis();  
}

void keypadInput(Menu &menu){
  currentTime = millis();
  if (currentTime - startTime >= buttonDelayTime){
    buttonAvailable = true;
  }
  if (buttonAvailable){ 
    char keypadButtonPressed = customKeypad.getKey();
    processButtonPressed(keypadButtonPressed, menu);
  }
  if (customKeypad.getState() == HOLD){
    buttonHeld = true;
    heldKey = currentKey;
  }
  if (buttonHeld == true && customKeypad.getState() == RELEASED){
    buttonHeld = false;
  }
  if (currentTime - buttonRepeatTimer >= buttonRepeatSpeed){
    buttonCanRepeat = true;
  }
  if (buttonHeld && buttonCanRepeat){
    // buttonActions(heldKey, menu);
    menu.execute(heldKey);
    buttonCanRepeat = false;
    buttonRepeatTimer = currentTime;
  }
}

byte checkIndexInBounds(byte index, byte numberOfLines){
  byte tempIndex = index;
  if(tempIndex == 255){
    tempIndex = numberOfLines - 1;
  }
  if(tempIndex > numberOfLines - 1){
    tempIndex = 0;
  }
  return tempIndex;
}

void Menu::execute(char buttonPressed){
  static const char A[] = "F1";
  static const char B[] = "F2";
  static const char C[] = "F3";
  static const char D[] = "F4";
  static const char E[] = "END";
  static const char S[] = "START";
  switch (buttonPressed){
    case '1':
      Serial.println(buttonPressed);
      break;
    case '2':
      Serial.println(buttonPressed);
      if (cursorMode){
        this->cursorIndex += -1;
      }
      if (this->cursorIndex == 255){
        this->cursorIndex = 0;
        this->createScreen(-1);
      }
      else {
        this->createScreen(0);
      }
      break;
    case '3':
      Serial.println(buttonPressed);
      break;
    case '4':
      Serial.println(buttonPressed);
      if (cursorMode){
      //move cursor left
      }
      break;
    case '5':
      Serial.println(buttonPressed);
      break;
    case '6':
      Serial.println(buttonPressed);
      if (cursorMode){
      //move cursor right
      }
      break;
    case '7':
      Serial.println(buttonPressed);
      break;
    case '8':
      Serial.println(buttonPressed);
      if (cursorMode){
        this->cursorIndex += 1;
      }    
      if (this->cursorIndex == 4){
        this->cursorIndex = 3;
        this->createScreen(1);
      }
      else {
        this->createScreen(0);
      }
      break;
    case '9':
      Serial.println(buttonPressed);
      break;
    case '0':
      Serial.println(buttonPressed);
      break;
    case 'A':
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

void Menu::createScreen(byte moveLines){
  lcd.clear();
  lcd.setCursor(1, 0);
  byte x = 0;
  byte tempIndex = this->lineIndex + moveLines;
  this->lineIndex = checkIndexInBounds(tempIndex, numberOfLines);
  for(byte i = 0; i < 4; i++){
    tempIndex = checkIndexInBounds(tempIndex, numberOfLines); 
    lcd.print(lines[tempIndex]);
    tempIndex++;
    lcd.setCursor(1, x + 1);
    x++;
  }
  drawCursor();
}
void Menu::drawCursor(){
  lcd.setCursor(0, cursorIndex);
  lcd.print('>');
}