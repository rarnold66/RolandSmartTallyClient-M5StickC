#define C_PLUS 0 //CHANGE TO 1 IF YOU USE THE M5STICK-C PLUS

#if C_PLUS == 1
#include <M5StickCPlus.h>
#else
#include <M5StickC.h>
#endif

#include <WiFi.h>
#include <HTTPClient.h>
#include <PinButton.h>
#include <Preferences.h>

#define GRAY  0x0020 //   8  8  8
#define GREEN 0x0200 //   0 64  0
#define RED   0xF800 // 255  0  0
#define ERROR  0xFE19 //pink

/* USER CONFIG VARIABLES
 *  Change the following variables before compiling and sending the code to your device.
 */

//Wifi SSID and password
const char * networkSSID = "Beelduitzending";
const char * networkPass = "V1d30_c0nnEcT";

//For static IP Configuration, change USE_STATIC to true and define your IP address settings below
bool USE_STATIC = false; // true = use static, false = use DHCP

IPAddress clientIp(192, 168, 2, 5); // Static IP
IPAddress subnet(255, 255, 255, 0); // Subnet Mask
IPAddress gateway(192, 168, 2, 1); // Gateway


//M5StickC variables
PinButton btnM5(37); //the "M5" button on the device
PinButton btnAction(39); //the "Action" button on the device
Preferences preferences;
uint8_t wasPressed();
const byte led_program = 10;
const int led_preview = 26;   //OPTIONAL Led for preview on pin G26
const int waittime = 100;
//RST variables
String TallyUrl = "http://192.168.100.240/tally/1/status"; //DeviceID's for XS62-S 1 tot 6
String DeviceName = "CAM 1";
String modus = "unselected";
String previousModus = "";

//General Variables
bool networkConnected = false;
int currentScreen = 0; //0 = Tally Screen, 1 = Settings Screen
int currentBrightness = 11; //12 is Max level

void setup() {
  pinMode (led_preview, OUTPUT);
  Serial.begin(115200);
  while (!Serial);

  // Initialize the M5StickC object
  logger("Initializing M5StickC+.", "info-quiet");
  M5.begin();
  setCpuFrequencyMhz(80);    //Save battery by turning down the CPU clock
  btStop();                 //Save battery by turning off BlueTooth
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0);
   M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  logger("Roland Smart Tally M5StickC+ http Client booting.", "info");

  delay(100); //wait 100ms before moving on
  connectToNetwork(); //starts Wifi connection
  while (!networkConnected) {
    delay(200);
  }

  // Enable interal led for program trigger
  pinMode(led_program, OUTPUT);
  digitalWrite(led_program, HIGH);
}

void loop() {
  btnM5.update();
  btnAction.update();
  getTallyData();
  showDeviceInfo();
  if (btnM5.isClick()) {
    showSettings();
    delay(10000);
    }
  if (btnAction.isClick()) {
    updateBrightness();
  }
  delay(waittime);
}

void showSettings() {
  //displays the current network connection and url / device
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("SSID: " + String(networkSSID));
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.println();
  M5.Lcd.println("Rolamd Smart Tally Url:");
  M5.Lcd.println(TallyUrl);
  M5.Lcd.print("DeviceName: " ); M5.Lcd.println(DeviceName);
  M5.Lcd.print("Modus: "); M5.Lcd.println(modus);

//  M5.Lcd.println("Battery:");
//  int batteryLevel = floor(100.0 * (((M5.Axp.GetVbatData() * 1.1 / 1000) - 3.0) / (4.07 - 3.0)));
//  batteryLevel = batteryLevel > 100 ? 100 : batteryLevel;
//   if(batteryLevel >= 100){
//  M5.Lcd.println("Battery charging...");   // show when M5 is plugged in
//  }
//  else {
//    M5.Lcd.println("Battery:" + String(batteryLevel) + "%");
//    }
}

void showDeviceInfo() {
  //displays the currently assigned device and tally data
  evaluateMode();
}

void updateBrightness() {
  if(currentBrightness >= 12) {
    currentBrightness = 7;
  }
  else {
    currentBrightness++;
  }
  M5.Axp.ScreenBreath(currentBrightness);
}

void logger(String strLog, String strType) {
  if (strType == "info") {
    Serial.println(strLog);
    M5.Lcd.println(strLog);
  }
  else {
    Serial.println(strLog);
  }
}

void connectToNetwork() {
  logger("Connecting to SSID: " + String(networkSSID), "info");

  WiFi.disconnect(true);
  WiFi.onEvent(WiFiEvent);

  WiFi.mode(WIFI_STA); //station
  WiFi.setSleep(false);

  if(USE_STATIC == true) {
    WiFi.config(clientIp, gateway, subnet);
  }

  WiFi.begin(networkSSID, networkPass);
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      logger("Network connected!", "info");
      logger(WiFi.localIP().toString(), "info");
      networkConnected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      logger("Network connection lost!", "info");
      networkConnected = false;
      break;
  }
}

void Flash() {
      M5.Lcd.fillScreen(TFT_BLACK);
      digitalWrite(led_program, LOW);
      delay(300);
      M5.Lcd.fillScreen(WHITE);
      digitalWrite(led_program, HIGH);
      delay(300);
}

void ShowError() {
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  Flash();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.fillScreen(PINK);
  digitalWrite(led_program, HIGH);
  digitalWrite (led_preview, HIGH);
  M5.Lcd.println("Error");
}

void getTallyData() {
  HTTPClient http;
  http.begin(TallyUrl); //HTTP
  int httpcode = http.GET();
  if(httpcode != HTTP_CODE_OK){
    modus = httpcode;
    //Flash();
  }
  else {
    modus = http.getString();
   }
}

void evaluateMode() {
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.setTextSize(5);
  if (modus != previousModus) {
    m5.Lcd.fillScreen(BLACK);
    if (modus == "selected") {
      logger("The device is in preview.", "info-quiet");
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.fillScreen(GREEN);
      digitalWrite(led_program, HIGH);
      digitalWrite (led_preview, HIGH);
      M5.Lcd.println(DeviceName);
      previousModus = modus;
    }
    else if (modus == "onair") {
      logger("The device is in program.", "info-quiet");
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.fillScreen(RED);
      digitalWrite(led_program, LOW);
      digitalWrite(led_preview, LOW);
      M5.Lcd.println(DeviceName);
      previousModus = modus;
    }
    else if (modus == "unselected") {
      logger("The device is unselected.", "info-quiet");
      digitalWrite(led_program, HIGH);
      digitalWrite(led_preview, LOW);
      M5.Lcd.setTextColor(GRAY);
      M5.Lcd.fillScreen(TFT_BLACK);
      M5.Lcd.println(DeviceName);
      previousModus = modus;
    }
    else {
      logger("Error getting http data.", "info-quiet");
      ShowError();
      previousModus = modus;
    }
  }
}
