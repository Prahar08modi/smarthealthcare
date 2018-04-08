  #include <ESP8266WiFi.h>
  
  //#include <SPI.h>
  //#include <MFRC522.h>
  
  //#define SS_PIN D4
  //#define RST_PIN D3
  //MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
  //byte readCard[4];  

  const char* ssid     = "Avengers";
  const char* password = "prahar@156";
  WiFiClient client;
  const int httpPort = 8021;
  const char* host = "192.168.43.17";
  
  void setup() {
    Serial.begin(115200); 
    delay(10);
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
      Serial.println();
      // Dump debug info about the card. PICC_HaltA() is automatically called.
      //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    while (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
    }
  
    Serial.println("Client Printing: Sending Data");
    client.println("2, 8, 5, 2, 6");
    delay(2000);
    Serial.println("Client Printing: Recieving Data");
    Serial.print("Message From Server: ");
    Serial.println(client.readStringUntil('Z'));
    delay(2000);
  }
