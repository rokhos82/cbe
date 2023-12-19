#include <iostream>
#include <string>
#include <vector>
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
    void engine::printCLI() {}
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
                    std::cerr << "No attackers file specified with [-a|--attackers]." << std::endl;
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
                    std::cerr << "No defenders file specified with [-d|--defenders]." << std::endl;
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
            else
            {
                std::cerr << "Unknown command: " << *cmd << std::endl;
            }
        }
    }
}
