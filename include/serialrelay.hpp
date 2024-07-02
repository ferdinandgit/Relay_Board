
#include <serialib.hpp>
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
    char  getState();     
    
    
protected:
    void send(char  data, unsigned long milliseconds);
    void recieve(int nbyte);
    void bufferrxAdd(char elt);
    void buffertxAdd(char elt);
    int id;
    int baudrate; 
    int relaynumber;
    char buffertx [8];
    char bufferrx [8]; 
    std::string device; 
    relayboard board;
    std::unique_ptr<serialib> boardinterface;
};

}