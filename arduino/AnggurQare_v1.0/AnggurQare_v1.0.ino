// the relay module was low level trigger which when its get a low signal the relay will be turn ON //
// the default system is open Loop A.K.A manual system //
#include <Wire.h>
#include <SPI.h>
#include <Nextion.h>
#include <BH1750.h>
#include <GravityTDS.h>
#include <DHT.h>
#include <ph4502c_sensor.h>
#include <NewPing.h>

#define soilPin A0
#define pHPin A1
#define tdsPin A2
#define waterPHPin A3 //PO
#define waterTempPin A4 //TO
#define relayPump1Pin 2 //AC PUMP
#define relayPump2Pin 7 //DC PUMP
#define relayMistPin 8
#define waterValvePin 3  
#define nutritionValvePin 4
#define echoPin 5
#define triggerPin 6
#define dhtPin 9
#define maxDistance 450
//#define pHTriggerPin 12
//#define pHCalibration 14.8f
//#define pHReadingInterval 100
//#define pHReadingCount 10
//#define ADCResolution 1024
unsigned long previousMillis_1 = 0;
unsigned long previousMillis_2 = 0;
unsigned long previousMillis_3 = 0;
unsigned long previousMillis_4 = 0;
unsigned long previousMillis_5 = 0;
unsigned long previousMillis_6 = 0;
unsigned long previousMillis_7 = 0;
unsigned long previousMillis_8 = 0;
unsigned long previousMillis_9 = 0;
unsigned long previousMillis_10 = 0;
const long interval = 500;


//-----------------UPTDATE WATER PH CODE (WILL UPDATE SOON)---------------------//
/*  #define ph_Pin  A3
float Po = 0;
float pH_step;
int nilai_analog_pH;
double TeganganPh;

float myPH4 = 4.200;
float myPH7 = 3.320; 

.................................................................

  nilai_analog_pH = analogRead(ph_Pin); -> PENTING
  Serial2.print("ADC : ");
  Serial2.println(nilai_analog_pH);
  TeganganPh = nilai_analog_pH * (5.0/1024.0); -> PENTING
  Serial2.print("Tegangan : ");
  Serial2.println(TeganganPh, 3);

  pH_step = (myPH4 - myPH7) /3; -> PENTING
  Po = 7.00 + ((myPH7 - TeganganPh) / pH_step); -> PENTING
  Serial2.print("Nilai PH: ");
  Serial2.println(Po, 2);
  delay(1000);

------------------------------------------------------------------

*/
NewPing waterLevel_ultrasonic (triggerPin, echoPin, maxDistance);
DHT myDHT(dhtPin, DHT22);
GravityTDS myTDS;
BH1750 lightMeter; //or change to 0x5C if trouble (default = 0x23)
PH4502C_Sensor PH4502C(
  waterPHPin, 
  waterTempPin
  );

//---------- Nextion HMI component for Waveform ----------//
// NexWaveform soilPHWave = NexWaveform(1, 1, "s0"); 
// NexWaveform soilMoistureWave = NexWaveform(2, 1, "s0"); 
// NexWaveform nutritionWave = NexWaveform(3, 1, "s0"); 
// NexWaveform waterPHWave = NexWaveform(4, 1, "s0"); 
// NexWaveform waterTempWave = NexWaveform(5, 1, "s0"); 
// NexWaveform waterLevelWave = NexWaveform(6, 1, "s0"); 
// NexWaveform temperatureWave = NexWaveform(7, 1, "s0"); 
// NexWaveform lightnessWave = NexWaveform(8, 1, "s0"); 
// NexWaveform humidityWave = NexWaveform(9, 1, "s0"); 

//---------- Nextion HMI component for Control Button ----------//
NexButton waterControlOn = NexButton(10, 1, "b0"); 
NexButton waterControlOff = NexButton(10, 2, "b1");
NexButton hydroponicsControlOn = NexButton(10, 3, "b2");
NexButton hydroponicsControlOff = NexButton(10, 4, "b3");
NexButton nutritionControlOn = NexButton(10, 5, "b4");
NexButton nutritionControlOff = NexButton(10, 6, "b5");
NexButton humidityControlOn = NexButton(10, 7, "b6");
NexButton humidityControlOff = NexButton(10, 8, "b7");

