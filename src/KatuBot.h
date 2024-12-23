#ifndef TIMER_H
#define TIMER_H

#include <objects.h>

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




#endif
