#ifndef CNCINDEXERVARIABLES_H
#define CNCINDEXERVARIABLES_H

#define ROWS 4
#define COLS 4
#define LCD_ROWS 4
#define LCD_COLS 20
#define BUTTON_REPEAT_SPEED 400
#define BUTTON_DELAY_TIME 250
//Motor driver 
#define DIR_PIN 2
#define STEP_PIN 3
#define CS_PIN 4
#define SLEEP_PIN 5
#define DELAY_BETWEEN_STEPS 2000
#define STEPPER_MOTOR_STEPS_RATIO 200
#define ROTARY_TABLE_RATIO 90
// #define STEPS_FOR_COMPLETE_CIRCLE STEPPER_MOTOR_STEPS_RATIO * ROTARY_TABLE_RATIO //18,000
#define stepsForCompleteCircle STEPPER_MOTOR_STEPS_RATIO * ROTARY_TABLE_RATIO //18,000
//Lcd
#define LCD_ADDRESS 0x27

#endif