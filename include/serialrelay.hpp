
#pragma once
#include <serialib.hpp>
#include <memory>
#include <string>
#include <vector>



using std::string;



typedef enum{
    USBRELAY = 1,
    USBMRELAY = 2,
    USBBRELAY = 3,
    NOBOARD = 100
}relayboard;

typedef struct{
    relayboard boardtype;
    int baudrate;
}boardinfo; 


class Serialrelay
{

public:
    
    Serialrelay(int id,relayboard board,const string& port,int relaynumber = 8);
    int openCom();  
    int closeCom();
    int initBoard();
    int setState(std::vector<int>);
    char getRx();
    char getTx();
    std::vector<int> getState();
    relayboard getType();
    int getSpeed();
    int getId();
    std::string getPort();
    int getRelayNumber();
    
    
private:
    int delay = 20;
    int send(char  data, unsigned long milliseconds);
    int send(std::vector<int> data, unsigned long milliseconds);
    int recieve(int nbyte);
    void bufferrxAdd(char elt);
    void buffertxAdd(char elt);
    int id;
    boardinfo boardinfo;
    int relaynumber;
    std::string device; 
    std::vector<char> buffertx = std::vector<char>(8);
    std::vector<char> bufferrx = std::vector<char>(8);
    std::vector<int> boardstate = std::vector<int>(16,0);
    std::unique_ptr<serialib> boardinterface;
};

std::vector<std::string> scanBoard();
void my_sleep(unsigned long milliseconds);