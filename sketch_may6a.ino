//Thu vien
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//Ket noi wifi
#define WIFI_SSID "Padoithong"
#define WIFI_PASSWORD "667788200801"

//Cau hinh Firebase
#define FIREBASE_HOST "baidoxethongminh-esp32-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "OxwOsQJ3RURRP5TFonoYxQ2ZCfF0oUl0fg57oGSQ"

//Cac chan du lieu su dung
#define ir_enter 35
#define ir_exit 19
#define ir_car1 32
#define ir_car2 33
#define ir_car3 25
#define ir_car4 26

LiquidCrystal_I2C lcd(0x27, 20, 4);


Servo servoEnter;              
Servo servoExit;

int temp = 0;
int slot = 4;

#define RXD2 16         // GPIO 16 => RX for uart
#define TXD2 17         // GPIO 17 => TX for uart

FirebaseData fbdb; //bien quan ly firebase

String qrcode;

String getSubstring(String data, String startDelimiter, String endDelimiter) {
  int startIndex = data.indexOf(startDelimiter);
  if (startIndex == -1) {
    return "";
  }
  startIndex += startDelimiter.length();
  int endIndex = data.indexOf(endDelimiter, startIndex);
  if (endIndex == -1) {
    return data.substring(startIndex);
  }
  return data.substring(startIndex, endIndex);
}

// Đặt múi giờ
const long utcOffsetInSeconds = 3600*7; // UTC+7

// Khởi tạo WiFi và NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String data_entry, data_exit;
unsigned long entryTimes[4] = {0, 0, 0, 0};
bool slotOccupied[4] = {false, false, false, false};
int totalMoney;

String getCurrentTimeString() {
  int h = timeClient.getHours();
  int m = timeClient.getMinutes();
  int s = timeClient.getSeconds();
  return String(h) + ":" + String(m) + ":" + String(s);
}