NexButton openLoopMode = NexButton(10, 10, "b9");
NexButton closeLoopMode = NexButton(10, 11, "b10");

//---------- Nextion HMI Number Box ----------//
// NexNumber mainPage_n0 = NexNumber(0, 1, "n0");
// NexNumber mainPage_n1 = NexNumber(0, 1, "n1");
// NexNumber mainPage_n2 = NexNumber(0, 2, "n2");
// NexNumber mainPage_n3 = NexNumber(0, 3, "n3");
// NexNumber mainPage_n4 = NexNumber(0, 4, "n4");
// NexNumber mainPage_n5 = NexNumber(0, 5, "n5");
// NexNumber mainPage_n6 = NexNumber(0, 6, "n6");
// NexNumber mainPage_n7 = NexNumber(0, 7, "n7");
// NexNumber mainPage_n8 = NexNumber(0, 8, "n8");

// NexNumber soilPH_n0 = NexNumber(1, 1, "n0");
// NexNumber soilMoist_n0 = NexNumber(2, 1, "n0");
// NexNumber nutritionTDS_n0 = NexNumber(3, 1, "n0");
// NexNumber waterPH_n0 = NexNumber(4, 1, "n0");
// NexNumber waterTemp_n0 = NexNumber(5, 1, "n0");
// NexNumber waterLevel_n0 = NexNumber(6, 1, "n0");
// NexNumber temperature_n0 = NexNumber(7, 1, "n0");
// NexNumber lightness_n0 = NexNumber(8, 1, "n0");
// NexNumber humidity_n0 = NexNumber(9, 1, "n0");

// NexText soilPH_t1 = NexText(1, 5, "t1");
// NexText soilMoist_t1 = NexText(2, 5, "t1");
// NexText nutritionTDS_t1 = NexText(3, 5, "t1");
// NexText waterPH_t1 = NexText(4, 5, "t1");
// NexText waterTemp_t1 = NexText(5, 5, "t1");
// NexText waterLevel_t1 = NexText(6, 5, "t1");
// NexText temperature_t1 = NexText(7, 5, "t1");
// NexText lightness_t1 = NexText(8, 5, "t1");
// NexText humidity_t1 = NexText(9, 5, "t1");


NexTouch *nex_listen_list[] = 
{
  &waterControlOn,
  &waterControlOff,
  &hydroponicsControlOn,
  &hydroponicsControlOff,
  &nutritionControlOn,
  &nutritionControlOff,
  &humidityControlOn,
  &humidityControlOff,
  &closeLoopMode,
  NULL
};

NexTouch *changeModeList[] =
{
  &openLoopMode,
  NULL
};

//---------- Monitoring Variables ----------//
float soilMoisture;
float waterLevel;
float temperature;
float humidity;
float waterPH;
float waterTemp;
float waterTemp_act; // deviding waterTemp by 10 to get real temp data
float soilPH;
float tdsValue;
float readLux;

float pH_out = 0;
float pH_step;
int nilai_analog_pH;
double Tegangan_pH;
float myPH4 = 4.200; //calibrating voltage when ph 4
float myPH7 = 3.320; //calibrating voltage when ph 7

bool operationMode = 0;
int readSoil;
bool waterState = true;
int readPH;
int Distance = 0;
String sendToESP = "";
String data_from_nextion = "";


//---------- HMI function for each button ----------//
void waterSetOn(void *ptr)
{
  digitalWrite(waterValvePin, LOW);
  digitalWrite(relayPump2Pin, LOW);
}

void waterSetOff(void *ptr)
{
  digitalWrite(relayPump2Pin, HIGH);
  digitalWrite(waterValvePin, HIGH);
}

void hydroponicsSetOn(void *ptr)
{
  digitalWrite(relayPump1Pin, LOW);
}

void hydroponicsSetOff(void *ptr)
{
  digitalWrite(relayPump1Pin, HIGH);
}

void nutritionSetOn(void *ptr)
{
  digitalWrite(nutritionValvePin, LOW);
  digitalWrite(relayPump2Pin, LOW); 
}

void nutritionSetOff(void *ptr)
{
  digitalWrite(relayPump2Pin, HIGH);
  digitalWrite(nutritionValvePin, HIGH);
}

void humiditySetOn(void *ptr)
{
  digitalWrite(relayMistPin, LOW); 
}

