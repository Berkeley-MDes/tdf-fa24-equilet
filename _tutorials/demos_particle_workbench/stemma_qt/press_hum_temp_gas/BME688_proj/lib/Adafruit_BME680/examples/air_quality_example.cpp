// Ingo Lohs, myclock-bme680-lcd-i2c works with Particle Photon v0.7.0
// a digital watch with BME680 on I2C
// 06.10.2018, v1.0

/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/3660

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
 
// https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas/arduino-wiring-test

// Pressure is returned in the SI units of Pascals. 100 Pascals = 1 hPa = 1 millibar. Often times barometric pressure is reported in millibar or inches-mercury. For future reference 1 pascal =0.000295333727 inches of mercury, or 1 inch Hg = 3386.39 Pascal. So if you take the pascal value of say 100734 and divide by 3386.39 you'll get 29.72 inches-Hg.

// Datasheet: https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf
// Github: https://github.com/BoschSensortec/BME680_driver

// Gas is returned as a resistance value in ohms. This value takes up to 30 minutes to stabilize! Once it stabilizes, you can use that as your baseline reading. Higher concentrations of VOC will make the resistance lower.
// The resistance (in Ohms) of the gas sensor.  This is proportional to the amount of VOC particles in the air.
// The result of combining humidity and gas indexes is a qualitative and so-called IAQ - Indoor Air Quality index value scaled from 0-100% (where 100% is good) and this is then scaled again from 0-500 where a 500 value is bad and descriptive values then applied in stages from good to hazardous air quality.

// Gas-Interpretation, source: Datasheet, Table 4
// IAQ Index   Air Quality
//   0 –  50   good
//  51 – 100   average
// 101 – 150   little bad
// 151 – 200   bad
// 201 – 300   worse
// 301 – 500   very bad

// https://github.com/G6EJD/BME680-Example/blob/master/Slide1.JPG

// LIBs
#include <LiquidCrystal_I2C_Spark.h>
#include <Adafruit_BME680.h>

// Objects
LiquidCrystal_I2C *lcd;
Adafruit_BME680 bme; // I2C > By default, the i2c address is 0x77. If you add a jumper from SDO to GND, the address will change to 0x76.
//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

// Variables init
double temperatureInC = 0;
double relativeHumidity = 0;
double pressureHpa = 0;
double gasResistanceKOhms = 0;
double approxAltitudeInM = 0;

float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

float hum_score, gas_score;
float gas_reference = 250000;
float hum_reference = 40;
int getgasreference_count = 0;

float air_quality_score = 0; 

// Definitions
const unsigned long UPDATE_PERIOD_MS = 5000;
unsigned long lastUpdate = 0;

// I2C wiring
// https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas/pinouts#i2c-logic-pins-2-4
// SCK - this is also the I2C clock pin, connect to your microcontrollers I2C clock line.
// SDI - this is also the I2C data pin, connect to your microcontrollers I2C data line.

//#define BME_MOSI D0
//#define BME_SCK D1

//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10

// Druck auf NormalNull - Meerwasser-Spiegel
#define SEALEVELPRESSURE_HPA (1013.25)

// for the values
char buf[64];
String Wochentag;

// *********************************

void setup()
{
    Serial.begin(9600);
    
    Serial.println(F("BME680 + LCDisplay 1602 / 2004"));
    
    if (!bme.begin()) {
        Serial.println("Could not find a valid BME680 sensor, check wiring!");
    } else {
        Serial.println("BME680 sensor works!");
    }

    // The address is typically 0x27. I2C Address: 0x3F
    // https://www.sainsmart.com/new-sainsmart-iic-i2c-twi-1602-serial-lcd-module-display-for-arduino-uno-mega-r3.html
    lcd = new LiquidCrystal_I2C(0x3F /*address*/, 20 /*columns*/, 4/*rows*/); // > for LCD2004
    lcd->init();
    lcd->backlight();
    lcd->clear();
    Time.zone(isDST() ? +2.00 : +1.00);  
    
    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
    
    // Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
    GetGasReference();
    
}
    
// *********************************

