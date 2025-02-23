#ifndef SIMPLEMENU_H
#define SIMPLEMENU_H

#include <LinkedList.h>
#include <U8g2lib.h>  //Display
#include <RtcDateTime.h>
#include <RtcAlarmManager.h>

// Base class
// display->drawXBM(0, current_line+1, display->getDisplayWidth(), display->getDisplayHeight()-13-16-3, checkers);
static unsigned char checkers[] = {
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
 0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};

class SimpleMenu {
protected:
  inline static U8G2 *display = 0;
  String title = "Menu";
  int16_t id = 0;

  uint8_t initialized = 0;

  void (*load_function)(SimpleMenu *) = 0;
  void (*save_function)(SimpleMenu *) = 0;

public:
  inline static SimpleMenu *current_menu = 0;
  SimpleMenu *parent_menu = 0;

  SimpleMenu(String title){
    this->title = title;
  }

  virtual String getTitle() {
    return this->title;
  }

  virtual void setTitle(String title) {
    this->title = title;
  }

  void setId(int16_t id){
    this->id = id;
  }

  int16_t getId() {
    return this->id;
  }

  static void setDisplay(U8G2 &display){
    SimpleMenu::display = &display;
  }

  void goBack() {
    current_menu = parent_menu;
    if (current_menu) current_menu->drawMenu();
    initialized = 0;
  }

  void setLoadFunction(void (*callback)(SimpleMenu *)){
    this->load_function = callback;
  }

  void setSaveFunction(void (*callback)(SimpleMenu *)){
    this->save_function = callback;
  }

  void load(){
    if (load_function) load_function(this);
    initialized = 1;
  }

  void save(){
    if (save_function) save_function(this);
  }


  virtual void Left(uint16_t count = 1) = 0;
  virtual void Right(uint16_t count = 1) = 0;
  virtual void Click(uint16_t count = 1) = 0;
  virtual void drawMenu() = 0;
};


// Derived classes

// =======================================================================================================
// =======================================================================================================

