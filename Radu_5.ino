#include <Servo.h>
#include <SoftwareSerial.h>
//#define bluetooth Serial1
int En_dreapta = 3;
int In1_dreapta = 2;
int In2_dreapta = 4;
int En_stanga = 5;
int In1_stanga = 6;
int In2_stanga = 7;
int Trig1 = 13;
int Echo1 = 12;
int Trig2 = 11;
int Echo2 = 10;

int var=2;
char data;
int acceleratie;
const int pingPin = 8;
long duration, inches, cm;
int obj = 1;

Servo servo;  

void Inainte(){
digitalWrite(In1_dreapta, HIGH);
digitalWrite(In2_dreapta, LOW);  
analogWrite(En_dreapta, 130);
digitalWrite(In1_stanga, HIGH);
digitalWrite(In2_stanga, LOW);  
analogWrite(En_stanga, 130);
}

void Dreapta(){
digitalWrite(In1_dreapta, LOW);
digitalWrite(In2_dreapta, LOW);  
analogWrite(En_dreapta, 0);
digitalWrite(In1_stanga, HIGH);
digitalWrite(In2_stanga, LOW);
analogWrite(En_stanga, 130);
}

void Dreapta_Spate(){
digitalWrite(In1_dreapta, LOW);
digitalWrite(In2_dreapta, HIGH);  
analogWrite(En_dreapta, 130);
digitalWrite(In1_stanga, LOW);
digitalWrite(In2_stanga, LOW);
analogWrite(En_stanga, 0);
}

void Stanga(){
digitalWrite(In1_dreapta, HIGH);
digitalWrite(In2_dreapta, LOW);  
analogWrite(En_dreapta, 130);
digitalWrite(In1_stanga, LOW);
digitalWrite(In2_stanga, LOW);
analogWrite(En_stanga, 0);
}

void Stop(){
digitalWrite(In1_dreapta, LOW);
digitalWrite(In2_dreapta, LOW);  
analogWrite(En_dreapta, 0);
digitalWrite(In1_stanga, LOW);
digitalWrite(In2_stanga, LOW);
analogWrite(En_stanga, 0);
}

void Inapoi(){
digitalWrite(In1_dreapta, LOW);
digitalWrite(In2_dreapta, HIGH);  
analogWrite(En_dreapta, 130);
digitalWrite(In1_stanga, LOW);
digitalWrite(In2_stanga, HIGH);  
analogWrite(En_stanga, 130);
}

int Distanta()
{ 
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
   delay(100);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void Parcare(){
 long durata_parcare1, durata_parcare2;
 int distanta_parcare1, distanta_parcare2;
 
  digitalWrite(Trig1, LOW);  //semnal de trigger 0 logic
  delayMicroseconds(5); // Intarziere 5 microsecunde
  digitalWrite(Trig1, HIGH); //semnal de trigger 1 logic
  delayMicroseconds(10); 
  digitalWrite(Trig1, LOW);
  durata_parcare1 = pulseIn(Echo1, HIGH);
  distanta_parcare1 = durata_parcare1 *0.034 /2; 
  Serial.print("Distanta1 = ");
  Serial.println(distanta_parcare1);
  
  digitalWrite(Trig2, LOW);  //semnal de trigger 0 logic
  delayMicroseconds(5); // Intarziere 5 microsecunde
  digitalWrite(Trig2, HIGH); //semnal de trigger 1 logic
  delayMicroseconds(10); 
  digitalWrite(Trig2, LOW);
  durata_parcare2 = pulseIn(Echo2, HIGH);
  distanta_parcare2 = durata_parcare2 *0.034 /2; 
  Serial.print("Distanta2 = ");
  Serial.println(distanta_parcare2);
  Serial.println();
 // delay(50);

if((distanta_parcare1>50)&&(distanta_parcare2>50)){
  Stop();
  delay(500);
  Inainte();
  delay(1000);
  Dreapta_Spate();
  delay(1000);
  Inapoi();
  delay(1000);
  Stop();
  delay(5000);
}
  else
  Inainte();  
}

void Autonomie(){
  servo.write(-88);
  Distanta();
  if (cm >= 20) {
  Serial.print(cm);
  Serial.print("cm");
    Serial.println();
    Serial.println("GOOD");
    Inainte();
    delay(1);
  } else
{
 Serial.print(cm);
 Serial.print("cm");
 Serial.println();
 Serial.println("DANGER"); 
   switch (obj) {
        case 1:
       Stop();
          delay(1);
          servo.write(60); //stanga
          delay(1000);
          Inapoi();
          delay(2000);  
          Distanta();
          if (cm > 10 ){
          Stanga();     
          delay(1500);
          servo.write(20);  //fata
          obj = 1;
          }
          if (cm < 10) {obj  = 2; }   
          break;
        case 2:
          Stop();
          delay(1);
          servo.write(-88);  //dreapta
          delay(1000);
          Distanta();
          if (cm > 10) {
            Dreapta();
            delay(1500);
            servo.write(20);  //fata
            obj = 1;
          }
          if (cm < 10) { 
          obj = 3;}
          break;
        case 3:
          Stop();
          delay(1);
          servo.write(20);  //fata
          Inapoi();
          delay(2000);
          Stanga();
          delay(1500);
          obj = 1;
          break;
          default : 
          break;
      }
    }
 }

void setup() {
 Serial.begin(9600);
 //bluetooth.begin(9600);
 servo.attach(9);
    
  pinMode(En_dreapta, OUTPUT);
  pinMode(En_stanga, OUTPUT);
  pinMode(In1_dreapta, OUTPUT);
  pinMode(In2_dreapta, OUTPUT);
  pinMode(In1_stanga, OUTPUT);
  pinMode(In2_stanga, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
}

void loop() {
// Parcare(); 
if(Serial.available() > 0)  // Trimite informatie doar cand primesti informatie
  {
    data = Serial.read();      // Citim informatia primita si o stocam intr-o varibila
 //   bluetooth.println(data);        //  Afisam valoarea
   // Serial.println(data);

  switch(data){
   case '1':
   Inainte(); 
   break;
   case '2':
   Stop();
   break;
   case '3':
   Dreapta();
   break;
   case '4':
   Stanga();
   break;
   
   case '5':
   {
   acceleratie += 10;
   analogWrite(En_dreapta, acceleratie);
   analogWrite(En_stanga, acceleratie);
   }
   break;
   
   case '6':
   {
   acceleratie -= 10;
   analogWrite(En_dreapta, acceleratie);
   analogWrite(En_stanga, acceleratie);
   }
   break;
   case '7':
   Inapoi();
   break;
   case '8':
      do
       Autonomie();
        while (Serial.available() == 0);     
        break; 
   break;
   case '9':
      do
       Parcare();
        while (Serial.available() == 0);     
        break;
     break; 
    case '0':
   Dreapta_Spate();
   break;
  }

  }  
  
}
