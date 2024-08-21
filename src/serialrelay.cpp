#include <serialrelay.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <serialib.hpp>
#include <usb_relay_device.hpp>
#pragma comment(lib, "usb_relay_device.lib")

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
boardinfo usbbrelay = {USBBRELAY,0}; 
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

Serialrelay::Serialrelay(int id,relayboard board,pusb_relay_device_info_t usbbrelayptr){
    this->id = id;
    this->usbbrelayptr=usbbrelayptr;
    this->boardinfo=usbbrelay;
};


int Serialrelay::openCom() {
    if(boardinfo.boardtype == USBBRELAY){
        if (usb_relay_init() != 0){
            return -1;
        }
        handle_usbb = usb_relay_device_open(usbbrelayptr);
        if (handle_usbb == 0){
            return -1;
        }       
    }
    else{
        this->boardinterface = std::make_unique<serialib>(); // Create a new serial interface
        const char *device = this->device.c_str();
        this->boardinterface->openDevice(device,boardinfo.baudrate); // Open device with baud rate
        my_sleep(1); // Sleep for 1 millisecond
        if (!this->boardinterface->isDeviceOpen()) { // Check if the device opened successfully
            return -1; // Return -1 if the device is not open
        }
        return 1; // Return 1 if the device is open
    }
    return -1;
}

int Serialrelay::closeCom() {
    if(boardinfo.boardtype == USBBRELAY){
        usb_relay_device_close(handle_usbb);
        return 1;
    }
    else{
        this->boardinterface->closeDevice(); // Close the device
        if (this->boardinterface->isDeviceOpen()) { // Check if the device closed successfully
            return -1; // Return -1 if the device is still open
        }
        return 1; // Return 1 if the device is closed
    }
    return 1;
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
        /*
        USBM-Relay-02,04,08 init protcol  
        */
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
        USBB-Relay init protocol
        */
        case USBBRELAY:
            relaynumber=usbbrelayptr->type;
            usb_relay_device_close_all_relay_channel(handle_usbb);
        break;
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
        case USBBRELAY:
            {
            int status; 
            for(int i = 1; i<= relaynumber; i++){
                if(commandarray[i-1]){
                    status = usb_relay_device_open_one_relay_channel(handle_usbb, i);
                    }
                else{
                    status = usb_relay_device_close_one_relay_channel(handle_usbb, i);
                    }
                if(status != 0)
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


onlinedev scanBoard(){
    onlinedev dev;
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
            dev.usbdevice.push_back(device_name);
            dev.usbbptr.push_back(NULL); 
            device->closeDevice();
        }
    }
    usb_relay_init();
    pusb_relay_device_info_t hiddevice;
    hiddevice = usb_relay_device_enumerate();
    if(hiddevice != NULL){
        while(hiddevice->next != NULL){
            dev.usbbptr.push_back(hiddevice);
            hiddevice=hiddevice->next;
        }
        dev.usbbptr.push_back(hiddevice);
        for(int k=0;k<dev.usbbptr.size();k++){
            if(dev.usbbptr[k] != NULL){
                string serialnumber = dev.usbbptr[k]->serial_number;
                serialnumber+="_"+std::to_string(k);
                dev.usbdevice.push_back(serialnumber);
            }
        }
    }
    return dev; 
}