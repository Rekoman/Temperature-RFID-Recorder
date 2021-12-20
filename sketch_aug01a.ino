
// Include Libraries
#include "Arduino.h"
#include "DS18B20.h"
#include "LiquidCrystal_PCF8574.h"
#include "RFID.h"
#include "Wire.h"
#include "RTClib.h"
#include "SD.h"


// Pin Definitions
#define DS18B20WP_1_PIN_DQ  2
#define DS18B20WP_2_PIN_DQ  3
#define RFID_PIN_RST  4
#define RFID_PIN_SDA  5
#define SDFILE_PIN_CS 10



// Global variables and defines
// There are several different versions of the LCD I2C adapter, each might have a different address.
// Try the given addresses by Un/commenting the following rows until LCD works follow the serial monitor prints. 
// To find your LCD address go to: http://playground.arduino.cc/Main/I2cScanner and run example.
#define LCD_ADDRESS 0x3F
//#define LCD_ADDRESS 0x27

// Define LCD characteristics
#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define SCROLL_DELAY 150
#define BACKLIGHT 255
// object initialization
DS18B20 ds18b20wp_1(DS18B20WP_1_PIN_DQ);
DS18B20 ds18b20wp_2(DS18B20WP_2_PIN_DQ);
LiquidCrystal_PCF8574 lcdI2C;
RFID rfid(RFID_PIN_SDA,RFID_PIN_RST);
RTC_PCF8523 rtcPCF;
File sdFile;


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    // initialize the lcd
    lcdI2C.begin(LCD_COLUMNS, LCD_ROWS, LCD_ADDRESS, BACKLIGHT); 
    //initialize RFID module
    rfid.init();
    if (! rtcPCF.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
    }
    if (! rtcPCF.initialized()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtcPCF.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtcPCF.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    // Set SPI SS pin to output otherwise the SD library functions will not work.
    // The SD is set to use SPI SS Arduino pin 10 as chip select(CS) by default.
    // To change the pin use SD.begin(SD_CS_PIN)
    pinMode(SDFILE_PIN_CS, OUTPUT);
    // Check if the card is present and can be initialized
    if (!SD.begin()) {
    Serial.println(F("Card failed, or not present"));
    while(1);
    }
    Serial.println(F("card initialized.")); 
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // DS18B20 1-Wire Temperature Sensor - Waterproof #1 - Test Code
    // Read DS18B20 temp sensor value in degrees celsius. for degrees fahrenheit use ds18b20wp_1.ReadTempF()
    float ds18b20wp_1TempC = ds18b20wp_1.readTempC();
    Serial.print(F("Temp: ")); Serial.print(ds18b20wp_1TempC); Serial.println(F(" [C]"));

    }
    else if(menuOption == '2') {
    // DS18B20 1-Wire Temperature Sensor - Waterproof #2 - Test Code
    // Read DS18B20 temp sensor value in degrees celsius. for degrees fahrenheit use ds18b20wp_2.ReadTempF()
    float ds18b20wp_2TempC = ds18b20wp_2.readTempC();
    Serial.print(F("Temp: ")); Serial.print(ds18b20wp_2TempC); Serial.println(F(" [C]"));

    }
    else if(menuOption == '3') {
    // LCD 16x2 I2C - Test Code
    // The LCD Screen will display the text of your choice.
    lcdI2C.clear();                          // Clear LCD screen.
    lcdI2C.print("  Sensor  ");                   // Print print String to LCD on first line
    lcdI2C.selectLine(2);                    // Set cursor at the begining of line 2
    lcdI2C.print("     Data  ");                   // Print print String to LCD on second line
    delay(1000);

    }
    else if(menuOption == '4') {
    // RFID Card Reader - RC522 - Test Code
    //Read RFID tag if present
    String rfidtag = rfid.readTag();
    //print the tag to serial monitor if one was discovered
    rfid.printTag(rfidtag);

    }
    else if(menuOption == '5') {
    // Adafruit PCF8523 Real Time Clock Assembled Breakout Board - Test Code
    //This will display the time and date of the RTC. see RTC.h for more functions such as rtcPCF.hour(), rtcPCF.month() etc.
    DateTime now = rtcPCF.now();
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print("  ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    delay(1000);
    }
    else if(menuOption == '6') {
    // Micro SD module - Test Code
    // The SD example code creates a datalog.txt file for logging sensor data
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    sdFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file exists in SD card, write sensor data
    if (sdFile) {
    //Write to file
    sdFile.println("ENTER SENSOR DATA HERE");
    // close the file
    sdFile.close();
    // Uncomment to print to the serial port
    //Serial.println("ENTER SENSOR DATA HERE");
    } 
    else {
    // if the file didn't open, print an error
    Serial.println(F("error opening file."));
    }
    }
    
   
}
