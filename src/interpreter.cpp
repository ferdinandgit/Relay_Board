#include <interpreter.hpp>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <vector>
#include <regex>
#include <serialrelay.hpp>
#include <thread>
#include <atomic>
bool validateYAML(const YAML::Node& node);

Interpreter::Interpreter(std::string file) : stopFlag(false) {
    try{
        prog = YAML::LoadFile(file);
        if(!validateYAML(prog)){
            configs = {};
            boardprograms = {};
            std::cout<<errorString;
            return;
        }
        if (prog["config"].IsSequence()) {
            for (const auto& configuration : prog["config"]) {
                configs.push_back(this->parse_conf(configuration));
            }
        } else {
            configs = {};
        }
        if (prog["program"].IsSequence()) {
            for (const auto& board : prog["program"]) {
                boardprogram myboardprogram = this->parse_inst(board["instructions"]);
                myboardprogram.id = board["board"].as<std::string>();
                boardprograms.push_back(myboardprogram);
            }
        } else {
            boardprograms = {};
        }
    }
    catch(const YAML::Exception& e){
        configs = {};
        boardprograms = {};
        errorString="Error: Fail to load file check syntax";
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
        } else if (command.function == "time") {
            command.intparameter = node["delay"].as<int>();
            myboardprogram.program.push_back(command);
        } else if (command.function == "mask") {
            for (std::size_t i = 0; i < node["commandarray"].size(); i++) {
                command.arrayparameter.push_back(node["commandarray"][i].as<int>());
            }
            myboardprogram.program.push_back(command);
        } else if (std::regex_match(command.function, pattern)) {
            command.intparameter = node["state"].as<int>();
            myboardprogram.program.push_back(command);
        } else {
            myboardprogram.program = {};
            return myboardprogram;
        }
    }
    return myboardprogram;
}

config Interpreter::parse_conf(const YAML::Node& configuration) {
    config boardconfig;
    boardconfig.id = configuration["board"].as<std::string>();
    boardconfig.relaynumber = configuration["relaynumber"].as<int>();
    boardconfig.timebase = configuration["timebase"].as<std::string>();
    boardconfig.loop = configuration["loop"].as<bool>();
    return boardconfig;
}

int Interpreter::match_conf_prog() {
    int validatedmatch = 0;
    if (configs.size() != boardprograms.size()) {
        return -1;
    }
    for (size_t k = 0; k < configs.size(); k++) {
        for (size_t i = 0; i < configs.size(); i++) {
            if (configs[k].id == configs[i].id && i != k){
                errorString = "Error : Same board identifier for different config";
                return -1;
            }
        }
    }
    for (size_t k = 0; k < boardprograms.size(); k++) {
        for (size_t i = 0; i < boardprograms.size(); i++) {
            if (boardprograms[k].id == boardprograms[i].id && i != k){
                errorString = "Error : Same board identifier for different program";
                return -1;
            }    
        }
    }
    for (size_t k = 0; k < configs.size(); k++) {
        for (size_t i = 0; i < boardprograms.size(); i++) {
            if (configs[k].id == boardprograms[i].id) {
                boardprograms[i].configuration = configs[k];
                validatedmatch++;
            }
        }
    }
    if (validatedmatch == boardprograms.size())
        return 1;
    else
        errorString = "Error : Please match configs and programs";
        return -1;
}

int Interpreter::match_hardware(Serialrelay* board, std::string id) {
    for (size_t k = 0; k < boardprograms.size(); k++) {
        if (boardprograms[k].id == id && boardprograms[k].configuration.relaynumber == board->getRelayNumber()) {
            boardprograms[k].board = board;
            return 1;
        }
    }
    errorString = "Fail to match a program and a hardware device";
    return -1;
}

std::vector<config> Interpreter::get_conf() {
    return configs;
}

std::vector<boardprogram> Interpreter::get_boardprogram() {
    return boardprograms;
}

void Interpreter::loop_command(boardprogram prog) {
    while (true) {
        for (size_t k = 0; k < prog.program.size(); k++) {
            if (stopFlag) {
                return;
            } 
            else {
                if(inst_to_command(prog, k)!=1){
                    errorString = "Error : " + prog.id + " Failed";
                    stopFlag = true;
                    return;   
                }
            }
        }
    }
}

void Interpreter::no_loop_command(boardprogram prog) {
    for (size_t k = 0; k < prog.program.size(); k++) {
        if (stopFlag) {
            return;
        } 
        else {
            if(inst_to_command(prog, k)!=1){
                errorString = "Error : " + prog.id + " Failed";
                stopFlag = true;
                return;   
            }
        }
    }
}

int Interpreter::inst_to_command(boardprogram prog, int instructionnumber){
    std::regex pattern("^k(1[0-6]|[1-9])$");
    inst instruction = prog.program[instructionnumber];
    Serialrelay* board = prog.board;
    config boardconfig = prog.configuration;
    int multiplier = 1;
    if(boardconfig.timebase == "s"){
        multiplier = 1000;
    }
    if (instruction.function == "all") {
        if (instruction.stringparameter == "1") {
            std::vector<int> commandarray(boardconfig.relaynumber, 1);
            if(board->setState(commandarray) != 1){
                return -1;
            }
        } else {
            std::vector<int> commandarray(boardconfig.relaynumber, 0);
           if(board->setState(commandarray) != 1){
                return -1;
            }
        }
    } 
    else if (instruction.function == "time") {
        my_sleep(instruction.intparameter*multiplier);
        return 1;
    } 
    else if (instruction.function == "mask") {
        if(board->setState(instruction.arrayparameter) != 1){
                return -1;
        }
    }
    else if(std::regex_match(instruction.function,pattern)){
        int relay = stoi(instruction.function.substr(1,1));
        std::vector<int> commandarray = board->getState();
        commandarray[relay-1] = instruction.intparameter;
       if(board->setState(commandarray) != 1){
                return -1;
            }
    }
    return 1;
}

