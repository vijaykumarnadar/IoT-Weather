#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <SNTPtime.h>
#include <Ticker.h>
#include "html_file.h"
#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT11
#define button D5
#define firmware "iotUpdater_v1.1"
#define update_server "example.com"
#define update_uri "/update.php"
String art = R"(

  _____   _______  __          __        _   _                  _____ _        _   _             
 |_   _| |__   __| \ \        / /       | | | |                / ____| |      | | (_)            
   | |  ___ | |     \ \  /\  / /__  __ _| |_| |__   ___ _ __  | (___ | |_ __ _| |_ _  ___  _ __  
   | | / _ \| |      \ \/  \/ / _ \/ _` | __| '_ \ / _ \ '__|  \___ \| __/ _` | __| |/ _ \| '_ \ 
  _| || (_) | |       \  /\  /  __/ (_| | |_| | | |  __/ |     ____) | || (_| | |_| | (_) | | | |
 |_____\___/|_|        \/  \/ \___|\__,_|\__|_| |_|\___|_|    |_____/ \__\__,_|\__|_|\___/|_| |_| -By Vijay
                                                                                                 
                                                                                                 

)";
const char* host = "example.com"; // host url
DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 pressure;
Ticker secondTick;
SNTPtime NTPch("in.pool.ntp.org"); // SNTP Indian Server, Replace it according to your location.
strDateTime dateTime;
const int analogInPin = A0;
int sensorValue = 0;
ESP8266WebServer server(80);
const char *ssid = "ESP";
const char *password = "password";
char ssidc[30];
char passwordc[30];
char latc[30];
char lonc[30];
String string_Ssid="";
String string_Password="";
String string_lat="";
String string_lon="";
volatile int watchdogCount = 0;
void ISRwatchdog(){
  watchdogCount++;
  if(watchdogCount >= 60){
    Serial.println();
    Serial.println("dog bites");
    ESP.reset();
  }
}
void setup() {
  delay(1000);
  Serial.begin(115200);
  EEPROM.begin(512);
  pinMode(button, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  secondTick.attach(1,ISRwatchdog); // ISR to run every second
  Serial.println();
  if(digitalRead(button) || checkconf()){
    Serial.println(art);
    WAP();
  }
  else
  {
    Serial.println(art);
    IOT();
  }
}
void loop(){}
void IOT()
{
  string_Ssid = read_string(30,5); 
  string_Password = read_string(30,100);
  string_lat = read_string(30,200); 
  string_lon = read_string(30,300);
  Serial.println("ssid: "+string_Ssid);
  Serial.println("Password: "+string_Password);
  Serial.println("lat: "+string_lat);
  Serial.println("lon: "+string_lon);
  string_Password.toCharArray(passwordc,30);
  string_Ssid.toCharArray(ssidc,30);  
  string_lat.toCharArray(latc,30);
  string_lon.toCharArray(lonc,30);
  dht.begin();
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    ESP.reset();
  }
  Serial.println();
  Serial.println();
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  Serial.print("Connecting to ");
  Serial.println(ssidc);
  WiFi.begin(ssidc, passwordc);
  int onoff=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(onoff==0){
      digitalWrite(LED_BUILTIN, HIGH);
      onoff=1;
    }else{
     digitalWrite(LED_BUILTIN, LOW); 
     onoff=0;
   }
 }
 Serial.println("Connected");
 Serial.println("");
 Serial.println("WiFi connected");  
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 Serial.print("Netmask: ");
 Serial.println(WiFi.subnetMask());
 Serial.print("Gateway: ");
 Serial.println(WiFi.gatewayIP());
 while (!NTPch.setSNTPtime()) Serial.print(".");
  while(1){   // local loop
    char status;
    double T,P,p0,a;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float rounded_T,rounded_P;
    String mac = WiFi.macAddress();
    int count = 0;
    for (int i = 0; mac[i]; i++) 
      if (mac[i] != ':') 
        mac[count++] = mac[i];
      mac[count] = '\0'; 
      mac = mac.substring(0,12);
      if (isnan(h) || isnan(t))
      {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      status = pressure.startTemperature();
      if (status != 0)
      {
        delay(status);
        status = pressure.getTemperature(T);
        if (status != 0)
        {
          rounded_T = ((int)(T * 100 + .5) / 100.0);
          status = pressure.startPressure(3);
          if (status != 0)
          {
            delay(status);
            status = pressure.getPressure(P,T);
            if (status != 0)
            {
              rounded_P = ((int)(P * 100 + .5) / 100.0);
            }
            else Serial.println("error retrieving pressure measurement\n");
          }
          else Serial.println("error starting pressure measurement\n");
        }
        else Serial.println("error retrieving temperature measurement\n");
      }
      else Serial.println("error starting temperature measurement\n");
      sensorValue = analogRead(analogInPin);
      float Average_temp = (t + rounded_T)/2;
      Serial.println(Average_temp);
      Serial.println(rounded_P);
      Serial.println(h);
      Serial.println(sensorValue);
      Serial.println(mac);
      Serial.print("connecting to ");
      Serial.println(host);
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed"); // tries to connect to the host.
        return;
      }
      String url = "/weather/insert.php?temp=" + String(Average_temp) + "&hum="+ String(h) + "&pres=" + String(rounded_P) + "&aqi="+ String(sensorValue) + "&mac=" + String(mac) + "&lat=" + String(string_lat) + "&lon=" + String(string_lon);
      Serial.print("Requesting URL: ");
      Serial.println(url);
      // requests the server with url (host url + query)
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
       "Host: " + host + "\r\n" + 
       "Connection: close\r\n\r\n");
      delay(5000);
      // waits for 5 seconds for response.
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      // prints the response in serial monitor.
      Serial.println();
      Serial.println("closing connection");
      delay(3000);
    dateTime = NTPch.getTime(5.5, 1); // get time from internal clock
    NTPch.printDateTime(dateTime);
    byte H = dateTime.hour;
    byte M = dateTime.minute;
    byte S = dateTime.second;
    if(H==9 && M==25 && (S>=00 || S<=59)){ // checks whether it's time for update.
      iotUpdater(true);
    }
    yield();
    watchdogCount = 0; // resets the watchdog timer.
  }
}
bool checkconf(){
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' ) return 0;
  else return 1;
}
void iotUpdater(bool debug) {
  if (debug) {
    Serial.println("start flashing......");
    Serial.println(update_server);
    Serial.println(update_uri);
    Serial.println(firmware);
  }
  t_httpUpdate_return ret = ESPhttpUpdate.update(update_server, 80, update_uri, firmware);
  switch (ret) {
    case HTTP_UPDATE_FAILED:
    if (debug) Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    break;

    case HTTP_UPDATE_NO_UPDATES:
    if (debug) Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

    case HTTP_UPDATE_OK:
    if (debug) Serial.println("HTTP_UPDATE_OK");
    break;
  }
}
String read_string(int l, int p){
  String temp;
  for (int n = p; n < l+p; ++n)
  {
   if(char(EEPROM.read(n))!=';'){
    if(isWhitespace(char(EEPROM.read(n)))){}
      else temp += String(char(EEPROM.read(n)));
  }
  else n=l+p;
}
return temp;
}
void WAP()
{
  Serial.print("Configuring access point...");
  Serial.print("Setting soft-AP configuration ... ");
  Serial.print("Setting soft-AP ... ");
  WiFi.softAP(ssid, password);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  while(1){
    server.handleClient();
    watchdogCount = 0;
  }
}
void handleRoot() {
 if (server.hasArg("ssid")&& server.hasArg("Password")&& server.hasArg("lat")&&server.hasArg("lon") ) {
  handleSubmit();
}
else {
  server.send(200, "text/html", PAGE_AdminMainPage);
}
}
void handleSubmit(){
  String response="<p>The ssid is ";
  response += server.arg("ssid");
  response +="<br>";
  response +="And the password is ";
  response +=server.arg("Password");
  response +="<br>";
  response +="And the lattitude is ";
  response += server.arg("lat");
  response +="<br>";
  response +="And the longitude is ";
  response +=server.arg("lon");
  response +="<br>"; response +="<H2><a href=\"/\">go home</a></H2><br>";
  server.send(200, "text/html", response);
  write_to_Memory(String(server.arg("ssid")),String(server.arg("Password")),String(server.arg("lat")),String(server.arg("lon")));
}
void write_to_Memory(String ssid,String pass,String lat, String lon){
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');
  ssid+=";";
  write_EEPROM(ssid,5);
  pass+=";";
  write_EEPROM(pass,100);
  lat+=";";
  write_EEPROM(lat,200); 
  lon+=";";
  write_EEPROM(lon,300); 
  EEPROM.commit();
  ESP.restart();
}
void write_EEPROM(String x,int pos){
  for(int n=pos;n<x.length()+pos;n++){
   EEPROM.write(n,x[n-pos]);
 }
}
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message +="<H2><a href=\"/\">go home</a></H2><br>";
  server.send(404, "text/plain", message);
}