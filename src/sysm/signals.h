#ifndef SIG_SIGNALS_TTG
#define SIG_SIGNALS_TTG
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#define SIGNALS_UPDATE_RATE_MS 100

#define SERVO_H_POS_ADDR 0x00
#define SERVO_L_POS_ADDR 0x01
#define SERVO_DELAY_ADDR 0x02
#define ID_ADDR 0x04
#define MAIN_STA_ADDR 0x05
#define MAIN_STA_MAX_LEN 30
#define MAIN_STA_LEN_ADDR 0x1F
#define TRY_STA_ADDR 0x20
#define TRY_STA_MAX_LEN 20
#define TRY_STA_LEN_ADDR 0x35
#define SELF_STA_ADDR 0x36
#define SELF_STA_MAX_LEN 20
#define SELF_STA_LEN_ADDR 0x4B
#define MAIN_STA_PASS_ADDR 0x4C
#define MAIN_STA_PASS_MAX_LEN 20
#define MAIN_STA_PASS_LEN_ADDR 0x61
#define CONF1_ADDR 0x62

class Signals
{
public:
  Signals();
  String getJsonSignals()const;
  virtual ~Signals();
  const static uint8_t update_rate = SIGNALS_UPDATE_RATE_MS;
  void setup();

  void override_values();

  void set_id(uint8_t);
  uint8_t get_id() const;

  void set_servo_h_pos(uint8_t);
  uint8_t get_servo_h_pos() const;

  void set_servo_l_pos(uint8_t);
  uint8_t get_servo_l_pos() const;

  void set_servo_delay(uint16_t);
  uint16_t get_servo_delay() const;

  void set_msg(String & s);
  String get_msg() const;
  void outdate_msg();

  bool MsgUpdated() const;

  unsigned long TimeSinceMsgUpdate() const;

  String get_main_sta() const;
  void set_main_sta(String & s);

  String get_main_sta_pass() const;
  void set_main_sta_pass(String & s);

  String get_try_sta() const;
  void set_try_sta(String & s);

  String get_self_sta() const;
  void set_self_sta(String & s);

  bool get_no_msg_timeout_reconnect() const;
  void set_no_msg_timeout_reconnect(bool);
  bool get_main_sta_on_flash() const;
  void set_main_sta_on_flash(bool);

private:
  enum CONF1 {
    NO_MSG_TIMEOUT_RECONNECT,
    MAIN_STA_ON_FLASH,
  };
  void set_conf1_bit(CONF1 pos, bool value);
  bool get_conf1_bit(CONF1 pos) const;
  bool msg_updated;
  String msg;
  unsigned long msg_update_time;
};


#endif
