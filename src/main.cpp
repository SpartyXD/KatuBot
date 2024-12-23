#include <KatuBot.h>

KatuBot* bot = nullptr;

void setup(){
  Serial.begin(9600);
  screen.init();
  // bot = new KatuBot();
  // bot->start();
}


void loop(){
  // bot->run();
  delay(100);
}


