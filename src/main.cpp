#include <objects.h>

//===================
//GLOBAL OBJECTS
Screen screen;
Speaker speaker;
Encoder encoder;
SwitchButton btn;

//===================
//Parameters

//Menus
int CURRENT_MENU = 0;
void idle_menu();
void tuning_menu();
void clock_running();


//Clock
unsigned long last_seconds_change = 0;
unsigned long time_now = 0;

int seconds_remaining = 0;
int MAX_SECONDS = 7200;
int STEP = 60;

//Time is up
unsigned long BEEP_DELAY = 800;
unsigned long last_beep = 0;

unsigned long SCREEN_DELAY = 800;
bool screen_on = true;
unsigned long last_screen_change = 0;


//Random things
int CURRENT_FACE = 0;
unsigned long last_message = 0;
unsigned long random_delay = 120000;

//===================

//Auxiliar
void change_random_things();
void time_is_up();


void setup(){
  Serial.begin(9600);
  Serial.println("Initializing systems...\n");
  // screen.init();
  speaker.init(BUZZER);
  encoder.init(CLK, DT);
  btn.init(SW);
  Serial.println("Systems up and running :D\n");
}


void loop(){
  //Message changes every 2 minutes
  time_now = millis();
  if(time_now-last_message > random_delay){
    last_message = time_now;
    change_random_things();
  }

  switch (CURRENT_MENU){
  case 0:
    idle_menu();
    break;
  
  case 1:
    tuning_menu();
    break;
  
  case 2:
    clock_running();
    break;
  
  default:
    break;
  }
}


//===================

//Menus

void idle_menu(){
  //Check button
  if(btn.isPressed()){
    speaker.actionBeep();
    // CURRENT_MENU = 1;
    return;
  }

  encoder.getState();
  // screen.showFace(CURRENT_FACE);
  // screen.show();
}


void tuning_menu(){
  //Check button
  if(btn.isPressed()){
    speaker.actionBeep();
    if(seconds_remaining == 0)
      CURRENT_MENU = 0; //idle
    else
      CURRENT_MENU = 2; //start timer
    
    return;
  }

  //Check encoder
  int current_state = encoder.getState();

  if(current_state == -1)
    seconds_remaining = constrain(seconds_remaining-STEP, 0, MAX_SECONDS); //Left (decrease)
  else if(current_state == 1)
    seconds_remaining = constrain(seconds_remaining+STEP, 0, MAX_SECONDS); //Right (increment)


  //Print current clock
  screen.printTune(seconds_remaining);
  screen.show();
}


void clock_running(){
  //Check button
  if(btn.isPressed()){
    speaker.actionBeep();
    CURRENT_MENU = 1; //Tunning
    return;
  }


  //Update time
  time_now = millis();
  if(time_now-last_seconds_change >= 1000){
    seconds_remaining = constrain(seconds_remaining-1, 0, MAX_SECONDS);
    last_seconds_change = time_now;
  }


  //Show clock
  screen.printClock(seconds_remaining);
  screen.show();


  //Time is up?
  if(seconds_remaining == 0){
    while(!btn.isPressed())
      time_is_up();

    //Show congrats face
    screen.showFace(1);
    screen.show();
    speaker.successBeep();
    delay(1300);
    CURRENT_MENU = 0; //Idle mode again
  }
}

//===================
//Auxiliar

void change_random_things(){
  //Face
  int idx = random(0, CANT_CARAS);
  while(idx == CURRENT_FACE)
    idx = random(0, CANT_CARAS);
  CURRENT_FACE = idx;
}


void time_is_up(){
  time_now = millis();

  //Beep
  if(time_now-last_beep > BEEP_DELAY){
    last_beep = time_now;
    speaker.timerBeep();
  }

  //Alternate screen number
  if(time_now-last_screen_change <= SCREEN_DELAY)
    return;

  last_screen_change = time_now;

  if(screen_on)
    screen.clear();
  else
    screen.printClock(0);
  
  screen_on = !screen_on;
  screen.show();
}