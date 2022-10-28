#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

namespace BE {
    class UnitInfo {
        private:
            string shipStr;
            string critStr;
        public:
            string name;

            unsigned long maxBeam;
            unsigned long curBeam;
            unsigned long maxShield;
            unsigned long curShield;
            unsigned long maxTorp;
            unsigned long curTorp;
            unsigned long maxHull;
            unsigned long curHull;

            long status;
            long ammo;
            long curDam;
            string special;

            long hits;
            long penHits;
            long bpAttackCrit;
            UnitInfo(string shipStr);
    };
};




BE::UnitInfo::UnitInfo(string shipStr) {
    this->shipStr = shipStr;
    cout << this->shipStr << endl;

    // Setup the array, stream, and temp (word) for parsing each part of the UDL
    vector<string> parts;
    stringstream str(shipStr);
    string word;
    
    // Get each part using a stringstream.  Way easier than doing this by hand...
    // Set the getline end-of-line character to a comma to get each cell in the UDL
    while(getline(str,word,',')) {
        // Insert the part into the parts array
        parts.push_back(word);
    }

    // Do we have the right number of parts (the correct number being 13)
    if(parts.size() == 13) {
        // Yes!  Sort the parts into the private members
        this->name = parts[0];
        this->curBeam = std::stoi(parts[1]);
        this->maxBeam = std::stoi(parts[2]);
        this->curShield = std::stoi(parts[3]);
        this->maxShield = std::stoi(parts[4]);
        this->curTorp = std::stoi(parts[5]);
        this->maxTorp = std::stoi(parts[6]);
        this->curHull = std::stoi(parts[7]);
        this->maxHull = std::stoi(parts[8]);
        this->special = parts[12];
    }
    else {
        // No, report the error
        cerr << "Unable to parse ship string.  Invalid number of parts.  Number of parts is " << parts.size() << " and should be 13!" << endl;
    }
}