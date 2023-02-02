//
// Created by Mark on 14/04/2021.
//

#ifndef FLUIDSIM_MYCONSOLE_H
#define FLUIDSIM_MYCONSOLE_H

#endif //FLUIDSIM_MYCONSOLE_H
#include <iostream>
#include "Fluid.h"
#include <fstream>
using std::cin;
using std::cout;
using std::string;
using std::endl;

class MyConsole
{
    /*
        Future implementations
        out commands
        /help, /
        no-out commands
        wasd, /exit, /select
        easter commands
        bonk, gachi
    */
public:
    Fluid fluidSim{};
    static bool fluid;
    static string input, output;
    string getInput(){return input;}
    void run(){
        output = "";
        getline(cin,input);
        if (ine(input, "/fluid")){
            fluidSim.run();
            fluidSim.done = false;
        } else if (ine(input, "/fluidshort")){
            fluidSim.run(100,50);
            fluidSim.done = false;
        } else if (ine(input, "/load")){
            std::ifstream file ("INIT_Fluid.txt");
            if (!(file.is_open())){
                std::ofstream file ("INIT_Fluid.txt");
                file << "10 10\n\n//First Integer is how long you would like your simulation to be in second\n//Second Integer is how long you would like to spawn waves in second";
                output = "System did not detect INIT_Fluid.txt, the file has been created.";
            } else {
                int fin, fin2;
                file >> fin;
                file >> fin2;
                fluidSim.run(fin,fin2);
                fluidSim.done = false;
            }
        }
    }
    bool ine(string a, string b){
        if (a.length()!=b.length()){return false;}
        int tmp = a.length();
        for (int i = 0; i < tmp; i++){
            if (a[i] >= 'a' && a[i] <= 'z' || a[i] >= 'A' && a[i] <= 'Z'){
                if (tolower(a[i]) != tolower(b[i])){return false;}
            }
            else if (a[i] != b[i]){return false;}
        }
        return true;
    }
    MyConsole(){}
};