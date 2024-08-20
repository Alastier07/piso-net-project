#include <TM1637Display.h>

#define CLK_A 2
#define DIO_A 3
#define SENSOR_A 4
#define RELAY_A 5

#define CLK_B 9
#define DIO_B 10
#define SENSOR_B 11
#define RELAY_B 12

TM1637Display display_a = TM1637Display(CLK_A, DIO_A);
TM1637Display display_b = TM1637Display(CLK_B, DIO_B);

long value = 1800000; // milliseconds (30 mins)
// long value = 1500000; // 25 mins
long loop_delay = 0; // 0.475 milliseconds
unsigned long time_elapse_start;
unsigned long time_elapse_end;

// PC A
long a_time = 0; // milliseconds
int a_sec = 0;
int a_min = 0;

// PC B
long b_time = 0; // milliseconds
int b_sec = 0;
int b_min = 0;

long start_a = 0;
long end_a = 0;

long start_b = 0;
long end_b = 0;

void setup() {
  Serial.begin(9600);
  // PC A
  pinMode(SENSOR_A, INPUT_PULLUP);
  pinMode(RELAY_A, OUTPUT);   
  digitalWrite(RELAY_A, LOW);
  display_a.clear();
  display_a.setBrightness(5);
  // PC B
  pinMode(SENSOR_B, INPUT_PULLUP);
  pinMode(RELAY_B, OUTPUT);   
  digitalWrite(RELAY_B, LOW);
  display_b.clear();
  display_b.setBrightness(5);
}

void loop() {
  time_elapse_start = millis();
  
  int sensor_a_status = 0;
  int sensor_b_status = 0;

  // Sensor A Trigger
  sensor_a_status = sensor_trigger(digitalRead(SENSOR_A));
  if(sensor_a_status == 1){
    end_a = millis();
    if((end_a - start_a) < 100){
      sensor_a_status = 0;
    }
    start_a = millis();
    end_a = 0; 
  }
  a_time = sensor_que(sensor_a_status, a_time); // Que
  sensor_a_status = 0; // Reset

  // Sensor B Trigger
  sensor_b_status = sensor_trigger(digitalRead(SENSOR_B));
  if(sensor_b_status == 1){
    end_b = millis();
    if((end_b - start_b) < 100){
      sensor_b_status = 0;
    }
    start_b = millis();
    end_b = 0; 
  }
  b_time = sensor_que(sensor_b_status, b_time); // Que
  sensor_b_status = 0; // Reset

  // On/Off
  digitalWrite(RELAY_A, (a_time <= 0) ? HIGH : LOW);
  digitalWrite(RELAY_B, (b_time <= 0) ? HIGH : LOW);

  // Convertion Process
  a_sec = get_secs_display(a_time);
  a_min = get_mins_display(a_time, a_sec);

  b_sec = get_secs_display(b_time);
  b_min = get_mins_display(b_time, b_sec);
    
  if(a_min >= 60){
    a_sec = convert_to_min(a_min);
    a_min = convert_to_hour(a_min, a_sec);
  }

  if(b_min >= 60){
    b_sec = convert_to_min(b_min);
    b_min = convert_to_hour(b_min, b_sec);
  }

  delay(loop_delay);

  // Display A
  display_time(display_a, a_min, a_sec);

  // Sensor A Trigger
  sensor_a_status = sensor_trigger(digitalRead(SENSOR_A));
  if(sensor_a_status == 1){
    end_a = millis();
    if((end_a - start_a) < 100){
      sensor_a_status = 0;
    }
    start_a = millis();
    end_a = 0; 
  }
  a_time = sensor_que(sensor_a_status, a_time); // Que
  sensor_a_status = 0; // Reset

  // Sensor B Trigger
  sensor_b_status = sensor_trigger(digitalRead(SENSOR_B));
  if(sensor_b_status == 1){
    end_b = millis();
    if((end_b - start_b) < 100){
      sensor_b_status = 0;
    }
    start_b = millis();
    end_b = 0; 
  }
  b_time = sensor_que(sensor_b_status, b_time); // Que
  sensor_b_status = 0; // Reset

  // Display B
  display_time(display_b, b_min, b_sec);

  time_elapse_end = millis();

  // Time Deduction
  if(a_time > 0){
    a_time = time_remaining(a_time, loop_delay, time_elapse_end, time_elapse_start);
  }

  if(b_time > 0){
    b_time = time_remaining(b_time, loop_delay, time_elapse_end, time_elapse_start);
  }

  // Serial.println(time_elapse_end - time_elapse_start); // 64 to 65
  delay(loop_delay);
}

// #Helper Functions
long sensor_que(int sensor, long time){
  //  if (sensor == HIGH) {
   if (sensor == 1) {
    // sensor = LOW;
      return time + value;
    // Serial.println(a_time);
  }

  return time;
}

int sensor_trigger(int sensor){
   if (sensor == HIGH) {
    return 1;
  } else{
    return 0;
  }
}

int get_secs_display(long mil_time) {
  return (mil_time / 1000) % 60;
}

int get_mins_display(long mil_time, int sec) {
  return ((mil_time / 1000) - sec) / 60;
}

int convert_to_min(int min) {
  return min % 60;
}

int convert_to_hour(int min, int sec) {
  return (min - sec) / 60;
}

void display_time(TM1637Display display, int min, int sec){
  display.showNumberDecEx(sec, 0b01000000, true, 2, 2);
  display.showNumberDecEx(min, 0b01000000, true, 2);
}

long time_remaining(long time, int delay, unsigned long end, unsigned long start) {
  long final_time = time - (delay + (end - start));

  if(final_time < 0){
    final_time = 0;
  }

  return final_time;
}