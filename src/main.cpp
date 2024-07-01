#include <string>
#include <iostream>
#include <cstdio>
#include <serialrelay.hpp>


// OS Specific sleep



using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using relay::relayboard;


int main(){
    int commandon[8] = {0,0,0,0,0,0,1,1};
    int commandoff[8] = {0,0,0,0,0,0,0,0};
    relay::relayboard board = relay::usbrelay;
    relay::Serialrelay relay(1,9600,"\\\\.\\COM3");
    relay.openCom();
    relay.initBoard(board);
    while(true){
    relay.setState(commandon);
    Sleep(1000);
    relay.setState(commandoff);
    Sleep(1000);
    }
    relay.closeCom();
    return 0;
}