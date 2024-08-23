#pragma once 
#include<string>
#include<vector>
#include <yaml-cpp/yaml.h>
#include <serialrelay.hpp>

typedef struct{
    std::string id;
    int relaynumber;
    std::string timebase;
    bool loop;
}config;


typedef struct{
    std::string function;
    std::string stringparameter; 
    int intparameter;
    std::vector<int> arrayparameter;
}inst;

typedef struct{
    std::string id;
    Serialrelay *board;
    std::vector<inst> program; 
    config configuration;
}boardprogram;

class Interpreter{

    public:
    
        Interpreter(std::string);
        std::vector<config> get_conf();
        std::vector<boardprogram> get_boardprogram();
        int match_conf_prog();
        int match_hardware(Serialrelay * board,std::string id);
        void generator(); 

    private:
        boardprogram parse_inst(const YAML::Node& instructions);
        config parse_conf(const YAML::Node& configuration);
        YAML::Node prog;
        std::vector<config> configs;
        std::vector<boardprogram> boardprograms;


};