void humiditySetOff(void *ptr)
{
  digitalWrite(relayMistPin, HIGH); 
}

void setOpenLoopMode(void *ptr)
{
  operationMode = 0;
  digitalWrite(LED_BUILTIN, LOW);
}

void setCloseLoopMode(void *ptr)
{
  operationMode = 1;
  digitalWrite(LED_BUILTIN, HIGH);
}

//---------- Ultrasonic Function ----------//
// int getDistance() {

//   float duration = 0;
//   int distance = 0;
//   int watchloop = 0;

//   // Clear the triggerPin by setting it LOW and wait for any pulses to expire:
//   digitalWrite(triggerPin, LOW);      // setting state to low and
//   delay(2);                           // waiting 2,000uS (or 686cm (>22ft) to eliminate "echo noise")

//   // only grab values under 20ft/610cm (for some reason, 676 is a common return error for ∞ distance)
//   while ( (distance == 0) || (distance > 610) ) {
//     // Trigger the sensor by setting the triggerPin high for 10 microseconds:
//     digitalWrite(triggerPin, HIGH);         // start sending the 40kHz wave...
//     delayMicroseconds(20);                  // sending for 20uS
//     digitalWrite(triggerPin, LOW);          // stop sending 40kHz wave

//     // Read the echoPin. pulseIn() duration of when the wave-echo stops (in microseconds):
//     duration = pulseIn(echoPin, HIGH);

//     // Calculate the distance:
//     distance = duration * 0.034 / 2;
//     //Serial2.print("distance=");Serial2.println(distance);

//     // Catch funky ∞ distance readings
//     watchloop++;        
//     if (watchloop > 20){      // If errant "676" readings 20 times
//       distance = 610;         // set distance to 610cm (20ft) 
//       break;                  // and break out of loop (not really needed if forced to 610)
//     }
    
//   }

//   return (distance);
// }

