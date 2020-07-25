#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 11 

#define NUMPIXELS 12 

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

//analog pins
int antenna = 1;
  
//digital pins
int photogate = 1;
int motor = 3;

//program varibles
unsigned long rotationSpeed = 0;

int delta = 20;

float readings[NUMPIXELS];

int numRead;

//motor control for the mosfet, 2-4 volts, avg. 3 volts (102-204, avg. 153)
int motorSpeed = 153;

void setup() {
	pixels.begin(); 
	pixels.setBrightness(50); 
	pixels.clear();
	
	pinMode(photogate,INPUT); 
	pinMode(motor,OUTPUT); 
	
	unsigned long startTime = millis();
	analogWrite(motor, motorSpeed);
	
  do {	
    if(digitalRead(photogate) == 1 && rotationSpeed == 0)
    {
      unsigned long stopMillis = millis();
      rotationSpeed = stopMillis - startTime;
     }   
  } while(digitalRead(photogate) == 0 && rotationSpeed == 0);
	
	analogWrite(motor, 0);

	//indicate that it has calibrated
	for(int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(0, pixels.Color(0,63,0));
		pixels.show();
	}
	delay(500);
	pixels.clear();
	
	
	numRead = floor(rotationSpeed/10);
	analogWrite(motor, motorSpeed);
	
}

void loop() {
	
	for(int i = 0; i< NUMPIXELS; i++)
	{
    int sum = 0; 
    int average = 0;
    int color = 0;
    
		for(int j = 0; j < numRead; j++)
		{
		  sum = sum + analogRead('a'+antenna);
		  delay(10);
		}

    average = floor(sum/numRead);

    color = map(average, 0, 1023, 0, 255);
    
	pixels.setPixelColor(i, pixels.Color(color, color, color));
    pixels.show();
	}
}
