#include "sysm/WiFi_connect.h"

#include "conf.h"

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

WiFi_connecter::WiFi_connecter(Signals s) : sig(s)
{

}
WiFi_connecter::~WiFi_connecter()
{

}

void scanDone(int n)
{

}

void WiFi_connecter::find_sta_and_connect()
{
  WiFi.mode(WIFI_AP_STA);
  Serial.println("find sta and connect");
  int n = WiFi.scanComplete();
  Serial.println(n);
  if (n == -2)
  {
    Serial.println("start scan");
  WiFi.scanNetworks(true);
  }
  if(n)
  {
    Serial.println("Scan Complete");
    for (int i = 0; i < n; i++)
    {
      if(WiFi.SSID(i)==String(MAIN_STA))
      {
        WiFi.begin(WiFi.SSID(i).c_str(), MAIN_STA_PW);
        if(WiFi.waitForConnectResult() == WL_CONNECTED)
        {
          return;
        }

      }
      if(WiFi.SSID(i).length() > 10)
      {
        if(WiFi.SSID(i).substring(0,10) == String("ESPWiFi_tg"))
        {
          WiFi.begin(WiFi.SSID(i).c_str(), "connectme");
          if(WiFi.waitForConnectResult() == WL_CONNECTED)
          {
            return;
          }
        }
      }

      WiFi.scanNetworks(true);

    }
  }

  if(n==-1)
  {
    Serial.println("Scan in prog");

  }

}

void WiFi_connecter::check_wifi_connections()
{

  switch (WiFi.getMode()) {
    case WIFI_AP_STA:
      if(!WiFi.isConnected() && sta_con_main())
      {
        find_sta_and_connect();

        //Serial.println("Wifi main STA disconnected");

      }
      else
      {
      }
      break;
      case WIFI_AP:
      find_sta_and_connect();

      break;
      default:
      Serial.println("err: unknown wifi mode");
      break;
  }
}

bool WiFi_connecter::sta_con_main()
{
  return WiFi.SSID()==String(MAIN_STA);
}

void WiFi_connecter::connect_to_main_sta()
{
  WiFi.begin(MAIN_STA, MAIN_STA_PW);


<<<<<<< HEAD
=======
  #if STA_SETUP == 0
  WiFi.begin(MAIN_STA, MAIN_STA_PW);
  #elif STA_SETUP == 1

  WiFi.begin(S1_STA, S1_STA_PW);
  #else

  WiFi.begin(MAIN_STA, MAIN_STA_PW);
  #endif


>>>>>>> a59ad040a7ac9928047695adf6d3be11a5556031
  delay(1000);
  uint8_t attempt = 0;
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection to STA Failed!");
    attempt++;
    delay(5000);
    if (attempt >= STA_WIFI_ATTEMPT)
    {
      WiFi.mode(WIFI_AP);
      return;
    }
  }
  WiFi.setAutoReconnect(true);

}
void WiFi_connecter::setup()
{
  Serial.println("Wifi connect setup");
  WiFi.mode(WIFI_AP_STA);
  //WiFi.hostname(HOSTNAME);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  String ssid_string = String(ssid_ap)+String(sig.get_id());
  WiFi.softAP(ssid_string.c_str(),password_ap);
  //connect_to_main_sta();
  find_sta_and_connect();




}

void WiFi_connecter::update()
{
<<<<<<< HEAD
  check_wifi_connections();
=======
  check_OTA();
  if (!WiFi.isConnected())
  {

  }
>>>>>>> a59ad040a7ac9928047695adf6d3be11a5556031

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
