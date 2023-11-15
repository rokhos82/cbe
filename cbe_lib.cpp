#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// #include "structs.h"
#include "cbe_lib.h"

using namespace std;

vector<vector<string>> loadCSVFile(string filename)
{
    vector<vector<string>> content; // This is a 2-dimensional array (vector<vector>) of strings to hold all of the CSV content.
    // Setup and open the filestream
    fstream file(filename, ios::in);

    // Test if the file is open
    if (file.is_open())
    {
        // Yes, we openned the file
        // Now read each line
        string line, word;
        vector<string> row;

        // Get each line of the file
        while (getline(file, line))
        {
            // Clear the row
            row.clear();
            // Convert the line to a stringstream
            stringstream str(line);

            // Get each individual word separated by commas
            // Use getline() with a custom end-of-line character ','
            while (getline(str, word, ','))
            {
                // Push the word into the current row
                row.push_back(word);
            }

            // Push the current row into the content array
            content.push_back(row);
        }
    }
    else
    {
        // We could not open the file.  Let the user know.
        cerr << "Could not open file: " << filename << endl;
    }

    // Return the content
    return content;
}

vector<string> loadCSVFileRows(string filename)
{
    vector<string> rows; // This is a 2-dimensional array (vector<vector>) of strings to hold all of the CSV content.
    // Setup and open the filestream
    fstream file(filename, ios::in);

    // Test if the file is open
    if (file.is_open())
    {
        // Yes, we openned the file
        // Now read each line
        string line;

        // Get each line of the file
        while (getline(file, line))
        {
            // Convert the line to a stringstream
            stringstream str(line);

            // Push the current row into the content array
            rows.push_back(line);
        }
    }
    else
    {
        // We could not open the file.  Let the user know.
        cerr << "Could not open file: " << filename << endl;
    }

    // Return the content
    return rows;
}

vector<string> parseCSVRow(string row)
{
    vector<string> parts;
    parts.clear();
    // cout << "Row: " << row << endl;

    // Convert the line to a stringstream
    stringstream ss(row);
    string word = "";

    // Get each individual word separated by commas
    // Use getline() with a custom end-of-line character ','
    while (getline(ss, word, ','))
    {
        // Push the word into the current row
        parts.push_back(word);
    }

    return parts;
}

BE::UnitInfo parseUnit(string line)
{
    BE::UnitInfo unit;

    vector<string> parts = parseCSVRow(line);
    // TODO: Test if the right number of parts are present
    unit.UnitName = parts[0];
    unit.MaxBeam = stol(parts[1]);
    unit.CurBeam = stol(parts[2]);
    unit.MaxShield = stol(parts[3]);
    unit.CurShield = stol(parts[4]);
    unit.MaxTorp = stol(parts[5]);
    unit.CurTorp = stol(parts[6]);
    unit.MaxHull = stol(parts[7]);
    unit.CurHull = stol(parts[8]);
    unit.CurDam = stol(parts[9]);
    unit.Status = stol(parts[10]);
    unit.Ammo = stol(parts[11]);
    unit.Special = parts[12];

    return unit;
}

BE::FleetInfo parseFleetHeader(string header)
{
    BE::FleetInfo info;

    vector<string> parts = parseCSVRow(header);

    // TODO: Test if the right number of parts are present
    info.RaceName = parts[0];
    info.FleetName = parts[1];
    info.BreakOff = stol(parts[2]);
    info.ShipsTotal = stol(parts[3]);
    info.FleetStrength = stol(parts[4]);
    info.ShipsLeft = stol(parts[5]);
    info.TargetBonus = stol(parts[6]);
    info.TargetPriority = stol(parts[7]);
    info.Reserve = stol(parts[8]);

    return info;
}

void debugPrintUnits()
{
    cout << "Number of attacking units: " << BE::AttShipsLeft << endl;
    for (int i = 0; i < BE::AttShipsLeft; i++)
    {
        cout << "Att Unit: " << attShipStr(i) << endl;
    }
    cout << "Number of defending units: " << BE::DefShipsLeft << endl;
    for (int i = 0; i < BE::DefShipsLeft; i++)
    {
        cout << "Def Unit: " << defShipStr(i) << endl;
    }
}

string attShipStr(int i)
{
    stringstream strm;
    strm << BE::AttShipStr[i] << "," << BE::MaxBeamA[i] << "," << BE::CurBeamA[i] << "," << BE::MaxShieldA[i] << "," << BE::CurShieldA[i];
    return strm.str();
}

string defShipStr(int i)
{
    stringstream strm;
    strm << BE::DefShipStr[i] << "," << BE::MaxBeamB[i];
    return strm.str();
}

std::string fixEOLCharacters(const std::string &original)
{
    // This function removes all /r and /n characters from a string and replaces them with a single /r character
    std::string temp = original;

    // Remove all end-of-line characters
    int pos = temp.find('\n');
    while (pos != std::string::npos)
    {
        temp.erase(pos, 1);         // Erase one character at the found position
        pos = temp.find('\n', pos); // Find the next occurrence
    }

    pos = temp.find('\r');
    while (pos != std::string::npos)
    {
        temp.erase(pos, 1);         // Erase one character at the found position
        pos = temp.find('\r', pos); // Find the next occurrence
    }

    // Put the correct line ending back on
    // temp += '\n';

    return temp;
}