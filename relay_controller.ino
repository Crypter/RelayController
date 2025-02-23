// relay_controller

// <PINOUT>
#define LED_BUILTIN 2

#define RTC_RESET 13
#define RTC_CLOCK 12
#define RTC_DATA  14

#define ENCODER_A 27
#define ENCODER_B 26
#define ENCODER_BUTTON 25

#define DISPLAY_SCL 33 
#define DISPLAY_SDA 32
// </PINOUT>

// <DEFINITIONS>
#define RELAY_COUNT 4
#define TIMER_COUNT 5

#define RELAY_PIN_1 17
#define RELAY_PIN_2 16
#define RELAY_PIN_3 4
#define RELAY_PIN_4 15
// </DEFINITIONS>

#include <RtcDS1302.h> //RTC
#include <U8g2lib.h> //Display
#include <Wire.h> //Display
#include <ArduinoJson.h> //Config
#include "SimpleMenu.h"
#include "SimpleSD.h"
#include "RelayDriver.h"

// <GLOBAL STATE MACHINE>
uint8_t screen_saver = 0;
uint16_t screen_saver_timeout = 60;
uint8_t trigger_display_update = 0;
uint8_t trigger_config_update = 0;
uint8_t trigger_timers_reload = 0;
RtcDateTime last_clock_sync;
// </GLOBAL STATE MACHINE>

// <GLOBAL VARIABLES>

//Hardware
uint8_t relay_hold_power = 50;
uint32_t relay_pwm_frequency = 1000;
RelayDriver relay[RELAY_COUNT] = {{RELAY_PIN_1, TIMER_COUNT},
                                  {RELAY_PIN_2, TIMER_COUNT},
                                  {RELAY_PIN_3, TIMER_COUNT},
                                  {RELAY_PIN_4, TIMER_COUNT}};

// RTC
int8_t relay_timer_map[RELAY_COUNT][TIMER_COUNT][2]; //[relay][alarm][end|start]

ThreeWire myWire(RTC_DATA, RTC_CLOCK, RTC_RESET); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
void alarm_callback_wrapper(uint8_t id, const RtcDateTime& alarm){
  uint8_t relay_found = 0;
  Serial.printf("Alarm fired with ID: %d\n", id);

  for (uint8_t r=0; r<RELAY_COUNT; r++){
    for (uint8_t t=0; t<TIMER_COUNT; t++){
      for (uint8_t state=0; state<=1; state++){
        Serial.printf("r=%u, t=%u, state=%u, id=%d\n", r, t, state, relay_timer_map[r][t][state]);
        if (id == relay_timer_map[r][t][state]) {
          Serial.printf("Found alarm at relay %d, timer %d, state %u ", r+1, t+1, state);
          relay_found = 1;
          relay[r].setState(state, t);
          break;
        }
      }
    }
    if (relay_found) break;
  }
  if (relay_found == 0) {
    Serial.printf("Alarm %d fired, but not matched to any relay-timer combination\n", id);
  }
}

RtcAlarmManager<alarm_callback_wrapper> TimerAlarmManager(RELAY_COUNT * TIMER_COUNT * 2);

// Simple Menu
SimpleMenu *menu = 0;
SimpleMenu *last_menu = 0;

// Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ DISPLAY_SCL, /* data=*/ DISPLAY_SDA);   // ESP32 Thing, HW I2C with pin remapping
String message_1;
String message_2;

// Config
SimpleSD card;
JsonDocument config;

// </GLOBAL VARIABLES>


void load_datetime(SimpleMenu *menu){
  ((SimpleDateTimeMenu*)menu)->setDateTime(getSystemTime());
}
void save_datetime(SimpleMenu *menu){
  RtcDateTime new_time = ((SimpleDateTimeMenu*)menu)->getDateTime();
  Rtc.SetDateTime(new_time);
  setSystemTime(new_time);
  trigger_config_update = 1;
}

