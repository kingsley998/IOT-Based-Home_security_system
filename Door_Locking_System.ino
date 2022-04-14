
//importing important libraries
#include<Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
#include <Servo.h> // the library to control the servo motor
Servo servo;
char incomingByte; 
String inputString;
#include <DFRobot_SIM808.h>
#include <SoftwareSerial.h>
#define PHONE_NUMBER "0996937551"  
 
//The content of messages sent
#define MESSAGE1  "security alert: door opened"
#define MESSAGE2  "security alert: door closed"
#define MESSAGE3  "security alert: someone entered the wrong password and the system turned on alarm automatically"

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

char phone[16];
char datetime[24];
#define PIN_TX    11
#define PIN_RX    12
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);

int Position = 0; // position of the servo
const byte ROWS = 4; // the amount of rows on the keypad
const byte COLS = 4; // the amount of columns on the keypad
char keys[ROWS][COLS]={
  {'1','2','3','A'}, 

  {'7','8','9','C'},

  {'4','5','6','B'},

  {'0','*','#','D'}
};
 
byte rowPin[ROWS]={9,8,7,6};
byte colPin[COLS]={5,4,3,2};
byte Locked[8] = { 
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11011,
  B11111
};
// open character
byte Opened[8] = {
  B01110,
  B00001,
  B00001,
  B11111,
  B11011,
  B11011,
  B11011,
  B11111
};

String password = "1234";  // The Pin Code.
int position = 0;

int wrong = 0; // Variable to calculate the wrong inputs.

//int redPin = 10;
//int greenPin = 11;
int buzzer = 13;
 Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,ROWS,COLS);
//Keypad myKeypad( makeKeymap(keys), rowPin, colPin, ROWS, COLS);
// MAPPING THE KEYPAD.


int total = 0; // Variable to determine the number of wrong attempts.

void setup()
{
  pinMode(13, OUTPUT);
 //pinMode(redPin,OUTPUT);
 //pinMode(greenPin,OUTPUT);
 //pinMode(buzzer, OUTPUT);
 mySerial.begin(9600);
  Serial.begin(9600);
  //pinMode(buzzer, OUTPUT);
  servo.attach(10); // attaches the servo to pin 10
  ServoClose(); 
  
  while(!mySerial.available()){
        mySerial.println("AT");
        delay(1000); 
        Serial.println("Connecting...");
        }
      Serial.println("Connected!");  
      mySerial.println("AT+CMGF=1");  //Set SMS to Text Mode 
      delay(1000);  
      mySerial.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
      delay(1000);
      mySerial.println("AT+CMGL=\"REC UNREAD\"");


  
  lcd.init(); // initializes the lcd 
  lcd.backlight(); // turns on the backlight
  lcd.setCursor(0,0); // sets the cursor on the lcd
  lcd.print("PASSWORD DOOR"); // prints the text/charater
  lcd.setCursor(0,1); // sets the cursor on the lcd
  lcd.print("LOCKING SYSTEM"); // prints text
  delay(4000); // waits 4 seconds
  lcd.clear(); // clears the lcd diplay

setLocked(true);
delay(20);
}

void loop()
{

if(mySerial.available()){
      delay(100);

      // Serial Buffer
      while(mySerial.available()){
        incomingByte = mySerial.read();
        inputString += incomingByte; 
        }

        delay(10);      

        Serial.println(inputString);
        inputString.toUpperCase(); // Uppercase the Received Message

        //turn servo ON or OFF
        if (inputString.indexOf("DOOR LOCK") > -1){      // Send sms for door lock
          lcd.print("Door Closed");
          ServoClose();
          }
            
           if (inputString.indexOf("DOOR UNLOCK") > -1){     //send sms for door unlock
           lcd.print("Door Opened");
           ServoOpen();
          }
        
        delay(50);

        //Delete Messages & Save Memory
        if (inputString.indexOf("OK") == -1){
        mySerial.println("AT+CMGDA=\"DEL ALL\"");

        delay(1000);}

        inputString = "";
  }


  
  char c;
  lcd.clear();
  lcd.print(" Enter Password: ");
  delay(100);
  
 char pressed=keypad.getKey();
 String key[4];
  
 if(pressed)
 {
  
  lcd.clear();
  lcd.print(" Enter Password: ");
  lcd.setCursor(position,1);
  lcd.print(pressed);
  delay(500);
    if(pressed == '*' || pressed == '#')
      {
          position = 0;
          setLocked(true);
          lcd.clear();
      }

    else if(pressed == password[position])
      {
          key[position]=pressed;
          position++;
      }
 
    else if (pressed != password[position] )
      {
          wrong++;
          position ++;
      }

    if(position == 4){
          if( wrong >0)
            {
                total++;
                wrong = 0;
                position = 0;
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print(" Wrong Password!     ");
                lcd.setCursor(0,1);
      lcd.print("Try Again In");
      lcd.setCursor(13,1);
      lcd.print("10");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("09");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("08");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("07");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("06");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("05");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("04");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("03");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("02");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("01");
      //tone(13,1000,2500);
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("00");
      delay(1000);
      lcd.clear();
                delay(1000);
                setLocked(true);
                //readsms();
            }

          else if(position == 4 && wrong == 0)
            {
                position = 0;
                wrong = 0;
                lcd.clear();
                //lcd.print(p
                lcd.setCursor(0,0);
                lcd.print(" Welcome!     ");
                lcd.setCursor(0,1);
                lcd.print(" Door Open");
                lcd.createChar(1, Opened);
                lcd.setCursor(14,0); // sets the cursor on the lcd
                lcd.write(1);
                delay(2000);
                send_sms();
                setLocked(false);
                delay(9000);
                setLocked(true);
            }

             if(total ==3)
            {
                total=0;
                send_sms3();
                buzzer_beep();
                delay(9000);
            }

        }

   }

   
}

void setLocked(int locked)
  {
    if (locked)
      {
          ServoClose();
          delay(1000);
      }
    else
      {
          ServoOpen();
          delay(2000);
      }
  }
void buzzer_beep()
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(" WARNING  !!!!   ");
   lcd.setCursor(0,1);
   lcd.print("   Access Denied");
   
   for (int i=0;i<10;i++){
   tone(13,1000,9000);
   tone(13,1000,9000);
   tone(13,1000,9000);
   tone(13,1000,9000);
   tone(13,1000,9000);
   tone(13,1000,9000);
   }
}
void ServoOpen() // opens the servo
{
  for (Position = 180; Position >= 0; Position -= 5) { // moves from 0 to 180 degrees
    servo.write(Position); // moves to the postion
    delay(15); // waits 15 milliseconds
  }
}
void ServoClose() // closes the servo
{
  for (Position = 0; Position <= 180; Position += 5) { // moves from position 0 to 180 degrees
    servo.write(Position); // moves to the position
    delay(15); // waits 15 milliseconds
  }
}
void send_sms(){
   while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }  
  Serial.println("Sim808 init success");
  Serial.println("Start to send message ...");

  //******** define phone number and text **********
  sim808.sendSMS(PHONE_NUMBER,MESSAGE1); 
}
void send_sms2(){
  while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }  
  Serial.println("Sim808 init success");
  Serial.println("Start to send message ...");

  //******** define phone number and text **********
  sim808.sendSMS(PHONE_NUMBER,MESSAGE2);
}
void send_sms3(){
  while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }  
  Serial.println("Sim808 init success");
  Serial.println("Start to send message ...");

  //******** define phone number and text **********
  sim808.sendSMS(PHONE_NUMBER,MESSAGE3);
}



