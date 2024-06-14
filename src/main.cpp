#include <string>
#include <iostream>
#include <cstdio>
#include <serialrelay.hpp>
#include <serial/serial.h>


// OS Specific sleep



using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using relay::relayboard;



/*
void enumerate_ports()
{
	vector<serial::PortInfo> devices_found = serial::list_ports();

	vector<serial::PortInfo>::iterator iter = devices_found.begin();

	while( iter != devices_found.end() )
	{
		serial::PortInfo device = *iter++;

		printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
     device.hardware_id.c_str() );
	}
}

void ASCIISentence(std::string str)
{
    int l = str.length();
    int convert;
    for (int i = 0; i < l; i++) {
        convert = str[i];
        cout << convert << endl;
    }
}

int main(){
    enumerate_ports();

    serial::Serial my_serial("/dev/ttyUSB0", 9600, serial::Timeout::simpleTimeout(1000));


    sleep(1);

    uint8_t ident[] = {0x50};
    uint8_t ready[] = {0x51};
    uint8_t offallrelay[] = {0xFF};
    uint8_t onallrelay[] = {0x00};

    cout << "SEND IDENT" << endl; 
    my_serial.write(ident,sizeof(ident));
    sleep(1);

    cout << "READ ANSER" << endl;
    string result = my_serial.read(1);
    ASCIISentence(result);
    sleep(1);

    cout << "SEND INIT" << endl; 
    my_serial.write(ready,sizeof(ready));
    sleep(1);

    cout << "SEND 00 (open  all relay)" << endl; 
    my_serial.write(onallrelay,sizeof(onallrelay));
    sleep(1);

    cout << "SEND FF (close all relay)" << endl; 
    my_serial.write(offallrelay,sizeof(onallrelay));
    sleep(1);

    cout << "FIN" << endl;
    return 0;
}
*/

int main(){
    int commandon[4] = {0,0,1,0};
    int commandoff[4] = {0,0,0,0};
    relay::relayboard board = relay::usbrelay;
    relay::Serialrelay relay(1,9600,"/dev/ttyUSB0");
    relay.openCom();
    relay.initBoard(board);
    while(true){
    relay.setState(commandon);
    relay.setState(commandoff);
    }
    relay.closeCom();

}