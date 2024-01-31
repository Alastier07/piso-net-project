#include <TM1637Display.h>

#define CLK_A 2
#define DIO_A 3
#define SENSOR_A 8

TM1637Display display_a = TM1637Display(CLK_A, DIO_A);

long value = 1200000; // milliseconds (20 mins)
int loop_delay = 20; // milliseconds
unsigned long time_elapse_start;
unsigned long time_elapse_end;

// PC A
long a_time = 0; // milliseconds
int a_sec = 0;
int a_min = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SENSOR_A, INPUT_PULLUP);

  display_a.clear();
  display_a.setBrightness(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  time_elapse_start = millis();
  
  if(digitalRead(SENSOR_A) == 1){
    a_time += value;
    Serial.println(a_time);
  }

  a_sec = get_secs_display(a_time);
  a_min = get_mins_display(a_time, a_sec);
    
  if(a_min >= 60){
    a_sec = convert_to_min(a_min);
    a_min = convert_to_hour(a_min, a_sec);
  }

  // display_a.showNumberDecEx(a_sec, 0b01000000, true, 2, 2);
  // display_a.showNumberDecEx(a_min, 0b01000000, true, 2);

  display_time(display_a, a_min, a_sec);

  time_elapse_end = millis();

  if(a_time >= 0){
    a_time = time_elaspe(a_time, loop_delay, time_elapse_end, time_elapse_start);
  }
  
  // Serial.println(time_elapse_end - time_elapse_start);
  delay(loop_delay);
}

// #Helper Functions
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

long time_elaspe(long time, int delay, unsigned long end, unsigned long start) {
  return time - (delay + (end - start));
}