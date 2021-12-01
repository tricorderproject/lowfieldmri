/* LED Blink, Teensyduino Tutorial #1
   http://www.pjrc.com/teensy/tutorial.html
 
   This example code is in the public domain.
*/

// Teensy 2.0 has the LED on pin 11
// Teensy++ 2.0 has the LED on pin 6
// Teensy 3.x / Teensy LC have the LED on pin 13
const int ledPin = 13;

// DIGIPOT
#define DIGIPOT_CS  8
#define DIGIPOT_SDI 7
#define DIGIPOT_SCK 6

// AD7680
#define ADC_CS    10
#define ADC_MISO  12
#define ADC_SCK   14

void shortDelay() {
  int a=0; 
  for (int i=0; i<44; i++) {
    __asm__("nop\n\t");
  }
}

uint16_t readAD7680() {
  uint32_t value = 0;  
  
  // Step 1: Activate CS
  digitalWrite(ADC_CS, 0);    
  shortDelay();
  
  for (int i=0; i<23; i++) {
    digitalWrite(ADC_SCK, 0);   // Clock down (data changes)
    // Do nothing
    shortDelay();
    
    digitalWrite(ADC_SCK, 1);   // Clock up (data valid)
    shortDelay();
    // Read bit
    uint8_t bitIn = digitalRead(ADC_MISO);
    //Serial.print("BIT: "); Serial.println(bitIn);
    value = value << 1;
    value = value + (bitIn & 0x01);    
  }
  shortDelay();
  // Set bus to idle
  idleAD7680();
  shortDelay();

  value = value >> 4;
  
  return (uint16_t)value;
}

void idleAD7680() {
  digitalWrite(ADC_CS, 1);    
  digitalWrite(ADC_SCK, 1);
}

void setupAD7680() {
  pinMode(ADC_CS, OUTPUT);
  pinMode(ADC_SCK, OUTPUT);
  pinMode(ADC_MISO, INPUT);

  idleAD7680();
}

void setupDigipot() {
  pinMode(DIGIPOT_CS, OUTPUT);
  pinMode(DIGIPOT_SDI, OUTPUT);
  pinMode(DIGIPOT_SCK, OUTPUT);
}

void idleDigipot() {
  digitalWrite(DIGIPOT_CS, 1);
  digitalWrite(DIGIPOT_SDI, 1);
  digitalWrite(DIGIPOT_SCK, 1);
}

// the setup() method runs once, when the sketch starts
void setup() {
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);

  setupAD7680();
  idleAD7680();

  setupDigipot();
  idleDigipot();

  Serial.begin(115200);
}

// the loop() methor runs over and over again,
// as long as the board has power

int count = 0;
#define MAX_SAMPLES   10000
void loop() {
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(100);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(100);                  // wait for a second

  //count += 1;
  //Serial.println(count);

  uint16_t data[MAX_SAMPLES];
  long startTime = millis();
  for (int i=0; i<MAX_SAMPLES; i++) {    
    uint16_t value = readAD7680();
    data[i] = value;
    //Serial.println(value);
  }
  long deltaTime = millis() - startTime;

  
  Serial.println("DATA:");
  for (int i=0; i<MAX_SAMPLES; i++) {
    Serial.println(data[i]);
  }  
  //Serial.print("DELTA TIME: ");
  //Serial.println(deltaTime);
  delay(1000);
  
}
