#ifndef TIMER_H
#define TIMER_H

#include <objects.h>

#define MAX_TIME 7200
#define STEP 300
#define RANDOM_DELAY 120000
#define ALARM_DELAY 800
#define SCREEN_DELAY 800

//===================
//GLOBAL OBJECTS
Screen screen;
Speaker speaker;
Encoder encoder;
SwitchButton btn;

//===================
//Messages

String messages[] = {"Keep going!", "Don't give up!", "C'mon Katu"};
int N_MESSAGES = 3;

//====================
//Screen

// void init_display(){
//     Serial.println("Initializing display...\n");

//     if (!display.begin(I2C_ADDRESS)) {
//         Serial.println("Failed to initialize display :(");
//         while (1);
//     }

//     Serial.println("Display initialized successfully!");
//     display.clearDisplay();
//     display.setCursor(0, 0);
//     display.setTextSize(1);
//     display.setTextColor(SH110X_WHITE);
//     display.println("Ready");
//     display.display();

//     delay(1000);
// }


// void printClock(int seconds, String message){
//     //Calculate
//     int m = seconds/60;
//     int s = seconds - m*60;

//     display.clearDisplay();
//     display.setTextSize(1);
//     display.setCursor(20, 6);

//     display.println(message);
//     display.setCursor(centerX, centerY);
//     display.setTextSize(2);

//     //Format
//     if(m < 10)
//         display.print("0");
//     display.print(m);

//     display.print(":");
    
//     if(s < 10)
//         display.print("0");
//     display.println(s);
// }


// void printCentered(String message, int text_size=1){
//     display.clearDisplay();
//     display.invertDisplay(false);
//     display.setTextSize(text_size);

//     display.setCursor(36, 26);
//     display.print(message);
// }


// void showFace(int idx){
//     idx = constrain(idx, 0, N_FACES-1);
//     display.clearDisplay();
//     display.drawBitmap(0, 0, Caras[idx], 128, 64, SH110X_WHITE);
// }


// void show(){
//     display.display();
// }


// void clear(){
//     display.clearDisplay();
// }

// void init_display();
// void show();
// void clear();
// void showFace(int idx);
// void printCentered(String message, int text_size=1);
// void printClock(int seconds, String message);











//====================================

class Timer{

private:
    //Clock
    bool running = false;

    unsigned long last_update_time = 0;
    unsigned long time_now = 0;
    int seconds_remaining = 0;

    //Alarm
    bool screen_on = true;
    bool time_is_up = false;

    unsigned long last_beep_time = 0;
    unsigned long last_toggle_time = 0;
    
public:

    Timer(){}

    int getTime(){
        return seconds_remaining;
    }

    void setTime(int t){
        seconds_remaining = constrain(t, 0, MAX_TIME);
    }

    void addTime(int t){
        setTime(seconds_remaining + t);
    }

    void pause(){
        running = false;
    }

    void start(){
        running = true;
        last_update_time = get_time();
    }


    void trigger_alarm(){
        if(btn.isPressed()){
            time_is_up = false;
            seconds_remaining = 0;
            running = false;
            screen_on = true;
        }

        time_now = get_time();

        //Beep
        if(time_now-last_beep_time > ALARM_DELAY){
            last_beep_time = time_now;
            speaker.timerBeep();
        }

        //Alternate screen on / off
        if(time_now-last_toggle_time <= SCREEN_DELAY)
            return;

        last_toggle_time = time_now;

        if(screen_on)
            screen.clear();
        else
            screen.printClock(0, "TIME'S UP");
        
        screen_on = !screen_on;
        screen.show();
    }

    // true-still running | false-timer_ready
    bool update(){
        if(running){
            //Update time
            time_now = get_time();
            if(time_now-last_update_time >= 1000){
                seconds_remaining = max(seconds_remaining-1, 0);
                last_update_time = time_now;
            }
        }
        
        //Time is up?
        if(seconds_remaining == 0){
            time_is_up = true;
            pause();

            last_beep_time = last_toggle_time = time_now;
            while(time_is_up)
                trigger_alarm();
            return false;
        }

        return true;
    }

};


