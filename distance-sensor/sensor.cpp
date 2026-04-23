#include <Arduino.h>
#define pinDistEcho 2 // yellow
#define pinDistTrigger 4 // orange

void sendPulse();
void readDistance();

volatile unsigned long pulseStart = 0;
volatile float distance = 0;
volatile int mode = -1; // -1 = idle, 0 = sending pulse, 1 = waiting for echo


void setup () {
    // Start serial communication and set pin modes
    Serial.begin(9600);

    // Set pin modes for the distance sensor
    pinMode(pinDistEcho, INPUT);
    pinMode(pinDistTrigger, OUTPUT);

    // Ensure the trigger pin is low to start with
    digitalWrite(pinDistTrigger, LOW);
    
    // Attach an interrupt to the echo pin to read distance when the signal changes
    attachInterrupt(digitalPinToInterrupt(pinDistEcho), readDistance, CHANGE);
}

void loop() {
    sendPulse();
    delay(25);

    Serial.println(" Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}


void sendPulse() { 
    digitalWrite(pinDistTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinDistTrigger, LOW);

    mode = 0; // Set mode to sending pulse
}

void readDistance() {
    switch (mode)
    {
    case 0:
        pulseStart = micros(); // Record the time when we sent the pulse
        mode = 1;
        break;
    case 1:
        distance = (float)(micros() - pulseStart) / 58.3; // Calculate distance based on the time it took for the echo to return
        pulseStart = 0;
        mode = -1;
        break;
    default:
        break;
    }
}