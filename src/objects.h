#ifndef OBJECTS_H
#define OBJECTS_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <caras.h>

//=====================
//Pines
const int buzzer = 9;
const int sw_pin = 3;

//=====================

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADRESS 0x3C
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


class Screen{

private:
    int centerX = 36;
    int centerY = 26;

public:
    Screen(){}

    void init(){
        display.begin(0x3C, true);
        delay(2000);
        display.clearDisplay();

        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0, 0);
        delay(500);
    }


    void printCentered(String message){
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(centerX, centerY);

        display.println(message);
    }


    void printTune(int seconds){
        //Calculate
        int m = seconds/60;
        int s = seconds - m*60;

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(20, 6);

        display.println("AJUSTAR TIEMPO\n");
        display.setCursor(centerX, centerY);
        display.setTextSize(2);

        //Format
        if(m < 10)
            display.print("0");
        display.print(m);

        display.print(":");
        
        if(s < 10)
            display.print("0");
        display.println(s);
    }


    void printClock(int seconds){
        //Calculate
        int m = seconds/60;
        int s = seconds - m*60;

        //Status message
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(32, 6);

        display.println("FOCUS TIME!");
        display.setCursor(centerX, centerY);
        display.setTextSize(2);

        //Format
        if(m < 10)
            display.print("0");
        display.print(m);

        display.print(":");
        
        if(s < 10)
            display.print("0");
        display.println(s);
    }


    void showFace(int idx){
        idx = constrain(idx, 0, N_FACES-1);
        display.clearDisplay();
        display.drawBitmap(0, 0, Caras[idx], 128, 64, SH110X_WHITE);
    }


    void show(){
        display.display();
    }


    void clear(){
        display.clearDisplay();
    }


};


//============



class Speaker{
public:
    Speaker(){}

    void init(){
        pinMode(buzzer, OUTPUT);    
    }

    void makeSound(double frec, int dur){
        tone(buzzer, frec);
        delay(dur);
        noTone(buzzer);
    }

};


//============


class SwitchButton{
private:
    unsigned long last_check;
    unsigned long time_now;

    bool last_state;
    bool current_state;
    unsigned long debounceDelay = 200;

    bool pressed = false;

    void update(){
        time_now = millis();
        current_state = digitalRead(sw_pin);

        //Debounce delay has not passed
        if(time_now-last_check > debounceDelay){
            if(last_state == HIGH && current_state == LOW){
                last_check = time_now;
                pressed = true;
                last_state = current_state;
                return;
            }
        }

        pressed = false;
        last_state = current_state;
    }

public:
    SwitchButton(){}

    void init(){
        pinMode(sw_pin, INPUT_PULLUP);
    }


    bool isPressed(){
        update();
        return pressed;
    }



};



#endif