void load_timer(SimpleMenu *menu){
  SimpleAlarmMenu *alarm_menu = (SimpleAlarmMenu*)menu;
  JsonObject this_menu_config = config["Relays"][alarm_menu->parent_menu->getTitle()]["Timers"][alarm_menu->getTitle(false)].as<JsonObject>();

  alarm_menu->enable( this_menu_config["enabled"] | 0 );
  alarm_menu->setInterval( this_menu_config["interval"] | "Every day" );

  RtcDateTime duration(2020, 1, 1,
    this_menu_config["duration"]["hour"] | 0,
    this_menu_config["duration"]["minute"] | 0,
    this_menu_config["duration"]["second"] | 0);
  
  RtcDateTime start_time(2020, 1, 1,
    this_menu_config["start time"]["hour"] | 0,
    this_menu_config["start time"]["minute"] | 0,
    this_menu_config["start time"]["second"] | 0);
  
  alarm_menu->setDuration( duration ) ;
  alarm_menu->setDateTime( start_time );

}

void save_timer(SimpleMenu *menu){
  SimpleAlarmMenu *alarm_menu = (SimpleAlarmMenu*)menu;
  config["Relays"][alarm_menu->parent_menu->getTitle()]["Timers"].createNestedObject(alarm_menu->getTitle(false));
  JsonObject this_menu_config = config["Relays"][alarm_menu->parent_menu->getTitle()]["Timers"][alarm_menu->getTitle(false)].as<JsonObject>();

  this_menu_config["enabled"] = alarm_menu->isEnabled();
  this_menu_config["interval"] = alarm_menu->getInterval();
  this_menu_config["id"] = alarm_menu->getId();
  
  RtcDateTime duration = alarm_menu->getDuration();
  this_menu_config["duration"]["hour"] = duration.Hour();
  this_menu_config["duration"]["minute"] = duration.Minute();
  this_menu_config["duration"]["second"] = duration.Second();
  
  RtcDateTime start_time = alarm_menu->getDateTime();
  this_menu_config["start time"]["hour"] = start_time.Hour();
  this_menu_config["start time"]["minute"] = start_time.Minute();
  this_menu_config["start time"]["second"] = start_time.Second();;
  
  trigger_config_update = 1;
  trigger_timers_reload = 1;
}

void load_brightness(SimpleMenu *menu){
  int brightness = config["system"]["brightness"] | 5;
  ((SimpleNumberMenu*)menu)->setValue(brightness);
}

void save_relay_enable(SimpleMenu *menu){
  uint8_t relay_id = menu->parent_menu->getId();
  uint8_t relay_enabled = ((SimpleOnOffMenu*)menu)->getValue();

  config["Relays"][menu->parent_menu->getTitle()][((SimpleOnOffMenu*)menu)->getTitle(false)] = relay_enabled;
  config["Relays"][menu->parent_menu->getTitle()]["id"] = relay_id;
  relay[relay_id-1].enable(relay_enabled);
  trigger_config_update = 1;
}

void load_relay_enable(SimpleMenu *menu){
  ((SimpleOnOffMenu*)menu)->setValue( config["Relays"][menu->parent_menu->getTitle()][((SimpleOnOffMenu*)menu)->getTitle(false)] | 0 );
}

void save_brightness(SimpleMenu *menu){
  int brightness = ((SimpleNumberMenu*)menu)->getValue();
  u8g2.setContrast( (brightness-1)*(254/4) +1);
  config["system"]["brightness"] = brightness;
  trigger_config_update = 1;
}

void do_reboot(SimpleMenu *menu){
    ESP.restart();
}

void load_relay_pwm(SimpleMenu *menu){
  ((SimpleNumberMenu*)menu)->setValue(relay_hold_power);
}
void save_relay_pwm(SimpleMenu *menu){
  relay_hold_power = ((SimpleNumberMenu*)menu)->getValue();
  
  config["system"]["relay hold power"] = relay_hold_power;

  load_relays();

  trigger_config_update = 1;
}

