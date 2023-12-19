#pragma once

#include <string>
#include <vector> 

namespace BE {
    class engine {
        public:
            engine();
            engine(const std::vector<std::string> args);
            void loadFleets(const std::string_view & fileName);
            void start(const std::vector<std::string_view> & args);
        private:
            bool headless;
            bool oneStep;
            
            void loadAttackingFleet(const std::string_view & fileName);
            void loadDefendingFleet(const std::string_view & fileName);

            void printUI();
            void printCLI();
            void parseArguments(const std::vector<std::string_view> & args);
    };
}