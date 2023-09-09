#include <LiquidCrystal_I2C.h>
#include <Keypad.h> 

//Keypad
const byte ROWS = 4; 
const byte COLS = 4;
byte lcdRow[2];
byte lcdcol[16];
LiquidCrystal_I2C lcd(0x27, 20, 4);
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
const int buttonRepeatSpeed = 500;
unsigned long buttonRepeatTimer;
char currentKey;
char heldKey; 


class Abstract{
  public:
      byte lineIndex = 0;
      byte cursorIndex = 0;
      byte staticCursorIndex = 0;
      bool cursorMode = true;
      byte numberOfLines = 6;
      String lines[6];
      virtual void processButtonPressed(char keyPressed) = 0;
      virtual void processCursorMode(char keyPressed) = 0;
      byte determineLineSelection();
      virtual void execute(byte lineSelection) = 0;
      virtual void createScrollScreen(byte moveLines) = 0;
      virtual void drawScrollCursor() = 0;
      virtual void createStaticScreen() = 0;
      virtual void drawStaticCursor(byte x, byte y) = 0;
      Menu (){
        Serial.println("constructor Abstract");
      }
};
class Menu: public Abstract{
  public:
    byte lineIndex = 0;
    byte cursorIndex = 0;
    byte staticCursorIndex = 0;
    bool cursorMode = true;
    byte numberOfLines = 6;
    String lines[6] = {{"Jog"}, {"Division"}, {"Degree"}, {"Continuos"}, {"Program"}, {"Setup"}};
    void processButtonPressed(char keyPressed);
    void processCursorMode(char keyPressed);
    byte determineLineSelection();
    void execute(byte lineSelection);
    void createScrollScreen(byte moveLines);
    void drawScrollCursor();
    void createStaticScreen();
    void drawStaticCursor(byte x, byte y);
    Menu (){
      Serial.println("constructor Menu");
    }
};

class Division: public Abstract{
  public:
    void processButtonPressed(char keyPressed){

    }
    void processCursorMode(char keyPressed){

    }
    void createScrollScreen(byte moveLines){

    }
    void drawScrollCursor(){

    }
    void createStaticScreen(){
      lcd.clear();
      lcd.home();
      lcd.print("Number of Divisions:");  
      lcd.setCursor(0, 2);
      lcd.print( "F1>Main Menu");    
      lcd.setCursor(0, 3);
      lcd.print( "F3>ESC      F4>Enter");
      drawStaticCursor(staticCursorIndex, 1);
    }
    void drawStaticCursor(byte x, byte y){
      lcd.setCursor(x, y);
      lcd.cursor_on();
    }
    void execute(byte lineSelection){
      Serial.println("Division execute");
    }
    Division (){
      Serial.println("constructor Division");
      cursorMode = false;
    }

};

//Menu flags
bool mainMenu = false;
Abstract *currentMenu[5];
byte currentMenuIndex = 0;
//Keypad declarations
void runCurrentScreen(Menu&);
void keypadInput(Menu&);
void buttonPressed(char, Menu&);
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
  menu.createScrollScreen(0);
  currentMenu[0] = &menu;

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

void Menu::processButtonPressed(char keyPressed){
  static const char A[] = "F1";
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
}
void Menu::processCursorMode(char keyPressed){  
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
      if (cursorIndex == 4){
        cursorIndex = 3;
        createScrollScreen(1);
      }
      else {
        createScrollScreen(0);
      }
      break;
    case 'S': //Enter
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
      // Division division;
      // division.createStaticScreen();
      // currentMenu[currentMenuIndex + 1] = &division;
      // currentMenuIndex++;
      break;
    case 2:
      Serial.println("Degree");
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
void Menu::createStaticScreen(){

}
void Menu::drawStaticCursor(byte x, byte y){

}