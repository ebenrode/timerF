#include <Encoder.h>
#include <TM1637Display.h>
#include <Arduino.h>




//пины энposition
int CLK_PIN = 2;
const int DT_PIN = 4;
const int buttonPin = 3;

const int DINMIK_PIN = 9;

// Module connection pins (Digital Pins) экран
#define CLK 6
#define DIO 5

int buttonState = 0;
unsigned long currentTime;
unsigned long loopTime;


long oldPosition, newPosition2  = -999; //установка значения переменной для хранения текущей позиции
long oldP, newP = 0;

int time2 = 8000;
int clVrem = 0;
//создание объекта типа Экнеодер
Encoder myEnc(DT_PIN, CLK_PIN);
TM1637Display display(CLK, DIO);


void setup() {


  Serial.begin(9600); //инициализация Serial-порта
  display.setBrightness(0x0f);



  pinMode(buttonPin, INPUT);



}

int displ(int dis, int toch) {
  int minu = (dis / 60) ;
  int sec = (dis % 60);
  String min2 = String(minu);
  String sec2 = String(sec);
  if (sec2.length() == 1) {
    sec2 = "0" + sec2;
  }
  String res = min2 + sec2;

  long res2 = res.toInt();

  display.showNumberDecEx( res2, 0b01000000);

  return;


}

int PROG2(int tim2) {

  while (time2 >= 1 ) {

currentTime = millis();                           // считываем время, прошедшее с момента запуска программы
  if(currentTime >= (loopTime + 1000)){              // сравниваем текущий таймер с переменной loopTime + 1 секунда
         
    loopTime = currentTime;                         // в loopTime записываем новое значение
 
  

  
    time2--;
    //Serial.println(tim2);

    displ(time2, 1);

    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
      //tone(DINMIK_PIN, 800, 1000);
      // tim = tim2;
      //Serial.println("strt");
      delay(800); PROG(time2);  break;
    }

  }
  }
  tone(DINMIK_PIN, 1000, 300);
  delay(500);
  tone(DINMIK_PIN, 1000, 300);

  tone(DINMIK_PIN, 1000, 300);
  delay(300);

}

int PROG(int tim3) {


  long newPosition = myEnc.read(); //считать новую позицию

  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    newPosition2 = newPosition ;
    clVrem = clVrem + 1;


    if (clVrem == 4) {

      clVrem = 0;



      newP = newPosition;
      if (newP > oldP) {
        if  (time2 > 60) {
          time2 = time2 - 60;
        } else
        {
          time2 = time2 - 1;
        }

        oldP = newP;
      };
      if (newP < oldP) {
        oldP = newP;
        time2 = time2 + 60;
      };


      if ( time2 >= 11999) {
        time2 = 11999;
      };

      if ( time2 < 0) {
        time2 = 0;
      };

      tone(DINMIK_PIN, 500 + time2 * 8, 20);
      clVrem = 0;
      displ(time2, 1);

    }
  }

  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    tone(DINMIK_PIN, 800, 300);
    delay(800);
    PROG2(time2 );
  }

}

void loop() {

  Serial.println(time2);

  displ(time2, 1);
  while (1)
  {

    PROG(time2);


  } ;


}
