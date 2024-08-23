#include <interpreter.hpp>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <vector>
#include <regex>
#include <serialrelay.hpp>

Interpreter::Interpreter(std::string file){
    prog = YAML::LoadFile(file);
    if (prog["config"].IsSequence()) {
        for (const auto& configuration : prog["config"]) {
            configs.push_back(this->parse_conf(configuration));
        }
    }
    else{
        configs = {};
    }
    if (prog["program"].IsSequence()) {
        for (const auto& board : prog["program"]) {
            boardprogram myboardprogram = this->parse_inst(board["instructions"]);
            myboardprogram.id = board["board"].as<std::string>();
            boardprograms.push_back(myboardprogram);
        }
    }
    else{
        boardprograms = {};
    }

}
boardprogram Interpreter::parse_inst(const YAML::Node& instructions) {
    boardprogram myboardprogram;
    std::regex pattern("^k(1[0-6]|[1-9])$");
    for (const auto& node : instructions) {
        inst command;
        command.function = node["inst"].as<std::string>();
        if (command.function == "all") {
            command.stringparameter = node["state"].as<std::string>();
            myboardprogram.program.push_back(command);
        } 
        else if (command.function == "time") {
            command.intparameter = node["delay"].as<int>();
            myboardprogram.program.push_back(command);
        } 
        else if (command.function == "mask") {
            for (std::size_t i = 0; i < node["commandarray"].size(); i++) {
                command.arrayparameter.push_back(node["commandarray"][i].as<int>());
            }
            myboardprogram.program.push_back(command);
        }
        else if (std::regex_match(command.function,pattern)){
            command.intparameter = node["state"].as<int>();
        }
        else {
            myboardprogram.program = {};
            return myboardprogram;
        }   
    }
    return myboardprogram;
}

config Interpreter::parse_conf(const YAML::Node& configuration){
    config boardconfig;
    boardconfig.id = configuration["board"].as<std::string>();
    boardconfig.relaynumber = configuration["relaynumber"].as<int>();
    boardconfig.timebase = configuration["timebase"].as<std::string>();
    boardconfig.loop = configuration["loop"].as<bool>();
    return boardconfig;
}

int Interpreter::match_conf_prog(){
    int validatedmatch = 0;
    if(configs.size() != boardprograms.size()){
        return -1;
    }
    for(int k=0;k<configs.size();k++){
        for(int i=0;i<configs.size();i++){
            if(configs[k].id == configs[i].id && i!=k)
                return -1;
        }
    }
    for(int k=0;k<boardprograms.size();k++){
        for(int i=0;i<boardprograms.size();i++){
            if(boardprograms[k].id == boardprograms[i].id && i!=k)
                return -1;
        }
    }
    for(int k=0;k<configs.size();k++){
        for(int i=0;i<boardprograms.size();i++){
            if(configs[k].id == boardprograms[i].id){
                boardprograms[i].configuration=configs[k];
                validatedmatch++;
            }
        }
    }
    if(validatedmatch == boardprograms.size())
        return 1;
    else 
        return-1;
}

int Interpreter::match_hardware(Serialrelay * board,std::string id){
    for(int k=0;k<boardprograms.size();k++){
        if(boardprograms[k].id==id && boardprograms[k].configuration.relaynumber == board->getRelayNumber()){
            boardprograms[k].board=board;
            return 1;
        }
        else {
            return -1;
        }
    }
    return 1;
}

void Interpreter::generator_loop(){
    std::vector<boardprogram> loop;
    std::vector<int> pc; 
    for(auto boardprogram : boardprograms){
        if(boardprogram.configuration.loop)
            loop.push_back(boardprogram);
            pc.push_back(0);
    }
    while(true){

    }

}

std::vector<config> Interpreter::get_conf(){
    return configs;
}

std::vector<boardprogram> Interpreter::get_boardprogram(){
    return boardprograms;
}

int main() {
    Serialrelay* board = new Serialrelay(0,USBRELAY,"COM8");
    board->openCom();
    board->initBoard();
    Interpreter program = Interpreter("test.yaml");
    program.match_conf_prog();
    program.match_hardware(board,"board1");
    std::cout<<"hey"<<std::endl;
    return 0;
}
