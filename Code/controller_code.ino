#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the arduino is connected to the DIN pin on the LED strip?
#define DATA_PIN 0

// Which pin on the arduino is wired to the momentary push button?
#define BUTTON_PIN 1

// Define the amount of templates/modes available
#define TEMPLATE_COUNT 13

// How many LEDs are on the strip attached to the Arduino?
#define NUMPIXELS 80

// NeoPixel Library setup by instantiating a NeoPixel strip object called pixels
Adafruit_NeoPixel strip(NUMPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// Mode control variables
boolean oldState = LOW; // Variable used to store previous state
short mode = 0; // Current mode

// Define colors
uint32_t WHITE = strip.Color(255, 255, 255);
uint32_t BLUE = strip.Color(0, 0, 255);
uint32_t RED = strip.Color(255, 0, 0);
uint32_t VIOLET = strip.Color(148, 0, 211);

// Function headers
void colorWipe(uint32_t, int, float=1);

void setup() 
{
  // Code for 16MHz controllers such as the ATTiny85
  //#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  //  clock_prescale_set(clock_div_1);
  //#endif

  pinMode(BUTTON_PIN, INPUT); // Set BUTTON_PIN to input
  pinMode(DATA_PIN, OUTPUT); // Set DATA_PIN to output

  strip.begin(); // INITIALIZE NeoPixel strip object
  strip.show();  // Initialize all pixels to 'off'
  strip.clear();
}

void loop() 
{
  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);

  // Check if state changed from low to high (button press).
  if((newState == HIGH) && (oldState == LOW))
  {
    // Short delay to debounce button.
    delay(60);
    // Check if button is still high after debounce.
    newState = digitalRead(BUTTON_PIN);

    if(newState == HIGH)
    {
      // Advance to next mode, wrap around after TEMPLATE_COUNT - 1
      if(++mode > (TEMPLATE_COUNT - 1))
        mode = 0;

      switch(mode)
      {
        case 0:
          colorWipe(strip.Color(0, 0, 0), 0);    // Black/off
          break;
        case 1:
          colorWipe(WHITE, 0); // White
          break;
        case 2:
          colorWipe(WHITE, 0, 0.5); // White, half intensity
          break;
        case 3:
          colorWipe(WHITE, 0, 0.25); // White, quarter intensity
          break;
        case 4:
          colorWipe(VIOLET, 0);
          break;
        case 5:
          colorWipe(VIOLET, 0, 0.5);
          break;
        case 6:
          colorWipe(VIOLET, 0, 0.25);
          break;
        case 7:
          colorWipe(RED, 0);
          break;
        case 8:
          colorWipe(RED, 0, 0.5);
          break;
        case 9:
          colorWipe(RED, 0, 0.25);
          break;
        case 10:
          colorWipe(BLUE, 0);
          break;
        case 11:
          colorWipe(BLUE, 0, 0.5);
          break;
        case 12:
          colorWipe(BLUE, 0, 0.25);
          break;
      }
    }
  }

  // Set the last-read button state to the old state.
  oldState = newState;
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait, float intensity=1) 
{
  // First, clear the strip
  strip.clear();

  // Get the color composition in RGB
  uint8_t redComp = (color >> 16) & 0xFF;
  uint8_t greenComp = (color >> 8) & 0xFF;
  uint8_t blueComp = color & 0xFF;

  // Apply intensity to desired color
  redComp = redComp*intensity;
  greenComp = greenComp*intensity;
  blueComp = blueComp*intensity;

  // Remake color
  color = strip.Color(redComp, greenComp, blueComp);

  // For each pixel in strip...
  for(short i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void plantAmbientColor()
{
  // First, clear the strip
  strip.clear();

  // Set smaller color order array's size
  short arraySize = 8;

  // Smallest color order
  // This is the order in which colors will appear in the smallest subset of our leds, which, because of the 1/2, 1/3 e 1/6 proportions, the smallest is 6 leds
  uint32_t colors_array[arraySize] = {WHITE, RED, WHITE, RED, WHITE, BLUE, WHITE, BLUE};

  // Current color
  short colorIndex = 0;

  // For each N pixels in strip according to proportion
  for(short i=0; i<strip.numPixels(); i++) 
  {
     strip.setPixelColor(i, colors_array[colorIndex]);         //  Set pixel's color (in RAM)
     strip.show();                          //  Update strip to match

     if(++colorIndex > arraySize-1)
        colorIndex = 0;
  }
}
