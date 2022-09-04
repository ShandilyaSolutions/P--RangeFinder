var Socket; //holds the socket object

function init() {
    Socket = new WebSocket('ws://'+window.location.hostname+':81');
    // Asks to connect to a web socket server with the IP address on 81
    Socket.onmessage = function(event) {
        // onmessage event occures everytime new sensor data is received and we call the processCommand function
        processCommand(event);
    };
}

function processCommand(event) {
    //this function is used to parse the incoming JSON holding sensor data and puts it's value in their respective places 
    var obj = JSON.parse(event.data);
    //inserting the data of accelerometer & gyroscope
    document.getElementById('Xa').innerHTML=obj.Xa;
    document.getElementById('Ya').innerHTML=obj.Ya;
    document.getElementById('Za').innerHTML=obj.Za;
    document.getElementById('Xr').innerHTML=obj.Xr;
    document.getElementById('Yr').innerHTML=obj.Yr;
    document.getElementById('Zr').innerHTML=obj.Zr;

    //displaying the variables
    console.log("Acceleration in X :",obj.Xa);
    console.log("Acceleration in Y :",obj.Ya);
    console.log("Acceleration in Z :",obj.Za);
    console.log("Rotation in X : ",obj.Xr);
    console.log("Rotation in Y : ",obj.Yr);
    console.log("Rotation in Z : ",obj.Zr);    
}

window.onload = function(event){
    init();
}
