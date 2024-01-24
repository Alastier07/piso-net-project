#include <TM1637Display.h>

#define CLK 2
#define DIO 3

TM1637Display display = TM1637Display(CLK, DIO);

long value = 900; // seconds (15 mins)
long my_time = 0;

void setup() {
  // put your setup code here, to run once:
  display.clear();
  display.setBrightness(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(my_time == 0){
    my_time = value;
  }

  int sec = my_time % 60;
  int min = (my_time - sec) / 60;

  display.showNumberDecEx(sec, 0b01000000, true, 2, 2);
  display.showNumberDecEx(min, 0b01000000, true, 2);
 
  my_time -= 1;
  delay(1000);
}
