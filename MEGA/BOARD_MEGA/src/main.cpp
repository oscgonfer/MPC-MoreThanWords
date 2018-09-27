#include <Arduino.h>
#include <avr/interrupt.h>

#define bit_is_false(x,mask) ((x & mask) == 0)

#define LED LED_BUILTIN

// RESET BUTTONS
#define RESET_BUTTON_PIN	2
#define RESET_BUTTON_LED_PIN  3

float buttonPow = 0;

int pin_check;
int message;

enum STATUS {
	RESET,
	NORMAL
};

volatile STATUS state = NORMAL;

void interrupt_button() {
	state = RESET;
	digitalRead(RESET_BUTTON_PIN);
}

void FadeButton(float _buttonPow) {
	float buttonLight = sin(_buttonPow) * 127.5 + 127.5;
  	analogWrite(RESET_BUTTON_LED_PIN, buttonLight);
}

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

	// BUTTON LED
	pinMode(RESET_BUTTON_LED_PIN, OUTPUT);

	// // BUTTON ISR
	attachInterrupt(digitalPinToInterrupt(RESET_BUTTON_PIN), interrupt_button, LOW);
	digitalWrite(RESET_BUTTON_PIN, HIGH);
}

void loop() {

	if (state == NORMAL) {
		uint16_t pin_status = PINK << 8;
		pin_status += PINF;
		message = 0;

		// CHECK PIN STATUS
		for (int i= 0; i <16; i++){
			pin_check = 1<<i;
			int bit = bit_is_false(pin_status,pin_check);
			Serial.print(bit, BIN);
		}

		Serial.print('\n');

		delay(10);

	} else if (state == RESET) {

		for (int i=0;i<16;i++) {
			Serial.print(1,BIN);
		}
		Serial.print('\n');
		for (int i=0;i<16;i++) {
			Serial.print(1,BIN);
		}
		Serial.print('\n');

		state = NORMAL;
		delay(500);
	}

  	buttonPow += 0.015;
  	if (buttonPow >= 6.283) buttonPow = 0;
  	FadeButton(buttonPow);

}