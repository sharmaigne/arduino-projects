/*
  El & Sharms buot buot
  Project 1.2 - Morse Code (Light and Sound)

  Prompts for a message through the serial monitor and outputs it in morse.

  Parts Required:
  - 220 ohm resistor
  - led light
  - passive buzzer

  created January 7, 2024
  by Sharmaigne Mabano
*/


int led = 8;
int buzzer = 9; // analog output

int dot = 150;  // ms for a dot. basis for the rest of the times
int dash = dot*3;

int within = dot;
int between = dash;
int words = dot*7;

int note;
int dotNote = 400;
int dashNote = 500;

// TIDBIT: Arduino UNO has a 32KB limit, so I think hashmaps are generally avoided? 
char* letters[] = { 
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // A-I
".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R 
"...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // S-Z
};

char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....",
"-....", "--...", "---..", "----."
};

void flashSequence(char* sequence);

void setup() {
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    tone(buzzer, 700, 2000);
}

void loop() {
    Serial.print("\nPrint your message: ");
    while(!Serial.available()) {} // empty while loop to wait for input lol
    String input = Serial.readString();

    input.toLowerCase();
    Serial.println(input);

    for (char ch : input) {
        Serial.print(ch);
        Serial.print(": "); // hack bc idk how to f strings yet and + doesn't work
        if (ch >= 'a' && ch <= 'z') flashSequence(letters[ch - 'a']);
        else if (ch >= '0' && ch <= '9') flashSequence(numbers[ch - '0']);
        else if (ch == ' ') delay(words);
    }

}

// outputs the morse of one character
// @param sequence: morse equivalent of a character. eg "-." for 'a'
void flashSequence(char* sequence) {
  int i = 0;
  while (sequence[i]) {
    digitalWrite(led, HIGH); // turn on LED
    int d = (sequence[i] == '.') ? dot : dash;
    Serial.print(sequence[i]);
    note = (sequence[i] == '.') ? dotNote : dashNote;
    tone(buzzer, note); // turn on buzzer
    delay(d);
    digitalWrite(led, LOW); // turn off the LED
    noTone(buzzer); // mute the buzzer
    delay(within);
    i++;
  }
  
  delay(between);
  Serial.println();
}