void load_relay_pwm_frequency(SimpleMenu *menu){
  ((SimpleNumberMenu*)menu)->setValue(relay_pwm_frequency);
}
void save_relay_pwm_frequency(SimpleMenu *menu){
  relay_pwm_frequency = ((SimpleNumberMenu*)menu)->getValue();
  
  config["system"]["relay pwm frequency"] = relay_pwm_frequency;

  load_relays();
  
  trigger_config_update = 1;
}

void load_screen_saver_timeout(SimpleMenu *menu){
  ((SimpleNumberMenu*)menu)->setValue(screen_saver_timeout);
}
void save_screen_saver_timeout(SimpleMenu *menu){
  screen_saver_timeout = ((SimpleNumberMenu*)menu)->getValue();
  config["system"]["display timeout"] = screen_saver_timeout;
  trigger_config_update = 1;
}

void init_Menu() {
  
  SimpleExitMenu *Exit = new SimpleExitMenu("Back");
  SimpleListMenu *main_menu = new SimpleListMenu("Settings");

  for (uint8_t i=1; i<=RELAY_COUNT; i++){
    char relay_id[50];
    sprintf(relay_id, "Relay %d", i);

    SimpleListMenu *relay_menu = new SimpleListMenu(relay_id);
    relay_menu->setId(i);

    SimpleOnOffMenu *relay_enable = new SimpleOnOffMenu("Enable");
    relay_enable->setLoadFunction(load_relay_enable);
    relay_enable->setSaveFunction(save_relay_enable);
    relay_menu->addMenu(relay_enable);
    
    for (uint8_t j=1; j<=TIMER_COUNT; j++){
      char timer_id[50];
      sprintf(timer_id, "Timer %d", j);

      SimpleAlarmMenu *relay_alarm = new SimpleAlarmMenu(timer_id);
      relay_alarm->setId(j);
      relay_alarm->setLoadFunction(load_timer);
      relay_alarm->setSaveFunction(save_timer);
      relay_menu->addMenu(relay_alarm);
    }

    relay_menu->addMenu(Exit);
    
    main_menu->addMenu(relay_menu);
  }


  SimpleListMenu *system_menu = new SimpleListMenu("System");
  SimpleMenu *DateAndTime = new SimpleDateTimeMenu("Date/Time");
  DateAndTime->setLoadFunction(load_datetime);
  DateAndTime->setSaveFunction(save_datetime);
  
  SimpleNumberMenu *Brightness = new SimpleNumberMenu("Brightness");
  Brightness->setMin(1);
  Brightness->setMax(5);
  Brightness->setLoadFunction(load_brightness);
  Brightness->setSaveFunction(save_brightness);
  
  SimpleNumberMenu *relay_pwm = new SimpleNumberMenu("Relay hold power");
  relay_pwm->setMin(0);
  relay_pwm->setMax(100);
  relay_pwm->setUnit("%");
  relay_pwm->setLoadFunction(load_relay_pwm);
  relay_pwm->setSaveFunction(save_relay_pwm);
  
  SimpleNumberMenu *relay_pwm_frequency = new SimpleNumberMenu("Relay PWM freq");
  relay_pwm_frequency->setMin(200);
  relay_pwm_frequency->setMax(20000);
  relay_pwm_frequency->setUnit("Hz");
  relay_pwm_frequency->setLoadFunction(load_relay_pwm_frequency);
  relay_pwm_frequency->setSaveFunction(save_relay_pwm_frequency);
  
  SimpleNumberMenu *timeout = new SimpleNumberMenu("Display timeout");
  timeout->setMin(0);
  timeout->setMax(300);
  timeout->setUnit("s");
  timeout->setLoadFunction(load_screen_saver_timeout);
  timeout->setSaveFunction(save_screen_saver_timeout);

  SimpleOnOffMenu *reboot_menu = new SimpleOnOffMenu("Reboot");
  reboot_menu->setSaveFunction(do_reboot);

  system_menu->addMenu(DateAndTime);
  system_menu->addMenu(relay_pwm);
  system_menu->addMenu(relay_pwm_frequency);
  system_menu->addMenu(Brightness);
  system_menu->addMenu(timeout);
  system_menu->addMenu(reboot_menu);
  system_menu->addMenu(Exit);

  main_menu->addMenu(system_menu);
  main_menu->addMenu(Exit);
  menu = main_menu;
}

