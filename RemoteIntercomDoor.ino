#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*Put your SSID & Password*/
const char* ssid = "ReineBlanche";  // Enter SSID here
const char* password = "marionetgwen";  //Enter Password here
IPAddress ip(192,168,1,87);   
IPAddress gateway(192,168,1,254);   
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LED1pin = D7;
bool LED1status = LOW;

void setup() {
  pinMode(D0, OUTPUT);
  
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
}

void handle_OnConnect() {
  LED1status = LOW;
  Serial.println("Porte fermée");
  server.send(200, "text/html", SendHTML(LED1status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  digitalWrite(D0, HIGH);
  Serial.println("Porte ouverte");
  server.send(200, "text/html", SendHTML(true)); 
  delay(5000);
  LED1status = LOW;
  digitalWrite(D0, LOW);
  Serial.println("Porte fermée");
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Remote Intercom Door</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Remote Intercom Door</h1>\n";
  ptr +="<a class=\"button button-on\" href=\"/led1on\">Open the door</a>\n";  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
