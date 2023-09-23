#include "Menu.h"
#include "Division.h"

Menu::Menu (byte rows, byte cols, byte lcdRows, byte lcdCols, LiquidCrystal_I2C &lcd) 
  : rows{rows}
  , cols{cols}
  , lcdRows{lcdRows}
  , lcdCols{lcdCols}
  , lcd{lcd}
{
  Serial.println("constructor Menu");
}
Menu::~Menu () {
    Serial.println("destructor Menu");
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
        // mainMenu = true;
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
      if (cursorIndex == lcdRows){
        cursorIndex = lcdRows - 1;
        createScrollScreen(1);
      }
      else {
        createScrollScreen(0);
      }
      break;
    case 'A': //F1-Main Menu
        Serial.println(A);
        // mainMenu = true;
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
        // Division *division = new Division();
        // currentMenuIndex++;
        // currentMenu[currentMenuIndex] = division;
        // division->createStaticScreen(staticScreenInput);
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
    // lcd.print(lines[tempIndex]);
    lcd.print("menu test");
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