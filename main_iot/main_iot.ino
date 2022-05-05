#include <WiFiUdp.h>
#include <ArduinoOTA.h> 
#include <WiFi.h>
#include <ESPmDNS.h>
#include <FastLED.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "homepage.h"
// LED //
#define LED_PIN     25
#define NUM_LEDS    90
#define BRIGHTNESS  128
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
// Web //
AsyncWebServer server(80);
const char* ssid     = "LabPi";     //改成您的SSID 
const char* password = "12345678";   //改成您的密碼

IPAddress local_IP(192, 168, 9, 139);
IPAddress gateway(192, 168, 9, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(101, 101, 101, 101); // optional
IPAddress secondaryDNS(168, 95, 1, 1); // optional

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<div class='box'><label class='switch'><p>3F選手室門禁</p><input type='checkbox'id=\"5\" " + outputState(5) + "><span></span></label></div>";
//    buttons += "<p>Output - GPIO 5</p><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(5) + "><span class=\"slider\"></span></label>";
    //buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    //buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}


void serverRoute(){
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  if(!request->authenticate(http_username, http_password))
    return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
    
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html, processor);
  });
  
  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
//    if(!request->authenticate(http_username, http_password))
//      return request->redirect("/");
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    delay(50000);
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}


void otaSetup(){
    // 埠號預設為8266
  // ArduinoOTA.setPort(8266);   // 沿用預設值
  ArduinoOTA.setHostname("TCIVSCSE-mainiot");
  
  // 預設無需驗證密碼
  ArduinoOTA.setPassword((const char *)"studyh0rd");  // 密碼設定為"12345678"
  // 使用COM Port 列出取得的IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //Get Current Hostname
  // OTA「開始」的事件處理程式
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  
  // OTA「結束」的事件處理程式
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  
  // OTA「進行中」的事件處理程式
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  // OTA「錯誤」的事件處理程式
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  // 啟用OTA
  ArduinoOTA.begin();
  // OTA End //

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

void wifiSetup(){
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  // 使用SSID 跟 password 連線基地台
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void setup() {
  delay( 3000 ); // power-up safety delay
  Serial.begin(115200);
  wifiSetup();
  otaSetup();
  serverRoute();
  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

void changeLEDMode(){
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void loop() {
  // OTA Start //
  ArduinoOTA.handle();
  // OTA End //
  
}
