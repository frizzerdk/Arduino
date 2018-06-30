#include <EEPROM.h>

int incomingByte = 0;   // for incoming serial data
int pin = 13;
int tx=1;
int rx=0;

int redPin = 10;
int greenPin = 9;
int bluePin = 3;
int coldPin = 5;
int warmPin = 6;
int fMem =100;

int r = 255;
int g = 255;
int b = 255;
int c= 0;
int w = 0;
int f=2000;

float dr = 0;
float dg = 0;
float db = 0;
float dc= 0;
float dw = 0;

boolean on = true;
int d =1000;
byte inSerial[15];
int i=0;
int m=0;

long lastMillis=millis();

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(pin,OUTPUT);
        pinMode(redPin,OUTPUT);
        pinMode(greenPin,OUTPUT);
        pinMode(bluePin,OUTPUT);
        pinMode(warmPin,OUTPUT);
        pinMode(bluePin,OUTPUT);
        pinMode(tx,OUTPUT);
        pinMode(rx,INPUT);
        r=EEPROM.read(redPin);
        g=EEPROM.read(greenPin);
        b=EEPROM.read(bluePin);
        w=EEPROM.read(warmPin);
        c=EEPROM.read(coldPin);
        f=EEPROM.read(fMem);
        if(f<10)
        f=2000;
        
        
}

void loop(){
    
                                               
    if (Serial.available() > 0) {   
            
       while (Serial.available() > 0) {
         inSerial[i]=Serial.read(); 
          Serial.println((char*)inSerial);
         i++;      
       }
       if(strstr((char*)inSerial,"#")){
         Serial.println("break");
       inSerial[i-1]='\0';
      Check_Protocol(inSerial);
      i=0;
      m=0;
       }
     }
     dynamicColor();
     analogWrite(redPin,dr);
     analogWrite(bluePin,db);
     analogWrite(greenPin,dg);
     analogWrite(coldPin,dc);
     analogWrite(warmPin,dw);
     } 
void dynamicColor(){

    dr= calcNew(dr,r,millis()-lastMillis);
    dg= calcNew(dg,g,millis()-lastMillis);
    db= calcNew(db,b,millis()-lastMillis);
    dw= calcNew(dw,w,millis()-lastMillis);
    dc= calcNew(dc,c,millis()-lastMillis);
    lastMillis=millis();
    
}
float calcNew(float dynamic, float target, float delta){
  float linearAd=255;
  if(target<dynamic) linearAd=-255;
  double change=(target/2-dynamic/2+linearAd)*(delta/f );
  
  double result=dynamic+change;
  if(abs(change)>=abs(target-dynamic)) result =target;
//   Serial.println("break");
//   Serial.println(result);
//   Serial.println(delta);
//   Serial.println(dynamic);
//   Serial.println(target);
  return result;
}

void Check_Protocol(byte inStr[]){   
  int i=0;
  int m=0;
  Serial.println((char*)inStr);
  if(strstr((char*)inSerial,"r")){
  
    r=atoi((char*)inStr);
    EEPROM.write(redPin,r);
    
  }else if(strstr((char*)inSerial,"g")){
  
   g=atoi((char*)inStr);
   EEPROM.write(greenPin,g);
   
  }else if(strstr((char*)inSerial,"b")){
  
   b=atoi((char*)inStr);
   EEPROM.write(bluePin,b);
  
  } else if(strstr((char*)inSerial,"c")){
  
   c=atoi((char*)inStr);
   EEPROM.write(coldPin,c);
  
  }else if(strstr((char*)inSerial,"w")){
  
   w=atoi((char*)inStr);
   EEPROM.write(warmPin,w);
  
  }else if(strstr((char*)inSerial,"f")){
  
   f=atoi((char*)inStr);
   EEPROM.write(fMem,f);
  
  }
    for(m=0;m<11;m++){
      inStr[m]=0;}
       i=0;
 }
       
   