// <SIMPLE DATE TIME MENU>
class SimpleDateTimeMenu : public SimpleMenu {
private:
  void printDateTime(const RtcDateTime &dt, char *buffer, bool date = true, bool time = true) {
    if (date && !time) {
      snprintf(buffer,
               11,
               PSTR("%04u-%02u-%02u"),
               dt.Year(),
               dt.Month(),
               dt.Day());
    }

    if (!date && time) {
      snprintf(buffer,
               9,
               PSTR("%02u:%02u:%02u"),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    }

    if (date && time) {
      snprintf(buffer,
               20,
               PSTR("%04u-%02u-%02u %02u:%02u:%02u"),
               dt.Year(),
               dt.Month(),
               dt.Day(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    }
  }

  char time[20];
  char date[20];
  RtcDateTime date_time;
  uint8_t display_time = 1;
  uint8_t display_date = 1;
  
  enum action {
    ACTION_CANCEL,
    ACTION_OK,
    ACTION_END
  };
  uint8_t selected_action = ACTION_CANCEL;

  enum date_time_fields {
    DT_YEAR=0,
    DT_MONTH,
    DT_DAY,
    DT_HOUR,
    DT_MINUTE,
    DT_SECOND,
    DT_ACTION,
    DT_END
  };
  uint8_t current_field = DT_YEAR;

public:
  SimpleDateTimeMenu(String title):SimpleMenu(title){}

  void drawMenu() {
    if (!initialized) load();

    printDateTime(date_time, date, true, false);
    printDateTime(date_time, time, false, true);
    display->clearBuffer();

    // <title>
    display->setDrawColor(1);
    display->setFont(u8g2_font_8x13_mr);
    display->drawStr(((display->getDisplayWidth() - (display->getUTF8Width(title.c_str()))) / 2), 1*13-3, title.c_str());

    display->setDrawColor(2);
    display->drawBox(0, 0, display->getDisplayWidth(), 1*13);
    // </title>

    display->setDrawColor(1);
    display->setFont(u8g2_font_9x15_mr);
    if (display_date) display->drawStr(((display->getDisplayWidth() - (display->getUTF8Width(date))) / 2), 1*13+1*16 -1, date);
    if (display_time) display->drawStr(((display->getDisplayWidth() - (display->getUTF8Width(time))) / 2), 1*13+2*16 -1, time);

    switch(current_field){
      case DT_YEAR:
        display->drawHLine(((display->getDisplayWidth() - (display->getUTF8Width(date))) / 2), 1*13+1*16, display->getUTF8Width("0000"));
        break;
      case DT_MONTH:
        display->drawHLine(((display->getDisplayWidth() - (display->getUTF8Width(date))) / 2) + display->getUTF8Width("0000-"), 1*13+1*16, display->getUTF8Width("00"));
        break;
      case DT_DAY:
        display->drawHLine(((display->getDisplayWidth() - (display->getUTF8Width(date))) / 2) + display->getUTF8Width("0000-00-"), 1*13+1*16, display->getUTF8Width("00"));
        break;
      case DT_HOUR:
        display->drawHLine(((display->getDisplayWidth() - (display->getUTF8Width(time))) / 2), 1*13+2*16, display->getUTF8Width("00"));
        break;
      case DT_MINUTE:
        display->drawHLine(((display->getDisplayWidth() - (display->getUTF8Width(time))) / 2) + display->getUTF8Width("00:"), 1*13+2*16, display->getUTF8Width("00"));
        break;
      case DT_SECOND:
        display->drawHLine(((display->getDisplayWidth() - (display->getUTF8Width(time))) / 2) + display->getUTF8Width("00:00:"), 1*13+2*16, display->getUTF8Width("00"));
        break;
    }

    display->setDrawColor(1);
    display->setFont(u8g2_font_8x13_mr);

    if (current_field == DT_ACTION && selected_action==ACTION_CANCEL)
      display->drawButtonUTF8(display->getDisplayWidth()/2/2, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW1|U8G2_BTN_SHADOW0, display->getDisplayWidth()/2-4-2,  1,  1, "Cancel" );
    else
      display->drawButtonUTF8(display->getDisplayWidth()/2/2, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW0, display->getDisplayWidth()/2-4-2,  1,  1, "Cancel" );

    if (current_field == DT_ACTION && selected_action==ACTION_OK)
      display->drawButtonUTF8(display->getDisplayWidth()/2/2*3, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW1|U8G2_BTN_SHADOW0, display->getDisplayWidth()/2-4-2,  1,  1, "OK" );
    else
      display->drawButtonUTF8(display->getDisplayWidth()/2/2*3, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW0, display->getDisplayWidth()/2-4-2,  1,  1, "OK" );


    display->sendBuffer();
  }

  void Left(uint16_t count = 1) {
    int32_t adjustment = 0;
    switch(current_field){
      case DT_YEAR:
        adjustment = 1*60*60*24*365;
        adjustment += (RtcDateTime::IsLeapYear(date_time.Year())) ? 1*60*60*24:0;
        break;
      case DT_MONTH:
        adjustment = 1*60*60*24*RtcDateTime::DaysInMonth(date_time.Year(), date_time.Month()-1);
        break;
      case DT_DAY:
        adjustment = 1*60*60*24;
        break;
      case DT_HOUR:
        adjustment = 1*60*60;
        break;
      case DT_MINUTE:
        adjustment = 1*60;
        break;
      case DT_SECOND:
        adjustment = 1;
        break;
      case DT_ACTION:
        selected_action = selected_action+ACTION_END-1;
        selected_action%=ACTION_END;
        break;
    }
    date_time = date_time - adjustment*count;
    drawMenu();
  }
  void Right(uint16_t count = 1) {
    int32_t adjustment = 0;
    switch(current_field){
      case DT_YEAR:
        adjustment = 1*60*60*24*365;
        adjustment += (RtcDateTime::IsLeapYear(date_time.Year()+1)) ? 1*60*60*24:0;
        break;
      case DT_MONTH:
        adjustment = 1*60*60*24*RtcDateTime::DaysInMonth(date_time.Year(), date_time.Month());
        break;
      case DT_DAY:
        adjustment = 1*60*60*24;
        break;
      case DT_HOUR:
        adjustment = 1*60*60;
        break;
      case DT_MINUTE:
        adjustment = 1*60;
        break;
      case DT_SECOND:
        adjustment = 1;
        break;
      case DT_ACTION:
        selected_action++;
        selected_action%=ACTION_END;
        break;
    }
    date_time = date_time + adjustment*count;
    drawMenu();
  }
  void Click(uint16_t count = 1) {
    if (current_field == DT_ACTION){
      if (selected_action == ACTION_OK) {
        save();
      }
      load();
      goBack();

      selected_action = ACTION_CANCEL; //reset
      current_field = DT_YEAR; //reset
      return;
    }
    current_field++;
    if (current_field == DT_END) current_field = DT_YEAR;
    drawMenu();
  }

  void setDateTime(RtcDateTime input) {
    this->date_time = input;
  }

  RtcDateTime getDateTime() {
    return this->date_time;
  }
};
// </SIMPLE DATE TIME MENU>

// =======================================================================================================
// =======================================================================================================

// <SIMPLE ALARM MENU>
class SimpleAlarmMenu : public SimpleMenu {
private:
  void printDateTime(const RtcDateTime &dt, char *buffer, bool date = true, bool time = true) {
    if (date && !time) {
      snprintf(buffer,
               11,
               PSTR("%04u-%02u-%02u"),
               dt.Year(),
               dt.Month(),
               dt.Day());
    }

    if (!date && time) {
      snprintf(buffer,
               9,
               PSTR("%02u:%02u:%02u"),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    }

    if (date && time) {
      snprintf(buffer,
               20,
               PSTR("%04u-%02u-%02u %02u:%02u:%02u"),
               dt.Year(),
               dt.Month(),
               dt.Day(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    }
  }

  uint8_t enabled = 0;
  char time[20];
  RtcDateTime date_time;
  RtcDateTime duration;

  enum action {
    ACTION_CANCEL,
    ACTION_OK,
    ACTION_END
  };
  uint8_t selected_action = ACTION_CANCEL;

  enum alarm_fields {
    AL_ENABLE=0,
    AL_HOUR,
    AL_MINUTE,
    AL_SECOND,
    AL_DURATION_HOUR,
    AL_DURATION_MINUTE,
    AL_DURATION_SECOND,
    AL_INTERVAL,
    AL_ACTION,
    AL_END
  };
  uint8_t current_field = AL_ENABLE;

  uint8_t interval = 0; //AL_EVERY_DAY;

public:
  enum intervals {
    AL_EVERY_DAY=0,
    AL_EVERY_2ND_DAY,
    AL_EVERY_HOUR,
    AL_EVERY_SUNDAY,
    AL_EVERY_MONDAY,
    AL_EVERY_TUESDAY,
    AL_EVERY_WEDNESDAY,
    AL_EVERY_THIRSDAY,
    AL_EVERY_FRIDAY,
    AL_EVERY_SATURDAY,
    AL_INTERVAL_END
  };

  inline static char interval_name[AL_INTERVAL_END][20]={
    "Every day",
    "Every 2nd day",
    "Every hour",
    "Every Sunday",
    "Every Monday",
    "Every Tuesday",
    "Every Wednesday",
    "Every Thursday",
    "Every Friday",
    "Every Saturday"
  };


  SimpleAlarmMenu(String title):SimpleMenu(title){
    date_time = RtcDateTime(2020, 1, 1, 0, 0, 0);
    duration = RtcDateTime(2020, 1, 1, 0, 0, 0);
  }

  String getTitle(uint8_t with_state) {
    if (with_state){
      if (!initialized) load();
      char buffer[50];
      sprintf(buffer, "%s %s", title.c_str(), enabled ? "(X)":"( )");
      initialized = 0;

      return buffer;
    } else {
      return title;
    }
  }
  
  String getTitle() {
    if (!initialized) load();
    char buffer[50];
    sprintf(buffer, "%s %s", title.c_str(), enabled ? "(X)":"( )");
    return buffer;
  }

  void enable(uint8_t state =1){
    this->enabled = state;
  }

  uint8_t isEnabled(){
    return enabled;
  }

  void setDateTime(RtcDateTime input) {
    this->date_time = input;
  }

  RtcDateTime getDateTime() {
    return this->date_time;
  }

  void setDuration(RtcDateTime input) {
    this->duration = input;
  }

  RtcDateTime getDuration() {
    return this->duration;
  }

  String getInterval(){
    return interval_name[interval];
  }

  void setInterval(String interval){
    for (uint8_t i=0; i<AL_INTERVAL_END; i++) {
      if (interval == String(interval_name[i])){
        this->interval = i;
        break;
      }
    }
  }

  void drawMenu() {
    if (!initialized) load();
    uint8_t current_line = 0;
    uint8_t current_page = 1;
    uint8_t max_page = 5;

    switch (current_field){
      case AL_ENABLE:
        current_page = 1;
        break;
      case AL_HOUR:
      case AL_MINUTE:
      case AL_SECOND:
        current_page = 2;
        break;
      case AL_DURATION_HOUR:
      case AL_DURATION_MINUTE:
      case AL_DURATION_SECOND:
        current_page = 3;
        break;
      case AL_INTERVAL:
        current_page = 4;
        break;
      case AL_ACTION:
        current_page = 5;
        break;
    }

    display->clearBuffer();

    // <title>
    display->setDrawColor(1);
    display->setFont(u8g2_font_8x13_mr);
    display->drawStr(2, 1*13-3, title.c_str());

    char buffer[50];
    sprintf(buffer, "%d/%d", current_page, max_page);
    display->drawStr(display->getDisplayWidth() - display->getUTF8Width(buffer), 1*13-3, buffer);

    display->setDrawColor(2);
    display->drawBox(0, 0, display->getDisplayWidth(), 1*13);
    current_line += 13;
    // </title>

    /*
    Layout
    13px header
    (64-13-16) = 35px body
    16px buttons
    */

    if (current_page == 1 || !enabled) {
      display->setFont(u8g2_font_9x15_mr);
      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width("Enable: [ ]")/2,
        current_line + (display->getDisplayHeight()-current_line-16)/2+(15/2),
        (enabled) ? ("Enable: [X]"):("Enable: [ ]"));

    } else if (current_page == 2 || current_page == 3) {
      RtcDateTime time_to_display;
      String field_description;

      if (current_page == 2) {
        time_to_display = date_time;
        field_description = "Start time";
      } else {
        time_to_display = duration;
        field_description = "Duration";
      }

      printDateTime(time_to_display, time, false, true);

      display->setFont(u8g2_font_9x15_mr);
      
      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width("00:00:00")/2,
        current_line + 15,
        time);

      switch(current_field){
        case AL_HOUR:
        case AL_DURATION_HOUR:
          display->drawHLine(
            display->getDisplayWidth()/2 - display->getUTF8Width("00:00:00")/2,
            current_line + 15 + 1,
            display->getUTF8Width("00"));
          break;
        case AL_MINUTE:
        case AL_DURATION_MINUTE:
          display->drawHLine(
            display->getDisplayWidth()/2 - display->getUTF8Width("00:00:00")/2 + display->getUTF8Width("00:"),
            current_line + 15 + 1,
            display->getUTF8Width("00"));
          break;
        case AL_SECOND:
        case AL_DURATION_SECOND:
          display->drawHLine(
            display->getDisplayWidth()/2 - display->getUTF8Width("00:00:00")/2 + display->getUTF8Width("00:00:"),
            current_line + 15 + 1,
            display->getUTF8Width("00"));
          break;
      }

      current_line += 15;
      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width(field_description.c_str())/2,
        current_line + 15,
        field_description.c_str()
      );

    } else if (current_page == 4){
      display->setFont(u8g2_font_8x13_mr);      

      display->drawStr(
        2,
        current_line + (display->getDisplayHeight()-current_line-16)/2+(15/2),
        interval_name[interval]);

      display->drawHLine(
        display->getUTF8Width("Every "),
        current_line + (display->getDisplayHeight()-current_line-16)/2+(15/2) + 2,
        display->getDisplayWidth() - display->getUTF8Width("Every ") - 2);

    } else { //confirm page
      display->setFont(u8g2_font_6x10_mr);

      printDateTime(date_time, time, false, true);
      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width("S: 00:00:00")/2,
        current_line + 10,
        "S: ");
      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width("S: 00:00:00")/2 + display->getUTF8Width("S: "),
        current_line + 10,
        time);
      current_line += 10;

      printDateTime(duration, time, false, true);
      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width("D: 00:00:00")/2,
        current_line + 10,
        "D: ");
      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width("D: 00:00:00")/2 + display->getUTF8Width("D: "),
        current_line + 10,
        time);
      current_line += 10;

      display->drawStr(
        display->getDisplayWidth()/2 - display->getUTF8Width(interval_name[interval])/2,
        current_line + 10,
        interval_name[interval]);

    }

