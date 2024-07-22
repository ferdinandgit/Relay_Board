
#pragma once
#include <serialib.hpp>
#include <memory>
#include <string>
#include <vector>



using std::string;



typedef enum{
    USBRELAY = 1,
    USBMRELAY = 2,
    USBBRELAY = 3
}relayboard;

typedef struct{
    relayboard boardtype;
    int baudrate;
}boardinfo; 


class Serialrelay
{

public:
    
    Serialrelay(int id,relayboard board,const string& port);
    void openCom();  
    void closeCom();
    void initBoard();
    void setState(int commandarray[]);
    char getState();
    int getType();
    int getSpeed();
    int getId();
    std::string getPort();
    int getRelayNumber();
 
    
private:

    void send(char  data, unsigned long milliseconds);
    void recieve(int nbyte);
    void bufferrxAdd(char elt);
    void buffertxAdd(char elt);
    int id;
    boardinfo boardinfo;
    int relaynumber;
    std::string device; 
    char buffertx [8];
    char bufferrx [8]; 
    std::unique_ptr<serialib> boardinterface;
};

std::vector<std::string> scanBoard();