void setup() {

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.begin (115200);
  servoEnter.attach(14);
  servoExit.attach(12);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);
  pinMode(ir_enter, INPUT);
  pinMode(ir_exit, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Smart Parking Place");
  delay(500);
  lcd.clear();

  Serial.print("Connecting to WIFI...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(5000);
    Serial.print(".");
  }
  //ket noi firebase database
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Serial.print("Connected with IP: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  // Khởi động NTP Client
  timeClient.begin();
  timeClient.update(); // Cập nhật thời gian lần đầu
}

void loop() {
  timeClient.update(); // Cập nhật thời gian thường xuyên
  if (slot > 0 && slot < 5)
    {
      lcd.setCursor(0, 0);
      lcd.print("Have slot: ");
      lcd.setCursor(12, 0);
      lcd.print(slot);
    }
  else
    {
        lcd.setCursor(0, 0);
        lcd.print("Full slot");
    }
    KiemTra();
    gate();
    time();
}

//Chuong trinh con
void gate() 
{
    if (digitalRead(ir_enter) == 0)
    {
      lcd.setCursor(0, 1);
      lcd.print("Scan QR to entry:");
      MaQRvao();
    }
    if (slot < 4){
      if (digitalRead(ir_exit) == 0)
      {
        lcd.setCursor(0, 1);
        lcd.print("Scan QR to exit:");
        MaQRra();
      }
    }  
}

void openDoorEnter() 
{
    // Mở cửa vào
    servoEnter.write(90);
    delay(2000);
    // Đóng cửa vào
    servoEnter.write(0);
    delay(1000);
}

void openDoorExit() 
{
    // Mở cửa ra
    servoExit.write(90);
    delay(2000);
    // Đóng cửa ra
    servoExit.write(0);
    delay(1000);   
}

void MaQRvao()
{
  while (Serial2.available())
  {
    qrcode = Serial2.readString();
    Serial.println(qrcode);
    String slotString = getSubstring(qrcode, "Slot: ", "\n");
    Serial.println("Gia tri Slot da cat: '" + slotString + "'"); // In ra giá trị slot đã cắt
        if (slotString == "Slot1" || slotString == "Slot2" || slotString == "Slot3" || slotString == "Slot4")
        {
            Serial.println("moi xe vao");
            openDoorEnter();     
            slot--;
            String basePath = "/Time/" + slotString;
            Firebase.setInt(fbdb, basePath + "/Trangthai", 0);
            lcd.clear();  
        } else {
      Serial.println("Khong hop le");}
  }
}
    

void MaQRra()
{
  while (Serial2.available())
  {
    qrcode = Serial2.readString();
    Serial.println(qrcode);
    String slotString = getSubstring(qrcode, "Slot: ", "\n");
    Serial.println("Gia tri Slot da cat: '" + slotString + "'"); // In ra giá trị slot đã cắt
        if (slotString == "Slot1" || slotString == "Slot2" || slotString == "Slot3" || slotString == "Slot4")
        {
            Serial.println("moi xe ra");
            openDoorExit();     
            slot++;
            String basePath = "/Time/" + slotString;
            Firebase.setInt(fbdb, basePath + "/Trangthai", 1);
            lcd.clear();  
        } else {
      Serial.println("Khong hop le");}
  }
}

void KiemTra()
{
  if(Firebase.getInt(fbdb, "Status/Slot1") == true)
  {
  
    if (digitalRead(ir_car1) == 0)
    {
      lcd.setCursor(0, 2);
      lcd.print("Slot1: No");
      Firebase.setString(fbdb, "Status/Slot1","No");
    }
    else
    {
      lcd.setCursor(0, 2);
      lcd.print("Slot1: Ok");
      Firebase.setString(fbdb, "Status/Slot1","Ok");
    }
  }

  if(Firebase.getInt(fbdb, "Status/Slot2" ) == true)
  {
   
    if (digitalRead(ir_car2) == 0)
    {
      lcd.setCursor(0, 3);
      lcd.print("Slot2: No");
      Firebase.setString(fbdb, "Status/Slot2","No");
    }
    else
    {
      lcd.setCursor(0, 3);
      lcd.print("Slot2: Ok");
      Firebase.setString(fbdb, "Status/Slot2","Ok");
    }
  }

  if(Firebase.getInt(fbdb, "Status/Slot3" ) == true)
  {
    
    if (digitalRead(ir_car3) == 0)
    {
    lcd.setCursor(10, 2);
    lcd.print("Slot3: No");
    Firebase.setString(fbdb, "Status/Slot3","No");
    }
    else
    {
    lcd.setCursor(10, 2);
    lcd.print("Slot3: Ok");
    Firebase.setString(fbdb, "Status/Slot3","Ok");
    }
  }

  if(Firebase.getInt(fbdb, "Status/Slot4" ) == true)
  {
    
    if (digitalRead(ir_car4) == 0)
    {
      lcd.setCursor(10, 3);
      lcd.print("Slot4: No");
      Firebase.setString(fbdb, "Status/Slot4","No");
    }
    else
    {
      lcd.setCursor(10, 3);
      lcd.print("Slot4: Ok");
      Firebase.setString(fbdb, "Status/Slot4","Ok");
    }
  }
}

void time()
{
  // Mảng cảm biến
  int sensors[] = {ir_car1, ir_car2, ir_car3, ir_car4};
  
  for (int i = 0; i < 4; i++) {
    if (digitalRead(sensors[i]) == 0) {
      // Xe vào
      if (!slotOccupied[i]) {
        data_entry = getCurrentTimeString();
        Serial.println(data_entry);
        entryTimes[i] = millis();
        slotOccupied[i] = true;
        Firebase.setString(fbdb, String("Time/Slot") + (i + 1) + "/Entry", data_entry);
      }
    } else {
      // Xe ra
      if (slotOccupied[i]) {
        data_exit = getCurrentTimeString();
        Serial.println(data_exit);
        unsigned long elapsedTime = millis() - entryTimes[i];
        entryTimes[i] = 0; 
        slotOccupied[i] = false; 
        totalMoney = tinhtien(elapsedTime);
        Firebase.setString(fbdb, String("Time/Slot") + (i + 1) + "/Exit", data_exit);
        Firebase.setString(fbdb, String("Time/Slot") + (i + 1) + "/Total", String(totalMoney));
      }
    }
  }
}

int tinhtien(unsigned long elapsedTime)
{
  if (elapsedTime < 7200000) { // Ít hơn 2 giờ
    return 20000;
  } else if (elapsedTime >= 7200000 && elapsedTime < 10800000) { // Từ 2 giờ đến dưới 3 giờ
    return 30000;
  } else { // Từ 3 giờ trở lên
    return 50000;
  }
}