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
            if (configs[k].id == configs[i].id && i != k)
                return -1;
        }
    }
    for (size_t k = 0; k < boardprograms.size(); k++) {
        for (size_t i = 0; i < boardprograms.size(); i++) {
            if (boardprograms[k].id == boardprograms[i].id && i != k)
                return -1;
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
        return -1;
}

int Interpreter::match_hardware(Serialrelay* board, std::string id) {
    for (size_t k = 0; k < boardprograms.size(); k++) {
        if (boardprograms[k].id == id && boardprograms[k].configuration.relaynumber == board->getRelayNumber()) {
            boardprograms[k].board = board;
            return 1;
        }
    }
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
            } else {
                inst_to_command(prog, k);
            }
        }
    }
}

void Interpreter::no_loop_command(boardprogram prog) {
    for (size_t k = 0; k < prog.program.size(); k++) {
        if (stopFlag) {
            return;
        } else {
            inst_to_command(prog, k);
        }
    }
}

void Interpreter::inst_to_command(boardprogram prog, int instructionnumber) {
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
            board->setState(commandarray);
        } else {
            std::vector<int> commandarray(boardconfig.relaynumber, 0);
            board->setState(commandarray);
        }
    } else if (instruction.function == "time") {
        my_sleep(instruction.intparameter*multiplier);
    } else if (instruction.function == "mask") {
        board->setState(instruction.arrayparameter);
    }
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

int main() {
    Interpreter program("test.yaml");
    /*Serialrelay* board1 = new Serialrelay(0, USBRELAY, "COM8");
    board1->openCom();
     if(board1->initBoard()!=1){
        std::cout<<"error";
    }
    
    Serialrelay* board2 = new Serialrelay(1, USBRELAY, "COM9");
    board2->openCom();
    if(board2->initBoard()!=1){
        std::cout<<"error";
    }
     
    Interpreter program("test.yaml");
    program.match_conf_prog();
    program.match_hardware(board1, "board1");
    program.match_hardware(board2, "board2");

    program.create_thread();
    program.start_thread();
    std::this_thread::sleep_for(std::chrono::seconds(30));
    program.stop_thread();*/

    return 0;
}

bool validateYAML(const YAML::Node& node) {
    // Check if "config" and "program" exist and are sequences
    if (!node["config"] || !node["config"].IsSequence()) {
        std::cerr << "Error: 'config' section is missing or is not a sequence." << std::endl;
        return false;
    }
    if (!node["program"] || !node["program"].IsSequence()) {
        std::cerr << "Error: 'program' section is missing or is not a sequence." << std::endl;
        return false;
    }

    // Validate the "config" section
    for (const auto& config : node["config"]) {
        if (!config["board"] || !config["board"].IsScalar()) {
            std::cerr << "Error: Each 'config' item must have a 'board' key of string type." << std::endl;
            return false;
        }
        if (!config["relaynumber"] || !config["relaynumber"].IsScalar()) {
            std::cerr << "Error: Each 'config' item must have a 'relaynumber' key of int type." << std::endl;
            return false;
        }
        if (!config["timebase"] || !config["timebase"].IsScalar()) {
            std::cerr << "Error: Each 'config' item must have a 'timebase' key of string type." << std::endl;
            return false;
        }
        if (!config["loop"] || !config["loop"].IsScalar()) {
            std::cerr << "Error: Each 'config' item must have a 'loop' key of bool type." << std::endl;
            return false;
        }
    }

    // Validate the "program" section
    for (const auto& program : node["program"]) {
        if (!program["board"] || !program["board"].IsScalar()) {
            std::cerr << "Error: Each 'program' item must have a 'board' key of string type." << std::endl;
            return false;
        }
        if (!program["instructions"] || !program["instructions"].IsSequence()) {
            std::cerr << "Error: Each 'program' item must have an 'instructions' key of sequence type." << std::endl;
            return false;
        }

        // Validate each instruction
        for (const auto& instruction : program["instructions"]) {
            if (!instruction["inst"] || !instruction["inst"].IsScalar()) {
                std::cerr << "Error: Each instruction must have an 'inst' key of string type." << std::endl;
                return false;
            }
            std::string inst = instruction["inst"].as<std::string>();
            if (inst == "all" || inst == "k1" || inst == "k2") {
                if (!instruction["state"] || !instruction["state"].IsScalar()) {
                    std::cerr << "Error: 'all' or 'kX' instructions must have a 'state' key of int type." << std::endl;
                    return false;
                }
            } else if (inst == "time") {
                if (!instruction["delay"] || !instruction["delay"].IsScalar()) {
                    std::cerr << "Error: 'time' instructions must have a 'delay' key of int type." << std::endl;
                    return false;
                }
            } else if (inst == "mask") {
                if (!instruction["commandarray"] || !instruction["commandarray"].IsSequence()) {
                    std::cerr << "Error: 'mask' instructions must have a 'commandarray' key of sequence type." << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error: Unknown instruction type: " << inst << std::endl;
                return false;
            }
        }
    }

    // If all checks passed
    return true;
}