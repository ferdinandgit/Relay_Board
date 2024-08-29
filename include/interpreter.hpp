#pragma once 
#include<string>
#include<vector>
#include <yaml-cpp/yaml.h>
#include <serialrelay.hpp>
#include <thread>
#include <atomic>

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

class Interpreter {
public:
    Interpreter(std::string file);
    int match_conf_prog();
    int match_hardware(Serialrelay* board, std::string id);
    std::vector<config> get_conf();
    std::vector<boardprogram> get_boardprogram();
    std::string get_errorString();
    int create_thread();
    int start_thread();
    int stop_thread();

private:
    YAML::Node prog;
    std::vector<config> configs;
    std::vector<boardprogram> boardprograms;
    std::vector<std::thread> threads;
    std::atomic<bool> stopFlag;
    std::string errorString = "";

    boardprogram parse_inst(const YAML::Node& instructions);
    config parse_conf(const YAML::Node& configuration);
    void loop_command(boardprogram prog);
    void no_loop_command(boardprogram prog);
    int inst_to_command(boardprogram prog, int instructionnumber);
    bool validateYAML(const YAML::Node& node);
};