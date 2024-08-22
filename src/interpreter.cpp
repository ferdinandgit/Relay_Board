#include<interpreter.hpp>
#include<yaml-cpp/yaml.h>
#include<iostream>



int main() {

    std::vector<config> configs;
    YAML::Node prog = YAML::LoadFile("test.yaml");

    if (prog["config"].IsSequence()) {
        for (const auto& config : prog["config"]) {
            std::string board = config["board"].as<std::string>();
            std::string timebase = config["timebase"].as<std::string>();
            bool loop = config["loop"].as<bool>();
            if (loop) {
                std::cout << board << ", " << timebase << ", true" << std::endl;
            } else {
                std::cout << board << ", " << timebase << ", false" << std::endl;
            }
        }
    } else {
        std::cerr << "'config' is not a sequence!" << std::endl;
    }

    return 0;
}