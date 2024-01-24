#include <TM1637Display.h>

#define CLK_A 2
#define DIO_A 3

TM1637Display display_a = TM1637Display(CLK_A, DIO_A);

long value = 900; // seconds (15 mins)

// pc 1
long my_time_a = 4500;
int sec_a = 0;
int min_a = 0;

void setup() {
  // put your setup code here, to run once:
  display_a.clear();
  display_a.setBrightness(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(my_time_a == 0){
    my_time_a = value;
  }

  sec_a = my_time_a % 60;
  min_a = (my_time_a - sec_a) / 60;
    
  if(min_a >= 60){
    sec_a = min_a % 60; // convert to min's
    min_a = (min_a - sec_a) / 60; // convert to hr's
  }

  display_a.showNumberDecEx(sec_a, 0b01000000, true, 2, 2);
  display_a.showNumberDecEx(min_a, 0b01000000, true, 2);
 
  my_time_a -= 1;

  delay(1000);
}