    display->setDrawColor(1);
    display->setFont(u8g2_font_8x13_mr);

    if (current_field == AL_ACTION && selected_action==ACTION_CANCEL)
      display->drawButtonUTF8(display->getDisplayWidth()/2/2, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW1|U8G2_BTN_SHADOW0, display->getDisplayWidth()/2-4-2,  1,  1, "Cancel" );
    else
      display->drawButtonUTF8(display->getDisplayWidth()/2/2, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW0, display->getDisplayWidth()/2-4-2,  1,  1, "Cancel" );

    if (current_field == AL_ACTION && selected_action==ACTION_OK)
      display->drawButtonUTF8(display->getDisplayWidth()/2/2*3, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW1|U8G2_BTN_SHADOW0, display->getDisplayWidth()/2-4-2,  1,  1, "OK" );
    else
      display->drawButtonUTF8(display->getDisplayWidth()/2/2*3, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW0, display->getDisplayWidth()/2-4-2,  1,  1, "OK" );


    display->sendBuffer();
  }
  
  void Left(uint16_t count = 1) {
    int32_t adjustment = 0;
    switch(current_field){
      case AL_ENABLE:
        enabled = !enabled;
        break;
      case AL_INTERVAL:
        interval = interval+AL_INTERVAL_END-1;
        interval%=AL_INTERVAL_END;
        break;
      case AL_HOUR:
      case AL_DURATION_HOUR:
        adjustment = 1*60*60;
        break;
      case AL_MINUTE:
      case AL_DURATION_MINUTE:
        adjustment = 1*60;
        break;
      case AL_SECOND:
      case AL_DURATION_SECOND:
        adjustment = 1;
        break;
      case AL_ACTION:
        selected_action = selected_action+ACTION_END-1;
        selected_action%=ACTION_END;
        break;
    }

    if (current_field == AL_HOUR || current_field == AL_MINUTE || current_field == AL_SECOND) {
      date_time = date_time - adjustment*count;
    } else {
      duration = duration - adjustment*count;
    }
    
    drawMenu();
  }
  void Right(uint16_t count = 1) {
    int32_t adjustment = 0;
    switch(current_field){
      case AL_ENABLE:
        enabled = !enabled;
        break;
      case AL_INTERVAL:
        interval = interval+1;
        interval%=AL_INTERVAL_END;
        break;
      case AL_HOUR:
      case AL_DURATION_HOUR:
        adjustment = 1*60*60;
        break;
      case AL_MINUTE:
      case AL_DURATION_MINUTE:
        adjustment = 1*60;
        break;
      case AL_SECOND:
      case AL_DURATION_SECOND:
        adjustment = 1;
        break;
      case AL_ACTION:
        selected_action++;
        selected_action%=ACTION_END;
        break;
    }

    if (current_field == AL_HOUR || current_field == AL_MINUTE || current_field == AL_SECOND) {
      date_time = date_time + adjustment*count;
    } else {
      duration = duration + adjustment*count;
    }

    drawMenu();
  }
  void Click(uint16_t count = 1) {
    if (current_field == AL_ACTION){
      if (selected_action == ACTION_OK) {
        save();
      }
      load();
      goBack();

      selected_action = ACTION_CANCEL; //reset
      current_field = AL_ENABLE; //reset

      return;
    }

    if (enabled)
      current_field++;
    else
      current_field = AL_ACTION;

    if (current_field == AL_END) current_field = AL_ENABLE;
    drawMenu();
  }

};
// </SIMPLE ALARM MENU>

