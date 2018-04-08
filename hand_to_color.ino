
#include <Adafruit_NeoPixel.h>


int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

//settings for setColor() function
#define ledPin 6
int ledLenght = 75;
int timeLen = 100;
int rint = 0, gint = 0, bint = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledLenght, ledPin, NEO_GRB + NEO_KHZ800);

static int const bufSize = 32;
static char ledBuffer[bufSize] = "";
String color = "";
String brightness = "";
String prevBright = "";
void setup() { 
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void isr() {
  static byte ind = 0;
  char endMarker = '\n';
  char c;
  boolean newData = false;
  while (Serial.available() && newData == false) {
    c = Serial.read();
    if (c != endMarker) { //write char to string
      ledBuffer[ind] = c;
      ind++;
      if (ind >= bufSize) {
        ind = bufSize - 1;
      }
    }
    else {
      ledBuffer[ind] = '\0'; //terminate the string
      newData = true;       //exit while loop
    }
  }
  for (int i = 0; i < sizeof(ledBuffer); i++) {
    if (ledBuffer[i] == 'c' || ledBuffer[i] == 'C'){
      color = ledBuffer[++i];
//      Serial.println("color");
//      Serial.println(color);

  }else if (ledBuffer[i] == 'b' || ledBuffer[i] == 'B'){
      brightness = ledBuffer[++i];
//      Serial.println("brightness");
//      Serial.println(brightness);
  }
  }
  prevBright="";
  prevBright.concat(brightness);
  if (prevBright.equals(brightness)){
    setColor(color.toInt(), brightness.toInt());
  }
}

void setColor(int color, int bright) {
  double brightness = bright-3;
  brightness *= 0.1;
  switch (color) {
    case 3:
      //blue
      rint = 0 * brightness;
      gint = 0 * brightness;
      bint = 255 * brightness;

      break;
    case 4:
      //green
      rint = 0 * brightness;
      gint = 255 * brightness;
      bint = 0 * brightness;

      break;
    case 5:
      //orange
      rint = 255 * brightness;
      gint = 125 * brightness;
      bint = 0 * brightness;
      break;
    case 6:
      //yellow
      rint = 255 * brightness;
      gint = 255 * brightness;
      bint = 0 * brightness;
      break;
    case 7:
      //cyan
      rint = 0 * brightness;
      gint = 200 * brightness;
      bint = 255 * brightness;
      break;
    case 8:
      //red
      rint = 255 * brightness;
      gint = 0 * brightness;
      bint = 0 * brightness;
      break;
    case 9:
      //purple
      rint = 255 * brightness;
      gint = 0 * brightness;
      bint = 255 * brightness;
      break;
    default:
      //pink
      rint = 255 * brightness;
      gint = 150 * brightness;
      bint = 255 * brightness;
      break;
  }
  //set color and intensity of lights      
//  Serial.print(rint);
//  Serial.println();
//
//  Serial.print(gint);
//  Serial.println();
//
//  Serial.print(bint);
//  Serial.println();

  theaterChase(strip.Color(rint, gint, bint), timeLen);

}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}


void loop() {

  //attachInterrupt(digitalPinToInterrupt(2), isr, RISING);
   isr();
}

