#include <serialrelay.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <serial/serial.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using relay::Serialrelay;
using relay::relayboard;
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


Serialrelay::Serialrelay(int id, int baudrate,const std::string &port){
    this->id = id;
    this->baudrate = baudrate;
    this->device = port;
};


void Serialrelay::openCom(){
    this->interface = std::make_unique<serial::Serial>(this->device, this->baudrate, serial::Timeout::simpleTimeout(1000)); 
    sleep(1);
    if(!this->interface->isOpen()){
        cout << "pas ouvert" << endl; 
    }
}

void Serialrelay::closeCom(){
    this->interface->close();
    if(this->interface->isOpen()){
        cout << "trow exep" << endl;
    }
}

void Serialrelay::bufferrxAdd(uint8_t elt){
    int lenght = sizeof(this->bufferrx);
    for(int k = lenght-1; k<=0;k--){
        this->bufferrx[k+1]=this->bufferrx[k];
    }
    this->bufferrx[0]=elt;
}

void Serialrelay::buffertxAdd(uint8_t elt){
    int lenght = sizeof(this->buffertx);
    for(int k = lenght-1; k<=0;k--){
        this->buffertx[k+1]=this->buffertx[k];
    }
    this->buffertx[0]=elt;
}

void Serialrelay::send(uint8_t data,unsigned long milliseconds){
        this->buffertxAdd(data);
        size_t byte = this->interface->write(this->buffertx,1);
        my_sleep(milliseconds);
}

void Serialrelay::recieve(int nbyte){
    for(int k = 1;k<=nbyte;k++){
        uint8_t tempbuffer[1];
        this->interface->read(tempbuffer,1);
        this->bufferrxAdd(tempbuffer[0]);
    }
}


void Serialrelay::initBoard(relayboard board){
    this->board = board;
    switch(board){
        /*
        USB-Relay-02,04,08 init protcol 
        ->send 0x50 for ident
        ->receive (0xad --> 2 relayboard) or (0xab --> 4 relayboard) or (0xac --> 8 relayboard) 
        */
        case 1:
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
    switch (this->board)
    {
        /*
        USB-Relay-02,04,08 control protocol
        */
        case 1:
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




