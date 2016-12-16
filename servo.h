#ifndef SERVO_H
#define SERVO_H
 
void servo_init();
void servo_set(uint8_t channel, uint8_t pos);
uint8_t servo_get(uint8_t channel);
 
void (*every40ms)();
 
#endif // SERVO_H
