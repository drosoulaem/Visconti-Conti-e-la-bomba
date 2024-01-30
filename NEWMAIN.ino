#include <Adafruit_LEDBackpack.h>
#include <Keypad.h>
#include <Key.h>

const int TOTAL_WIRES = 3; //total wires that can be cut 
const int CUTTABLE_WIRES[TOTAL_WIRES] = { A1, A2, A3}; //in which ports/pins are the wires
const bool WIRES_TO_CUT[TOTAL_WIRES] = { 0, 1, 0}; //if it's 0, it's the wrong wire and if it's 1, it's the correct wire 

//const unsigned long TOTAL_TIME = 3600 * 1000L;
//unsigned long remainingTime = TOTAL_TIME;
//unsigned long lastTimeUpdatedAt = 0;
bool wireStates[TOTAL_WIRES];

Adafruit_7segment led_display1 = Adafruit_7segment();

int minutes = 20; //how many minutes you want the timer to display 
int seconds = 00; //

int analogpin = A0; //this pin is the one the buttons are connected to 
//int buttonState = 0;        
//int lastButtonState = 0;

int buttonValue = 0; //this variable equals the value of a button when pressed

const int buzzer = 3; //explosion

const int ledPin = 6;  //when this led lights means a puzzle was completed
const int ledPiin = 4; //when this led lights means a puzzle was completed
const int leedPin = 5; //when this led lights means a puzzle was completed

bool buttons = false; //when this variable becomes true means the player pressed the right buttons
bool timerRunning = true;
bool wireall = false; //when this variable becomes true means the player cut the right wires

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},  
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {9, 8, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String password = "1852"; //password 
String input_password;
bool passwordCorrect = false; //when this variable becomes true means the player pressed the right password
void setup()
{  
 
  led_display1.begin();
//  pinMode(A1, INPUT);
  //pinMode(analogpin, INPUT); 

  pinMode(buzzer, OUTPUT); 
  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPiin, OUTPUT);
  pinMode(leedPin, OUTPUT);

  Serial.begin(9600);
  input_password.reserve(32); // maximum input characters is 33, change if needed

  for (int i = 0; i < TOTAL_WIRES; i++) {
    pinMode(CUTTABLE_WIRES[i], INPUT_PULLUP);
    wireStates[i] = LOW; // wires are initially tied to GND
  }

  Serial.println("Ready");

}

int detectWireStateChange() {
  for (int i = 0; i < TOTAL_WIRES; i++) {
    int newValue = digitalRead(CUTTABLE_WIRES[i]);
    if (newValue != wireStates[i]) {
      wireStates[i] = newValue;
      return i;
    }
  }
  return -1;
}


unsigned long previousMillis = 0; 
const long interval = 1000; 

void loop()
{
  unsigned long currentMillis = millis();
  

  buttonValue = analogRead(A0);  //Read in the button value
//Serial.println(buttonValue)  ;
 
// buttonState = analogRead(analogpin);


 
 
//if (buttonValue<=50){
  //  buttonstate = true;}
//else{ buttonstate = false;}
  
  if (timerRunning == true && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

if (buttonValue>=900 && buttonValue<=1023){  //is pressed - 2 lepta
 
 //if(buttonstate = false){ 
  minutes -=  2;
 //} 
    if (minutes < 0) {
      minutes = 0;
      seconds = 0;
    }
 }
 
  if (buttonValue>=380 && buttonValue<=600){
    digitalWrite(ledPin, HIGH);
    buttons = true;
  }
if (buttonValue>=100 && buttonValue<=240){
    minutes -=  1;
  }

    if (minutes >= 0) {
      //&& !passwordCorrect && !buttons
      char buffer[5];
      sprintf(buffer, "%02d%02d", minutes, seconds); //format as MMSS
      led_display1.println(buffer);
      led_display1.writeDisplay();
      
      if (seconds == 0 & minutes==0)
      {
        //explosion
        
        tone(buzzer, 1000); 
        delay(10000);  
        noTone(buzzer);     
        delay(100);    
      }
      
      if (seconds == 0)
      {
        seconds = 59;
        minutes--;
      }
      else
      {
        seconds--;
      }
      
    
    }
    //delay(50);

  }
   

  char key = keypad.getKey();

  if (key){
    Serial.println(key);

    if(key == '*') {
      input_password = ""; // clear input password
    } else if(key == '#') {
      if(password == input_password) {
        Serial.println("password is correct");
        digitalWrite(leedPin, HIGH);
        passwordCorrect = true;
     
      } else {
        Serial.println("password is incorrect, try again");
      }

      input_password = ""; // clear input password
    } else {
      input_password += key; // append new character to input password string
    }
  }

 int wireWithNewState = detectWireStateChange();
  if (wireWithNewState >= 0) {
    Serial.print("Wire ");
    Serial.print(wireWithNewState);
    if (wireStates[wireWithNewState]) {
      Serial.print(" was cut");
      if (WIRES_TO_CUT[wireWithNewState]) {
        Serial.println(" => correct");
        delay(100);
      } else {
        Serial.println(" => INCORRECT");
        seconds=00;
        minutes=00;
      }
    } else {
      Serial.println(" was reconnected");
    }
  }
 bool allCorrectWiresCut = true;
  for (int i = 0; i < TOTAL_WIRES; i++) {
    if (WIRES_TO_CUT[i] && !wireStates[i]) {
      allCorrectWiresCut = false; 
      break;
    }
  }

  if (allCorrectWiresCut) {
//    Serial.println("Congrats! All the correct wires have been cut.");
    wireall = true;
   digitalWrite(ledPiin, HIGH);
  }
if(wireall == true && passwordCorrect == true && buttons== true){
 timerRunning = false;  
 //Serial.println("it works");
 Serial.end();
  
}
}