int Interpreter::create_thread() {
    for (size_t k = 0; k < boardprograms.size(); k++) {
        if(boardprograms[k].board != NULL){
            if (boardprograms[k].configuration.loop) {
                threads.emplace_back(&Interpreter::loop_command, this, boardprograms[k]);
            } else {
                threads.emplace_back(&Interpreter::no_loop_command, this, boardprograms[k]);
            }
        }
    }
    if(threads.empty()){
        errorString = "Error: No thread to start";
        return -1;
    }
    return 1;
}

int Interpreter::start_thread() {
    stopFlag = false;
    for (auto& th : threads) {
        if (th.joinable()) {
            th.detach();
        }
    }
    return 1;
}

int Interpreter::stop_thread() {
    stopFlag = true;
    return 1;
}

std::string Interpreter::get_errorString(){
    return errorString;
}


bool Interpreter::validateYAML(const YAML::Node& node) {
    // Check if "config" and "program" exist and are sequences
    if (!node["config"] || !node["config"].IsSequence()) {
        errorString = "Error: 'config' section is missing or is not a sequence.";
        return false;
    }
    if (!node["program"] || !node["program"].IsSequence()) {
        errorString = "Error: 'program' section is missing or is not a sequence.";
        return false;
    }

    // Validate the "config" section
    for (const auto& config : node["config"]) {
        if (!config["board"] || !config["board"].IsScalar()) {
            errorString = "Error: Each 'config' item must have a 'board' key of string type.";
            return false;
        }
        if (!config["relaynumber"] || !config["relaynumber"].IsScalar()) {
            errorString = "Error: Each 'config' item must have a 'relaynumber' key of int type.";
            return false;
        }
        if (!config["timebase"] || !config["timebase"].IsScalar()) {
            errorString = "Error: Each 'config' item must have a 'timebase' key of string type.";
            return false;
        }
        std::string timebase = config["timebase"].as<std::string>();
        if (timebase != "ms" && timebase != "s") {
            errorString = "Error: 'timebase' must be either 'ms' or 's'. Found: " + timebase;
            return false;
        }
        if (!config["loop"] || !config["loop"].IsScalar()) {
            errorString = "Error: Each 'config' item must have a 'loop' key of bool type.";
            return false;
        }
    }

    // Validate the "program" section
    for (const auto& program : node["program"]) {
        if (!program["board"] || !program["board"].IsScalar()) {
            errorString = "Error: Each 'program' item must have a 'board' key of string type.";
            return false;
        }
        if (!program["instructions"] || !program["instructions"].IsSequence()) {
            errorString = "Error: Each 'program' item must have an 'instructions' key of sequence type.";
            return false;
        }

        // Validate each instruction
        for (const auto& instruction : program["instructions"]) {
            if (!instruction["inst"] || !instruction["inst"].IsScalar()) {
                errorString = "Error: Each instruction must have an 'inst' key of string type.";
                return false;
            }
            std::string inst = instruction["inst"].as<std::string>();

            if (inst == "all" || std::regex_match(inst, std::regex("^k(1[0-6]|[1-9])$"))) {
                // Validate 'state' for instructions 'all' and 'k1' to 'k16'
                if (!instruction["state"] || !instruction["state"].IsScalar()) {
                    errorString = "Error: '" + inst + "' instructions must have a 'state' key of int type.";
                    return false;
                }
                // Check that 'state' can be interpreted as an integer
                try {
                    instruction["state"].as<int>();
                } catch (const YAML::Exception&) {
                    errorString = "Error: 'state' must be an integer for '" + inst + "' instruction.";
                    return false;
                }
            } else if (inst == "time") {
                // Validate 'delay' for 'time' instructions
                if (!instruction["delay"] || !instruction["delay"].IsScalar()) {
                    errorString = "Error: 'time' instructions must have a 'delay' key of int type.";
                    return false;
                }
                // Check that 'delay' can be interpreted as an integer
                try {
                    instruction["delay"].as<int>();
                } catch (const YAML::Exception&) {
                    errorString = "Error: 'delay' must be an integer for 'time' instruction.";
                    return false;
                }
            } else if (inst == "mask") {
                // Validate 'commandarray' for 'mask' instructions
                if (!instruction["commandarray"] || !instruction["commandarray"].IsSequence()) {
                    errorString = "Error: 'mask' instructions must have a 'commandarray' key of sequence type.";
                    return false;
                }
                // Check that all elements in "commandarray" are integers
                for (const auto& value : instruction["commandarray"]) {
                    if (!value.IsScalar()) {
                        errorString = "Error: All elements in 'commandarray' must be of int type.";
                        return false;
                    }
                    try {
                        value.as<int>();
                    } catch (const YAML::Exception&) {
                        errorString = "Error: All elements in 'commandarray' must be integers.";
                        return false;
                    }
                }
            } else {
                errorString = "Error: Unknown instruction type: " + inst;
                return false;
            }
        }
    }

    // If all checks passed
    return true;
}