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
#define BUZZER 9
#define SW 3
#define DT 4
#define CLK 2

//=====================

class Encoder{
private:
    int dtPin, clkPin;
    volatile int last_s, current_s;
    volatile bool clockwise = false;
    volatile bool is_turned = false;
    static Encoder* instance;

    unsigned long debounce = 50, time_now,last_check=0;

    void handleRotation(){
        time_now = millis() % 100000000000;

        //debounce
        if(time_now-last_check <= debounce)
            return;

        current_s = (digitalRead(clkPin) << 1 | digitalRead(dtPin));

        if(current_s != last_s){
            is_turned = true;
            clockwise = !(last_s == 0b00 || last_s == 0b11);
        }
        else
            is_turned = false;
        
        last_s = current_s;
    }

    static void handler(){
        if(instance != nullptr)
            instance->handleRotation();
    }

public:
    Encoder(){}

    void init(int clk, int dt){
        clkPin = clk, dtPin=dt;
        instance = this;

        attachInterrupt(digitalPinToInterrupt(clkPin), handler, CHANGE);
        pinMode(dt, INPUT);
        Serial.println("Encoder ready!\n");
    }

    int getState(){
        if(!is_turned)
            return 0;
        
        is_turned = false;
        Serial.println("ROTATED!: " + String(clockwise) + "\n");
        return (clockwise ? 1 : -1);
    }
};

Encoder* Encoder::instance = nullptr;


//===============================================================

#define I2C_ADDRESS 0x3c 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1   

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


class Screen{

private:
    int centerX = 36;
    int centerY = 26;

public:
    Screen(){}

    void init(){
        display.begin(I2C_ADDRESS, true);

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0, 0);
        display.display();

        Serial.println("Display ready!\n");
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


    void printCentered(String message, int text_size=1){
        display.clearDisplay();
        display.invertDisplay(false);
        display.setTextSize(text_size);

        display.setCursor(46, 26);
        display.print(message);
    }


    void showFace(int idx){
        idx = constrain(idx, 0, CANT_CARAS-1);
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


//===============================================================


class Speaker{
private:
    int pin;
public:
    Speaker(){}

    void init(int p){
        pin = p;
        pinMode(pin, OUTPUT);  
        Serial.println("Speaker ready!\n");  
    }

    void makeSound(double frec, int dur){
        tone(pin, frec);
        delay(dur);
        noTone(pin);
    }

    void wakeUpBeep(){
        makeSound(700, 100);
        delay(50);
        makeSound(1000, 100);
        delay(50);
        makeSound(1300, 100);
        delay(150);
        makeSound(1000, 150);
    }

    void notificationBeep(){
        makeSound(1000, 200);
        delay(100);
        makeSound(800, 300);
    }

    void actionBeep(){
        makeSound(1000, 200);
    }

    void successBeep(){
        makeSound(700, 100);
        delay(50);
        makeSound(1000, 100);
        delay(50);
        makeSound(1300, 100);
    }

    void timerBeep(){
        makeSound(1000, 200);
        delay(100);
        makeSound(800, 300);
    }
};


//============


class SwitchButton{
private:
    unsigned long last_check;
    unsigned long time_now;

    int pin;

    bool last_state;
    bool current_state;
    unsigned long debounceDelay = 200;

    bool pressed = false;

    void update(){
        time_now = millis() % 10000000000;
        current_state = digitalRead(pin);

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

    void init(int p){
        pin = p;
        pinMode(pin, INPUT_PULLUP);
        Serial.println("Button ready!\n");
    }


    bool isPressed(){
        update();
        return pressed;
    }

};



#endif