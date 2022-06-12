#ifndef LOGICTOOLS_LOGIC_TOOLS_H
#define LOGICTOOLS_LOGIC_TOOLS_H

#include "json.hpp"
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

using json = nlohmann::json;

// Notation
const char TRUE_SYMBOL           = '1';
const char FALSE_SYMBOL          = '0';
const char LOGICAL_AND           = '&';
const char LOGICAL_OR            = '|';
const char LOGICAL_STRONG_OR     = '!';
const char LOGICAL_IF            = '>';
const char LOGICAL_NOT           = '~';
const char LOGICAL_BICONDITIONAL = '=';

// Functions prototypes
std::string sendTruthTable(const std::string& jsonRequest);
void normalize(std::string& formula);
bool isWellFormed(std::string& formula);
std::string createTruthTable(std::string& formula);
bool solve(std::string& formula);
std::string getNewFormula(int it = 5, int variables = 3);
std::string getRandomFormula(int it = 5, int variables = 3);

// Logical connectives
bool logical_and(const std::string& formula);
bool logical_or(const std::string& formula);
bool logical_if(const std::string& formula);
bool logical_biconditional(const std::string& formula);
bool logical_strong_or(const std::string& formula);

#endif //LOGICTOOLS_LOGIC_TOOLS_H