void process_menu(){
  if (last_menu != SimpleMenu::current_menu){
    last_menu = SimpleMenu::current_menu;
    if (last_menu == 0) trigger_display_update = 1; //redraw on getting out of the menu
  }
}
// </SIMPLE MENU>

// <ENCODER>
volatile uint8_t clicks = 0;
volatile uint32_t counter=0;
volatile uint32_t button_last_event =0;
volatile uint8_t button_state =1;
volatile uint8_t button_last_state =1;

volatile int32_t encoder_location = 0;
int32_t last_location = 0;
volatile int32_t encoder_position = 0;
volatile uint32_t encoder_last_event =0;
volatile uint32_t recent_event_counter =0;
volatile uint32_t total_event_counter =0;
volatile uint8_t encoder_fast_mode =0;
volatile uint8_t encoder_last_mode =0;
uint8_t encoder_at_rest = 0;
uint8_t encoder_encode_states[4] ={0b11, 0b10, 0b00, 0b01};
uint8_t encoder_decode_states[4] ={2, 3, 1, 0};
volatile uint8_t encoder_state =0;

void encoder_moved(int32_t direction, uint32_t since_last_move){

  if (SimpleMenu::current_menu){
    uint8_t count = 1;

    if (since_last_move > 500) { //reset
      encoder_fast_mode = 0;
      encoder_last_mode = 0;
      recent_event_counter = 0;
      total_event_counter = 0;
    }

    if (since_last_move < 20) {
      if (encoder_last_mode == 1) recent_event_counter++;
      else recent_event_counter = 0;
      encoder_last_mode = 1;
    } else {
      if (encoder_last_mode == 0) recent_event_counter++;
      else recent_event_counter = 0;
      encoder_last_mode = 0;
    }

    if (encoder_last_mode == 0 && recent_event_counter>2) {
      encoder_fast_mode = 0;
      total_event_counter = 0;
    }

    total_event_counter++;
    if (encoder_last_mode == 1){
      if (total_event_counter > 5) encoder_fast_mode = 10;
      if (total_event_counter > 20) encoder_fast_mode = 100;
      if (total_event_counter > 50) encoder_fast_mode = 500;
      
    }

    if (encoder_fast_mode) count *= encoder_fast_mode;

    if (direction>0){
      (SimpleMenu::current_menu)->Right(count);
    } else if (direction<0){
      (SimpleMenu::current_menu)->Left(count);
    }
  }
}

void process_encoder(){
  if (encoder_state == encoder_at_rest && encoder_position) {
      int8_t step_correction = (encoder_position > 0) ? (2):(-2);
      int16_t rounding = (encoder_position+step_correction)/4;
      encoder_location += rounding;
      encoder_position = 0;
      if (rounding) {
        if (screen_saver) {
          screen_saver = 0;
          trigger_display_update = 1;
        }
        if (last_location != encoder_location){
          encoder_moved(encoder_location - last_location, millis()-encoder_last_event);
          last_location = encoder_location;
        }
        encoder_last_event = millis();
      }
  }

}

void IRAM_ATTR encoder_rotation_interrupt(){
  uint8_t encoder_fresh_state = encoder_decode_states[ (digitalRead(ENCODER_A) << 1) | (digitalRead(ENCODER_B) << 0) ];
  if (encoder_fresh_state != encoder_state){
    if (encoder_fresh_state == (encoder_state+4-1)%4) {
      encoder_position--;
    } else if (encoder_fresh_state == (encoder_state+4+1)%4) {
      encoder_position++;
    }
    encoder_state = encoder_fresh_state;
  }
}

void button_down(){
  // Serial.println("Button down");
  if (screen_saver) return;

  if (!SimpleMenu::current_menu) { //1 click to enter menu with no change
    SimpleMenu::current_menu = menu;
    (SimpleMenu::current_menu)->drawMenu();
  } else {
    (SimpleMenu::current_menu)->Click();
  }
}