void loop() { 

  lcd->setCursor(0 /*columns*/,0 /*rows*/);
  lcd->print(Time.format(Time.now(), "%d.%m.%y")); // %A für Weekday = English Output
  
  if ((Time.format(Time.now(), "%H:%M:%S")) == "00:00:00")
  {
        lcd->clear();
  }

  // https://docs.particle.io/reference/firmware/photon/#weekday-
  int wDayName = Time.weekday();
  
  if (wDayName != wDayName)
  {
      lcd->clear();
  }
  
  if (wDayName == 1) 
  {
      Wochentag = "Sonntag";
  }
  else if (wDayName == 2) 
  {
      Wochentag = "Montag";
  }
  else if (wDayName == 3) 
  {
      Wochentag = "Dienstag";
  }
  else if (wDayName == 4) 
  {
      Wochentag = "Mittwoch";
  }
  else if (wDayName == 5) 
  {
      Wochentag = "Donnerstag";
  }
  else if (wDayName == 6) 
  {
      Wochentag = "Freitag";
  }
   else if (wDayName == 7) 
  {
      Wochentag = "Samstag";
  }
   
  lcd->setCursor(10,0);
  lcd->print(Wochentag);
  
  lcd->setCursor(0,1);
  lcd->print(Time.format(Time.now(), "KW: %W"));
  lcd->setCursor(10,1);
  lcd->print(Time.format(Time.now(), "%H:%M:%S"));

    // read the BME280 sensor
	if (millis() - lastUpdate >= UPDATE_PERIOD_MS) {
		lastUpdate = millis();
		
	if (! bme.performReading()) {
        Serial.println("BME680-Log - Failed to perform reading!");
            lcd->clear();
		    lcd->setCursor(0,0);
			lcd->print("Problem with BME680!");
    } else {
        // read values
        temperatureInC = bme.temperature;
        relativeHumidity = bme.humidity;
        pressureHpa = bme.pressure / 100.0;
        gasResistanceKOhms = bme.gas_resistance / 1000.0;
        approxAltitudeInM = bme.readAltitude(SEALEVELPRESSURE_HPA);
		
		// serial output
    	Serial.print("Temperature = ");
    	double temperatureInC = bme.temperature; // degrees C
        Serial.print(temperatureInC);
        Serial.println(" *C");

        Serial.print("Humidity = ");
        double relativeHumidity = bme.humidity; // % 
        Serial.print(relativeHumidity);
        Serial.println(" %");
    
        Serial.print("Approx. Altitude = ");
        double approxAltitudeInM = (bme.readAltitude(SEALEVELPRESSURE_HPA)); 
        Serial.print(approxAltitudeInM);
        Serial.println(" m");
    
        Serial.print("Pressure = ");
    	double pressureHpa = (bme.pressure / 100.0); // hPa
        Serial.print(pressureHpa);
        Serial.println(" hPa");
        
        Serial.print("Gas = ");
    	double gasResistanceKOhms = (bme.gas_resistance / 1000.0); // KOhms
        Serial.print(gasResistanceKOhms);
        Serial.println(" KOhms");

/*
 This software, the ideas and concepts is Copyright (c) David Bird 2018. All rights to this software are reserved.
 
 Any redistribution or reproduction of any part or all of the contents in any form is prohibited other than the following:
 1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
 2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
 3. You may not, except with my express written permission, distribute or commercially exploit the content.
 4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.
 The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the
 software use is visible to an end-user.
 
 THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT. FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY 
 OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 See more at http://www.dsbird.org.uk
*/    

// David Bird - Start - Routine

        //Calculate humidity contribution to IAQ index
        float current_humidity = bme.readHumidity();
        if (current_humidity >= 38 && current_humidity <= 42)
            hum_score = 0.25*100; // Humidity +/-5% around optimum 
        else
        { //sub-optimal
        if (current_humidity < 38) 
            hum_score = 0.25/hum_reference*current_humidity*100;
        else
        {
            hum_score = ((-0.25/(100-hum_reference)*current_humidity)+0.416666)*100;
        }
        }
    
        //Calculate gas contribution to IAQ index
        int gas_lower_limit = 5000;   // Bad air quality limit
        int gas_upper_limit = 50000;  // Good air quality limit 
        if (gas_reference > gas_upper_limit) gas_reference = gas_upper_limit; 
        if (gas_reference < gas_lower_limit) gas_reference = gas_lower_limit;
        gas_score = (0.75/(gas_upper_limit-gas_lower_limit)*gas_reference -(gas_lower_limit*(0.75/(gas_upper_limit-gas_lower_limit))))*100;
          
        //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
        float air_quality_score = hum_score + gas_score;
        
        Serial.println("Air Quality = "+String(air_quality_score,1)+"% derived from 25% of Humidity reading and 75% of Gas reading - 100% is good quality air");
        Serial.println("Humidity element was : "+String(hum_score/100)+" of 0.25");
        Serial.println("     Gas element was : "+String(gas_score/100)+" of 0.75");
        
        if (bme.readGas() < 120000) Serial.println("***** Poor air quality *****");
        Serial.println();
        if ((getgasreference_count++)%10==0) GetGasReference(); 
        Serial.println(CalculateIAQ(air_quality_score));

        Serial.println("---next reading---");

// David Bird - End - Routine

        // check for value
		if (temperatureInC != NULL) {
	    
	        // print out to LCD 
			snprintf(buf, sizeof(buf), "%.2f C", temperatureInC);
			lcd->setCursor(0,2);
			lcd->print(buf);

			//snprintf(buf, sizeof(buf), "%.2f F", temperatureInC * 9.0 / 5.0 + 32.0);

			snprintf(buf, sizeof(buf), "%.2f %%", relativeHumidity);
            lcd->setCursor(10,2);
			lcd->print(buf);

			//snprintf(buf, sizeof(buf), "%.2f % m", approxAltitudeInM);
			//lcd->setCursor(0,3);
			//lcd->print(buf);

		    //snprintf(buf, sizeof(buf), "%.0f %%hPa", pressureHpa);
		    //lcd->setCursor(0,3);
		    //cd->print(buf);
        	
        	// Gas - Option A - Value Output 
        	//snprintf(buf, sizeof(buf), "%.1f Gas", bme.readGas());
			//lcd->setCursor(0,3);
			//lcd->print(buf);
			
			// Gas - Option B - Value Output in KOhm
        	//snprintf(buf, sizeof(buf), "%.1f KOhm", gasResistanceKOhms);
			//lcd->setCursor(10,3);
			//lcd->print(buf);
        	
        	// Gas - Option C - Interpretation Output as String
			lcd->setCursor(0,3);
			lcd->print(CalculateIAQ(air_quality_score));
       	}
    }
	}
}

