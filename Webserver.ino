#include <Adafruit_MPU6050.h> //For Interfacing with MPU6050
#include <Adafruit_Sensor.h>
#include <Wire.h> //For implementing I2C communication protocol
#include <WiFiManager.h> //For WiFi
#include <ArduinoJson.h> //For data transfer using JSON

Adafruit_MPU6050 mpu; //creates an instance of Adafruit_MPU6050 class
WiFiManager wm; //creates an instance of WiFiManager class

//Global Variables
bool res; //to report the result of WiFi connection attempt done by us

void setupWiFi(){
  //Function to setup WiFi
  while (!Serial)
    delay(10); //it will pause the board until Serial Monitor is opened
  WiFi.mode(WIFI_STA); //Setts the esp32 in station mode so that it can connect to the Wifi network

  res = wm.autoConnect("AutoConnectAP","password"); //Autoconnect to password protected Access point

  if(!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
  }
}

void setupMPU6050(){
  //Function to setup MPU6050 chip
  while(!Serial)
    delay(10); //it will pause the board until Serial Monitor is opened

  Serial.println("Adafruit MPU6050 test!");

  //Try to initialize
  if(!mpu.begin()){
    Serial.println("Failed to find MPU6050 chip");
    while(1){
      delay(10);
      //pause the code indefinitely
    }
  }
  Serial.println("MPU6050 found!");

  //Setting up the chip sensor range and filter bandwidth
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); //Sets accelerometer measurement range
    //Range selected is +- 8g, other values are +- 2g (default), +- 4g, +- 16g
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange())//getAccelerometerRange() gets the range selected for the sensor
  {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // Sets the gyroscope measurement range to 500
    //The available options are: +-250 deg/s(default), +-500 deg/s, +-1000 deg/s, +-2000 deg/s
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) 
  {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); // Sets the filter bandwidth of digital Low pass filter
    // Available bandwidths : 5,10,21,44,94,184,260 Hz. 260 Hz disables the filter
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) // Gets bandwidth of the Digital Low Pass Filter.
  {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

  Serial.println("");
}

void setup(void) {
  Serial.begin(115200);
  
  /*We need to setup WiFi before MPU6050 chip.*/
  //Calling the function to setup WiFi connection
  int a = setupWiFi();
  if (a==1){
      Serial.println("Connected to WiFi Successfully.");
      //Calling the function to setup the MPU6050 chip
      int b = setupMPU6050();
      if (b==1){
        Serial.println("MPU6050 chip set successfully!");
      }
  }
}

void loop() {

  /* Get new sensor events with the readings. We will get all the data but not display the temperature as it is not required for our application.
      The option to not fetch the temp data was not supported by the library as it caused errors on compilation.*/
  sensors_event_t a, g, temp;
  //a = accelerometer, g = gyroscope, temp = temperature sensor
  mpu.getEvent(&a, &g, &temp);

  /*writing sensor readings into the json document*/
  doc["Xa"]= a.acceleration.x;
  doc["Ya"]= a.acceleration.y;
  doc["Za"]= a.acceleration.z;
  doc["Xr"]= g.gyro.x;
  doc["Yr"]= g.gyro.y;
  doc["Zr"]= g.gyro.z;

  serializeJson(doc,output);// Write JSON to output buffer
  // Alternatively generate a prettified JSON document
  //serializeJsonPretty(doc, output);

  
  Serial.println("");
  delay(500);
}