// =======================================================================================================
// =======================================================================================================

// <SIMPLE LIST MENU>
class SimpleListMenu : public SimpleMenu {
private:
  LinkedList<SimpleMenu *> menu_items;
  SimpleMenu *selected_item;
  uint16_t scroll_index = 0;
  uint8_t page_size = 3;
  uint8_t print_index = 0;

public:
  int16_t selected_index = 0;
  SimpleListMenu(String title):SimpleMenu(title){}

  void addMenu(SimpleMenu *item){
    item->parent_menu = this;
    menu_items.add(item);
  }

  void Left(uint16_t count = 1){
    selected_index = selected_index+menu_items.size()-1;
    selected_index %= menu_items.size();

    drawMenu();
  }

  void Right(uint16_t count = 1){
    selected_index = selected_index+1;
    selected_index %= menu_items.size();

    drawMenu();
  }

  void Click(uint16_t count = 1){
    selected_item = menu_items.get(selected_index);
    selected_item->parent_menu = this; //we override the parent anyway because "SimpleExitMenu" may have multiple parents
    current_menu = selected_item;
    if (current_menu) current_menu->drawMenu();
  }

  void drawMenu(){
    if (!initialized) load();

    if (selected_index < scroll_index ) scroll_index = selected_index;
    if (selected_index >= scroll_index+page_size ) scroll_index = selected_index-page_size+1;
    scroll_index %= menu_items.size();

    display->clearBuffer();

    // <title>
    display->setDrawColor(1);
    display->setFont(u8g2_font_7x13_mr);
    display->drawStr(((display->getDisplayWidth() - (display->getUTF8Width(title.c_str()))) / 2), 1*13-3, title.c_str());

    display->setDrawColor(2);
    display->drawBox(0, 0, display->getDisplayWidth(), 1*13);
    // </title>

    display->setDrawColor(1);
    // display->setFont(u8g2_font_9x15_mr);
    display->setFont(u8g2_font_6x13_mr);
    
    uint16_t index_to_print = 0;
    char item_title[50];

    index_to_print = scroll_index;
    if (index_to_print < menu_items.size()){
      if (print_index)
        sprintf(item_title, "%d.%s", index_to_print+1, menu_items.get(index_to_print)->getTitle().c_str());
      else
        sprintf(item_title, "%s", menu_items.get(index_to_print)->getTitle().c_str());
      display->drawButtonUTF8(
        2,
        (display->getDisplayHeight()-14)/3*1 +9,
        (index_to_print == selected_index) ? (U8G2_BTN_SHADOW0|U8G2_BTN_BW1):(U8G2_BTN_BW0),
        (index_to_print == selected_index) ? (display->getDisplayWidth()-4-1):(display->getDisplayWidth()),
        1, 1,
        item_title);
    }

    index_to_print = scroll_index+1;
    if (index_to_print < menu_items.size()){
      if (print_index)
        sprintf(item_title, "%d.%s", index_to_print+1, menu_items.get(index_to_print)->getTitle().c_str());
      else
        sprintf(item_title, "%s", menu_items.get(index_to_print)->getTitle().c_str());
      display->drawButtonUTF8(
        2,
        (display->getDisplayHeight()-14)/3*2 +9,
        (index_to_print == selected_index) ? (U8G2_BTN_SHADOW0|U8G2_BTN_BW1):(U8G2_BTN_BW0),
        (index_to_print == selected_index) ? (display->getDisplayWidth()-4-1):(display->getDisplayWidth()),
        1, 1,
        item_title);
    }

    index_to_print = scroll_index+2;
    if (index_to_print < menu_items.size()){
      if (print_index)
        sprintf(item_title, "%d.%s", index_to_print+1, menu_items.get(index_to_print)->getTitle().c_str());
      else
        sprintf(item_title, "%s", menu_items.get(index_to_print)->getTitle().c_str());
      display->drawButtonUTF8(
        2,
        (display->getDisplayHeight()-14)/3*3 +9,
        (index_to_print == selected_index) ? (U8G2_BTN_SHADOW0|U8G2_BTN_BW1):(U8G2_BTN_BW0),
        (index_to_print == selected_index) ? (display->getDisplayWidth()-4-1):(display->getDisplayWidth()),
        1, 1,
        item_title);
    }

    display->sendBuffer();
  }
};
// </SIMPLE LIST MENU>

