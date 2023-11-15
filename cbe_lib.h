#pragma once

#include "structs.h"

BE::UnitInfo parseUnit(string line);
BE::FleetInfo parseFleetHeader(string header);

void debugPrintUnits();
string attShipStr(int i);
string defShipStr(int i);

std::string fixEOLCharacters(const std::string &original);