#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "UnitInfo.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

namespace BE {
    class FleetInfo {
        private:
            string fleetStr;
            string* unitStrs;

            vector<BE::UnitInfo> units;

            unsigned long breakOff;
            unsigned long numUnits;
            unsigned long totalHull;
            string name;
            string race;

        public:
            FleetInfo(vector<string> fleetRows);
            void listUnits();
    };
};




BE::FleetInfo::FleetInfo(vector<string> fleetRows) {
    // The first line in a fleet file is always the fleet info
    this->fleetStr = fleetRows[0];
    cout << this->fleetStr << endl;

    // Accumulator
    unsigned long thull = 0; // This is to calculate the actual total hull

    // Need to parse out the fleet info
    stringstream str(fleetRows[0]);
    string word;
    vector<string> parts;

    while(getline(str,word,',')) {
        parts.push_back(word);
    }

    // Do we have the correct number of parts?
    // The correct number of parts is 10.
    if(parts.size() == 10) {
        // Yes!  Parse the parts into the member variables
        this->race = parts[0];
        this->name = parts[1];
        this->breakOff = std::stoi(parts[2]);
        this->numUnits = std::stoi(parts[3]);
        this->totalHull = std::stoi(parts[4]);
    }
    else {
        // No, output the error for the user to see
        cerr << "ERROR: Fleet line does not have the correct number of parts.  Parts is " << parts.size() << " and should be 9." << endl;
    }

    // Setup a UnitInfo object for each following row
    for(int i = 1;i < fleetRows.size();i++) {
        UnitInfo unit(fleetRows[i]);
        thull += unit.curHull;
        this->units.push_back(unit);
    }

    // Do the number of units match?
    if(this->units.size() != this->numUnits) {
        // No, then output a warning and save the actual number over the stated number.
        cerr << "WARN: Number of units found and number of units specified do not match." << endl;
        this->numUnits = this->units.size();
    }

    // Does the total hull match?
    if(this->totalHull != thull) {
        // No, then output a warning and save the actual total hull.
        cerr << "WARN: Calculated total hull and specified total hull do not match." << endl;
        this->totalHull = thull;
    }
}

void BE::FleetInfo::listUnits() {
    for(int i = 0;i < this->units.size();i++) {
        cout << this->units[i].name << endl;
    }
}