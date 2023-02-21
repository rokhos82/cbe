#pragma once

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

class CriticalHitTable
{
public:
    // Add a new entry to the table
    void addEntry(long weight, const std::string &output, long critType)
    {
        entries.emplace_back(std::make_pair(weight, std::make_pair(output, critType)));
        totalWeight += weight;
    }

    // Get a random output from the table, based on the weights
    std::pair<std::string, long> getRandomOutput() const
    {
        long dice = rand() % totalWeight;
        long sum = 0;
        for (const auto &entry : entries)
        {
            sum += entry.first;
            if (dice < sum)
            {
                return std::make_pair(entry.second.first, entry.second.second);
            }
        }
        // Shouldn't reach here, but just in case
        return std::make_pair("", -1);
    }

private:
    // std::vector<std::pair<std::pair<long, std::string>, long>> entries;
    std::vector<std::pair<long, std::pair<std::string, long>>> entries;
    long totalWeight = 0;
};

std::vector<std::string> split_string(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> substrings;
    size_t pos = 0;
    size_t next_pos = 0;

    while ((next_pos = str.find(delimiter, pos)) != std::string::npos)
    {
        substrings.push_back(str.substr(pos, next_pos - pos));
        pos = next_pos + delimiter.length();
    }

    substrings.push_back(str.substr(pos));
    return substrings;
} //*/

/*std::vector<CriticalHitTable> load_crits_file(std::string filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<CriticalHitTable> tables;
    int current_index = -1;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        if (line[0] == '[' && line[line.length() - 1] == ']')
        {
            // This line marks the beginning of a new table
            current_index = std::stoi(line.substr(1, line.length() - 2));
            tables.emplace_back(CriticalHitTable());
        }
        else
        {
            // This line contains a weight-output-critType triple for the current table
            auto tokens = split_string(line, ",");
            // std::cerr << line << std::endl;
            if (tokens.size() != 3)
            {
                throw std::runtime_error("Invalid data format in file: " + filename);
            }

            long weight = std::stol(tokens[0]);
            long critType = std::stoi(tokens[1]);
            std::string output = tokens[2];
            tables.back().addEntry(weight, output, critType);
        }
    }

    return tables;
} //*/

std::unordered_map<int, CriticalHitTable> load_crits_file(std::string filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::unordered_map<int, CriticalHitTable> tables;
    int current_index = -1;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        if (line[0] == '[' && line[line.length() - 1] == ']')
        {
            // This line marks the beginning of a new table
            current_index = std::stoi(line.substr(1, line.length() - 2));
            tables.emplace(current_index, CriticalHitTable());
        }
        else
        {
            // This line contains a weight-output-critType triple for the current table
            auto tokens = split_string(line, ",");
            // std::cerr << line << std::endl;
            if (tokens.size() != 3)
            {
                throw std::runtime_error("Invalid data format in file: " + filename);
            }

            long weight = std::stol(tokens[0]);
            long critType = std::stoi(tokens[1]);
            std::string output = tokens[2];
            tables[current_index].addEntry(weight, output, critType);
        }
    }

    return tables;
}
