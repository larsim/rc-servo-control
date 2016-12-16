//---------- Servo Driver ----------------------------------------------
// Written By:  Paal Alexander & Lars Ivar
// Date:        TUE 28/10-2008 / FRI 20/04-2012
// Written for: ATMega168 / ATMega16
// Language:    AVR-LIBC
//
// Modified 2012: Enabled separate ports
//
//---------- Include ---------------------------------------------------
#include "globals.h"
#include "servo.h"
//---------- Define ----------------------------------------------------
 
#define TIME_1MS 0x0100
#define TIME_20MS 0x1387
#define MIDDLE 0x0177
 
// ------ EDIT HERE
#define SERVO_COUNT 4
const uint8_t chPin[SERVO_COUNT]      = {PD7,    PD6,    PD4,    PB4};
volatile uint8_t *chPort[SERVO_COUNT] = {&PORTD, &PORTD, &PORTD, &PORTB};
volatile uint8_t *chDir[SERVO_COUNT]  = {&DDRD,  &DDRD,  &DDRD,  &DDRB};
volatile uint16_t chTime[SERVO_COUNT] = {MIDDLE, MIDDLE, MIDDLE, MIDDLE};
// ------
 
volatile uint8_t currentCh = 0;
volatile uint16_t totalTime = 0;
 
volatile uint8_t timer = 0;
 
//---------- Interrupt Routine ------------------------------------------
ISR(TIMER1_COMPA_vect) 
{
    if(currentCh <= SERVO_COUNT) 
    {
        *(chPort[currentCh-1]) &= ~(1<<chPin[currentCh-1]);
        OCR1A = (TIME_20MS - totalTime);
        currentCh = 0;
        totalTime = 0;
 
        // Call tortoise stuff 0-1-2-3
        if(++timer>1) 
        {
            (*every40ms)();
            timer = 0;
        }
    }
    else
    {
        *(chPort[currentCh]) |= (1<<chPin[currentCh]);
        if(currentCh > 0) 
            *(chPort[currentCh-1]) &= ~(1<<chPin[currentCh-1]);
        OCR1A = chTime[currentCh];
        totalTime += chTime[currentCh];
        currentCh++;
    }
}
 
//---------- Functions -------------------------------------------------
void servo_init()
{
    // Set output direction for all servo pins
    uint8_t ch;
    for(ch=0; ch<SERVO_COUNT; ch++)
    {
        *(chDir[ch]) |= (1<<chPin[ch]);
    }
 
    currentCh = 0;
 
    // Set up timer
 
    TCCR1A = 0;
 
    // WGM12 ctc Clear Timer on Compare match
    // CS11 CS10 gives prescaler of 64, so 1ms = 125 in OCR1A for 16MHz
    // That means we have 125 steps for 90 degrees of servo movement
    TCCR1B = (1<<WGM12)|(1<<CS11)|(1<<CS10);                          
 
    // Enable Output Compare Register 1A
    // Add OCIE1A interrupt vector enable    
    TIMSK1 = (1<<OCIE1A);
    OCR1A = 1;
}
 
void servo_set(uint8_t channel, uint8_t pos)
{
    // If the chosen channel is invalid, do nothing
    if(channel > SERVO_COUNT) return;  
 
    chTime[channel] = (uint16_t)pos|TIME_1MS;
}
 
uint8_t servo_get(uint8_t channel)
{
    // If the chosen channel is invalid, return nothing
    if(channel > SERVO_COUNT) return 0;
 
    return (chTime[channel] & ~TIME_1MS);
}
