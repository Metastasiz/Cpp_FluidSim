//
// Created by Mark on 13/04/2021.
//

#ifndef FLUIDSIM_BASICFRAME_H
#define FLUIDSIM_BASICFRAME_H

#endif //FLUIDSIM_BASICFRAME_H
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "MyConsole.h"
#define Height 90
#define Width 260
using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::chrono_literals::operator""ms;

class BasicFrame
{
public:
    MyConsole console{};
    const static int h = Height, w = Width;
    string out[h][w];
    void INIT(){
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                out[i][j] = " ";
        INIT_border();
    }
    void INIT_border(){
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                string t = out[i][j];
                if (mulEq(t,"╠╣╩╦")){}
                else if (i == 0 && j == 0) out[i][j] = "╔";
                else if (i == 0 && j == w-1) out[i][j] = "╗";
                else if (i == h-1 && j == 0) out[i][j] = "╚";
                else if (i == h-1 && j == w-1) out[i][j] = "╝";
                else if (i == h-1 || i == 0) out[i][j] = "═";
                else if (j == w-1 || j == 0) out[i][j] = "║";
            }
        }
    }
    bool checkIfBorder(string a){
        string tmp = "╔╩╗╚╝═║╬╣╠╦";
        for (int i = 0; i < tmp.length(); i++){
            if (tmp.find(a)!=string::npos){return true;}
        }
        return false;
    }
    void autoBorder(){
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                out[i][j] = fixBorder(i, j, out[i][j]);
    }
    bool mulEq(string value, string matcher){
        for (int i = 0; i < matcher.length(); i++)
        {
            if (value.find(matcher[i])!=string::npos) return true;
        }
        return false;
    }
    string fixBorder(int i, int j, string a){
        if (checkIfBorder(a)) {
            bool up = false, down = false, left = false, right = false;
            if (j>0)    if (mulEq(out[i][j-1],"═╠╔╚╦╩╬")) left = true;
            if (j<w-1)  if (mulEq(out[i][j+1],"═╣╗╝╦╩╬")) right = true;
            if (i>0)    if (mulEq(out[i-1][j],"║╣╗╔╦╠╬")) up = true;
            if (i<h-1)  if (mulEq(out[i+1][j],"║╣╚╝╩╠╬")) down = true;
            if (up&&!down&&left&&!right) return "╝";
            if (up&&!down&&!left&&right) return "╚";
            if (!up&&down&&left&&!right) return "╗";
            if (!up&&down&&!left&&right) return "╔";
            if (up&&down&&!left&&!right) return "║";
            if (up&&down&&left&&!right) return "╣";
            if (up&&down&&!left&&right) return "╠";
            if (!up&&!down&&left&&right) return "═";
            if (!up&&down&&left&&right) return "╦";
            if (up&&!down&&left&&right) return "╩";
            if (up&&down&&left&&right) return "╬";
        }
        return a;
    }
    void appendString(string a, int y, int x){
        int mxx = this->w-x<a.length()?this->w-x:a.length();
        for (int j = x; j < x+mxx; j++) {
            out[y][j] = a[j - x];
        }
    }
    void render(){
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++)
                cout << (out[i][j]);
            cout << "\n";
        }
    }
    void cutscene(){
        string tmp;

        system("clear");
        tmp = "THE SIMULATION WILL START SOON";
        appendString(tmp,h/2,w/2-tmp.length()/2);
        render();
        std::this_thread::sleep_for(3000ms);
        INIT();

        system("clear");
        tmp = "PLEASE MAKE SURE THE TOP OF THE WINDOW SCREEN";
        appendString(tmp,h/2-2,w/2-tmp.length()/2);
        tmp = "MATCHES THE TOP OF THE TERMINAL SCREEN";
        appendString(tmp,h/2,w/2-tmp.length()/2);
        tmp = "FOR YOUR BEST EXPERIENCE";
        appendString(tmp,h/2+2,w/2-tmp.length()/2);
        render();
        std::this_thread::sleep_for(4000ms);
        INIT();

        system("clear");
        tmp = "INTRODUCING";
        appendString(tmp,h/2-1,w/2-tmp.length()/2);
        tmp = "FLUID SIMULATION";
        appendString(tmp,h/2+1,w/2-tmp.length()/2);
        render();
        std::this_thread::sleep_for(3000ms);
        INIT();

        system("clear");
        tmp = "INSPIRED BY";
        appendString(tmp,h/2-1,w/2-tmp.length()/2);
        tmp = "NAVIER-STOKES EQUATION";
        appendString(tmp,h/2+1,w/2-tmp.length()/2);
        render();
        std::this_thread::sleep_for(3000ms);
        INIT();

        system("clear");
        tmp = "PRESENTED TO YOU BY";
        appendString(tmp,h/2-4,w/2-tmp.length()/2);
        tmp = "THANONDRAK ARUNSANGSIRINAK, S22130 12C";
        appendString(tmp,h/2-2,w/2-tmp.length()/2);
        //
        tmp = "INSTRUCTED BY LUKASZ LESNIEWSKI";
        appendString(tmp,h/2+2,w/2-tmp.length()/2);
        tmp = "LECTURED BY TOMASZ WERNER";
        appendString(tmp,h/2+4,w/2-tmp.length()/2);
        tmp = "THANKS TO EWA TURSKA";
        appendString(tmp,h/2+6,w/2-tmp.length()/2);
        tmp = "AT POLISH-JAPANESE ACADEMY OF INFORMATION TECHNOLOGY";
        appendString(tmp,h/2+9,w/2-tmp.length()/2);
        render();
        std::this_thread::sleep_for(7000ms);
        INIT();
    }
    void refresh(){
        string tmp;
        //Use I/O to communicate with user";
        tmp = "Use command /fluid to initiate fluid simulation";
        appendString(tmp,h/2-4,w/2-tmp.length()/2);
        tmp = "Use command /fluidshort to initiate short-length fluid simulation";
        appendString(tmp,h/2-2,w/2-tmp.length()/2);
        tmp = "Use command /load to load the parameters for fluid simulation";
        appendString(tmp,h/2,w/2-tmp.length()/2);
        tmp = console.output;
        appendString(tmp,h/2+2,w/2-tmp.length()/2);
        tmp = console.input;
        appendString(tmp,h/2+4,w/2-tmp.length()/2);
    }
    BasicFrame() {
        INIT();
        cutscene();
        while (true) {
            INIT();
            system("clear");
            refresh();
            render();
            cout << "Enter >> ";
            console.run();
        }
    }
};
