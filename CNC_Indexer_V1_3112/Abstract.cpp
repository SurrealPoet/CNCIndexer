#include "Abstract.h"

extern bool mainMenu;
extern Abstract *currentMenu[];
extern byte currentMenuIndex;
extern LiquidCrystal_I2C lcd;

byte Abstract::determineLineSelection() {
  byte lineSelection = lineIndex + cursorIndex;
  if(lineSelection >= numberOfLines && lineSelection < 200){
    lineSelection -= numberOfLines;
  }
  return lineSelection;
}
byte Abstract::checkIndexInBounds(byte index, byte numberOfLines){
  byte tempIndex = index;
  if(tempIndex == 255){
    tempIndex = numberOfLines - 1;
  }
  if(tempIndex >= numberOfLines){
    tempIndex = 0;
  }
  return tempIndex;
}

void Abstract::goBackOneScreen(){
  Serial.print("currentMenuIndex");
  Serial.println(currentMenuIndex);
  delete currentMenu[currentMenuIndex];
  currentMenuIndex--;
  if(currentMenu[currentMenuIndex]->cursorMode == true)
    currentMenu[currentMenuIndex]->createScrollScreen(0);
  else {
    currentMenu[currentMenuIndex]->createStaticScreen(currentMenu[currentMenuIndex]->staticScreenInput, lcd);
  }
}

void Abstract::goToMainMenu(){
  lcd.cursor_off();
  Serial.print("Outside: currentMenuIndex: ");
  Serial.println(currentMenuIndex);
  for (byte i = 1; i <= currentMenuIndex; i++){
    Serial.print("currentMenuIndex: ");
    Serial.println(currentMenuIndex);
    delete currentMenu[i];
  }
  currentMenuIndex = 0;
  mainMenu = true;
}

// Sends a pulse on the STEP pin to tell the driver to take one step, and also
//delays to control the speed of the motor.
void Abstract::step()
{
  // The STEP minimum high pulse width is 1.9 microseconds.
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(3); //3
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(3); //3
}

// Writes a high or low value to the direction pin to specify what direction to
// turn the motor.
void Abstract::setDirection(bool dir)
{
  // The STEP pin must not change for at least 200 nanoseconds before and after
  // changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(DIR_PIN, dir);
  delayMicroseconds(1);
}

// byte Abstract::getLineIndex() {
//  return lineIndex;
// }
// void Abstract::setLineIndex(byte lineIndex) {

// }
// byte Abstract::getCursorIndex() {

// }
// void Abstract::setCursorIndex(byte cursorIndex) {

// }
// byte Abstract::getStaticCursorIndex() {

// }
// void Abstract::setStaticCursorIndex(byte staticCursorIndex) {

// }
// String Abstract::getStaticScreenInput() {

// }
// void Abstract::setStaticScreenInput(String staticScreenInput) {

// }
// bool Abstract::getCursorMode() {

// }
// void Abstract::setCursorMode(bool cursorMode) {

// }
// byte Abstract::getNumberOfLines() {

// }
// void Abstract::setNumberOfLines(byte numberOfLines) {

// }