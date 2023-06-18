#include <SocketIoClient.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsClient.h>

String sensorpir, sensorultra;

const char *ssid = "Adilonapsh";
const char *password = "keposiahehe";

const char *ssid1 = "Study hard";
const char *password1 = "creads418";

IPAddress ip(192, 168, 43, 102);
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

const int pir = 5;
const int echo = 4;
const int trig = 0;

const int Relay1 = 14;
const int Relay2 = 12;
const int Relay3 = 13;

long durasi;
int Jarak;

int detek = false;
bool Relays1 = false;
bool Relays2 = false;
bool Relays3 = false;

char host[] = "192.168.43.104";
int port = 2001;
char path[] = "/socket.io/?transport=websocket";

SocketIoClient websocket;

void setup()
{
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);

  Serial.begin(9600);
  delay(10);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid1, password1);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected !");
  Serial.print("SSID = ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address Route = ");
  Serial.println(WiFi.localIP());
  Serial.print("IP Address Local = ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Connecting to ..");

  websocket.begin(host, port, path);
  websocket.on("connect", socket_connect);

  server.on("/datarelay", HTTP_POST, dataRelay);
  server.on("/alloff", HTTP_GET, alloff);
  server.begin();
  Serial.println("Server Berjalan..");
}

void loop()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  durasi = pulseIn(echo, HIGH);
  Jarak = durasi * 0.034 / 2;

  Serial.print("Jarak : ");
  Serial.println(Jarak);

  long state = digitalRead(pir);
  if (state == HIGH)
  {
    detek = true;
    Serial.println("Motion detected!");
  }
  else
  {
    detek = false;
    Serial.println("Motion absent!");
  }

  char ultra[255];
  char pirss[255];
  char relay1[255];
  char relay2[255];
  char relay3[255];

  sprintf(ultra, "\"%d\"", Jarak);
  sprintf(pirss, "\"%d\"", detek);
  sprintf(relay1, "\"%d\"", Relays1);
  sprintf(relay2, "\"%d\"", Relays2);
  sprintf(relay3, "\"%d\"", Relays3);

  websocket.emit("Ultra", ultra);
  websocket.emit("Pir", pirss);
  websocket.emit("Relay1", relay1);
  websocket.emit("Relay2", relay2);
  websocket.emit("Relay3", relay3);

  websocket.loop();
  cekData();
  server.handleClient();
}

void cekData()
{
  if (digitalRead(Relay1) == HIGH)
  {
    Relays1 = true;
  }
  else if (digitalRead(Relay1) == LOW)
  {
    Relays1 = false;
  }
  if (digitalRead(Relay2) == HIGH)
  {
    Relays2 = true;
  }
  else if (digitalRead(Relay2) == LOW)
  {
    Relays2 = false;
  }
  if (digitalRead(Relay3) == HIGH)
  {
    Relays3 = true;
  }
  else if (digitalRead(Relay3) == LOW)
  {
    Relays3 = false;
  }
}

void dataRelay()
{
  if (server.hasArg("pin") && server.hasArg("state"))
  {
    int statusPin = server.arg("state").toInt();
    String namaPin = server.arg("pin");
    static const uint8_t pinAddress[] = {14, 12, 13};

    digitalWrite(pinAddress[namaPin.toInt()], statusPin);
    server.send(200, "text/plain", "Command Done ! ");
  }
  else
  {
    server.send(200, "text/plain", "Command Not Found !");
  }
}

void alloff()
{
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  server.send(200, "text/plain", "Your Command Is Done Beybeh !!");
}

void socket_connect(const char *payload, size_t len)
{
  Serial.println("Connected..");
}
