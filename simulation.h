#pragma once

#include <string>
#include <vector>

#include "faction.h"
#include "statistics.h"
#include "log.h"
#include "report.h"

namespace BE {
    class simulation {
        private:
            std::string name;
            std::string id;
            std::string comments;
            std::vector<faction> factions;
            statistics statistics;
            log log;
            report report;
    };
};