// =======================================================================================================
// =======================================================================================================

// <SIMPLE ON/OFF MENU>
class SimpleOnOffMenu : public SimpleMenu {
  private:
  uint8_t state = 0;

  public:
  SimpleOnOffMenu(String title):SimpleMenu(title){}
  
  String getTitle(uint8_t with_state) {
    if (with_state){
      if (!initialized) load();
      char buffer[50];
      sprintf(buffer, "%s %s", title.c_str(), state ? "(X)":"( )");
      return buffer;
    } else {
      return title;
    }
  }
  
  String getTitle() {
    if (!initialized) load();
    char buffer[50];
    sprintf(buffer, "%s %s", title.c_str(), state ? "(X)":"( )");
    return buffer;
  }

  uint8_t getValue() {return state;}
  void setValue(uint8_t state) {this->state = state;}
  void toggle() {state = !state;}

  void Left(uint16_t count = 1){};
  void Right(uint16_t count = 1){};
  void Click(uint16_t count = 1){};

  void drawMenu(){
    toggle();
    save();
    current_menu = parent_menu; //stay on current menu on draw call
    if (current_menu) current_menu->drawMenu();
  };
};
// </SIMPLE ON/OFF MENU>

// =======================================================================================================
// =======================================================================================================

// <SIMPLE NUMBER MENU>
class SimpleNumberMenu : public SimpleMenu {
  private:
  int32_t value = 0;
  int32_t min_value = 0;
  int32_t max_value = 100;
  String unit = "";

 
  enum action {
    ACTION_CANCEL,
    ACTION_OK,
    ACTION_END
  };
  uint8_t selected_action = ACTION_CANCEL;

