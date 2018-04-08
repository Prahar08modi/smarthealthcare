  #include <ESP8266WiFi.h>
  #include <Wire.h>

  // MPU6050 Slave Device Address
  const uint8_t MPU6050SlaveAddress = 0x68;
  
  // Select SDA and SCL pins for I2C communication 
  const uint8_t scl = D6;
  const uint8_t sda = D7;
  
  // sensitivity scale factor respective to full scale setting provided in datasheet 
  const uint16_t AccelScaleFactor = 16384;
  const uint16_t GyroScaleFactor = 131;
  
  // MPU6050 few configuration register addresses
  const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
  const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
  const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
  const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
  const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
  const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
  const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
  const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
  const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
  const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
  const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;
  
  int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;
  //#include <SPI.h>
  //#include <MFRC522.h>
  
  //#define SS_PIN D4
  //#define RST_PIN D3
  //MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
  //byte readCard[4];  

  const char* ssid     = "Avengers";
  const char* password = "prahar@156";
  WiFiClient client;
  const int httpPort = 8080;
  const char* host = "192.168.43.239";
  
  void setup() {
    Serial.begin(9600);
    delay(10);
    Wire.begin(sda, scl);
    MPU6050_Init(); 
    pinMode(5,OUTPUT);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    while (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
    }

//    SPI.begin();      // Init SPI bus
//    mfrc522.PCD_Init(); // Init MFRC522 card
//    Serial.println("Scan PICC to see UID and type...");
    
  }
  
  int value = 0;
  
  void loop() {
  //  client.print("closing connection");
  //  delay(2000);
  //  client.print("Aagam");
  //  delay(2000);
  //  analogWrite(5,200);
  //  delay(500);
  //  Serial.println(client.readStringUntil('Z'));


//      if ( ! mfrc522.PICC_IsNewCardPresent()) {
//        return;
//      }
//    
//      // Select one of the cards
//      if ( ! mfrc522.PICC_ReadCardSerial()) {
//        return;
//      }
//    
//      for (int i = 0; i < 4; i++) {  //
//        readCard[i] = mfrc522.uid.uidByte[i];
//        Serial.print(readCard[i], HEX);
//      }

      double Ax, Ay, Az, T, Gx, Gy, Gz;
      
      Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
      
      //divide each with their sensitivity scale factor
      Ax = (double)AccelX/AccelScaleFactor;
      Ay = (double)AccelY/AccelScaleFactor;
      Az = (double)AccelZ/AccelScaleFactor;
      T = (double)Temperature/340+36.53; //temperature formula
      Gx = (double)GyroX/GyroScaleFactor;
      Gy = (double)GyroY/GyroScaleFactor;
      Gz = (double)GyroZ/GyroScaleFactor;
    
      Ax = (double)Ax*9.80665;
      Ay = (double)Ay*9.80665;
      Az = (double)Az*9.80665;
    
      Serial.print("Ax: "); Serial.print(Ax);
      Serial.print(" Ay: "); Serial.print(Ay);
      Serial.print(" Az: "); Serial.print(Az);
      //Serial.print(" T: "); Serial.print(T);
      Serial.print(" Gx: "); Serial.print(Gx);
      Serial.print(" Gy: "); Serial.print(Gy);
      Serial.print(" Gz: "); Serial.println(Gz);
    
      delay(100);
      Serial.println();
      // Dump debug info about the card. PICC_HaltA() is automatically called.
      //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    while (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
    }
  String value;
    value= (String)Ax + "," + (String)Ay + "," + (String)Az+ "," + (String)Gx+ "," + (String)Gy+ "," + (String)Gz + "l";
    Serial.println("Client Printing: Sending Data");
//    client.println("\"");
    client.print(value);
    //client.print(",");
  //  client.print((String)Ay);
//    client.print(",");
//    client.print(Az);
//    client.print(",");
//    //client.println(Ax);
//    //client.println(",");
//    client.print(Gx);
//    client.print(",");
//    client.print(Gy);
//    client.print(",");
//    client.print(Gz);
////  client.println(Ax);
    //client.println("l");
    //delay(500);
    Serial.println("Client Printing: Recieving Data");
    Serial.print("Message From Server: ");
    Serial.println(client.readStringUntil('Z'));
    //delay(2000);
}

void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
