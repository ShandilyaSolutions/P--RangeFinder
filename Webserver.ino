/*Code Version 3.0*/
//FOr MPU6050 chip
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
//For I2C communication
#include <Wire.h>
//For WiFi
#include <WiFiManager.h>
//For JSON
#include <ArduinoJson.h>
//For WebSocket 
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


//Classes 
Adafruit_MPU6050 mpu;
WiFiManager wm;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); //handles the connections on the /ws path.

//Global Variables
String jsonString;
bool res;
char output[1024];

//Webpage code : 
//const char index_html[] PROGMEM = R"rawliteral()rawliteral";

int setupWiFi()
{
    //For Setting up WiFi connection
    while(!Serial)
        delay(10);
    WiFi.mode(WIFI_STA); //Set esp to WiFi Station

    res = wm.autoConnect("AutoConnectAP","password"); //Name and password of WiFi Network

    if(!res){
        return 0;
    }
    else {
        return 1;
    }
}

//void notifyClients(doc) {
//    //sends the json document 'doc' to the client --> our python code
//    ws.textAll(doc);
//}

void realFunction(){
    /*Code to get sensor readings convert into JSON and send it to webSocket*/
    
    //Create the JSON document
    const int capacity = JSON_OBJECT_SIZE(6);
    StaticJsonDocument<capacity> doc;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    doc["Xa"] = a.acceleration.x;
    doc["Ya"] = a.acceleration.y;
    doc["Za"] = a.acceleration.z;
    doc["Xr"] = g.gyro.x;
    doc["Yr"] = g.gyro.y;
    doc["Zr"] = g.gyro.z;

    serializeJson(doc, output);
    //Alternatively : serializeJsonPretty(doc, output);

    serializeJson(doc, jsonString); // serialize the object and save the result to the string variable.
    //Serial.println( jsonString ); // print the string for debugging
    
    //send the JSON document to webSocket
    //notifyClients(doc);
    doc = ""; //clears the document for using it again.
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  Serial.println (WiFi.localIP());
}

int setupMpu(){
    //Function to set up our MPU6050 chip
    while(!Serial)
        delay(10);
    
    //Serial.println("MPU6050 test");

    if (!mpu.begin()){
        //Serial.println("Failed to find MPU6050 chip");
        return 0;
    }
    Serial.println("Setting up MPU6050 ...");
    
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.println("Accelerometer range set to +- 8g");
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.println("Gyroscope range set to +- 500 deg");
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("Filter bandwidth set to 21 hz");
    return 1;
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) 
{
    //Add event listener to handle different asynchronous steps of the WebSocket protocol
  switch (type) {
    //type represents events that occurs
    case WS_EVT_CONNECT:
      // when a client has logged in
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      // when a client has logged out
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      // when data packet is received from client
      //handleWebSocketMessage(arg, data, len);
      //break;
    case WS_EVT_PONG:
        //in response to ping request
    case WS_EVT_ERROR:
        //if an error is received from client
      break;
  }
}

/*void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}*/

void setup(){
    Serial.begin(115200);
    int a,b;
    a = setupWiFi();
    if(a==1){
        Serial.println("WiFi connection successful");
        b = setupMpu();
        if(b==1){
            Serial.println("MPU6050 chip set correctly");
            initWebSocket();
        }
        else if (b==0)
        {
            Serial.println("MPU6050 chip could not be set correctly !");
        }

        //Start Server
        server.begin();
    }
    else if (a == 0)
    {
        Serial.println("WiFi connection failed!");
        // ESP.restart();
    }
    
}

void loop() {
  ws.cleanupClients();
  //digitalWrite(ledPin, ledState);
}
