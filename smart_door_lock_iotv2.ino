#include <ArduinoJson.h> // Install first on library manager
#include <CTBot.h> // Install first on library manager
#include <Servo.h> // // Install first on library manager / menyertakan library servo ke dalam program 
CTBot myBot;
Servo myServo; //define servo name

// Replace with your network credentials
String ssid = "LAPTOP"; // SSID WiFi
String pass = "12345678"; // Password WiFi

// Initialize Telegram BOT
String token = "YOUR_TOKEN"; // Telegram Token Channel
// your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
const int CHAT_ID = 01234567; // Your Chat ID

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

int pos = 0;
int statusPintu = 0;
int servoPin = D5;
int ledGreenPin = D12;
int ledRedPin = D11;
int buzzerPin = D10;

// Handle what happens when you receive new messages
void setup() {
  Serial.begin(115200);

  Serial.println("Starting TelegramBot...");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection()) {
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Jelek");
  }
  myServo.attach(servoPin); //servo pin
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
}


void kirim(){
  if(statusPintu){
    myBot.sendMessage(CHAT_ID, "Pintu Berhasil Terbuka!", "");
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledGreenPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    myServo.write(0); //Posisi servo pada 0 derajat
    delay(1000); //Delay
    myServo.detach();
    digitalWrite(buzzerPin, LOW);    
  }else{
    myBot.sendMessage(CHAT_ID, "Pintu Berhasil Terkunci!", "");
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledGreenPin, LOW);
    delay(1000);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);          
    myServo.write(180); //Posisi servo pada 0 derajat
    delay(1000); //Delay
    myServo.detach();
    digitalWrite(buzzerPin, LOW);    
  }
}


void loop() {
    TBMessage pesan;
    if (myBot.getNewMessage(pesan)) {
      if(pesan.sender.id != CHAT_ID)
      {
        myBot.sendMessage(pesan.sender.id, "Kamu tidak memiliki AKSES!!!");
      }else{
        if (pesan.text.equalsIgnoreCase("/start")) {
          String welcome = "Welcome!\n";
          welcome += "Use the following commands to control your outputs.\n\n";
          welcome += "/open_door to turn GPIO ON \n";
          welcome += "/close_door to turn GPIO OFF \n";
          welcome += "/state to request current GPIO state \n";
          myBot.sendMessage(CHAT_ID, welcome, "");
        }
        if (pesan.text.equalsIgnoreCase("/open_door")) {
          statusPintu = 1;
          kirim();
        }
      
        if (pesan.text.equalsIgnoreCase("/close_door")) {
          statusPintu = 0;
          kirim();
        }
      
        if (pesan.text.equalsIgnoreCase("/state")) {
          if (statusPintu){
            myBot.sendMessage(CHAT_ID, "Pintu Status Terbuka!");
          }
          else{
            myBot.sendMessage(CHAT_ID, "Pintu Status Tertutup!");
          }
        }
      }
    }else{
      Serial.println("Tidak ada pesan");
    }
}