  enum number_fields {
    NUM_MODIFY,
    NUM_ACTION,
    NUM_END
  };
  uint8_t current_field = NUM_MODIFY;


  public:
  SimpleNumberMenu(String title):SimpleMenu(title){}
  
  String getTitle(uint8_t with_state) {
    if (with_state){
      if (!initialized) load();
      char buffer[50];
      sprintf(buffer, "%s (%ld)", title.c_str(), value);
      return buffer;
    } else {
      return title;
    }
  }
  
  String getTitle() {
    if (!initialized) load();
    char buffer[50];
    sprintf(buffer, "%s (%ld)", title.c_str(), value);
    return buffer;
  }

  int32_t getValue() {return value;}
  void setValue(int32_t value) {this->value = constrain(value, min_value, max_value);}
  void setMin(int32_t min_value) {this->min_value = min_value;}
  void setMax(int32_t max_value) {this->max_value = max_value;}
  void setUnit(String unit) {this->unit = unit;}
  void Left(uint16_t count = 1){
    if (current_field == NUM_MODIFY){
      value = constrain(value-count, min_value, max_value);
      
    } else if (current_field == NUM_ACTION) {
      selected_action = selected_action+ACTION_END-1;
      selected_action %= ACTION_END;
    }
    drawMenu();
  }

