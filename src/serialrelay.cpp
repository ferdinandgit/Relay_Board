#include <serialrelay.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <serialib.hpp>


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


using std::string;
using std::cerr;
using std::cout;
using std::endl;

void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}

 
boardinfo usbrelay  = {USBRELAY, 9600}; 
boardinfo usbbrelay = {USBBRELAY,9600}; 
boardinfo usbmrelay = {USBMRELAY,9600}; 

Serialrelay::Serialrelay(int id,relayboard board,const std::string &port){
    this->id = id;
    this->device = port;
    switch(board){
        case USBRELAY:
            this->boardinfo=usbrelay;
        break;
        case USBBRELAY:
            this->boardinfo=usbbrelay;
        break;
        case USBMRELAY: 
            this->boardinfo=usbmrelay;
        break;  
    }
};


void Serialrelay::openCom(){
    this->boardinterface = std::make_unique<serialib>();
    const char * device = this->device.c_str();
    this->boardinterface->openDevice(device,boardinfo.baudrate); 
    my_sleep(1);
    if(!this->boardinterface->isDeviceOpen()){
        cout << "pas ouvert" << endl; 
    }
}

void Serialrelay::closeCom(){
    this->boardinterface->closeDevice();
    if(this->boardinterface->isDeviceOpen()){
        cout << "trow exep" << endl;
    }
}

void Serialrelay::bufferrxAdd(char elt){
    int lenght = sizeof(this->bufferrx);
    for(int k = lenght-1; k<=0;k--){
        this->bufferrx[k+1]=this->bufferrx[k];
    }
    this->bufferrx[0]=elt;
}

void Serialrelay::buffertxAdd(char elt){
    int lenght = sizeof(this->buffertx);
    for(int k = lenght-1; k<=0;k--){
        this->buffertx[k+1]=this->buffertx[k];
    }
    this->buffertx[0]=elt;
}

void Serialrelay::send(char data,unsigned long milliseconds){
        this->buffertxAdd(data);
        size_t byte = this->boardinterface->writeChar(this->buffertx[0]);
        my_sleep(milliseconds);
}

void Serialrelay::recieve(int nbyte){
    for(int k = 1;k<=nbyte;k++){
        char tempbuffer[1];
        this->boardinterface->readChar(tempbuffer);
        this->bufferrxAdd(tempbuffer[0]);
    }
}

int Serialrelay::getRelayNumber(){
    return relaynumber;
}

int Serialrelay::getSpeed(){
    return boardinfo.baudrate;
}

std::string Serialrelay::getPort(){
    return device;
}

int Serialrelay::getId(){
    return id;
}

void Serialrelay::initBoard(){
    switch(boardinfo.boardtype){
        /*
        USB-Relay-02,04,08 init protcol 
        ->send 0x50 for ident
        ->receive (0xad --> 2 relayboard) or (0xab --> 4 relayboard) or (0xac --> 8 relayboard) 
        */
        case USBRELAY:
            this->send(0x50,200);
            this->recieve(1);
            uint8_t data = this->bufferrx[0];
            data = static_cast<int>(data);
            switch(data){
                case 0xad:
                    this->relaynumber = 2;
                    this->send(0x51,200);
                    this->send(0xff,200);
                break;
                case 0xab:
                    this->relaynumber = 4;
                    this->send(0x51,200);
                    this->send(0xff,200);
                break;
                case 0xac: 
                    this->relaynumber = 8;
                    this->send(0x51,200);
                    this->send(0xff,200);
                break;
                default:
                cout << "trow exep" << endl;
                break;
            }

        break;
        /*
        In oder to add other init protocol:
        -> add a board in the enum boardtype with a number in serialrelay.hpp
        -> add a case associated to the number in the init function 
        */
    }
}

 void Serialrelay::setState(int commandarray[]){
    switch (boardinfo.boardtype)
    {
        /*
        USB-Relay-02,04,08 control protocol
        */
        case USBRELAY:
            uint8_t com = !commandarray[this->relaynumber-1];
            for(int k=this->relaynumber-2;k>=0;k--){
                if(commandarray[k]==0){
                    com=com<<1;
                    com++;
                }
                else{
                com=com<<1;
                }
            }
            this->send(com,200);
        break;
        /*
        Add others boards protocols 
        */
    }
 }

std::vector<std::string> scanBoard(){
    std::vector<std::string> portlist;
    std::string device_name;
    serialib* device = new  serialib();
    for (int i=1;i<99;i++){
        // Check for Windows COM port
        #if defined (_WIN32) || defined( _WIN64)
            device_name = "\\\\.\\COM"+std::to_string(i);
        #endif

        // Check for Linux port
        #ifdef __linux__
            device_name = "/dev/ttyACM"+std::to_string(i-1);
         #endif

        if (device->openDevice(device_name.c_str(),115200)==1){
            portlist.push_back(device_name); 
            device->closeDevice();
        }
    }
    return portlist; 
}


