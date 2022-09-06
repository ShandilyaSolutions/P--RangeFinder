/*Code V1*/
#include <Adafruit_MPU6050.h> //For Interfacing with MPU6050
#include <Adafruit_Sensor.h>
#include <Wire.h> //For implementing I2C communication protocol
#include <WiFiManager.h> //For WiFi
#include <ArduinoJson.h> //For Data transfer using JSON 

/*Libraries for Websocket*/
#include <WebSocketsServer.h> //For WebSocket Server creation
#include <WebServer.h>

Adafruit_MPU6050 mpu; //creates an instance of Adafruit_MPU6050 class
WiFiManager wm; //creates an instance of WiFiManager class


//Global Variables
bool res; //to report the result of WiFi connection attempt done by us
char output[1024];//buffer to hold output results. Size of 1024 i.e. 2^10 is picked arbitarily
int interval = 100; // virtual delay of 100ms
unsigned long previousMillis = 0; // Tracks the time since last event fired

//Creating JSON document
const int capacity = JSON_OBJECT_SIZE(6);
StaticJsonDocument<capacity> doc;

WebServer server(80);  // create instance for web server on port "80"
WebSocketsServer webSocket = WebSocketsServer(81);  //create instance for webSocket server on port"81"

String web = "<!DOCTYPE html><html><head><title>ESP32 WebServer</title><style>h2 { font-family: \"Georgia\", Times, serif; font-style: italic; font-weight: bolder; font-size: 25px; color: #f05513; text-shadow: 3px 3px 3px #ababab;}table { border: 1px; border-style: solid; border-collapse: collapse; width: 100%; background-color: azure;}tr { height: 80px; border: 1px; border-style: solid; vertical-align: middle; text-align: center;}td { border: 1px ; border-style: solid; height: 80px; text-align: left; vertical-align: middle;}h5 { margin-left: 25px; font-family: \"Georgia\", Times, serif; font-style: normal; font-size: medium; font-weight: normal; text-shadow: 3px 3px 3px #ababab;}p { color: black; font-size: 15px; font-family: \"Georgia\", Times, serif;}div { text-align: left;}h4 { font-family: Georgia, 'Times New Roman', Times, serif; font-size: 20px; font-style: italic; font-weight: lighter;}.sensorReadings { width: 70px; height: 30px; padding: 5px; position: relative; left: 230px; bottom: 60px; margin-right: 10px;}.dataTable { text-align: center;}</style></head><body><script>var Socket; //holds the socket objectfunction init() { Socket = new WebSocket('ws://'+window.location.hostname+':81'); // Asks to connect to a web socket server with the IP address on 81 Socket.onmessage = function(event) { // onmessage event occures everytime new sensor data is received and we call the processCommand function processCommand(event); };}function processCommand(event) { //this function is used to parse the incoming JSON holding sensor data and puts it's value in their respective places var obj = JSON.parse(event.data); //inserting the data of accelerometer & gyroscope document.getElementById('Xa').innerHTML=obj.Xa; document.getElementById('Ya').innerHTML=obj.Ya; document.getElementById('Za').innerHTML=obj.Za; document.getElementById('Xr').innerHTML=obj.Xr; document.getElementById('Yr').innerHTML=obj.Yr; document.getElementById('Zr').innerHTML=obj.Zr; //displaying the variables console.log(\"Acceleration in X :\",obj.Xa); console.log(\"Acceleration in Y :\",obj.Ya); console.log(\"Acceleration in Z :\",obj.Za); console.log(\"Rotation in X : \",obj.Xr); console.log(\"Rotation in Y : \",obj.Yr); console.log(\"Rotation in Z : \",obj.Zr); }window.onload = function(event){ init();}</script><h2 style=\"text-align:center\">MPU6050 data</h2><table class=\"dataTable\"><tr><td><h5 class=\"grid-item\" style=\"text-align:left\">Accelerometer in X-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Xa\" name=\"Xa\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Accelerometer in Y-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Ya\" name=\"Ya\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Accelerometer in Z-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Za\" name=\"Za\" autofocus readonly></input></td></tr><tr><td><h5 class=\"grid-item\" style=\"text-align:left\">Gyroscope in X-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Xr\" name=\"Xr\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Gyroscope in Y-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Yr\" name=\"Yr\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Gyroscope in Z-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Za\" name=\"Zr\" autofocus readonly></input></td></tr></table><div><!--<br> tag is used for printing new line on the screen for cleaner look--><br><br><h4><u>Percentage Error : </u></h4><p>Accelerometer X : </p><p>Accelerometer Y : </p><p>Accelerometer Z : </p><p>Gyroscope X : </p><p>Gyroscope Y : </p><p>Gyroscope Z : </p></div></body></html>";

int setupWiFi(){
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
  return 1;
}

int setupMPU6050(){
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
  return 1;
}

void mpuData(){
  //This function collects the sensor data, convert them into a JSON and send it over websocket everytime it is called
  
  /* Get new sensor events with the readings. We will get all the data but not display the temperature as it is not required for our application.
      The option to not fetch the temp data was not supported by the library as it caused errors on compilation.*/
  sensors_event_t a, g, temp;
  //a = accelerometer, g = gyroscope, temp = temperature sensor
  mpu.getEvent(&a, &g, &temp);

  /*putting sensor readings into the json document*/
  doc["Xa"]= a.acceleration.x;
  doc["Ya"]= a.acceleration.y;
  doc["Za"]= a.acceleration.z;
  doc["Xr"]= g.gyro.x;
  doc["Yr"]= g.gyro.y;
  doc["Zr"]= g.gyro.z;

  serializeJson(doc,output);// Write JSON to output buffer
  // Alternatively generate a prettified JSON document
  //serializeJsonPretty(doc, output);

  webSocket.broadcastTXT(doc); // send the JSON object through the websocket
  doc = ""; // clear the String.
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: // enum that read status this is used for debugging.
      Serial.print("WS Type ");
      Serial.print(type);
      Serial.println(": DISCONNECTED");
      break;
    case WStype_CONNECTED:  // Check if a WebSocket client is connected or not
      Serial.print("WS Type ");
      Serial.print(type);
      Serial.println(": CONNECTED");
    }
}

void setup() {
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

  //Code for setup of webserver(could be made into another function
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  
  // Initialize a web server on the default IP address. and send the webpage as a response.
  server.on("/", []() {
    server.send(200, "text", web); //send the html webpage to the server
  });
  server.begin(); // init the server
  webSocket.begin();  // init the Websocketserver
  webSocket.onEvent(webSocketEvent); //init the webSocketEvent function when a websocket event occurs 
}

void loop() {

  server.handleClient();  // webserver methode that handles all Client
  webSocket.loop(); // websocket server methode that handles all Client
  unsigned long currentMillis = millis();

  if ((unsigned long)(currentMillis - previousMillis) >= interval) { // How much time has passed, accounting for rollover with subtraction!
    mpuData(); //call the function 
    previousMillis = currentMillis;   // Use the snapshot to set track time until next event
  }


  delay(500);
}
