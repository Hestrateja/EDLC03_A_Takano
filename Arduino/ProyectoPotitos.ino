#include <ServoTimer2.h>
#include <TimerOne.h>
#include <Volume3.h>




#define speakerPin 10


ServoTimer2 servo;
int deathMod =0;
int boton1 = 2;
int boton2 = 3;
int boton3 = 4;
int buzzPin = 8;
int peligro = 1;
int precaucion = 4;
int bajo = 8;
int normal = 5000;
int diff = 9;
int cambio;
long peligroCurr = 0;
int nivel = 0;
unsigned long beepTimer = 0;
int lastMillis = 0;
unsigned long lastTick = 0;
int deathTimer = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cambio = random(5,10);
  pinMode(buzzPin,OUTPUT);
  pinMode(A0, INPUT);
  pinMode(boton1,INPUT);
  pinMode(boton2,INPUT);
  pinMode(boton3,INPUT);
  pinMode(boton1+diff,INPUT);
  pinMode(boton2+diff,INPUT);
  pinMode(boton3+diff,INPUT);
  servo.attach(5);
  servo.write(1);
}

void loop() {
  //Serial.println(digitalRead(boton1));
  //Serial.println(digitalRead(boton2));
  //Serial.println(digitalRead(boton3));
  currentDanger(peligroCurr);
  if(cambio>100)
  {
    cambio = 100;
  }
  if(cambio<-100)
  {
    cambio = -100;
  }
  if(peligroCurr>2250){
    peligroCurr=2250;  
  }
  servo.write(2250-(peligroCurr*15+750));
  if (millis() - lastTick >= 1000UL) 
  {
     lastTick = millis();
     peligroCurr = ((peligroCurr+(cambio*peligroCurr)/100)+cambio);
     beepTimer++;
     deathTimer = deathTimer+deathMod;
     lastMillis++;
     for(int i=2 ; i<=4 ; i++)
      checkPush (i);
  }
  if(lastMillis>=nivel)
  {
    alarm(nivel);
    lastMillis=0;
  }
  if(millis()-lastTick>=50)
  {
    vol.noTone();
   
  }
  if(deathTimer<=0)
  {
    //gameOver();
  }/*
  if (millis() - lastMillis >= nivel*1000UL) 
  {
   lastMillis = millis();  //get ready for the next iteration
   
  }
  if (millis() - deathTimer >= 30000) 
  {
   gameOver();
  }
  if (millis() - beepTimer >= 50UL) 
  {
     vol.noTone();
  }*/
  
  
}
void currentDanger(int dangerLevel){
    if(dangerLevel<=25){
      if(nivel!=bajo){
        lastMillis = 0;
        deathMod = -1;
      }
      nivel = bajo;
    }else if(dangerLevel>25&&dangerLevel<=50){
      deathTimer = 30;
      deathMod = 0;
      if(nivel!=normal)
      {
        lastMillis = 0;
        beep();
      }
      nivel = normal;
    }else if(dangerLevel>50&&dangerLevel<=75){
      if(nivel!=precaucion)
      {
        lastMillis = 0;
        deathTimer = 0;
        deathMod = 0;
      }
      
      nivel = precaucion; 
    }else{
      if(nivel!=peligro){
        lastMillis = 0;
        deathMod = -1;
      }
      nivel = peligro;
    }
}
void beep(){
  for(int i = 0; i<10; i++)
  {
    digitalWrite(buzzPin,HIGH);
    delay(4);
    digitalWrite(buzzPin,LOW);
    delay(4); 
  }
  
}

void gameOver(){
  for(int i = 0; i<10; i++)
  {
    digitalWrite(buzzPin,HIGH);
    delay(4);
    digitalWrite(buzzPin,LOW);
    delay(4); 
  }
}
void alarm(int nivel){
  beepFreq(220+26*nivel,300+50*nivel);
}

void checkPush(int pinNumber)
{
  int pushed = digitalRead(pinNumber);  // read input value
  if (pushed == HIGH){ // check if the input is HIGH (button released)
    if(pinNumber==2){
      cambio-=5;
    }else if(pinNumber==3){
      cambio+=10;
    }else{
      cambio+=20;
    }
    digitalWrite(pinNumber+9, HIGH);  // turn LED OFF
  }
  else{
    digitalWrite(pinNumber+9, LOW);  // turn LED ON
  }
    
}

//sonidos
void beepFreq(int freq,int volu){
  vol.tone(speakerPin,freq,volu);
}
