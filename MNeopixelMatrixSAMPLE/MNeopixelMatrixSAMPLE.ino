#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "noise.h"
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif


#include "WS2812_Definitions.h"

#define W 2
#define H 2
#define PIN_LED 3
#define PIN_LIGHT_OUT 2
#define PIN_LIGHT_IN A0
#define LED_COUNT 4
#define LIGHT_BUFFER_SIZE 5
long lastRead = 0;
long readCD = 500;
float timePerCycle = 1000;
float lightValues[LIGHT_BUFFER_SIZE] = {0};
int lightReadingIndex = 0;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(W, H, PIN_LED,
                            NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                            NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                            NEO_RBG                   + NEO_KHZ800);
//#include "FastLED.h"
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

void setup() {
  Serial.begin(250000);
  pinMode(PIN_LIGHT_IN, INPUT);
  pinMode(PIN_LIGHT_OUT, INPUT);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(100);
  matrix.setTextSize(0);
  matrix.setTextColor(colors[0]);

  //m.setBrightness(5);

}


void loop() {
  //fire();
  noisy();
  //adjustBrightness();
  matrix.show();
  long t = millis();
  static long last;
  Serial.println(t - last);
  last = t;
}

void noisy() {
  float colorRange = 360;
  uint32_t zoom = 65535 * 3;
  float s = 1;
  int sx = 0 * s * 10;
  int sy = 0 * s * 20;
  int st = 0 * s * 110;
  int sh = s * 1;
  float mh = 1;
  static uint16_t range = 65535;
  static uint32_t t = 0;
  static uint16_t xo = 0;
  static uint16_t yo = 0;
  static uint32_t x = 0;
  static uint32_t y = 0;
  static int h = 0;
  while (h >= 360.0) {
    h -= 360;
  }
  for (int i = 0; i < W; i++) {
    for (int u = 0; u < H; u++) {
      //  matrix.fillScreen(0);
      x = map(i, 0, W, 0, zoom) + xo;
      y = map(u, 0, W, 0, zoom) + yo;
      float fi = inoise16(x, y, t);
      float f = map(fi, 65535 / 2, 65535, -100, 100);
      float b = map(fi, 65535 / 2 - range / 2, 65535 / 2 + range / 2, 0, 100);
      //  Serial.println(fi);
      //Serial.println(b);

      long mL = HSBtoRGB(b * 6 + h * mh, 1, 1);
      // long mL = HSBtoRGB(300, 0.3, 1.1-(float)(b)/(100));
      matrix.drawPixel(i, u, matrix.Color((mL >> 16), mL >> 8, mL));
      //matrix.drawPixel(i+1,u,colors[pass]); if(++pass>=3)pass=0;

      // delay(100);


    }
  }
  h += sh;
  t += st;
  //Serial.println(t);
  xo += sx;
  yo += sy;
}
void fire() {
  float colorRange = 360;
  uint32_t zoom = 65535 * 3;
  float s = 100;
  int sx = 1 * s * 200;
  int sy = 1 * s * 100;
  int st = 1 * s * 100;
  int sh = 0 * 1;
  float mh = 1;
  static uint16_t range = 65535;
  static uint32_t t = 0;
  static uint32_t xo = 0;
  static uint32_t yo = 0;
  static uint32_t x = 0;
  static uint32_t y = 0;
  static int h = 200;
  while (h >= 360.0) {
    h -= 360;
  }
  for (int i = 0; i < W; i++) {
    for (int u = 0; u < H; u++) {
      //  matrix.fillScreen(0);
      x = map(i, 0, W, 0, zoom) + xo;
      y = map(u, 0, W, 0, zoom) + yo;
      float fi = inoise16(x, y, t);
      float f = map(fi, 65535 / 2, 65535, -100, 100);
      float b = map(fi, 65535 / 2 - range / 2, 65535 / 2 + range / 2, 0, 20);
      //  Serial.println(fi);
      //Serial.println(b);

      long mL = HSBtoRGB(constrain(b * 6 + h * mh + ((float)i * i) / (W * W) * 80, 5 * 6 + h * mh, 20 * 6 + h * mh - 25), 1, (((float)(i * i) / (W * W)) * 50 + 50) / 100);
      // long mL = HSBtoRGB(300, 0.3, 1.1-(float)(b)/(100));
      matrix.drawPixel(i, u, matrix.Color((mL >> 16), mL >> 8, mL));
      //matrix.drawPixel(i+1,u,colors[pass]); if(++pass>=3)pass=0;

      // delay(100);


    }
  }
  h += sh;
  t += st;
  //Serial.println(t);
  xo += sx;
  yo += sy;
}
void rainbow() {
  static int x = matrix.width();
  static int pass = 0;
  static int o = 0;
  if (++o > LED_COUNT)o = 0;
  for (int i = 0; i < W; i++) {
    for (int u = 0; u < H; u++) {
      //  matrix.fillScreen(0);
      float f = ((i * 7.0f + u + o) / (W * H)) * 360;
      //Serial.println(f);
      long mL = HSBtoRGB(f, 0.9, 1);
      matrix.drawPixel(i, u, matrix.Color((mL >> 16), mL >> 8, mL));
      //matrix.drawPixel(i+1,u,colors[pass]); if(++pass>=3)pass=0;

      // delay(100);


    }

  }

}
long HSBtoRGB(float _hue, float _sat, float _brightness) {
  float red = 0.0;
  float green = 0.0;
  float blue = 0.0;

  if (_sat == 0.0) {
    red = _brightness;
    green = _brightness;
    blue = _brightness;
  } else {
    while (_hue >= 360.0) {
      _hue -= 360;
    }

    int slice = _hue / 60.0;
    float hue_frac = (_hue / 60.0) - slice;

    float aa = _brightness * (1.0 - _sat);
    float bb = _brightness * (1.0 - _sat * hue_frac);
    float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));

    switch (slice) {
      case 0:
        red = _brightness;
        green = cc;
        blue = aa;
        break;
      case 1:
        red = bb;
        green = _brightness;
        blue = aa;
        break;
      case 2:
        red = aa;
        green = _brightness;
        blue = cc;
        break;
      case 3:
        red = aa;
        green = bb;
        blue = _brightness;
        break;
      case 4:
        red = cc;
        green = aa;
        blue = _brightness;
        break;
      case 5:
        red = _brightness;
        green = aa;
        blue = bb;
        break;
      default:
        red = 0.0;
        green = 0.0;
        blue = 0.0;
        break;
    }
  }

  long ired = red * 255.0;
  long igreen = green * 255.0;
  long iblue = blue * 255.0;

  return long((ired << 16) | (igreen << 8) | (iblue));
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
void adjustBrightness() {
  if (lastRead + readCD < millis()) {
    int x = matrix.getBrightness();
    //matrix.setBrightness(0);
    //matrix.show();
    //  delay(100);
    lightValues[lightReadingIndex] = analogRead(PIN_LIGHT_IN);
    //matrix.setBrightness(x);
    // matrix.show();
    //Serial.println(lightValues[lightReadingIndex]);
    lightReadingIndex++;
    if (lightReadingIndex >= LIGHT_BUFFER_SIZE) lightReadingIndex = 0;

    int light = median(LIGHT_BUFFER_SIZE, lightValues);
    // Serial.println(light);
    // Serial.println("N");
    if (light <= 5) {
      matrix.setBrightness(1);
    } else {
      matrix.setBrightness(false ? (light / 5) * 5 : light);
    }
    lastRead = millis();

  }
}