//---------- GET AND SEND SOIL PH DATA ----------//
void readPH_function()
{
   //Monitoring Soil pH use Soil pH sensor
  readPH = analogRead(pHPin);
  // soilPH = (-0.0693 * readPH) + 7.3855;
  soilPH = map(readPH, 0.0, 320.0, 0.0, 14.0);
  Serial2.print("mainPage.n0.val=");
  Serial2.print(soilPH,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("soilPH.n0.val=");
  Serial2.print(soilPH,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n0.setValue(soilPH);
  // soilPH_n0.setValue(soilPH);
  // soilPHWave.addValue(0, soilPH);
  if(soilPH < 6.5)
  {
    Serial2.print("soilPH.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Acidic");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // soilPH_t1.setText("Acidic");
  }
  else if(soilPH > 7.5)
  {
    Serial2.print("soilPH.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Alkaline");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // soilPH_t1.setText("Alkaline");

  }
  else
  {
    Serial2.print("soilPH.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Neutral");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // soilPH_t1.setText("Neutral");

  }
}

//---------- GET AND SEND SOIL MOISTURE DATA ----------//
void soilMoisture_function()
{
  //WaterPump for Hydroponics Set-Up use Soil Moist sensor
  readSoil = analogRead(soilPin);
  soilMoisture = map(readSoil, 0, 1023, 100, 0);
  Serial2.print("mainPage.n1.val=");
  Serial2.print(soilMoisture,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("soilMoist.n0.val=");
  Serial2.print(soilMoisture,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n1.setValue(soilMoisture);
  // soilMoist_n0.setValue(soilMoisture);
  // soilMoistureWave.addValue(0, soilMoisture);
  if(soilMoisture < 50)
  {
    Serial2.print("soilMoist.n1.txt=");
    Serial2.print("\"");
    Serial2.print("Dry");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // soilMoist_t1.setText("Dry");

  }
  else if(soilMoisture > 70)
  {
    Serial2.print("soilMoist.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Too Wet");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // soilMoist_t1.setText("Over");

  }
  else
  {
    Serial2.print("soilMoist.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Hydrated");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // soilMoist_t1.setText("Hydrated");

  }
}

//---------- GET AND SEND NUTRITION DATA ----------//
void tdsValue_function()
{
  //Monitoring myTDS and Setting up the Nutrition use TDS sensor
  myTDS.setTemperature(25); //or use "temperature" from DHT22
  myTDS.update();
  tdsValue = myTDS.getTdsValue();
  Serial2.print("mainPage.n2.val=");
  Serial2.print(tdsValue,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("nutritionTDS.n0.val=");
  Serial2.print(tdsValue,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n2.setValue(tdsValue);
  // nutritionTDS_n0.setValue(tdsValue);
  // nutritionWave.addValue(0, tdsValue);
  if(tdsValue < 800)
  {
    Serial2.print("nutritionTDS.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Low");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // nutritionTDS_t1.setText("Low");
  }
  else if(tdsValue > 1500)
  {
    Serial2.print("nutritionTDS.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Over");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // nutritionTDS_t1.setText("Over");
  }
  else
  {
    Serial2.print("nutritionTDS.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Optimum");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // nutritionTDS_t1.setText("Optimum");
  }
}

//---------- GET AND SEND WATER PH DATA ----------//
void waterPH_function()
{

  nilai_analog_pH = analogRead(waterPHPin); //-> PENTING
  Tegangan_pH = nilai_analog_pH * (5.0/1024.0); //-> PENTING
  pH_step = (myPH4 - myPH7) /3; //-> PENTING
  waterPH = 7.00 + ((myPH7 - Tegangan_pH) / pH_step); //-> PENTING
  //Monitoring pH and Temp of Water and Nutrition use PH4502C
  //waterPH = PH4502C.read_ph_level();
  Serial2.print("mainPage.n3.val=");
  Serial2.print(waterPH,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("waterPH.n0.val=");
  Serial2.print(waterPH,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n3.setValue(waterPH);
  // waterPH_n0.setValue(waterPH);
  // waterPHWave.addValue(0, waterPH);
  if(waterPH < 6.5)
  {
    Serial2.print("waterPH.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Acidic");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterPH_t1.setText("Acidic");

  }
  else if(waterPH > 7.5)
  {
    Serial2.print("waterPH.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Alkaline");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterPH_t1.setText("Alkaline");
    
  }
  else
  {
    Serial2.print("waterPH.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Neutral");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterPH_t1.setText("Neutral");

  }
}

//---------- GET AND SEND WATER TEMPERATURE DATA ----------//
void waterTemp_function()
{
  waterTemp = PH4502C.read_temp();
  waterTemp_act = (waterTemp / 10.0) - 50;
  Serial2.print("mainPage.n4.val=");
  Serial2.print(waterTemp_act,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("waterTemp.n0.val=");
  Serial2.print(waterTemp_act,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n4.setValue(waterTemp_act);
  // waterTemp_n0.setValue(waterTemp_act);
  // waterTempWave.addValue(0, waterTemp_act);
  if(waterTemp_act < 20)
  {
    Serial2.print("waterTemp.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Cool");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterTemp_t1.setText("Cool");

  }
  else if(waterTemp_act > 40)
  {
    Serial2.print("waterTemp.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Hot");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterTemp_t1.setText("Hot");

  }
  else
  {
    Serial2.print("waterTemp.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Normal");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterTemp_t1.setText("Normal");

  }
}

//---------- GET AND SEND WATER LEVEL DATA ----------//
void waterLevel_function()
{
  //WaterPump for Tank Set-Up use Ultrasonic sensor
  Distance = (waterLevel_ultrasonic.ping()/US_ROUNDTRIP_CM);
  waterLevel = map(Distance, 19, 50, 100, 0);
  Serial2.print("mainPage.n5.val=");
  Serial2.print(waterLevel,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("waterLevel.n0.val=");
  Serial2.print(waterLevel,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n5.setValue(waterLevel);
  // waterLevel_n0.setValue(waterLevel);
  // waterLevelWave.addValue(0, waterLevel);
  if(waterLevel < 20)
  {
    Serial2.print("waterLevel.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Low");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterLevel_t1.setText("Low");

  }
  else if(waterLevel > 80)
  {
    Serial2.print("waterLevel.t1.txt=");
    Serial2.print("\"");
    Serial2.print("High");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterTemp_t1.setText("High");
  }
  else
  {
    Serial2.print("waterLevel.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Moderate");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // waterTemp_t1.setText("Moderate");
  }
}

//---------- GET AND SEND TEMPERATURE DATA ----------//
void temperature_function()
{
  //MistMaker and DHT Set-Up use DHT22 sensor
  temperature = myDHT.readTemperature();
  Serial2.print("mainPage.n6.val=");
  Serial2.print(temperature,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("temperature.n0.val=");
  Serial2.print(temperature,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n6.setValue(temperature);
  // temperature_n0.setValue(temperature);
  // temperatureWave.addValue(0, temperature);
  if(temperature < 20)
  {
    Serial2.print("temperature.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Cool");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // temperature_t1.setText("Cool");

  }
  else if(temperature > 40)
  {
    Serial2.print("temperature.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Hot");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // temperature_t1.setText("Hot");

  }
  else
  {
    Serial2.print("temperature.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Normal");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // temperature_t1.setText("Normal");

  }
}

//---------- GET AND SEND LIGHTNESS DATA ----------//
void readLux_function()
{
  //Monitoring Light Intensity use BH1750
  readLux = lightMeter.readLightLevel();
  Serial2.print("mainPage.n7.val=");
  Serial2.print(readLux,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("lightness.n0.val=");
  Serial2.print(readLux,0);
  Serial2.write(0xff);
  Serial2.write(0xff); 
  Serial2.write(0xff);
  // mainPage_n7.setValue(readLux);
  // lightness_n0.setValue(readLux);
  // lightnessWave.addValue(0, readLux);
  if(readLux < 10000)
  {
    Serial2.print("lightness.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Dark");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // lightness_t1.setText("Dark");

  }
  else if(readLux > 30000)
  {
    Serial2.print("lightness.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Bright");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // temperature_t1.setText("Bright");

  }
  else
  {
    Serial2.print("lightness.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Optimum");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    // temperature_t1.setText("Optimum");

  }
}

//---------- GET AND SEND HUMIDITY DATA ----------//
void humidity_function()
{
  humidity = myDHT.readHumidity();
  Serial2.print("mainPage.n8.val=");
  Serial2.print(humidity,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("humidity.n0.val=");
  Serial2.print(humidity,0);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  // mainPage_n8.setValue(humidity);
  // humidity_n0.setValue(humidity);
  // humidityWave.addValue(0, humidity);
  if(humidity < 60)
  {
    Serial2.print("humidity.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Dry");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    //humidity_t1.setText("Dry");

  }
  else if(humidity > 80)
  {
    Serial2.print("humidity.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Over");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    //humidity_t1.setText("Too Humid");
  }
  else
  {
    Serial2.print("humidity.t1.txt=");
    Serial2.print("\"");
    Serial2.print("Optimum");
    Serial2.print("\"");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    //humidity_t1.setText("Optimum");

  }
}

//---------- send data to ESP -----------//
void sendToESP_function()
{
  sendToESP = "";
  sendToESP += soilPH;
  sendToESP += ";";
  sendToESP += soilMoisture;
  sendToESP += ";";
  sendToESP += tdsValue;
  sendToESP += ";";
  sendToESP += waterPH;
  sendToESP += ";";
  sendToESP += waterTemp;
  sendToESP += ";";
  sendToESP += waterLevel;
  sendToESP + ";";
  sendToESP += temperature;
  sendToESP += ";";
  sendToESP += readLux;
  sendToESP += ";";
  sendToESP += humidity;
  Serial3.println(sendToESP);
  
  if(Serial3.available())
  {
   String msg = "";
   while(Serial3.available())
   {
    msg += char(Serial3.read());
    delay(50);
   }

  Serial.println(msg);
  }
  
}

//----------- HMI NO NEXTION LIBRARY BUTTON ----------//

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(115200);
  Serial3.begin(115200);
  Wire.begin();
  nexInit();
  myDHT.begin();
  lightMeter.begin();
  pinMode(relayPump1Pin, OUTPUT);
  digitalWrite(relayPump1Pin, HIGH);
  pinMode(relayPump2Pin, OUTPUT);
  digitalWrite(relayPump2Pin, HIGH);
  pinMode(relayMistPin, OUTPUT);
  digitalWrite(relayMistPin, HIGH);
  pinMode(waterValvePin, OUTPUT);
  digitalWrite(waterValvePin, HIGH);
  pinMode(nutritionValvePin, OUTPUT);
  digitalWrite(nutritionValvePin, HIGH);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT); //or use INPUT_PULLUP if trouble
  myTDS.setPin(tdsPin);
  myTDS.setAref(5.0);
  myTDS.setAdcRange(1024);
  myTDS.begin();
  PH4502C.init();
  digitalWrite(triggerPin, LOW);

  //Nextion initialization
  waterControlOn.attachPush(waterSetOn, &waterControlOn);
  waterControlOff.attachPush(waterSetOff, &waterControlOff);
  hydroponicsControlOn.attachPush(hydroponicsSetOn, &hydroponicsControlOn);
  hydroponicsControlOff.attachPush(hydroponicsSetOff, &hydroponicsControlOff);
  nutritionControlOn.attachPush(nutritionSetOn, &nutritionControlOn);
  nutritionControlOff.attachPush(nutritionSetOff, &nutritionControlOff);
  humidityControlOn.attachPush(humiditySetOn, &humidityControlOn);
  humidityControlOff.attachPush(humiditySetOff, &humidityControlOff);
  openLoopMode.attachPush(setOpenLoopMode, &openLoopMode);
  closeLoopMode.attachPush(setCloseLoopMode, &closeLoopMode);
}

void loop() {
  // put your main code here, to run repeatedly:

  nexLoop(changeModeList);
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis_1 >= interval) 
  {     // Your code that was inside the delay(1000) block here
    readPH_function();
  
    previousMillis_1 = currentMillis;
  }
  if (currentMillis - previousMillis_2 >= interval)
  {
    soilMoisture_function();
    previousMillis_2 = currentMillis;
  }
  if (currentMillis - previousMillis_3 >= interval)
  {
    tdsValue_function();
    previousMillis_3 = currentMillis;
  }
  if (currentMillis - previousMillis_4 >= interval)
  {
    waterPH_function();
    previousMillis_4 = currentMillis;
  } 
  if (currentMillis - previousMillis_5 >= interval)
  {
    waterTemp_function();
    previousMillis_5 = currentMillis;
  } 
  if (currentMillis - previousMillis_6 >= interval)
  {
    waterLevel_function();
    previousMillis_6 = currentMillis;
  } 
  if (currentMillis - previousMillis_7 >= interval)
  {
    temperature_function();
    previousMillis_7 = currentMillis;
  } 
  if (currentMillis - previousMillis_8 >= interval)
  {
    readLux_function();
    previousMillis_8 = currentMillis;
  } 
  if (currentMillis - previousMillis_9 >= interval)
  {
    humidity_function();
    previousMillis_9 = currentMillis;
  } 
  if (currentMillis - previousMillis_10 >= interval)
  {
    sendToESP_function();
    previousMillis_10 = currentMillis;
  }
  

  //---------- MAIN LOGIC FOR OPERATING ----------//

  if (operationMode == 1)
  {
    //Logic for fill Water Tank
  if (waterLevel < 90 && waterState == true)
    {
      digitalWrite(waterValvePin, LOW);
      digitalWrite(relayPump2Pin, LOW);
      }
  else
    {
      digitalWrite(relayPump2Pin, HIGH);
      digitalWrite(waterValvePin, HIGH);
      waterState = false;

      if (waterLevel <= 10)
        {
          waterState = true;
        }
    }

    //Logic for Refill Nutrition 
  if (waterLevel >= 90  && tdsValue < 1000 && waterState == false)
    {
      digitalWrite(nutritionValvePin, LOW);
      digitalWrite(relayPump2Pin, LOW);
    }
  else
    {
      digitalWrite(relayPump2Pin, HIGH);
      digitalWrite(nutritionValvePin, HIGH);
    }
  
  //Logic for Mist Maker to humid the environment
  if (humidity < 60 /* && tdsValue > 900 && waterState == false */)
    {
      digitalWrite(relayMistPin, LOW);
    }
  else
    {
      digitalWrite(relayMistPin, HIGH);  
    }
  
  //Logic for Watering the Hydroponics
  if (soilMoisture < 60)
    {
      digitalWrite(relayPump1Pin, LOW);
    }
  else
    {
      digitalWrite(relayPump1Pin, HIGH);
    }
    
  }  
  else if(operationMode == 0)
  {
    nexLoop(nex_listen_list); //for manual operation
  }
}