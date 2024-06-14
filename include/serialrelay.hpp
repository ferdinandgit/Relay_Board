
#include <serial/serial.h>
#include <memory>
#include<string>



using std::string;

namespace relay{

typedef enum{
    usbrelay = 1,
}relayboard;





class Serialrelay
{

public:
    Serialrelay(int id, int baudrate,const string& port);
    void openCom();  
    void closeCom();
    void initBoard(relayboard board);
    void setState(int commandarray[]);
    int  getState();     
    
    
protected:
    void send(uint8_t  data, unsigned long milliseconds);
    void recieve(int nbyte);
    void bufferrxAdd(uint8_t elt);
    void buffertxAdd(uint8_t elt);
    int id;
    int baudrate; 
    int relaynumber;
    uint8_t buffertx [8];
    uint8_t bufferrx [8]; 
    std::string device; 
    relayboard board;
    std::unique_ptr<serial::Serial> interface;
};
}