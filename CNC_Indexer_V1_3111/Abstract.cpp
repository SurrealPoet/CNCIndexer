#include "Abstract.h"

// byte Abstract::determineLineSelection() {
//   byte lineSelection = lineIndex + cursorIndex;
//   if(lineSelection >= numberOfLines && lineSelection < 200){
//     lineSelection -= numberOfLines;
//   }
//   return lineSelection;
// }
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