class KatuBot{

private:
    // 0-idle | 1-tune | 2-running | 3-pause
    int CURRENT_MENU = 0;

    //Idle menu
    int CURRENT_FACE = 0;
    unsigned long last_random_time = 0;
    unsigned long time_now = 0;

    //Clock
    Timer clock;

    //Running menu
    unsigned long last_message_time = 0;
    String current_message;

    //Pause menu
    // 0-continue | 1-adjust | 2-finish
    int CURRENT_OPTION = 0;
    int N_OPTIONS = 3;

    //====================================
    //Auxiliar 

    void change_random_things(){
        time_now = get_time();

        //Face
        if(time_now-last_random_time > RANDOM_DELAY){
            last_random_time = time_now;

            int idx = random(0, N_FACES);
            while(idx == CURRENT_FACE)
                idx = random(0, N_FACES);
            CURRENT_FACE = idx;
            Serial.println("New face: " + String(CURRENT_FACE));
        }

        //Message
        if(time_now-last_message_time > RANDOM_DELAY){
            last_message_time = time_now;

            int idx = random(0, N_MESSAGES);
            while(messages[idx] == current_message)
                idx = random(0, N_MESSAGES);
            current_message = messages[idx];
            Serial.println("New message: " + String(current_message));
        }
    }


public:

    KatuBot(){}

    void start(){
        Serial.println("Initializing systems...\n");



        screen.printCentered("Loading....");
        screen.show();
        delay(1200);

        // speaker.init(BUZZER);
        // // encoder.init(CLK, DT);
        // // // btn.init(SW);
        // Serial.println("Systems up and running :D\n");

        // speaker.successBeep();
        // // // screen.printCentered("KATU BOT :D");
        // // screen.show();
        // delay(1200);
    }


    void idle_menu(){
        //Change to tunning?
        if(btn.isPressed()){
            speaker.actionBeep();
            CURRENT_MENU = 1;
            return;
        }

        //Show the current random face
        screen.showFace(CURRENT_FACE);
        screen.show();
    }


    void tuning_menu(){
        //Button press
        if(btn.isPressed()){
            if(clock.getTime() == 0){
                speaker.actionBeep();
                CURRENT_MENU = 0; //idle
            }
            else{
                speaker.notificationBeep();
                clock.start();
                CURRENT_MENU = 2; //start timer
            }
            return;
        }


        //Check encoder
        int current_state = encoder.getState();

        if(current_state == -1)
            clock.addTime(STEP);
        else if(current_state == 1)
            clock.addTime(-STEP);


        //Print current clock
        screen.printClock(clock.getTime(), "SET TIME");
        screen.show();
    }


    void running_menu(){
        //Pause timer?
        if(btn.isPressed()){
            speaker.actionBeep();
            clock.pause();
            CURRENT_MENU = 3; 
            return;
        }

        //Update and show clock
        if(clock.update()){
            screen.printClock(clock.getTime(), current_message);
            screen.show();
        }
        else{
            //Show congrats face
            screen.showFace(1);
            screen.show();
            speaker.successBeep();
            delay(1300);
            CURRENT_MENU = 0; //Idle mode again
        }
    }


    void pause_menu(){
        //Button?
        if(btn.isPressed()){
            speaker.actionBeep();
            CURRENT_MENU = 2; 
            clock.start();
            return;
        }

        screen.printClock(clock.getTime(), "PAUSED");
        screen.show();
    }


    void run(){
        change_random_things();
        switch (CURRENT_MENU){
        case 0:
            idle_menu();
            break;
        
        case 1:
            tuning_menu();
            break;
        
        case 2:
            running_menu();
            break;
        
        case 3:
            pause_menu();
            break;
        
        default:
            break;
        }
        delay(100);
    }

};


#endif
