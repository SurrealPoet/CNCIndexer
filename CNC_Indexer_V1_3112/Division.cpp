#include "Division.h"

// extern STEPS_FOR_COMPLETE_CIRCLE;

Division::Division (byte _lcdCols, LiquidCrystal_I2C _lcd) 
  : lcdCols{_lcdCols}
  , lcd{_lcd}
{
  Serial.println("constructor Division");
  // this->rows = rows;
  // this->cols = cols;
  // this->lcdRows = lcdRows;
  // this->lcdCols = lcdCols;
  cursorMode = false;
}
Division::~Division (){
    Serial.println("destructor Division");
}
void Division::createStaticScreen(String lineInput, LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.home();
  lcd.print("Number of Divisions:");
  lcd.setCursor(0, 1);
  lcd.print(lineInput);
  lcd.setCursor(0, 2);
  lcd.print( "F1>Main Menu F2>Back");    
  lcd.setCursor(0, 3);
  lcd.print( "F3>Delete   F4>Enter");
  drawStaticCursor(staticCursorIndex, 1, lcd);
}
void Division::drawStaticCursor(byte x, byte y, LiquidCrystal_I2C lcd){
  lcd.setCursor(x, y);
  lcd.cursor_on();
}
void Division::execute(byte lineSelection){
// void Division::execute(byte lineSelection, byte LCD_COLS,LiquidCrystal_I2C lcd,Abstract *currentMenu){
  Serial.println("Division execute");
}
void Division::staticUpdate(char keyPressed){
  if (staticCursorIndex < lcdCols - 1){
    staticCursorIndex++;
    staticScreenInput += keyPressed;
  }
  createStaticScreen(staticScreenInput, lcd);
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
        createStaticScreen(staticScreenInput, lcd);
        break;
      case 'D': //F4-Enter
        Serial.println(D);
        Serial.println(stepsForCompleteCircle/staticScreenInput.toFloat());
        Serial.println(stepsForCompleteCircle/staticScreenInput.toInt());
        break;
      case 'S': //Start
      Serial.println(S);
      //Test at 11, 14 because int caste 17,999.9 to 17,999
      // temp += stepsForCompleteCircle/staticScreenInput.toFloat();
      // Serial.println(temp);
      // Serial.println(int(temp));
      counter++;
      // Serial.println(counter); 

      digitalWrite(SLEEP_PIN, HIGH);
      setDirection(0);
      for(unsigned int x = 0; x < 1000; x++)
      {
        step();
        delayMicroseconds(DELAY_BETWEEN_STEPS);
      }
      digitalWrite(SLEEP_PIN, LOW);
    }
  }    
}
void Division::processCursorMode(char keyPressed){
  Serial.println("CursorMode");
}