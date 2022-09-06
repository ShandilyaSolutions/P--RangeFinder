/*Code V2.0*/

//For MPU6050 chip
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
//For I2C communication
#include <Wire.h>
//For WiFi
#include <WiFiManager.h>
//For sending sensor readings to websocket
#include <ArduinoJSON.h>
//For websocket implementation
#include <WebServer.h>
#include <WebSocketsServer.h>

Adafruit_MPU6050 mpu;
WiFiManager wm;
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

/*Global variables....*/
String jsonString;
bool res;
char output[1024];
int interval = 100;
unsigned long previousMillis = 0;

/*Website*/
String web = "<!DOCTYPE html><html><head><title>ESP32 WebServer</title><style>h2 { font-family: \"Georgia\", Times, serif; font-style: italic; font-weight: bolder; font-size: 25px; color: #f05513; text-shadow: 3px 3px 3px #ababab;}table { border: 1px; border-style: solid; border-collapse: collapse; width: 100%; background-color: azure;}tr { height: 80px; border: 1px; border-style: solid; vertical-align: middle; text-align: center;}td { border: 1px ; border-style: solid; height: 80px; text-align: left; vertical-align: middle;}h5 { margin-left: 25px; font-family: \"Georgia\", Times, serif; font-style: normal; font-size: medium; font-weight: normal; text-shadow: 3px 3px 3px #ababab;}p { color: black; font-size: 15px; font-family: \"Georgia\", Times, serif;}div { text-align: left;}h4 { font-family: Georgia, 'Times New Roman', Times, serif; font-size: 20px; font-style: italic; font-weight: lighter;}.sensorReadings { width: 70px; height: 30px; padding: 5px; position: relative; left: 230px; bottom: 60px; margin-right: 10px;}.dataTable { text-align: center;}</style></head><body><script>var Socket; //holds the socket objectfunction init() { Socket = new WebSocket('ws://'+window.location.hostname+':81'); // Asks to connect to a web socket server with the IP address on 81 Socket.onmessage = function(event) { // onmessage event occures everytime new sensor data is received and we call the processCommand function processCommand(event); };}function processCommand(event) { //this function is used to parse the incoming JSON holding sensor data and puts it's value in their respective places var obj = JSON.parse(event.data); //inserting the data of accelerometer & gyroscope document.getElementById('Xa').innerHTML=obj.Xa; document.getElementById('Ya').innerHTML=obj.Ya; document.getElementById('Za').innerHTML=obj.Za; document.getElementById('Xr').innerHTML=obj.Xr; document.getElementById('Yr').innerHTML=obj.Yr; document.getElementById('Zr').innerHTML=obj.Zr; //displaying the variables console.log(\"Acceleration in X :\",obj.Xa); console.log(\"Acceleration in Y :\",obj.Ya); console.log(\"Acceleration in Z :\",obj.Za); console.log(\"Rotation in X : \",obj.Xr); console.log(\"Rotation in Y : \",obj.Yr); console.log(\"Rotation in Z : \",obj.Zr); }window.onload = function(event){ init();}</script><h2 style=\"text-align:center\">MPU6050 data</h2><table class=\"dataTable\"><tr><td><h5 class=\"grid-item\" style=\"text-align:left\">Accelerometer in X-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Xa\" name=\"Xa\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Accelerometer in Y-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Ya\" name=\"Ya\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Accelerometer in Z-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Za\" name=\"Za\" autofocus readonly></input></td></tr><tr><td><h5 class=\"grid-item\" style=\"text-align:left\">Gyroscope in X-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Xr\" name=\"Xr\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Gyroscope in Y-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Yr\" name=\"Yr\" autofocus readonly></input></td><td><h5 class=\"grid-item\" style=\"text-align:left\">Gyroscope in Z-axis : </h5><input class=\"sensorReadings\" type=\"hidden\" id=\"Za\" name=\"Zr\" autofocus readonly></input></td></tr></table><div><!--<br> tag is used for printing new line on the screen for cleaner look--><br><br><h4><u>Percentage Error : </u></h4><p>Accelerometer X : </p><p>Accelerometer Y : </p><p>Accelerometer Z : </p><p>Gyroscope X : </p><p>Gyroscope Y : </p><p>Gyroscope Z : </p></div></body></html>";


/*JSON  Document*/
//const int capacity = JSON_OBJECT_SIZE(6);
//StaticJsonDocument<capacity> json;

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

int setWebServer(){
    //Code to set up Webserver 
    Serial.print(" IP address : ");
    Serial.println(WiFi.localIP());

    //Initilize the server
    server.on("/",[](){
        server.send(200, "text",web);
        //send the webpage to IP as text.
        //Alternatively : server.send(200, "text\html", web);
    });

    server.begin();     //start the server
    webSocket.begin();  //start the websocket

    webSocket.onEvent(WebSocketEvent);
}

int realFunction(){
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

    serializeJson(doc, jsonString); // serialize the object and save teh result to teh string variable.
    Serial.println( jsonString ); // print the string for debugging
    webSocket.broadcastTXT(doc); //send the JSON document to webSocket

    doc = ""; //clears the document for using it again.
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length){
    switch (type){
    {
    case WStype_DISCONNECTED:
        Serial.print("WS Type ");
        Serial.print(type);
        Serial.println(" : Disconnected");
        break;
    
    case WStype_CONNECTED:
        Serial.print("WS Type ");
        Serial.print(type);
        Serial.println(" : Connected");
        break;

    case WStype_TEXT:
        /*Checks response from the client. if the response is 'Compute' then it will call a python function...*/
        Serial.println(payload[0]); //payload variable stores tech status internally
        if (payload[0]=='COMPUTE'){
            Serial.println("Calling the Python Function to Compute the Task!"); 
        }
        break;
    }
}


void setup(){
    Serial.begin(115200);
    int a,b;
    a = setupWiFi();
    if(a==1){
        Serial.println("WiFi connection successful");
        b = setupMpu();
        if(b==1)
            Serial.println("MPU6050 chip set correctly");
        else if (b==0)
        {
            Serial.println("MPU6050 chip could not be set correctly !");
        }
    }
    else if (a == 0)
    {
        Serial.println("WiFi connection failed!");
        // ESP.restart();
    }
    
}

void loop(){
    server.handleClient(); //to handle all server clients
    websocket.loop();   //to handle all websocket clients
    webSocket.onEvent(WebSocketEvent);  //init the webSocketEvent function when a websocket event occurs 
    //realFunction();
}
