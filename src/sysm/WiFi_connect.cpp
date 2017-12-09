#include "sysm/WiFi_connect.h"

static char ssid_ap[] = SSID_AP;
static char password_ap[] = PASS_AP;

// Hölö wifi
static char ssid_st_h[] = "TeliaGateway9C-97-26-49-11-55";
static char password_st_h[] = "179A1021DD";

// Södertälje wifi
static char ssid_st_s[] = "Wifi_ToV";
static char password_st_s[] = "65635032";

// hack wifi
const char* ssid_x = "TN_24GHz_C587A5";
const char* password_x = "68E84C8ED7";


IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);


// Initialize the client library
WiFiClient WiFi_connecter::client = WiFiClient();

WiFi_connecter::WiFi_connecter()
{

}
WiFi_connecter::~WiFi_connecter()
{

}

void WiFi_connecter::setup()
{
  Serial.println("Wifi connect setup");
  WiFi.mode(WIFI_AP_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid_ap,password_ap);

  WiFi.begin(ssid_st_s, password_st_s);
  delay(1000);
  uint8_t attempt = 0;
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection to STA Failed!");
    attempt++;
    delay(5000);
    if (attempt >= STA_WIFI_ATTEMPT)
    {
      WiFi.mode(WIFI_AP);
      break;
    }
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void WiFi_connecter::update()
{
  check_OTA();

}
void WiFi_connecter::check_OTA()
{
  ArduinoOTA.handle();
}

void WiFi_connecter::debugg()
{
  //Serial.print("AP: ");Serial.println(ssid_ap);
  WiFiMode_t wm = WiFi.getMode();
  Serial.print(" M:");Serial.print(wm);
  Serial.print(" IP: ");
  if (wm==WIFI_AP) Serial.print(WiFi.softAPIP());
  else Serial.print(WiFi.localIP());
}


extern "C" {
#include<user_interface.h>
}
struct station_info *stat_info;
struct ip_addr *IPaddress;
IPAddress address;
void WiFi_connecter::find_clients()
{
  Serial.print(" Clients:\r\n");
  stat_info = wifi_softap_get_station_info();

  while (stat_info != NULL)
  {
    IPaddress = &stat_info->ip;
    address = IPaddress->addr;
    Serial.print("\t");
    Serial.print(address);
    Serial.print("\r\n");
    stat_info = STAILQ_NEXT(stat_info, next);
  }
}

void WiFi_connecter::print_wl_status()
{
  switch(WiFi.status())
  {
    case WL_CONNECTED:
    Serial.println("wl_connected");
    break;
    default:
    Serial.println("Can't handle status");
    break;
  }
}
