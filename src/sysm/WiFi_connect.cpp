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
prefered_wifi(sig.get_main_sta()),
try_out_wifi_string(sig.get_try_sta()),
connect_time(0),
scan_time(20000),
main_state(NEW_SEARCH),
avoid_pos(0)
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
  //static uint8_t state = 0;
  int n = -100;
  //Serial.print("find sta and connect state: ");Serial.println(state);
  switch (main_state)
  {
    case NEW_SEARCH:
    if(TimeSinceLastScan()>10000)
    {
    WiFi.mode(WIFI_AP_STA);
    //WiFi.setAutoReconnect(false);
    WiFi.scanNetworks(true);
    Serial.println("start scan");
    scan_time = millis();

  }
    main_state = SEARCHING;
    break;

    case SEARCHING:
    n = WiFi.scanComplete();
    //Serial.println(n);
    if (n == -2)
    {
      //Serial.println("Still scanning");
      break;
    }
    if(n==-1)
    {
      //Serial.println("Scan in prog");
      break;
    }
    if (n>=0)
    {
      //Serial.println("scan complete");

      String sta = "";
      String m_sta = MAIN_STA;

      int connect_to = select_wifi_to_connect(n);
      if(connect_to >= 0)
      {
        WiFi.disconnect();
        WiFi.begin(WiFi.SSID(connect_to).c_str(), MAIN_STA_PW);
        Serial.print("Try to connect to ");Serial.print(WiFi.SSID(connect_to).c_str());
        uint8_t times = 0;
        while(WiFi.status() != WL_CONNECTED)
        {
          times++;
          delay(500);
          Serial.print(".");
          if(times>10)
          {
            Serial.println("Failed");
            main_state = NEW_SEARCH;
            AddToAvoidList(WiFi.SSID(connect_to).c_str());
            break;
          }
        }
        if(WiFi.status() == WL_CONNECTED)
        {
          Serial.println("Connected!");
          connect_time = millis();
          main_state = CONNECTED;
          break;
        }
      }
      main_state = NEW_SEARCH;

    }
    break;

    case CONNECTED:
    break;

    default:
    Serial.println("Uknown state err");
    break;
  }
}

int WiFi_connecter::select_wifi_to_connect(int n)
{
  int suggested = -1;
  int last_chans = -1;
  for (int i = 0; i < n; i++)
  {
    String sta =WiFi.SSID(i);

    //Serial.print(sta);
    // Is it the prefered_wifi select it directly
    if (sta == prefered_wifi)
    {
      if (!sta_in_avoid_list(sta))
      {
        //Serial.println(" Selected!");
        return i;
      }
      else
      {
        //Serial.println(" - In avoid list");
        last_chans = i;
      }
    }

      // If the first part mach try out string check if in avoid list else connect
    else if(sta.length() >= try_out_wifi_string.length() )
    {
      if(WiFi.SSID(i).substring(0,try_out_wifi_string.length())==try_out_wifi_string)
      {
        if (!sta_in_avoid_list(sta))
        {
          //Serial.println(" - Suggested!");
          suggested = i;
        }
        else
        {
          //Serial.println(" - In avoid list");
          last_chans = i;
        }
      }
      else
      {
        //Serial.println(" - Not valid");
      }
    }
    else
    {
      //Serial.println(" - Not valid");
    }

  }

  if(suggested==-1) {
    if(last_chans==-1)
    {
        //Serial.println("**** NO WIFI TO CONNECT ****");
    }
    return last_chans;
  }
  return suggested;
}

bool WiFi_connecter::sta_in_avoid_list(String & sta) const
  {
    for(int b =0; b< AVOID_WIFI_COUNT; b++)
    {
      if(avoid_wifi_list[b] == sta) return true;
    }
    return false;
  }

  void WiFi_connecter::AddToAvoidList(String s)
  {
    Serial.print("Add to avoid list: ");Serial.println(s);
    if(avoid_pos >= AVOID_WIFI_COUNT) avoid_pos=0;
    avoid_wifi_list[avoid_pos] = s;
    avoid_pos++;
  }
  unsigned long WiFi_connecter::TimeSinceLastConnect() const
  {
    return millis() - connect_time ;
  }
  unsigned long WiFi_connecter::TimeSinceLastScan() const
  {
    return millis() - scan_time ;
  }
  void WiFi_connecter::check_wifi_connections()
  {

    switch (WiFi.getMode()) {
      case WIFI_AP_STA:
      if(!WiFi.isConnected())
      {
        Serial.println("**** No connection look for new connection *****");
        main_state = NEW_SEARCH;
        //find_sta_and_connect();
        break;
      }
      if((sig.TimeSinceMsgUpdate() > 10000) && (TimeSinceLastConnect() > 10000))
      {
        Serial.println("**** No msg timeout look for new connection *****");
        main_state=NEW_SEARCH;
        if(!sta_con_main())
        {
          AddToAvoidList(WiFi.SSID());
        }
        break;
        //find_sta_and_connect();
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
    String m_sta = sig.get_main_sta();

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
    IPAddress gateway(172,17,sig.get_id(),22);
    IPAddress subnet(255,255,255,0);
    Serial.println("Wifi connect setup");
    WiFi.mode(WIFI_AP_STA);
    WiFi.hostname(HOSTNAME);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    String ssid_string = String(ssid_ap)+String(sig.get_id());
    WiFi.softAP(ssid_string.c_str(),password_ap);
    //connect_to_main_sta();

    find_sta_and_connect();
  }

  void WiFi_connecter::update()
  {
    switch(main_state)
    {
      case NEW_SEARCH:
      find_sta_and_connect();
      break;
      case SEARCHING:
      find_sta_and_connect();
      break;
      case CONNECTED:
      check_wifi_connections();
      break;
      default:
      Serial.println("Err shouldent be here");
    }

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
