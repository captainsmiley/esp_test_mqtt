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
#define MAIN_STA_LEN_ADDR 0x36
#define TRY_STA_ADDR 0x37
#define TRY_STA_MAX_LEN 20
#define TRY_STA_LEN_ADDR 0x58
#define SELF_STA_ADDR 0x59
#define SELF_STA_MAX_LEN 20
#define SELF_STA_LEN_ADDR 0x80

class Signals
{
public:
  Signals();
  virtual ~Signals();
  const static uint8_t update_rate = SIGNALS_UPDATE_RATE_MS;
  void setup();

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

  String get_try_sta() const;
  void set_try_sta(String & s);

  String get_self_sta() const;
  void set_self_sta(String & s);

private:
  bool msg_updated;
  String msg;
  unsigned long msg_update_time;
};


#endif