  void Right(uint16_t count = 1){
    if (current_field == NUM_MODIFY){
      value = constrain(value+count, min_value, max_value);

    } else if (current_field == NUM_ACTION) {
      selected_action++;
      selected_action %= ACTION_END;
    }
    drawMenu();
  }

  void Click(uint16_t count = 1){
    if (current_field == NUM_ACTION){
      if (selected_action == ACTION_OK) {
        save();
      }

      load();
      goBack();
      selected_action = ACTION_CANCEL; //reset
      current_field = NUM_MODIFY; //reset
      return;
    }
    current_field++;
    if (current_field == NUM_END) current_field = NUM_MODIFY;
    drawMenu();
  }

  void drawMenu(){
    if (!initialized) load();
    uint8_t current_line = 0;
    display->clearBuffer();

    // <title>
    display->setDrawColor(1);
    display->setFont(u8g2_font_8x13_mr);
    display->drawStr(((display->getDisplayWidth() - (display->getUTF8Width(title.c_str()))) / 2), 1*13-3, title.c_str());

    display->setDrawColor(2);
    display->drawBox(0, 0, display->getDisplayWidth(), 1*13);
    current_line += 13;
    // </title>

    char buffer[50];
    sprintf(buffer, "%d%s", value, unit.c_str());
    display->setDrawColor(1);
    display->setFont(u8g2_font_9x15_mr);
    display->drawStr(((display->getDisplayWidth() - (display->getUTF8Width(buffer))) / 2), current_line+3+14, buffer);
    if (current_field == NUM_MODIFY) display->drawHLine(((display->getDisplayWidth() - (display->getUTF8Width(buffer))) / 2), current_line+3+14+1, display->getUTF8Width(buffer));
    current_line += 16;

    // progress bar
    display->setFont(u8g2_font_6x13_mr);
    sprintf(buffer, "%d", min_value);
    display->drawStr(0, current_line+1+5, buffer);
    sprintf(buffer, "%d", max_value);
    display->drawStr(display->getDisplayWidth()-display->getUTF8Width(buffer), current_line+1+5, buffer);
    current_line += 3;
    display->drawFrame(0, current_line+1+5, display->getDisplayWidth(), 6);
    display->drawBox(0+2, current_line+1+5+2, (display->getDisplayWidth()-2-2)*((float)(value-min_value)/(max_value-min_value)), 6-2-2);


    display->setDrawColor(1);
    display->setFont(u8g2_font_9x15_mr);
    if (current_field == NUM_ACTION && selected_action==ACTION_CANCEL)
      display->drawButtonUTF8(display->getDisplayWidth()/2/2, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW1|U8G2_BTN_SHADOW0, display->getDisplayWidth()/2-4-2,  1,  1, "Cancel" );
    else
      display->drawButtonUTF8(display->getDisplayWidth()/2/2, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW0, display->getDisplayWidth()/2-4-2,  1,  1, "Cancel" );

    if (current_field == NUM_ACTION && selected_action==ACTION_OK)
      display->drawButtonUTF8(display->getDisplayWidth()/2/2*3, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW1|U8G2_BTN_SHADOW0, display->getDisplayWidth()/2-4-2,  1,  1, "OK" );
    else
      display->drawButtonUTF8(display->getDisplayWidth()/2/2*3, display->getDisplayHeight()-12/2, U8G2_BTN_HCENTER|U8G2_BTN_BW0, display->getDisplayWidth()/2-4-2,  1,  1, "OK" );

    display->sendBuffer();
  };
};
// </SIMPLE NUMBER MENU>

// =======================================================================================================
// =======================================================================================================

// <SIMPLE EXIT MENU>
class SimpleExitMenu : public SimpleMenu {
  public:
  SimpleExitMenu(String title):SimpleMenu(title){}

  void Left(uint16_t count = 1){};
  void Right(uint16_t count = 1){};
  void Click(uint16_t count = 1){};

  void drawMenu(){
    ((SimpleListMenu*)parent_menu)->selected_index = 0; //we assume this menu was called from a list menu, should be done via dynamic cast
    
    current_menu = parent_menu->parent_menu; //jump back on draw call
    if (current_menu) current_menu->drawMenu();
  };
};
// </SIMPLE EXIT MENU>
#endif