void button_up(){
  // Serial.println("Button up");
  if (screen_saver) {
    screen_saver = 0;
    trigger_display_update = 1;
  }
}

void process_button(){
  if (button_state != button_last_state && millis()-button_last_event >= 50){  //50ms debounce time
        button_state = button_last_state;
        if (button_state == 0) {
          button_down();
        } else {
          button_up();
        }
  }
}

void IRAM_ATTR encoder_button_interrupt(){
  button_last_state = digitalRead(ENCODER_BUTTON);
  button_last_event = millis();
}

void init_Encoder(){
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);

  encoder_at_rest = encoder_decode_states[ (digitalRead(ENCODER_A) << 1) | (digitalRead(ENCODER_B) << 0) ];

  attachInterrupt(ENCODER_A, encoder_rotation_interrupt, CHANGE);
  attachInterrupt(ENCODER_B, encoder_rotation_interrupt, CHANGE);
  attachInterrupt(ENCODER_BUTTON, encoder_button_interrupt, CHANGE);
}
// </ENCODER>

// <RTC>
#define countof(a) (sizeof(a) / sizeof(a[0]))

void setSystemTime(const RtcDateTime& dt){
  struct timeval tv;
  tv.tv_sec = dt.Unix32Time();  // enter UTC UNIX time (get it from https://www.unixtimestamp.com )
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);
}

RtcDateTime getSystemTime(){
  struct timeval tv;
  gettimeofday(&tv, NULL);

  RtcDateTime rtcNow;
  rtcNow.InitWithEpoch32Time(tv.tv_sec);

  return rtcNow;
}

RtcDateTime getBootTime(){
  struct timeval tv;
  gettimeofday(&tv, NULL);

  RtcDateTime rtcNow;
  rtcNow.InitWithEpoch32Time(tv.tv_sec - millis()/1000);

  return rtcNow;
}

RtcDateTime getUpTime(){

  RtcDateTime rtcNow(millis()/1000);

  return rtcNow;
}

