#include <Arduino.h>
#include <avr/interrupt.h>

#define bit_is_false(x,mask) ((x & mask) == 0)

#define LED LED_BUILTIN

int pin_check;
int message;

void setup() {
	Serial.begin(9600);

	// Init pins as input
	DDRK = B00000000;
	DDRF = B00000000;

	// Init pins as pull-up
	PORTK = B11111111;
	PORTF = B11111111;

	// LED
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
}

void loop() {

	uint16_t pin_status = PINK << 8;
	pin_status += PINF;
	message = 0;

	// CHECK PIN STATUS
	for (int i= 0; i <16; i++){
		pin_check = 1<<i;
		int bit = bit_is_false(pin_status,pin_check);
		// message += bit << i;
		Serial.print(bit, BIN);
	}

	Serial.print('\n');
	// Serial.println(message, BIN);

	delay(100);

}