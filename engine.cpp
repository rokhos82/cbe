#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "engine.h"

namespace BE
{
    engine::engine(){};
    void engine::loadFleets(const std::string_view &fileName) {}
    void engine::loadAttackingFleet(const std::string_view &fileName) {}
    void engine::loadDefendingFleet(const std::string_view &fileName) {}

    void engine::start(const std::vector<std::string_view> &args)
    {
        this->parseArguments(args);
    }
    void engine::printUI() {}
    void engine::printCLI()
    {
        std::cout << "Usage: cbe.exe [-a|--attackers attack.csv][-d|--defenders defend.csv][-e|--fight][-h|--hit number][-1|--onestep][-n|--name nameOfSimulation]" << std::endl;
        std::cout << "\t-a|--attackers filename.csv - This option loads the specified file as the attacking fleet." << std::endl;
        std::cout << "\t-d|--defenders filename.csv - This option loads the specified file as the defending fleet." << std::endl;
        std::cout << "\t-e|--fight - This options tells the combat engine to run headless." << std::endl;
    }
    void engine::parseArguments(const std::vector<std::string_view> &args)
    {
        for (auto cmd = args.begin(), end = args.end(); cmd != end; cmd++)
        {
            if (*cmd == "-a" || *cmd == "--attackers")
            {
                cmd++;
                if (cmd != end)
                {
                    this->loadAttackingFleet(*cmd);
                }
                else
                {
                    throw std::invalid_argument("No attackers file specified with [-a|--attackers]");
                }
            }
            else if (*cmd == "-d" || *cmd == "--defenders")
            {
                cmd++;
                if (cmd != end)
                {
                    this->loadDefendingFleet(*cmd);
                }
                else
                {
                    throw std::invalid_argument("No defenders file specified with [-d|--defenders]");
                }
            }
            else if (*cmd == "-e" || *cmd == "--fight")
            {
                // Set headless here
                this->headless = true;
            }
            else if (*cmd == "-h" || *cmd == "--hit")
            {
                cmd++;
                if (cmd != end)
                {
                    // Set baseToHit here
                }
                else
                {
                    std::cerr << "No number specified with [-h|--hit]." << std::endl;
                }
            }
            else if (*cmd == "-1" || *cmd == "--onestep")
            {
                // Set one-step here
                this->oneStep;
            }
            else if (*cmd == "-n" || *cmd == "--name")
            {
                // Set simulation name here
            }
            else if(*cmd == "-t" || *cmd == "--turns") {}
            else
            {
                std::cerr << "Unknown command: " << *cmd << std::endl;
                this->printCLI();
            }
        }
    }
}
