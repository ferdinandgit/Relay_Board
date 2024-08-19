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

Serialrelay::Serialrelay(int id,relayboard board,const std::string &port,int relaynumber){
    this->id = id;
    this->device = port;
    this->relaynumber = relaynumber;
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


int Serialrelay::openCom() {
    this->boardinterface = std::make_unique<serialib>(); // Create a new serial interface
    const char *device = this->device.c_str();
    this->boardinterface->openDevice(device,boardinfo.baudrate); // Open device with baud rate
    my_sleep(1); // Sleep for 1 millisecond
    if (!this->boardinterface->isDeviceOpen()) { // Check if the device opened successfully
        return -1; // Return -1 if the device is not open
    }
    return 1; // Return 1 if the device is open
}

int Serialrelay::closeCom() {
    this->boardinterface->closeDevice(); // Close the device
    if (this->boardinterface->isDeviceOpen()) { // Check if the device closed successfully
        return -1; // Return -1 if the device is still open
    }
    return 1; // Return 1 if the device is closed
}

void Serialrelay::bufferrxAdd(char elt){
    int lenght = bufferrx.size();
    for(int k = lenght-1; k<=0;k--){
        this->bufferrx[k+1]=this->bufferrx[k];
    }
    this->bufferrx[0]=elt;
}

void Serialrelay::buffertxAdd(char elt){
    int lenght = buffertx.size();
    for(int k = lenght-1; k<=0;k--){
        this->buffertx[k+1]=this->buffertx[k];
    }
    this->buffertx[0]=elt;
}

int Serialrelay::send(char data, unsigned long milliseconds) {
    this->buffertxAdd(data); // Add data to transmit buffer
    int status = this->boardinterface->writeChar(buffertx[0]); // Write data to device
    my_sleep(milliseconds); // Sleep for the specified time
    return status; // Return the status of the write operation
}

int Serialrelay::send(std::vector<int> data, unsigned long milliseconds) {
    char buffer[data.size()] = {0};
    for(int i = 0; i <= data.size() - 1; i++) {
        this->buffertxAdd(data[i]);
        buffer[i] = data[i];
    }
    int status = this->boardinterface->writeBytes(buffer, data.size()); // Write data to device
    my_sleep(milliseconds); // Sleep for the specified time
    return status; // Return the status of the write operation
}

int Serialrelay::recieve(int nbyte) {
    int status;
    for (int k = 1; k <= nbyte; k++) {
        char tempbuffer[2];
        status = this->boardinterface->readChar(tempbuffer, 500); // Read character with 500ms timeout
        this->bufferrxAdd(tempbuffer[0]); // Add received character to buffer
        if (status != 1) {
            return status; // Return status if read operation failed
        }
    }
    return status; // Return status of the last read operation
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

char Serialrelay::getRx(){
    return bufferrx[0];
}

char Serialrelay::getTx(){
    return buffertx[0];
}

relayboard Serialrelay::getType(){
    return boardinfo.boardtype;
}

int Serialrelay::initBoard(){
    
    switch(boardinfo.boardtype){
        /*
        USB-Relay-02,04,08 init protcol 
        ->send 0x50 for ident
        ->receive (0xad --> 2 relayboard) or (0xab --> 4 relayboard) or (0xac --> 8 relayboard) 
        */
        case USBRELAY:
            {
            if (this->send(0x50, 200) != 1) // Send initialization command
                return -1;
            if (this->recieve(1) != 1) // Receive response
                return -1;
            uint8_t data = this->bufferrx[0];
            data = static_cast<int>(data);
            switch (data) { // Set relay number based on response
                case 0xad:
                    this->relaynumber = 2;
                    if (this->send(0x51, 10) != 1) // Send additional initialization commands
                        return -1;
                    if (this->send(0xff, 10) != 1)
                        return -1;
                break;
                case 0xab:
                    this->relaynumber = 4;
                    if (this->send(0x51, 10) != 1)
                        return -1;
                    if (this->send(0xff, 10) != 1)
                        return -1;
                break;
                case 0xac:
                    this->relaynumber = 8;
                    if (this->send(0x51, 10) != 1)
                        return -1;
                    if (this->send(0xff, 10) != 1)
                        return -1;
                break;
                }
            }
        break;
        case USBMRELAY:
            {
             int status;
            for(int i = 1; i <= relaynumber; i++) {
                std::vector<int> buffer = {0xA0, i, 0, 0xA0 + i};
                status = send(buffer, delay);
                if(status != 1) {
                    return -1;
                }
            }
            return 1;
            }
        /*
        In oder to add other init protocol:
        -> add a board in the enum boardtype with a number in serialrelay.hpp
        -> add a case associated to the number in the init function 
        */
    }
    return 1;
}

 int Serialrelay::setState(std::vector<int> commandarray){
    switch (boardinfo.boardtype)
    {
        /*
        USB-Relay-02,04,08 control protocol
        */
        case USBRELAY:
            {
            uint8_t com;
            switch (relaynumber) {
                case 2:
                    com = commandarray[this->relaynumber - 1]; // Set state for 2 relays boards
                    for (int k = this->relaynumber - 2; k >= 0; k--) {
                        if (commandarray[k] == 1) {
                            com = com << 1;
                            com++;
                        } else {
                            com = com << 1;
                        }
                    }
                    if (this->send(com, 50) != 1)
                        return -1;
                    for(int i=0;i<=relaynumber-1;i++){
                        boardstate[i]=commandarray[i];
                    }
                    break;
                default:
                    com = !commandarray[this->relaynumber - 1]; // Set state for more than 2 relays boards
                    for (int k = this->relaynumber - 2; k >= 0; k--) {
                        if (commandarray[k] == 0) {
                            com = com << 1;
                            com++;
                        } else {
                            com = com << 1;
                        }
                    }
                    if (this->send(com, 50) != 1)
                        return -1;
                    for(int i=0;i<=relaynumber-1;i++){
                        boardstate[i]=commandarray[i];
                    }
                break;
                }
            }
            return 1; // Return 1 if the state is successfully set
        break;
        case USBMRELAY:
            {
            int status;
            for(int i = 1; i <= relaynumber; i++) {
                std::vector<int> buffer = {0xA0, i, commandarray[i - 1], 0xA0 + i + commandarray[i - 1]};
                status = send(buffer, delay);
                if(status != 1)
                    return -1;
                boardstate[i - 1] = commandarray[i - 1];
                }
            return 1;
            }
        break;
    }
    return -1;
}

std::vector<int> Serialrelay::getState(){
        return boardstate;
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