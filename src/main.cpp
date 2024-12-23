#include <objects.h>


//====================
//Globals
Speaker speaker;
SwitchButton btn;
Screen screen;

//Encoder
const int dtPin = 4;
const int clkPin = 2;

volatile bool encoderTurned = false;
volatile bool encoderDirection = false; //True = right
int lastStateCLK, currenStateCLK;

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
int STEP = 10;

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

//====================
//Functions

//Encoder
int encoder_state();
void encoderISR();

//Auxiliar
void change_random_things();
void time_is_up();

//Tones
void wakeUpBeep();
void successBeep();
void actionBeep();
void timerBeep();


//===================


void setup(){
  //Init all
  Serial.begin(9600);
  screen.init();

  screen.printCentered("Loading...");
  screen.show();
  delay(1200);

  btn.init();
  speaker.init();

  screen.printCentered("KATUBOT :D");
  screen.show();
  wakeUpBeep();
  delay(500);
  

  //Encoder
  attachInterrupt(digitalPinToInterrupt(clkPin), encoderISR, CHANGE);
  lastStateCLK = digitalRead(clkPin);
}


void loop(){
  //Message changes every 2 minutes
  time_now = get_time();
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
    actionBeep();
    CURRENT_MENU = 1;
    return;
  }

  screen.showFace(CURRENT_FACE);
  screen.show();
}


void tuning_menu(){
  //Check button
  if(btn.isPressed()){
    actionBeep();
    if(seconds_remaining == 0)
      CURRENT_MENU = 0; //idle
    else
      CURRENT_MENU = 2; //start timer
    
    return;
  }

  //Check encoder
  int current_state = encoder_state();

  if(current_state == 1)
    seconds_remaining = constrain(seconds_remaining-STEP, 0, MAX_SECONDS); //Left (decrease)
  else if(current_state == 2)
    seconds_remaining = constrain(seconds_remaining+STEP, 0, MAX_SECONDS); //Right (increment)


  //Print current clock
  screen.printClock(seconds_remaining, "Adjust time");
  screen.show();
}


void clock_running(){
  //Check button
  if(btn.isPressed()){
    actionBeep();
    CURRENT_MENU = 1; //Tunning
    return;
  }


  //Update time
  time_now = get_time();
  if(time_now-last_seconds_change >= 1000){
    seconds_remaining = constrain(seconds_remaining-1, 0, MAX_SECONDS);
    last_seconds_change = time_now;
  }


  //Show clock
  screen.printClock(seconds_remaining, "Focus time!");
  screen.show();


  //Time is up?
  if(seconds_remaining == 0){
    while(!btn.isPressed())
      time_is_up();

    //Show congrats face
    screen.showFace(1);
    screen.show();
    successBeep();
    delay(1300);
    CURRENT_MENU = 0; //Idle mode again
  }
}

//===================
//Auxiliar

void change_random_things(){
  //Face
  int idx = random(0, N_FACES);
  while(idx == CURRENT_FACE)
    idx = random(0, N_FACES);
  CURRENT_FACE = idx;
}


void time_is_up(){
  time_now = get_time();

  //Beep
  if(time_now-last_beep > BEEP_DELAY){
    last_beep = time_now;
    timerBeep();
  }

  //Alternate screen number
  if(time_now-last_screen_change <= SCREEN_DELAY)
    return;

  last_screen_change = time_now;

  if(screen_on)
    screen.clear();
  else
    screen.printClock(0, "Time is up!");
  
  screen_on = !screen_on;
  screen.show();
}


//===================

//Encoder


//TRUE = DERECHA
void encoderISR(){
  bool dtState = digitalRead(dtPin);
  bool clkState = digitalRead(clkPin);

  if(clkState != lastStateCLK){
    encoderTurned = true;
    encoderDirection = (clkState != dtState);
  }

  lastStateCLK = clkState;
}


//0 = No | 1 = izq | 2 = der
int encoder_state(){
  if(!encoderTurned) 
    return 0;

  encoderTurned = false;

  if(encoderDirection)
    return 2; //Derecha
  else
    return 1; //Izquierda
}


//===================
//TONES

void wakeUpBeep(){
  speaker.makeSound(700, 100);
  delay(50);
  speaker.makeSound(1000, 100);
  delay(50);
  speaker.makeSound(1300, 100);
  delay(150);
  speaker.makeSound(1000, 150);
}

void notificationBeep(){
  speaker.makeSound(1000, 200);
  delay(100);
  speaker.makeSound(800, 300);
}

void actionBeep(){
  speaker.makeSound(1000, 100);
}

void successBeep(){
  speaker.makeSound(700, 100);
  delay(50);
  speaker.makeSound(1000, 100);
  delay(50);
  speaker.makeSound(1300, 100);
}

void timerBeep(){
  speaker.makeSound(1000, 200);
  delay(100);
  speaker.makeSound(800, 300);
}