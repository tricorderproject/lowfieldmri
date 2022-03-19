// Pins
#define DIGIPOT_CS    3
#define DIGIPOT_MOSI  5
#define DIGIPOT_SCLK  4

#define SPI_CS        53

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
    int bitIn = digitalRead(ADC_MISO);
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


// Read AD7680 16-bit ADC
uint16_t ReadAD7680() {
  uint8_t sendBuffer[3] = {0, 0, 0};
  uint8_t receiveBuffer[3] = {0, 0, 0};
  
  // Bring SS low to begin the transaction.
  //  spi.setSelect(LOW);
  digitalWrite(SPI_CS, LOW);


  // Delay a short while
  //Delay10us();

  // Clock data from the AD7680.  It takes 3 bytes per conversion. 
  //## spi.transfer(3, sendBuffer, receiveBuffer);
  
  // Bring SS high to end the transaction.
//  spi.setSelect(HIGH);
  digitalWrite(SPI_CS, HIGH);

  // Extract the 16-bit ADC value from the 24-bit buffer.  
  // 4 leading and 4 trailing zeros.    
  uint16_t out = (receiveBuffer[0] << 12) + (receiveBuffer[1] << 4) + (receiveBuffer[2] >> 4);
  /*
  Serial.print(uint16_t(receiveBuffer[0]));
  Serial.print(",");
  Serial.print(uint16_t(receiveBuffer[1]));
  Serial.print(",");
  Serial.print(uint16_t(receiveBuffer[2]));
  Serial.println("");
  */
  return out;
}


//  Delay10us
void Delay10us() {
  volatile int  cnt;
  for (cnt = 0; cnt < 100; cnt++) {
  }
}


/*
 * Digipot
 */ 
void setDigipot(uint8_t value) {
  // Idle
  uint8_t val = value;
  digitalWrite(DIGIPOT_SCLK, 1);
  delay(2);
  digitalWrite(DIGIPOT_CS, 0);  // Active
  delay(2);  
  
  for (int i=0; i<8; i++) {
    digitalWrite(DIGIPOT_SCLK, 0);        // Inactive
    delay(1);
    
    // Data
    if ((val & 0x80) > 0) {
      digitalWrite(DIGIPOT_MOSI, 1);
      //Serial.print("1");
    } else {
      digitalWrite(DIGIPOT_MOSI, 0);
      //Serial.print("0");      
    }
    val = val << 1;
    delay(1);
    
    digitalWrite(DIGIPOT_SCLK, 1);      // Active
    delay(2);
      
  }
  
  digitalWrite(DIGIPOT_SCLK, 0);      // Inactive
  delay(2);
  
  digitalWrite(DIGIPOT_CS, 1);   // Inactive
  // Serial.println("");
}


void setup() {

  // Setup pins for Digipot  
  pinMode(DIGIPOT_CS, OUTPUT);
  pinMode(DIGIPOT_MOSI, OUTPUT);
  pinMode(DIGIPOT_SCLK, OUTPUT);
  
  setupAD7680();
  
  // Serial console
  Serial.begin(115200);
  Serial.println("Initializing...");
  
  Serial.println("before");
  //setDigipot(128);
  //setDigipot(37);
  Serial.println("after");
  setDigipot(255);
}

#define MAX_VALUES 50000
//#define MAX_VALUES 50
int count = 0;
void loop() {
  digitalWrite(35, 0);
  delay(10);
  digitalWrite(35, 1);
  delay(10);  

  while (Serial.available() == 0) {
    // Spin
  }
  

  if (Serial.read() == 'r') {
    uint16_t values[MAX_VALUES];
    for (int i=0; i<MAX_VALUES; i++) {  
      // Read ADC
      //    values[i] = ReadAD7680();    
      values[i] = readAD7680();    
    }

    Serial.println("DATA:");
    for (int i=0; i<MAX_VALUES; i++) {
      Serial.println(values[i]);    
    }
    //Serial.println("--------");
    //Serial.println(count);
    count += 1;
    //delay(100);
  }
  
}
