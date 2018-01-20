


/***** WiFi settings **************/
#define DEF_STA        "Wifi_ToV"
#define DEF_STA_PW     "65635032"

#define S1_STA "ESPWiFi_TrainController"
#define S1_STA_PW "connectme"


// Hölö wifi
#define S2_STA "TeliaGateway9C-97-26-49-11-55"
#define S2_STA_PW "179A1021DD"

// Hack wifi
#define S3_STA "TN_24GHz_C587A5"
#define S3_STA_PW "68E84C8ED7"

#define STA_SETUP 1



#define TG_VERSION 8



#define SSID_AP         "ESPWiFi_tg"
#define PASS_AP         "connectme"

#define HOSTNAME        "tgtest_esp"

#define STA_WIFI_ATTEMPT 1


/***** Enable components settings ***********/
#define MQTT_ENABLED 0
#define COMMANDS_ENABLED 1
#define MESH_ENABLED 1



#if STA_SETUP == 0
#define MAIN_STA DEF_STA
#define MAIN_STA_PW DEF_STA_PW
#elif STA_SETUP == 1
#define MAIN_STA S1_STA
#define MAIN_STA_PW S1_STA_PW
#elif STA_SETUP ==2
#define MAIN_STA S2_STA
#define MAIN_STA_PW S2_STA_PW
#elif STA_SETUP ==3
#define MAIN_STA S3_STA
#define MAIN_STA_PW S3_STA_PW
#else
#define MAIN_STA        "Wifi_ToV"
#define MAIN_STA_PW     "65635032"
#endif
