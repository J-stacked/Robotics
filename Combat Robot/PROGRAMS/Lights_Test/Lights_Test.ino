#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define NEO_PIN 8

// What type of NeoPixel strip is attached to the Arduino?
#define NEO_PTYPE	NEO_GRBW

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS	59

#define BRIGHTNESS	128 // set max brightness

#define IWAIT		2000
#define SWAIT		25
#define LWAIT		50
#define HWAIT		500

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (f.e. SK6812)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_PTYPE + NEO_KHZ800);

void setup() {
#ifdef __AVR_ATtiny85__
  // This is for Trinket 5V 16MHz
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  // End of trinket special code
#endif

#ifdef LED_BUILTIN
  // Turn the onboard LED off by making the voltage LOW
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);
#endif

  strip.begin(); // This initializes the NeoPixel library.
  strip.setBrightness(BRIGHTNESS); // set brightness
  strip.show(); // Initialize all pixels to 'off'

#ifdef IWAIT
  delay(IWAIT);
#endif
}

void loop() {
  //red
  colorFromSide(strip.Color(255,0,0,0), LWAIT);

  //blue
  colorFromFarSide(strip.Color(0,0,255,0), LWAIT);
  
  //white
  colorFromMiddle(strip.Color(0,0,0,255), LWAIT);

  //green
  colorFromEdges(strip.Color(0,255,0,0), LWAIT);

  //blue
  colorFromSide(strip.Color(0,0,255,0), LWAIT);

  //red
  colorFromFarSide(strip.Color(255,0,0,0), LWAIT);
  
  //white
  colorFromEdges(strip.Color(0,0,0,255), LWAIT);
  fadeOff(LWAIT);
  strip.clear(); // turn all pixels off
  strip.setBrightness(BRIGHTNESS); // restore max brightness value
}

// color from both edges
void colorFromEdges(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip.numPixels()/2+1; i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(strip.numPixels()-i, c);
    strip.show();
    delay(wait);
  }
}

//color from near side
void colorFromSide(uint32_t c, uint16_t wait){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//color from farthest side
void colorFromFarSide(uint32_t c, uint16_t wait){
  for(uint16_t i=60; i>0; i-=1) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//color from middle
void colorFromMiddle(uint32_t c, uint16_t wait){
  for(uint16_t i=30; i>0; i-=1) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(60-i, c);
    strip.show();
    delay(wait);
  }
}

//progressively fades out all lights
void fadeOff(uint16_t wait) {
  byte brightness;
  while ((brightness = strip.getBrightness()) > 0) {
    strip.setBrightness(--brightness);
    strip.show(); // This sends the updated pixel brightness to the hardware.
    delay(wait); // Delay for a period of time (in milliseconds).
  }
}

//turns off all lights from the middle
void fadeFromMiddle(uint16_t wait){
  for(uint16_t i=30; i>0; i-=1) {
    strip.setPixelColor(i, strip.Color(0,0,0,0));
    strip.setPixelColor(60-i, strip.Color(0,0,0,0));
    strip.show();
    delay(wait);
  }
}