void printDateTime(const RtcDateTime& dt, char *buffer, bool date=true, bool time=true) {
  if (date && !time) {
      snprintf(buffer, 
        11,
        PSTR("%02u/%02u/%04u"),
        dt.Day(),
        dt.Month(),
        dt.Year() );
  }

  if (!date && time) {
      snprintf(buffer, 
        9,
        PSTR("%02u:%02u:%02u"),
        dt.Hour(),
        dt.Minute(),
        dt.Second() );
  }

  if (date && time) {
    snprintf(buffer, 
            20,
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
  }
}


void init_RTC(){
  
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // char date_buffer[20];
    // printDateTime(compiled, date_buffer);
    // Serial.println(date_buffer);

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    now = Rtc.GetDateTime();
    if (now.IsValid()) {
        Serial.println("Setting System clock according to RTC");
        setSystemTime(now);
    } else {
        Serial.println("RTC not found, setting to default time");
        RtcDateTime defaultTime(2000, 1, 1, 0, 0, 0);
        setSystemTime(defaultTime);
    }

}

void load_timers() {
  RtcDateTime now = getSystemTime();
  TimerAlarmManager = RtcAlarmManager<alarm_callback_wrapper>(RELAY_COUNT * TIMER_COUNT * 2);
  TimerAlarmManager.Sync(now);

  memset(relay_timer_map, -1, sizeof(relay_timer_map));

  JsonObject relays = config["Relays"].as<JsonObject>();
  for (JsonPair relay : relays) {
    uint8_t relay_id = relay.value()["id"];
    for (JsonPair timer : relay.value()["Timers"].as<JsonObject>()) {

      JsonObject this_menu_config = timer.value().as<JsonObject>();
      uint8_t timer_id = timer.value()["id"];
      if (this_menu_config["enabled"] | 0) {
        String interval_string = this_menu_config["interval"] | "Every day";
        uint8_t interval = 0;

        for (uint8_t i = 0; i < SimpleAlarmMenu::AL_INTERVAL_END; i++) {
          if (interval_string == String(SimpleAlarmMenu::interval_name[i])) {
            interval = i;
            break;
          }
        }

        int32_t interval_in_seconds = 0;

        int8_t target_day = now.Day();
        if (interval >= SimpleAlarmMenu::AL_EVERY_SUNDAY && interval <= SimpleAlarmMenu::AL_EVERY_SATURDAY) {
          target_day += (interval - SimpleAlarmMenu::AL_EVERY_SUNDAY) - (int8_t)now.DayOfWeek();  // target day of the month = current day + (target week day - current week day)
          interval_in_seconds = c_WeekAsSeconds;
        }

        if (interval == SimpleAlarmMenu::AL_EVERY_DAY) {
          interval_in_seconds = c_DayAsSeconds;
        }

        if (interval == SimpleAlarmMenu::AL_EVERY_2ND_DAY) {
          interval_in_seconds = c_DayAsSeconds * 2;
        }

        if (interval == SimpleAlarmMenu::AL_EVERY_HOUR) {
          interval_in_seconds = c_HourAsSeconds;
        }


        RtcDateTime start_time(now.Year(), now.Month(), target_day,
                               this_menu_config["start time"]["hour"] | 0,
                               this_menu_config["start time"]["minute"] | 0,
                               this_menu_config["start time"]["second"] | 0);

        while (start_time < now) {
          start_time += interval_in_seconds;
        }

        int32_t duration_in_seconds = (this_menu_config["duration"]["second"] | 0) + (this_menu_config["duration"]["minute"] | 0) * c_MinuteAsSeconds + (this_menu_config["duration"]["hour"] | 0) * c_HourAsSeconds;
        RtcDateTime end_time(start_time + duration_in_seconds);

        if (duration_in_seconds > 0) {
          int8_t alarm_id = 0;
          char buffer[50];

          alarm_id = TimerAlarmManager.AddAlarm(end_time, interval_in_seconds);
          relay_timer_map[relay_id - 1][timer_id - 1][0] = alarm_id;
          printDateTime(end_time, buffer);
          Serial.printf("R: %d, T: %d, S: %d, I: %d, %s\n", relay_id, timer_id, alarm_id % 2, alarm_id, buffer);

          alarm_id = TimerAlarmManager.AddAlarm(start_time, interval_in_seconds);
          relay_timer_map[relay_id - 1][timer_id - 1][1] = alarm_id;
          printDateTime(start_time, buffer);
          Serial.printf("R: %d, T: %d, S: %d, I: %d, %s\n", relay_id, timer_id, alarm_id % 2, alarm_id, buffer);
        }
      }
    }
  }
}

void process_timers(){
  TimerAlarmManager.ProcessAlarms();
  if (trigger_timers_reload) {
    load_timers();
    trigger_timers_reload = 0;
  }

  if (getSystemTime() >= last_clock_sync + 3*c_HourAsSeconds){
    last_clock_sync = getSystemTime();
    Serial.println("Syncing alarm tracking time from RTC");
    TimerAlarmManager.Sync(getSystemTime());
  }
}
// </RTC>

// <DISPLAY>
char display_date[20];
char display_time[20];

void Display_update(){
  // Screen saver
  if (screen_saver) return;

  u8g2.clearBuffer();

  if (screen_saver_timeout && millis()-button_last_event>(screen_saver_timeout*1000) && millis()-encoder_last_event>(screen_saver_timeout*1000)) {
      u8g2.clearDisplay();
      u8g2.sendBuffer();

      screen_saver = 1;
      return;
  }

  if (SimpleMenu::current_menu){
    (SimpleMenu::current_menu)->drawMenu();
  } else {
  u8g2.setFont(u8g2_font_9x15_mr);
  u8g2.drawStr(((u8g2.getDisplayWidth() - (u8g2.getUTF8Width(display_date))) / 2), 1*16, display_date);
  u8g2.drawStr(((u8g2.getDisplayWidth() - (u8g2.getUTF8Width(display_time))) / 2), 2*16, display_time);

  u8g2.setFont(u8g2_font_7x13_mr);
  char buffer[50];
  printDateTime(getUpTime(), buffer, false, true);
  message_1 = buffer;
  u8g2.drawStr(((u8g2.getDisplayWidth() - (u8g2.getUTF8Width(message_1.c_str()))) / 2), 3*16, message_1.c_str());

  printDateTime(getBootTime(), buffer, false, true);
  message_2 = buffer;
  u8g2.drawStr(((u8g2.getDisplayWidth() - (u8g2.getUTF8Width(message_2.c_str()))) / 2), 4*16, message_2.c_str());

  u8g2.sendBuffer();
  }

}
void process_display(){
  if (trigger_display_update){
    Display_update();
    trigger_display_update = 0;
  }
}
void init_Display(){

  // u8g2.setBusClock(1000000);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.sendBuffer();

}
// </DISPLAY>

// <RELAYS>
void load_relays(){
  JsonObject relays = config["Relays"].as<JsonObject>();
  for (JsonPair relay_config : relays) {
    uint8_t relay_id = relay_config.value()["id"];
    uint8_t relay_enabled = relay_config.value()["Enable"] | 0;
    relay[relay_id-1].enable(relay_enabled);
  }
  for (uint8_t relay_id=0; relay_id<RELAY_COUNT; relay_id++){
    relay[relay_id].setPwmHoldPower(relay_hold_power);
    relay[relay_id].setPwmFrequency(relay_pwm_frequency);
  }
}
// </RELAYS>

// <CONFIG
void load_config(){

  // System settings
  message_1 = (config["system"]["message1"] | "");
  message_2 = (config["system"]["message2"] | "");
  int brightness = (config["system"]["brightness"] | 5);
  u8g2.setContrast( (brightness-1)*(254/4) +1);
  relay_hold_power = (config["system"]["relay hold power"] | 50);
  relay_pwm_frequency = (config["system"]["relay pwm frequency"] | 1000);
  screen_saver_timeout = (config["system"]["display timeout"] | 60);

  // Relay timers
  load_timers();
  load_relays();
}

void save_config(){
  RtcDateTime now = getSystemTime();
  char buffer[50];
  printDateTime(now, buffer);
  config["metadata"]["timestamp"] = buffer;

  card.rm("/config-backup.json");
  card.mv("/config.json", "/config-backup.json");

  File config_file = card.open("/config.json", FILE_WRITE);

  serializeJsonPretty(config, config_file);

  card.close();
}

void process_reset(){
  if (screen_saver) return;

  if (button_state == 0 && millis() - button_last_event > 20000){
    card.rm("/config.json");
    
    Serial.println("All settings reset to default, rebooting!");
    delay(500);
    ESP.restart();

  }
}

void process_config(){
  if (trigger_config_update) {
    save_config();
    // load_config();
    trigger_config_update = 0;
  }
}

void init_Config(){
  card.begin();

  File config_file = card.open("/config.json");
  deserializeJson(config, config_file);
  card.close();
  
  Serial.println("== CONFIG ==");
  serializeJsonPretty(config, Serial);
  Serial.println();
  Serial.println("============");

  load_config();

}

// </CONFIG>
void setup() {
  Serial.begin(115200);
  Serial.print("Build date: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  init_Encoder();
  init_Display();
  init_RTC();
  init_Menu();
  init_Config(); //config gets loaded when everything is prepared
  process_timers();
  
  SimpleMenu::setDisplay(u8g2);
}

int32_t epoch = 0;
void loop() {
  process_button();
  process_encoder();
  process_reset();
  process_display();
  process_menu();
  process_config();
  process_timers();
  for (uint8_t r=0; r<RELAY_COUNT; r++ ){
    relay[r].run();
  }

  RtcDateTime now = getSystemTime();
  if (now.Unix32Time() != epoch){
    epoch = now.Unix32Time();

    printDateTime(now, display_date, true, false);
    printDateTime(now, display_time, false, true);

    if (SimpleMenu::current_menu == 0) trigger_display_update = 1;
  }

}
