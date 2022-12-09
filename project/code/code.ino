#include "pitches.h" //library to play melidies with piezo
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

int melody[] = {  //simple melody on piezo when the game when 1 person wins
// created by https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4  
};

int melody_twowinners[] = { //game of thrones opening on piezo when 2 people win
//created by Robson Couto 
//https://github.com/robsoncouto/arduino-songs/tree/master/gameofthrones

  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, //1
  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,-4, NOTE_C4,-4,//5
  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16, //6
  NOTE_D4,4, //7 and 8
};

int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};

// assign LEDs and button to pins
int ledPin1[] = {4,5,6}; //red
int ledPin2[] = {7,8,10}; //green
int ledPin_current1 = ledPin1[0];
int ledPin_current2 = ledPin2[0];
int playerOneButton = 2; //pin for player1
int playerTwoButton = 3; //pin for player2
int blueLED1 = 9; //pin for blueLED1
int blueLED2 = 12; //pin for blueLED1
const int buzzer = 11; //pin for piezo

// declare variables
int delayTime=400; // time delay between lights on/off
int randNumber;
int randNumber2;
int counter1=0; //count of points for player 1
int counter2=0; //count of points for player 2

//setup interrupt, button input and LED outputs
void setup() {
  //initializing
  myservo.attach(13);

  for (int i=0; i<3; i++){
    pinMode(ledPin1[i], OUTPUT);
  }
  for (int i=0; i<3; i++){
    pinMode(ledPin2[i], OUTPUT);
  }
  pinMode(playerOneButton, INPUT);
  pinMode(playerTwoButton, INPUT);
  pinMode(blueLED1, OUTPUT);
  pinMode(blueLED2, OUTPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
}

//run main program loop
void loop() {

  attachInterrupt(0, playerOneInput, FALLING); // interrupt routine player1
  attachInterrupt(1, playerTwoInput, FALLING); // interrupt routine player2
  
  victory(); //checks if one or both players win

  randNumber = random(3); // select a random number
  ledPin_current1 = ledPin1[randNumber];
  digitalWrite(ledPin1[randNumber], HIGH); // light the LED with this number
  delay(rand() % 500 + 100); //absolutely random duration for moles
  digitalWrite(ledPin1[randNumber], LOW);
  delay(rand() % 500 + 100);
  
  randNumber2 = random(3); // select a random number
  ledPin_current2 = ledPin2[randNumber2];
  digitalWrite(ledPin2[randNumber2], HIGH); // light the LED with this number
  delay(rand() % 500 + 100);
  digitalWrite(ledPin2[randNumber2], LOW);
  delay(rand() % 500 + 100);

  //functions below turn off blue lights if they are on
  checkHigh(blueLED1); 
  checkHigh(blueLED2);
}


void playerOneInput() {
  if(digitalRead(ledPin_current1) == HIGH){
    digitalWrite(blueLED1, HIGH);
    counter1++;
    tone(buzzer, 1000, 100); //sound output (for correct button presses)
  }
}

void playerTwoInput() {
  if(digitalRead(ledPin_current2) == HIGH){
    digitalWrite(blueLED2, HIGH);
    counter2++;
    tone(buzzer, 1000, 100); //sound output (for correct button presses)
  }
}

void checkHigh(int blueLED) //turns off blue LEDS at the end of the loop()
{
  if(digitalRead(blueLED) == HIGH){
    digitalWrite(blueLED, LOW);}
}

void victory() //check who wins
{
  if(counter1 == 10 && counter2 ==10)
  {
    for (int i=0; i<3; i++){
    digitalWrite(ledPin1[i], HIGH);
    }
    for (int i=0; i<3; i++){
    digitalWrite(ledPin2[i], HIGH);
    }
    two_winners(); //game of thrones opening plays
    delay(delayTime+300);
    for (int i=0; i<3; i++){
    digitalWrite(ledPin1[i], LOW);
    }
    for (int i=0; i<3; i++){
    digitalWrite(ledPin2[i], LOW);
    }
  }
  else if(counter1 == 10)
  {
    one_winner(ledPin1);
  }
  else if(counter2 == 10)
  {
    one_winner(ledPin2);
  }

  //condition below sets counters to 0 and spins a servo

  if(counter1==10 || counter2==10)
  {
    counter1=0;
    counter2=0;
    myservo.write(0);
    delay(1000);
    myservo.write(180);
    delay(1300);
  }
}

void one_winner(int ledPin[])
{
  for (int i=0; i<3; i++){
  digitalWrite(ledPin[i], HIGH);
  }
  win_music(); //simple melody
  delay(delayTime+300);
  for (int i=0; i<3; i++){
  digitalWrite(ledPin[i], LOW);
  }
}

void win_music() //created by https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody
{
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / noteDurations[thisNote];

    tone(11, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    // stop the tone playing:

    noTone(11);

  }
}

void two_winners() //created by Robson Couto
{
  int tempo = 85;
  int notes = sizeof(melody_twowinners) / sizeof(melody_twowinners[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody_twowinners[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody_twowinners[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

