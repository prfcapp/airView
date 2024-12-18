

#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_SSD1306.h>
#include "Adafruit_CCS811.h"
#include <Adafruit_BME280.h>

#define OLED_DC 19
#define OLED_CS 5
#define OLED_CLK 18
#define OLED_MOSI 23
#define OLED_RESET 4

int ledPin = 2;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float pres = 0;
float temp = 0;
float hum = 0;

U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/ OLED_CLK, /* data=*/ OLED_MOSI, /* cs=*/ OLED_CS, /* dc=*/ OLED_DC, /* reset=*/ OLED_RESET);
Adafruit_CCS811 ccs;
Adafruit_BME280 bme;


void setup() {
  // put your setup code here, to run once:
  delay(2500);
  Serial.begin(9600);
  u8g2.begin();
    pinMode(ledPin, OUTPUT);
  
  // Light flashes to indicate setup initialization
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }

  // Flash LED to indicate start of setup
  digitalWrite(ledPin, HIGH);
  delay(3000);
  delay(5000); // Wait for 5 seconds
  digitalWrite(ledPin, LOW);


  if(!ccs.begin()){
    Serial.println("Failed to start CCS sensor! Please check your wiring.");
  }
  
  
  if(!bme.begin(0x76)){
    Serial.println("Failed to start BME sensor! Please check your wiring.");
  }

  //while(!ccs.available());
}

void loop() {
  
  ccs.readData();


  char co2buff[20]; // Create a buffer to hold the string representation of the CO2 value
  char tvocbuff[20]; // Create a buffer to hold the string representation of the CO2 value
    

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.drawStr(25,13,"AirView");
    
    u8g2.setFont(u8g2_font_ncenB08_tr);

    ///////////////////////////////////////////////////////
    // read information -> to be tidied up to a 1 liner
    //////////////////////////////////////////////////////
    size_t co2_value = ccs.geteCO2(); // Get the CO2 value
    sprintf(co2buff, "%zu", co2_value); // Convert the size_t value to a string
    size_t tvoc_value = ccs.getTVOC(); // Get the CO2 value
    sprintf(tvocbuff, "%zu", tvoc_value); // Convert the size_t value to a string
    temp = bme.readTemperature();
    pres = bme.readPressure() / 100.0F;
    hum = bme.readHumidity();
    

    // left column
    u8g2.setCursor(3, 30);
    u8g2.print("PPM:");
    u8g2.setCursor(35, 30);
    u8g2.print(ccs.geteCO2());
    Serial.println("CO2: ");
    Serial.println(ccs.geteCO2());
    u8g2.setCursor(3, 45);
    u8g2.print("TVOC:");
    u8g2.setCursor(40, 45);
    u8g2.print(ccs.getTVOC());
    Serial.println("TVOC: ");
    Serial.println(ccs.getTVOC());

    // right column
    u8g2.setCursor(65, 30);
    u8g2.print("temp:");
    u8g2.setCursor(97, 30);
    u8g2.print(bme.readTemperature());
    Serial.println("Temp:");
    Serial.println(bme.readTemperature());
    u8g2.setCursor(65, 45);
    u8g2.print("pres:");
    u8g2.setCursor(93, 45);
    u8g2.print(bme.readPressure() / 100.0F);
    Serial.println("Pressure:");
    Serial.println(bme.readPressure() / 100.0F);
    u8g2.setCursor(65, 60);
    u8g2.print("hum:");
    u8g2.setCursor(95, 60);
    u8g2.print(bme.readHumidity());
    Serial.println("Humidity:");
    Serial.println(bme.readHumidity());

  } while ( u8g2.nextPage() );
    delay(100);
}



