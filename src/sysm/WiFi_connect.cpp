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





// Initialize the client library
WiFiClient WiFi_connecter::client = WiFiClient();

WiFi_connecter::WiFi_connecter(Signals & s) : sig(s),
avoid_wifi_list({}),
prefered_wifi("ESPWiFi_tg99"),
try_out_wifi_string("ESPWiFi_tg")
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
  static uint8_t state = 0;
  int n = -100;
  Serial.print("find sta and connect state: ");Serial.println(state);
  switch (state)
  {
    case 0:
    WiFi.mode(WIFI_AP_STA);
    //WiFi.setAutoReconnect(false);
    WiFi.scanNetworks(true);
    Serial.println("start scan");
    state = 1;
    break;
    case 1:
    n = WiFi.scanComplete();
    Serial.println(n);
    if (n == -2)
    {
      Serial.println("Still scanning");
      break;
    }
    if(n==-1)
    {
      Serial.println("Scan in prog");
    }
      if (n)
    {
      Serial.println("scan complete");

      String sta = "";
      String m_sta = MAIN_STA;

      int connect_to = select_wifi_to_connect(n);
      if(connect_to >= 0)
      {
            WiFi.begin(WiFi.SSID(connect_to).c_str(), MAIN_STA_PW);
            if(WiFi.waitForConnectResult() == WL_CONNECTED)
            {
              Serial.print("Connected to: ");Serial.println(WiFi.SSID(connect_to));
              state = 2;
              break;
            }
      }

      /*

      for (int i = 0; i < n; i++)
      {
        sta =WiFi.SSID(i);
        Serial.println(sta);
        if(sta.length() >= m_sta.length() )
        {
          if(WiFi.SSID(i).substring(0,m_sta.length())==m_sta)
          {
            WiFi.begin(WiFi.SSID(i).c_str(), MAIN_STA_PW);
            if(WiFi.waitForConnectResult() == WL_CONNECTED)
            {
              Serial.print("Connected to: ");Serial.println(WiFi.SSID(i));
              state = 2;
              break;
            }
          }
        }
      }


      if(!WiFi.isConnected())
      {
        for (int i = 0; i < n; i++)
        {
          if(WiFi.SSID(i).length() > 10)
          {
            if(WiFi.SSID(i).substring(0,10) == String("ESPWiFi_tg"))
            {
              WiFi.begin(WiFi.SSID(i).c_str(), "connectme");
              if(WiFi.waitForConnectResult() == WL_CONNECTED)
              {
                Serial.print("Connected to: ");Serial.println(WiFi.SSID(i));
                state = 2;
                break;
              }
            }
          }
        }
      }
      */
      state = 2;
    }
    break;

    case 2:
    Serial.println("prep new scan");
    state = 0;
    break;
    default:
    Serial.println("Uknown state err");
    break;
  }
}

int WiFi_connecter::select_wifi_to_connect(int n)
{
  for (int i = 0; i < n; i++)
  {
    String sta =WiFi.SSID(i);

    // Is it the prefered_wifi select it directly
    if (sta == prefered_wifi)
    {
      Serial.print(sta);
      Serial.println(" Selected!");
      return i;
    }

    // If the first part mach try out string check if in avoid list else connect
    if(sta.length() >= try_out_wifi_string.length() )
    {
      if(WiFi.SSID(i).substring(0,try_out_wifi_string.length())==try_out_wifi_string)
      {
        if (!sta_in_avoid_list(sta))
        {
          Serial.print(sta);
          Serial.println(" Selected!");
          return i;
        }
      }
    }
    Serial.println(sta);
  }

  Serial.println("**** NO WIFI TO CONNECT ****");
  return -1;
}

bool WiFi_connecter::sta_in_avoid_list(String & sta) const
{
  for(int b =0; b< AVOID_WIFI_COUNT; b++)
  {
    if(avoid_wifi_list[b] == sta) return true;
  }
  return false;
}

void WiFi_connecter::check_wifi_connections()
{

  switch (WiFi.getMode()) {
    case WIFI_AP_STA:
    if(!WiFi.isConnected() || !sta_con_main())
    {
      if(sig.TimeSinceMsgUpdate() > 10000)
      find_sta_and_connect();
      ;

      //Serial.println("Wifi main STA disconnected");

    }
    else
    {
    }
    break;
    case WIFI_AP:
    //find_sta_and_connect();

    break;
    default:
    Serial.println("err: unknown wifi mode");
    break;
  }
}

bool WiFi_connecter::sta_con_main()
{
  String m_sta = MAIN_STA;

  if(WiFi.SSID().length() >= m_sta.length() )
  {
    if(WiFi.SSID().substring(0,m_sta.length())==m_sta)
    {
      return true;

    }
  }
  return false;
}

void WiFi_connecter::connect_to_main_sta()
{
  WiFi.begin(MAIN_STA, MAIN_STA_PW);


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

  IPAddress local_IP(172,17,sig.get_id(),22);
  IPAddress gateway(172,17,7,22);
  IPAddress subnet(255,255,255,0);
  Serial.println("Wifi connect setup");
  WiFi.mode(WIFI_AP_STA);
  //WiFi.hostname(HOSTNAME);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  String ssid_string = String(ssid_ap)+String(sig.get_id());
  WiFi.softAP(ssid_string.c_str(),password_ap);
  //connect_to_main_sta();

  find_sta_and_connect();

  /*
  WiFi.mode(WIFI_AP);
  WiFi.hostname("train_switcher");
  WiFi.softAPConfig(local_IP, gateway, subnet);

  String ssid_string = String(ssid_ap)+String(sig.get_id());
  WiFi.softAP(ssid_string.c_str(),password_ap);
  //WiFi.softAP(ssid_ap,password_ap);
  delay(1000);
  */




}

void WiFi_connecter::update()
{
  //Serial.println("WiFi update");
  check_wifi_connections();

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
