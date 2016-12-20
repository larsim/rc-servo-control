# rc-servo-control
Here I will present a servo control code I wrote to run on most AtMega AVRs.

I wanted to write a servo code that used any port and any pin, any timer and even leaves the timer ISRs open to be used for other things. This would allow me to simplify the electronics layout for my project.

= Timers on AtMega16U4 =

The AtMega16U4 comes with four timers
* 8-bit timer (TIMER0)
* 2x 16-bit timers (TIMER1 & TIMER3)
* 10-bit timer (TIMER4)

The 10-bit timer is special for the usb-core models, and can run up to 64Mhz.
Using the code

The servo code consists of a set, get and init function. It is limited to a maximum of 8 servos per timer. The servo_set function takes an unsigned char as input, where 0 gives a 1ms pulse and 255 gives approx 2ms.

= Configuration =

Each servo can be set up on any pin of any port, by editing the following part of the servo.c file:
<code>
// ------ EDIT HERE
#define SERVO_COUNT 3
const uint8_t chPin[SERVO_COUNT]      = {PD7,    PC0,    PB4};
volatile uint8_t *chPort[SERVO_COUNT] = {&PORTD, &PORTC, &PORTB};
volatile uint8_t *chDir[SERVO_COUNT]  = {&DDRD,  &DDRC,  &DDRB};
volatile uint16_t chTime[SERVO_COUNT] = {MIDDLE, MIDDLE, MIDDLE};
</code>
	
This example configuration sets up three servos, on port D, C and B. They all start at the servos middle position. The code also calls a function pointer every 40ms, which can be used for anything. Point like this:

every40ms = &myFunction;

The code is set up to use a 16MHz crystal, so to use it for other frequencies the prescalers must be changed.
