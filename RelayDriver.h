#ifndef PIN_HANDLER_H
#define PIN_HANDLER_H

class RelayDriver{
private:
uint8_t gate_count = 1;
uint8_t power_state = 0;
uint8_t logic_state = 0;
uint8_t last_state = 0;
int32_t since_state_change = 0;
uint8_t *states = 0;
uint8_t invert = 0;
uint8_t enabled = 1;
int8_t gpio_pin = -1;
uint8_t pwm_hold_power = 50;
uint32_t pwm_frequency = 1000;
uint8_t logic = 0;
uint8_t initialized = 0;

public:

enum state_logic {
  PIN_LOGIC_OR = 0,
  PIN_LOGIC_AND,
  PIN_LOGIC_XOR //effectively toggle mode
};


RelayDriver(uint8_t gpio_pin, uint8_t gate_count = 1, uint8_t logic = 0, uint8_t invert = 0){
  this->gate_count = gate_count;
  this->logic = logic;
  this->invert = invert;
  this->gpio_pin = gpio_pin;

  this->states = new uint8_t[gate_count];
  memset(states, 0, gate_count);
  
}

~RelayDriver(){
  delete[] this->states;
}

void run(){
  if (gate_count){

    if (!initialized){
      pinMode(gpio_pin, OUTPUT);
      initialized = 1;
    }

    if (this->enabled){
      this->logic_state = states[0];
      for (uint8_t i=1; i<gate_count; i++){
        if (logic == PIN_LOGIC_OR) {
          this->logic_state = this->logic_state | states[i];
        } else if (logic == PIN_LOGIC_AND){
          this->logic_state = this->logic_state & states[i];
        } else if (logic == PIN_LOGIC_XOR) {
          this->logic_state = this->logic_state ^ states[i];
        }
      }

    } else {
      this->logic_state = 0;
    }

    if (last_state != logic_state){
      power_state = logic_state*2; //0 or 2
      analogWrite(gpio_pin, 255*(logic_state^invert));
      since_state_change = millis();
      last_state = logic_state;
    }

    if (millis()-since_state_change>200 && power_state == 2) { //200ms hold time should be plenty
      uint8_t pwm_value = pwm_hold_power*2.55;
      analogWrite(gpio_pin, (invert) ? (255-pwm_value) : (pwm_value) );
      analogWriteFrequency(gpio_pin, pwm_frequency); //out of audible range to prevent coil whine
      power_state = 1;
    }
  }
}

void setState(uint8_t state, uint8_t gate){
  this->states[gate] = state;
}

void setPwmHoldPower(uint8_t hold_power){
  this->pwm_hold_power = hold_power;
  if (power_state == 1) power_state = 2; //trigger PWM state refresh
}

void setPwmFrequency(uint32_t frequency){
  this->pwm_frequency = frequency;
  if (power_state == 1) power_state = 2; //trigger PWM state refresh
}

void enable(uint8_t enabled = 1){
  this->enabled = enabled;
}

};


#endif