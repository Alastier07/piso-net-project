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
long loop_delay = 500; // 0.475 milliseconds
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

// long start_a = 0;
// long end_a = 0;

// long start_b = 0;
// long end_b = 0;
int impulsACount=0;
int impulsBCount=0;

void incomingAImpuls() {
  impulsACount++;
}

void incomingBImpuls(){
  impulsBCount++;
}

void setup() {
  Serial.begin(9600);
  // PC A
  pinMode(SENSOR_A, INPUT_PULLUP);
  attachInterrupt(0, incomingAImpuls, FALLING);
  pinMode(RELAY_A, OUTPUT);   
  digitalWrite(RELAY_A, LOW);
  display_a.clear();
  display_a.setBrightness(5);
  // PC B
  pinMode(SENSOR_B, INPUT_PULLUP);
  attachInterrupt(0, incomingBImpuls, FALLING);
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
  if(impulsACount >= 5){
    a_time = sensor_que(a_time); // Que
    impulsACount = impulsACount - 5;
  }
  // Sensor B Trigger
  if(impulsBCount >= 5){
    b_time = sensor_que(b_time); // Que
    impulsBCount = impulsBCount - 5;
  }

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
long sensor_que(long time){
  return time + value;
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