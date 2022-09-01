#include <Adafruit_MPU6050.h> //For Interfacing with MPU6050
#include <Adafruit_Sensor.h>
#include <Wire.h> //For implementing I2C communication protocol

Adafruit_MPU6050 mpu; //

void setup(void) {
  Serial.begin(115200);
  //Works on connection with MPU6050 
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

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
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  //sensors_event_t a, g, temp; (original)
  //a = accelerometer, g = gyroscope, temp = temperature sensor
  sensors_event_t a, g; //no need of temp data now
  //mpu.getEvent(&a, &g, &temp); (original)
  mpu.getEvent(&a, &g);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  // Serial.print("Temperature: ");
  // Serial.print(temp.temperature);
  // Serial.println(" degC");
  /*No need for temperature sensor readings*/
  
  Serial.println("");
  delay(500);
}
