/*
    El & Sharms buot buot
    Project 2.2 - This is Not A Bomb

    Trigger a ticking countdown when a thing gets close enough to the sensor.

    Parts Required:
    - 1-digit 7-segment display
    - 8 220 ohm resistors
    - passive buzzer
    - HC-SR04 ultrasonic sensor

    created January 8, 2024
    by Sharmaigne Mabano
*/

#include "SevSeg.h" // library for 7-seg display manipulation

// create object
SevSeg sevseg;

//7 SEGMENT DISPLAY
byte numDigits = 1; // num of digits in display
byte digitPins[] = {}; // for more than 1 digit
byte segmentPins[] = {6,5,2,3,4,7,8,9};
bool resistorsOnSegments = true;
byte hardwareConfig = COMMON_CATHODE; //display type

// HC-SRO4 Ultrasonic Sensor
#define trigPin 10
#define echoPin 13

float duration, distance;
const float distanceThreshold = 70; // in cm (0-400)

// passive buzzer
#define buzzer 11
// const int note = 880;
const int note = 200; // switch to this when the 880 one gets annoying lol

void detonate();
void beep(uint8_t pin, int note, int sound, int mute);

void setup() {
    Serial.begin(9600);

    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    sevseg.setBrightness(50);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    tone(buzzer, 100, 100);
}

void loop() {
    // Write a pulse to the trigger pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the response from echo pin, calculate for distance

    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) * 0.0343; // 343m/s = speed of sound

    Serial.print("Distance: "); // NOTE: does anyone know unsaon nig combine, it's so ugly rn
    Serial.print(distance);
    Serial.println("cm");

    if (distance <= distanceThreshold) detonate();
    
    delay(500); // measure every half a second
}

// TO FIX: Buzzer and numbers are out of sync for some reason? 1 is not printing
void detonate() {
    // 7-segment countdown
    for (int digit = 9; digit > 0; digit--) {
        sevseg.setNumber(digit);

        if (digit > 3) beep(buzzer, note, 300, 700);
        else {
            for (int _ = 0; _ <= 5; _++)
                beep(buzzer, note, 100, 100);
        }

        sevseg.refreshDisplay();
    }
    sevseg.setNumber(0);
    tone(buzzer, note, 1000);
    sevseg.blank();
}

void beep(uint8_t pin, int note, int sound, int mute) {
    tone(pin, note);
    delay(sound);
    noTone(pin);
    delay(mute);
}