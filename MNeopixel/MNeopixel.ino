#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 3
#define PIN_LIGHT_OUT 2
#define PIN_LIGHT_IN A0
#define MH 8
#define MW 23
#define LED_COUNT 184
#define LIGHT_BUFFER_SIZE 5
float timePerCycle = 1000;
float lightValues[LIGHT_BUFFER_SIZE]={0};
int lightReadingIndex=0;

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
 // Serial.begin(9600);
  pinMode(PIN_LIGHT_IN, INPUT);
   pinMode(PIN_LIGHT_OUT, INPUT);
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.
  leds.setBrightness(5);

}

void loop() {

  // put your main code here, to run repeatedly:
for(int i = 50; i<50+50;i++){
 adjustBrightness();
rainbow(i);
delay(10*timePerCycle/LED_COUNT);
}
}
void rainbow(byte startPosition) 
{
  // Need to scale our rainbow. We want a variety of colors, even if there
  // are just 10 or so pixels.
  int rainbowScale = 500 / LED_COUNT;
  
  // Next we setup each pixel with the right color
  for (int i=0; i<LED_COUNT; i++)
  {
    // There are 192 total colors we can get out of the rainbowOrder function.
    // It'll return a color between red->orange->green->...->violet for 0-191.
    leds.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
  }
  // Finally, actually turn the LEDs on:
  leds.show();
}
// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
//  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
uint32_t rainbowOrder(byte position) 
{
  // 6 total zones of color change:
  if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
  {
    return leds.Color(0xFF, position * 8, 0);
  }
  else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
  {
    position -= 31;
    return leds.Color(0xFF - position * 8, 0xFF, 0);
  }
  else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
  {
    position -= 63;
    return leds.Color(0, 0xFF, position * 8);
  }
  else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
  {
    position -= 95;
    return leds.Color(0, 0xFF - position * 8, 0xFF);
  }
  else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
  {
    position -= 127;
    return leds.Color(position * 8, 0, 0xFF);
  }
  else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
  {
    position -= 159;
    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
  }
}

float median(int n, float x[]) { // return the median value for the array x
  float temp;
  int i, j;
  // the following two loops sort the array x in ascending order
  for (i = 0; i < n - 1; i++) {
    for (j = i + 1; j < n; j++) {
      if (x[j] < x[i]) {
        // swap elements
        temp = x[i];
        x[i] = x[j];
        x[j] = temp;
      }
    }
  }
  if (n % 2 == 0) {
    // if there is an even number of elements, return mean of the two elements in the middle
    return ((x[n / 2] + x[n / 2 - 1]) / 2.0);
  } else {
    // else return the element in the middle
    return x[n / 2];
  }
}

float median(int n, int x[]) { // return the median value for the array x
  float temp;
  int i, j;
  // the following two loops sort the array x in ascending order
  for (i = 0; i < n - 1; i++) {
    for (j = i + 1; j < n; j++) {
      if (x[j] < x[i]) {
        // swap elements
        temp = x[i];
        x[i] = x[j];
        x[j] = temp;
      }
    }
  }
  if (n % 2 == 0) {
    // if there is an even number of elements, return mean of the two elements in the middle
    return ((x[n / 2] + x[n / 2 - 1]) / 2.0);
  } else {
    // else return the element in the middle
    return x[n / 2];
  }
}
void adjustBrightness(){
  lightValues[lightReadingIndex]= analogRead(PIN_LIGHT_IN);
  Serial.println(lightValues[lightReadingIndex]);
  lightReadingIndex++;
  if(lightReadingIndex>=LIGHT_BUFFER_SIZE) lightReadingIndex=0;
  
   int light = median(LIGHT_BUFFER_SIZE,lightValues);
  Serial.println(light);
   Serial.println("N");
  if(light<=5){
    leds.setBrightness(1);
  }else{
     leds.setBrightness(false?(light/5)*5:light);
    }
}

// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

