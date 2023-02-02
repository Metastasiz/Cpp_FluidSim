//
// Created by Mark on 14/04/2021.
//

#ifndef FLUIDSIM_FLUID_H
#define FLUIDSIM_FLUID_H

#endif //FLUIDSIM_FLUID_H
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#define Height 90
#define Width 260
#define Gravity 10
#define MinimumDegree M_PI/8
using std::cout;
using std::string;
using std::endl;
using std::chrono_literals::operator""ms;

class Fluid {
public:
    const static int h = Height, w = Width;
    const static double g;
    const static double deg;
    const double denMul = 1;
    bool done = false;
    int iter = 0;
    int posh, posw;
    double posvy, posvx;

    double denMx = 0, denMn = 0, curSum = 0, curCnt = 0;
    double vy[Height][Width], vx[Height][Width], vyn[Height][Width], vxn[Height][Width], den[Height][Width], denn[Height][Width];
    bool debug = !true;
    double stadenMn = 0, stadenMx = 0, stadenCnt = 0;
    int debugcounter = 0;
    string out[Height][Width];

    void INIT(){
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                out[i][j] = " ";
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++){
                vy[i][j] = 9999;
                vx[i][j] = 9999;
                den[i][j] = 9999;
            }
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++){
                vy[i][j] = 0;
                vx[i][j] = 0;
                den[i][j] = 1000;
            }
    }
    const string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }
    double getV(int y, int x){
        return sqrt(vy[y][x]*vy[y][x]+vx[y][x]*vx[y][x]);
    }
    void addFluid(int y, int x, double vey, double vex){
        vy[y][x] += vey;
        vx[y][x] += vex;
    }
    void run(){
        run(9999, 9999);
    }
    void run(double fs, double as){
        addFluid(h/2, w/2, 2,3);
        posh = h/2;
        posw = w/2;
        posvy = 2;
        posvx = 3;
        render();
        double sec = 0;
        while (sec < fs){
            if(!debug) {
                system("clear");
                srand(time(nullptr));
                int tmp = (int)(rand()%2);
                if (tmp == 1 && sec < as) {
                    iter = 0;
                    int addh = (int) (rand()%(h - 20)) + 10;
                    int addw = (int) (rand()%(w - 20)) + 10;
                    double vey = ((fmod((double)rand()/1000,6)+1) - 3);
                    double vex = ((fmod((double)rand()/1000,6)+1) - 3);
                    posh = addh;
                    posw = addw;
                    posvy = vey;
                    posvx = vex;
                    cout << "WAVE SPAWNED AT (X:"<< addw << ", Y:" << addw << ") WITH VELOCITY OF (X-AXIS:" << vex << ", Y-AXIS:" << vey << ") AT " << iter << " STEPS AGO";
                    addFluid(addh, addw, vey, vex);
                }
                else {iter++; cout << "WAVE SPAWNED AT (X:"<< posw << ", Y:" << posh << ") WITH VELOCITY OF (X-AXIS:" << posvx << ", Y-AXIS:" << posvy << ") AT " << iter << " STEPS AGO";}
            }
            runVelo();
            runDen();
            applyNext();
            render();
            std::this_thread::sleep_for(250ms);
            sec+=0.25;
            debugcounter++;
        }
        screenshot();
        done = true;
    }
    static bool inBwn(double value, double top, double bot){ return value <= top && value >= bot; }
    static double conv(double rad){if (rad < 0) rad += M_PI*2; return rad;}

    void printTest(){
        if (debug){
            if (!true){
                for (int i = 0; i < h; i++) {
                    for (int j = 0; j < w; j++) {
                        cout << den[i][j] << " ";
                    }
                    cout << "\n";
                }
            }
            for (int i = 1; i < h-1; i++) {
                for (int j = 1; j < w - 1; j++) {
                    cout << den[i][j] << " ";
                }
                cout << "\n";
            }
        }
        cout << "\n\n";
    }

    double magDen(int i, int j){
        return den[i][j]*sqrt(vy[i][j]*vy[i][j]+vx[i][j]*vx[i][j]);
    }
    double getSurDen(int y, int x){
        double avg = 0;
        for (int i = -1; i < 1; i++)
            for (int j = -1; j < 1; j++){
                if (i == 0 && j == 0) continue;
                avg += den[y+i][x+j];
            }
        return avg/8;
    }
    void runVelo(){
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++){
                if (!(vy[i][j] == 0 && vx[i][j]==0)) {
                    if (abs(vy[i][j])+abs(vx[i][j]) < 0.00625){
                        vy[i][j] = 0;
                        vx[i][j] = 0;
                        continue;
                    }
                    else if (den[i][j] < getSurDen(i,j)){
                        vy[i][j]/=2;vx[i][j]/=2;
                        continue;
                    }
                    if (i == 1          ) {vy[i][j]*=-1;vx[i][j]*=-1;}
                    else if (i == h-2   ) {vy[i][j]*=-1;vx[i][j]*=-1;}
                    else if (j == 1     ) {vy[i][j]*=-1;vx[i][j]*=-1;}
                    else if (j == w-2   ) {vy[i][j]*=-1;vx[i][j]*=-1;}
                    double rad = atan2(vy[i][j],vx[i][j]);
                    rad = conv(rad);
                    double mxrad = fmod(conv(rad+deg),(M_PI*2)), mnrad = fmod(conv(rad-deg),(M_PI*2));
                    int quarmn = -1, quarmx = -1;
                    for (int k = 0; k < 8; k++) {
                        if (k == 0) {
                            if (inBwn(mxrad, deg, 0)||inBwn(mxrad, 0, conv(0-deg))){quarmx = k;}
                            if (inBwn(mnrad, deg, 0)||inBwn(mnrad, 0, conv(0-deg))){quarmn = k;}
                        } else {
                            if (inBwn(mxrad, (deg*k*2)+deg, (deg*k*2)-deg)) {quarmx = k;}
                            if (inBwn(mnrad, (deg*k*2)+deg, (deg*k*2)-deg)) {quarmn = k;}
                        }
                    }
                    if (quarmx == 1 && quarmn == 7) {quarmx = 0; quarmn = 0;}
                    else if (quarmx == 2 && quarmn == 0) {quarmx = 1; quarmn = 1;}
                    else if (quarmx == 3 && quarmn == 1) {quarmx = 2; quarmn = 2;}
                    else if (quarmx == 4 && quarmn == 2) {quarmx = 3; quarmn = 3;}
                    else if (quarmx == 5 && quarmn == 3) {quarmx = 4; quarmn = 4;}
                    else if (quarmx == 6 && quarmn == 4) {quarmx = 5; quarmn = 5;}
                    else if (quarmx == 7 && quarmn == 5) {quarmx = 6; quarmn = 6;}
                    else if (quarmx == 0 && quarmn == 6) {quarmx = 7; quarmn = 7;}
                    double magL = ((deg*(quarmx+1)*2) - (mxrad+deg))/(deg*2);
                    double magU = 1-magL;
                    switch (quarmx){
                        case 0: {
                            vyn[i][j+1]+=magU*vy[i][j]/2;
                            vxn[i][j+1]+=magU*vx[i][j]/2;
                            denn[i][j+1]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                        case 1: {
                            vyn[i-1][j+1]+=magU*vy[i][j]/2;
                            vxn[i-1][j+1]+=magU*vx[i][j]/2;
                            denn[i-1][j+1]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                        case 2: {
                            vyn[i-1][j]+=magU*vy[i][j]/2;
                            vxn[i-1][j]+=magU*vx[i][j]/2;
                            denn[i-1][j]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                        case 3: {
                            vyn[i-1][j-1]+=magU*vy[i][j]/2;
                            vxn[i-1][j-1]+=magU*vx[i][j]/2;
                            denn[i-1][j-1]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                        case 4: {
                            vyn[i][j-1]+=magU*vy[i][j]/2;
                            vxn[i][j-1]+=magU*vx[i][j]/2;
                            denn[i][j-1]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                        case 5: {
                            vyn[i+1][j-1]+=magU*vy[i][j]/2;
                            vxn[i+1][j-1]+=magU*vx[i][j]/2;
                            denn[i+1][j-1]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                        case 6: {
                            vyn[i+1][j]+=magU*vy[i][j]/2;
                            vxn[i+1][j]+=magU*vx[i][j]/2;
                            denn[i+1][j]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                        case 7: {
                            vyn[i+1][j+1]+=magU*vy[i][j]/2;
                            vxn[i+1][j+1]+=magU*vx[i][j]/2;
                            denn[i+1][j+1]+=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magU*den[i][j]/g;
                        }break;
                    }
                    switch (quarmn){
                        case 0: {
                            vyn[i][j+1]+=magL*vy[i][j]/2;
                            vxn[i][j+1]+=magL*vx[i][j]/2;
                            denn[i][j+1]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                        case 1: {
                            vyn[i-1][j+1]+=magL*vy[i][j]/2;
                            vxn[i-1][j+1]+=magL*vx[i][j]/2;
                            denn[i-1][j+1]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                        case 2: {
                            vyn[i-1][j]+=magL*vy[i][j]/2;
                            vxn[i-1][j]+=magL*vx[i][j]/2;
                            denn[i-1][j]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                        case 3: {
                            vyn[i-1][j-1]+=magL*vy[i][j]/2;
                            vxn[i-1][j-1]+=magL*vx[i][j]/2;
                            denn[i-1][j-1]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                        case 4: {
                            vyn[i][j-1]+=magL*vy[i][j]/2;
                            vxn[i][j-1]+=magL*vx[i][j]/2;
                            denn[i][j-1]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                        case 5: {
                            vyn[i+1][j-1]+=magL*vy[i][j]/2;
                            vxn[i+1][j-1]+=magL*vx[i][j]/2;
                            denn[i+1][j-1]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                        case 6: {
                            vyn[i+1][j]+=magL*vy[i][j]/2;
                            vxn[i+1][j]+=magL*vx[i][j]/2;
                            denn[i+1][j]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                        case 7: {
                            vyn[i+1][j+1]+=magL*vy[i][j]/2;
                            vxn[i+1][j+1]+=magL*vx[i][j]/2;
                            denn[i+1][j+1]+=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                            den[i][j]-=mag(vy[i][j],vx[i][j])*magL*den[i][j]/g;
                        }break;
                    }
                    //unstable
                    vy[i][j]/=2;vx[i][j]/=2;
                }
            }
        printTest();
    }
    void runDen(){
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++){
                double tmp;
                if (den[i][j] > den[i-1][j])    {tmp = ((den[i][j] - den[i-1][j])  * g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i-1][j])  * g/(g*8*denMul)):(den[i][j] - den[i-1][j])  /2; den[i-1][j]  +=tmp; denn[i][j]-=tmp;}
                if (den[i][j] > den[i+1][j])    {tmp = ((den[i][j] - den[i+1][j])  * g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i+1][j])  * g/(g*8*denMul)):(den[i][j] - den[i+1][j])  /2; den[i+1][j]  +=tmp; denn[i][j]-=tmp;}
                if (den[i][j] > den[i][j+1])    {tmp = ((den[i][j] - den[i][j+1])  * g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i][j+1])  * g/(g*8*denMul)):(den[i][j] - den[i][j+1])  /2; den[i][j+1]  +=tmp; denn[i][j]-=tmp;}
                if (den[i][j] > den[i][j-1])    {tmp = ((den[i][j] - den[i][j-1])  * g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i][j-1])  * g/(g*8*denMul)):(den[i][j] - den[i][j-1])  /2; den[i][j-1]  +=tmp; denn[i][j]-=tmp;}
                if (den[i][j] > den[i-1][j-1])  {tmp = ((den[i][j] - den[i-1][j-1])* g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i-1][j-1])* g/(g*8*denMul)):(den[i][j] - den[i-1][j-1])/2; den[i-1][j-1]+=tmp; denn[i][j]-=tmp;}
                if (den[i][j] > den[i-1][j+1])  {tmp = ((den[i][j] - den[i-1][j+1])* g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i-1][j+1])* g/(g*8*denMul)):(den[i][j] - den[i-1][j+1])/2; den[i-1][j+1]+=tmp; denn[i][j]-=tmp;}
                if (den[i][j] > den[i+1][j-1])  {tmp = ((den[i][j] - den[i+1][j-1])* g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i+1][j-1])* g/(g*8*denMul)):(den[i][j] - den[i+1][j-1])/2; den[i+1][j-1]+=tmp; denn[i][j]-=tmp;}
                if (den[i][j] > den[i+1][j+1])  {tmp = ((den[i][j] - den[i+1][j+1])* g/(g*8*denMul)) > 1 ? ((den[i][j] - den[i+1][j+1])* g/(g*8*denMul)):(den[i][j] - den[i+1][j+1])/2; den[i+1][j+1]+=tmp; denn[i][j]-=tmp;}
            }
    }
    void applyNext(){
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++) {
                den[i][j]+=denn[i][j];
                denn[i][j]=0;
                vx[i][j]+=vxn[i][j];
                vxn[i][j]=0;
                vy[i][j]+=vyn[i][j];
                vyn[i][j]=0;
            }

    }
    string convertDen(double a){
        //select version
        string out = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
        //out = "@$#*!=;:~-,. ";
        //
        double step;
        double max = 1300;
        double min = 850;
        step = (max-min)/out.length();
        for (int i = 0; i < out.length(); i++){
            string tmp = "";
            if (i == out.length()-1) {if (a>max-(2*step)){return tmp+(out[(out.length()-1-i)]);}}
            else if (a<=max-(out.length()*step)+(i*step)){return tmp+(out[(out.length()-1-i)]);}
        }

        cout << "\n\n\n\n\nValue is not in bound - Value: " << a;
        return "T";
    }
    double mag(double y, double x){
        return 1+sqrt(y*y+x*x);
    }
    void sim(){
        denMx = den[1][1]; denMn = den[1][1]; curSum = 0; curCnt = 0;
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++){
                if (den[i][j] < denMn){denMn = den[i][j];}
                if (den[i][j] > denMx){denMx = den[i][j];}
                curSum += den[i][j]; curCnt++;
            }
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++)
                out[i][j] = convertDen(den[i][j]);
        stadenMn += denMn; stadenMx += denMx; stadenCnt++;
    }
    void render(){
        if (true){
            if (debug){
                printTest();
                for (int i = 1; i < h-1; i++) {
                    for (int j = 1; j < w - 1; j++){
                        if (vy[i][j]==0)cout<<"Y:0.------";
                        else cout<<"Y: " << vy[i][j];
                        if (vx[i][j]==0)cout<<"X:0.------";
                        else cout<<"X: " << vx[i][j];
                        cout << " ";
                    }
                    cout << "\n";
                }
            }
        }
        denMx = den[1][1]; denMn = den[1][1]; curSum = 0; curCnt = 0;
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++){
                if (den[i][j] < denMn){denMn = den[i][j];}
                if (den[i][j] > denMx){denMx = den[i][j];}
                curSum += den[i][j]; curCnt++;
            }
        for (int i = 1; i < h-1; i++)
            for (int j = 1; j < w-1; j++)
                out[i][j] = convertDen(den[i][j]);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++)
                cout << out[i][j];
            cout << "\n";
        }
        stadenMn += denMn; stadenMx += denMx; stadenCnt++;
        cout << "Max: " << denMx << ", Min: " << denMn <<", Current Avg: " << curSum/curCnt << ", Avg Max: "<< stadenMx/stadenCnt << ", Avg Min: " << stadenMn/stadenCnt << endl;
    }
    void screenshot(){
        string filename = "Screenshot_";
        filename.append(currentDateTime());
        filename.append(".txt");
        std::ofstream file (filename);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++)
                file << out[i][j];
            file << endl;
        }
    }
    void INIT_border(){
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                string t = out[i][j];
                if ((t == "╠") || (t == "╣") || (t == "╩") || (t == "╦"));
                else if (i == 0 && j == 0) out[i][j] = "╔";
                else if (i == 0 && j == w-1) out[i][j] = "╗";
                else if (i == h-1 && j == 0) out[i][j] = "╚";
                else if (i == h-1 && j == w-1) out[i][j] = "╝";
                else if (i == h-1 || i == 0) out[i][j] = "═";
                else if (j == w-1 || j == 0) out[i][j] = "║";
            }
        }
    }
    Fluid(){
        INIT();
        INIT_border();
    }
};