// FUNCTION TO CHECK GAS-Reference-Value ---------------------
// David Bird

void GetGasReference(){
  // Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
  Serial.println("Getting a new gas reference value");
  int readings = 10;
  for (int i = 0; i <= readings; i++){ // read gas for 10 x 0.150mS = 1.5secs
    gas_reference += bme.readGas();
  }
  gas_reference = gas_reference / readings;
}

String CalculateIAQ(float score) {
  
  String IAQ_text = "Air Quality: "; // Air Quality is: 
  score = (100-score)*5;
  if      (score >= 301)                  IAQ_text += "poor";
  else if (score >= 201 && score <= 300 ) IAQ_text += "worse";
  else if (score >= 151 && score <= 200 ) IAQ_text += "bad";
  else if (score >= 101 && score <= 150 ) IAQ_text += "lit bad";
  else if (score >=  51 && score <= 100 ) IAQ_text += "average";
  else if (score >=  00 && score <=  50 ) IAQ_text += "good";
  return IAQ_text;
}

// FUNCTION TO CHECK SUMMER/WINTER-TIME ---------------------

bool isDST()
{ // Central European Summer Timer calculation
  int dayOfMonth = Time.day();
  int month = Time.month();
  int dayOfWeek = Time.weekday() - 1; // make Sunday 0 .. Saturday 6

  if (month >= 4 && month <= 9)
  { // March to September definetly DST
    return true;
  }
  else if (month < 3 || month > 10)
  { // before March or after October is definetly normal time
    return false;
  }

  // March and October need deeper examination
  boolean lastSundayOrAfter = (dayOfMonth - dayOfWeek > 24);
  if (!lastSundayOrAfter)
  { // before switching Sunday
    return (month == 10); // October DST will be true, March not
  }

  if (dayOfWeek)
  { // AFTER the switching Sunday
    return (month == 3); // for March DST is true, for October not
  }

  int secSinceMidnightUTC = Time.now() % 86400;
  boolean dayStartedAs = (month == 10); // DST in October, in March not
                                        // on switching Sunday we need to consider the time
  if (secSinceMidnightUTC >= 1 * 3600)
  { // 1:00 UTC (=2:00 CET/3:00 CEST)
    return !dayStartedAs;
  }

  return dayStartedAs;
}
