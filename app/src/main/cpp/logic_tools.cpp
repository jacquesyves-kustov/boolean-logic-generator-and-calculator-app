#include "logic_tools.h"

std::string getNewFormula(int it, int variables)
{
    // Set random seed before recursive function will be called
    std::srand((unsigned)std::time(nullptr));

    // Call recursive function
    std::string res = getRandomFormula(it, variables);

    // Return deserialized JSON to Java
    json newFormula;
    newFormula["Formula"] = res;

    return newFormula.dump();
}


std::string getRandomFormula(int it, int variables)
{
    /*
     * 'it' - counter of recursive calls
     * 'variables' - maximum number of variables in formula
     */


    // refresh complexity flag every recursive step
    --it;


    // Allocate memory for auxiliary variables
    int randVal;           // rand() results
    std::string temp = ""; // subformulas or single variables will be stored here


    // If recursive is deep, it will return a variable
    if (it <= 0)
    {
        // Choose a variable randomly
        randVal = (rand() % variables);
        temp = std::string(1, 'a' + randVal);

        // Probably, it will add negation
        randVal = (rand() % 2);

        if (randVal)
            return LOGICAL_NOT + temp;
        else
            return temp;
    }


    // choose logical connective randomly
    std::string conn = "";
    randVal = (rand() % 5);

    switch (randVal)
    {
        case 0:  // Add conjunction
        {
            conn += LOGICAL_AND;
            break;
        }

        case 1:  // Add disjunction
        {
            conn += LOGICAL_OR;
            break;
        }

        case 2:  // Add disjunction
        {
            conn += LOGICAL_STRONG_OR;
            break;
        }

        case 3:  // Add implication
        {
            conn += '-';
            conn += LOGICAL_IF;
            break;
        }

        case 4:  // Add biconditional
        {
            conn += LOGICAL_BICONDITIONAL;
            break;
        }

        default: // Add conjunction
        {
            conn += LOGICAL_AND;
        }
    }

    // Recursive call for the left and the right operands
    temp = "(" + getRandomFormula(it) + " " + conn + " " + getRandomFormula(it - 1) + ")";


    // Probably, it will add negation before
    randVal = (rand() % 2);

    if (randVal)
        return LOGICAL_NOT + temp;
    else
        return temp;
}


std::string allSteps(const std::string& JSONstr)
{
    // Get JSON from Java
    json request = json::parse(JSONstr);

    // Define given formula
    std::string given_formula = request["Formula"];

    // Normalize it
    normalize(given_formula);

    // If it can be solved
    if (isWellFormed(given_formula))
    {
        // Return truth table
        return createTruthTable(given_formula);
    }
    else
    {
        // If it cannot be solved, it will return "CANNOT BE SOLVED" as JSON
        // Structure of error report is equal to correct answer
        json error;
        error["Values"] = "CANNOT";
        error["Result"] = "BE SOLVED";

        json report;
        report["truthTable"] = error;

        return report.dump();
    }

}


void normalize(std::string& formula)
{
    std::string normalized = "";

    for (auto ch : formula)
    {
        if (ch == ' ')
            continue;
        else if ('A' <= ch && ch <= 'Z')
            normalized += tolower(ch);
        else
            normalized += ch;
    }

    formula = std::move(normalized);
}


bool isWellFormed(std::string& formula)
{
    /*
    THERE ARE 2 CHECKS:
        1) COUNTERS
        2) NESTED PARENTHESES
    */


    /*
    STEP 1
    */

    // Counters
    auto* ptConnectivesCounter = new unsigned short(0); // number of logical connectives in formula
    auto* ptLeftParCounter = new unsigned short(0);     // number of left parentheses
    auto* ptRightParCounter = new unsigned short(0);    // number of right parentheses
    auto* ptVariablesCounter = new unsigned short(0);   // number of characters (variables)


    for (unsigned int charIndex = 0; charIndex < formula.length(); ++charIndex)
    {
        // Propositional variables
        if (isalpha(formula[charIndex]))
        {
            (*ptVariablesCounter)++;
            continue;
        }

        // Left parentheses
        if (formula[charIndex] == '(')
        {
            (*ptLeftParCounter)++;
            continue;
        }

        // Right parentheses
        if (formula[charIndex] == ')')
        {
            (*ptRightParCounter)++;
            continue;
        }

        // Logical connectives
        if (formula[charIndex] == LOGICAL_OR ||
            formula[charIndex] == LOGICAL_IF ||
            formula[charIndex] == LOGICAL_AND ||
            formula[charIndex] == LOGICAL_BICONDITIONAL ||
            formula[charIndex] == LOGICAL_STRONG_OR)
        {
            (*ptConnectivesCounter)++;
            continue;
        }
    }

    // Checks

    // If there are two variables and no connectives, it cannot be solved
    if (*ptVariablesCounter > 2 && *ptConnectivesCounter < 1)
        return false;


    // if numbers of parentheses are not equal, it cannot be solved
    if (*ptLeftParCounter != *ptRightParCounter)
        return false;

    delete ptRightParCounter;


    // numbers of variable should be less than 27 and more than 1
    if (*ptVariablesCounter > 26 || *ptVariablesCounter < 2)
        return false;

    delete ptVariablesCounter;


    // if numbers of left parentheses are not equal to number of logical connectives, it cannot be solved
    if (*ptConnectivesCounter != *ptLeftParCounter)
    {
        // usually external parentheses are skipped
        formula = "(" + formula + ")";

        (*ptLeftParCounter)++;

        if (*ptConnectivesCounter != *ptLeftParCounter)
            return false;
    }

    delete ptConnectivesCounter;
    delete ptLeftParCounter;




    /*
    STEP 2 - NESTED PARENTHESES
    */

    // Store only parentheses
    std::string parentheses = "";

    for (auto ch : formula)
    {
        if (ch == '(' || ch == ')')
            parentheses += ch;
    }


    unsigned int leftParIndex;
    unsigned int rightParIndex;

    while (parentheses.find('(') != std::string::npos)
    {
        leftParIndex = parentheses.rfind('(');

        if (parentheses.find(')', leftParIndex) == std::string::npos)
            return false;
        else
        {
            rightParIndex = parentheses.find(')', leftParIndex);
            parentheses.erase(leftParIndex, rightParIndex - leftParIndex + 1);
        }
    }


    // If all steps were passed, then the formula is correct
    return true;
}


std::string createTruthTable(std::string& formula)
{
    // Collect all propositional variables: (p -> (q -> p)) == "pq"
    std::string variables = "";

    for (auto ch : formula)
    {
        if (variables.find(ch) == std::string::npos && ('a' <= ch && ch <= 'z'))
            variables += ch;
    }


    // Selection sort
    unsigned minIndex;
    for (unsigned int i = 0; i < variables.length() - 1; ++i)
    {
        minIndex = i;

        for (unsigned int j = i + 1; j < variables.length(); ++j)
        {
            if (variables[j] < variables[minIndex])
                minIndex = j;
        }

        std::swap(variables[minIndex], variables[i]);
    }


    // Define number of rows in truth table (2 ** number of variables)
    int rowsNumber = 1;

    for (int i = 0; i < variables.length(); ++i)
    {
        rowsNumber *= 2;
    }


    // Define truth table rows as 2D vector of strings
    std::vector<std::string> truthTableRows;

    // Create empty string for every row
    for (int i = 0; i < rowsNumber; ++i)
    {
        truthTableRows.push_back("");
    }

    // Fill truth table by columns
    int counter = rowsNumber;
    int tempCounter;

    // for every variable
    for (unsigned int var = 0; var < variables.length(); ++var)
    {
        counter /= 2;
        tempCounter = 1;

        // for every row
        for (int rowIndex = 0; rowIndex < rowsNumber; ++rowIndex)
        {
            truthTableRows[rowIndex] += ((tempCounter <= counter) ? FALSE_SYMBOL : TRUE_SYMBOL);
            tempCounter++;

            if (tempCounter > counter * 2)
                tempCounter = 1;
        }
    }


    // solve formula for every row in truth table
    std::string tempFormula;
    std::vector<bool> values;

    // for every row in truth table
    for (auto row : truthTableRows)
    {
        // copy given formula
        tempFormula = formula;

        // swap every propositional variable in 'tempFormula' with its value
        // for every variable
        for (unsigned int varIndex = 0; varIndex < variables.length(); ++varIndex)
        {
            // for every character in formula
            for (unsigned int charIndex = 0; charIndex < tempFormula.length(); ++charIndex)
            {
                if (tempFormula[charIndex] == variables[varIndex])
                    tempFormula[charIndex] = row[varIndex];
            }
        }

        values.push_back(solve(tempFormula));
    }

    // Return result as JSON
    json row;        // 'values->result' pair
    json truthTable; // list of 'row's
    std::string temp;

    // for every row in truth table
    for (unsigned int i = 0; i < rowsNumber; ++i)
    {
        temp = "";

        for (unsigned int j = 0; j < variables.length(); ++j)
        {
            temp += std::string(1, variables[j]) + " = " + std::string(1, (truthTableRows[i][j] == '1') ? 'T' : 'F');
            temp += (j + 1 == variables.length()) ? "" : ", ";
        }

        row["Result"] = ((values[i]) ? "TRUE" : "FALSE");
        row["Values"] = temp;
        truthTable.push_back(row);
        row.clear();
    }

    // Return 'truthTable' as serialized JSON
    json result;
    result["truthTable"] = truthTable;

    return result.dump();
}


bool solve(std::string& formula)
{
    bool res;
    unsigned int leftParIndex;
    unsigned int rightParIndex;
    std::string tempSubstr;


    // For every (substring)
    while (formula.rfind('(') != std::string::npos && formula.find(')', formula.rfind('(')) != std::string::npos)
    {
        // Define substr
        leftParIndex = formula.rfind('(');
        rightParIndex = formula.find(')', leftParIndex);
        tempSubstr = formula.substr(leftParIndex, rightParIndex - leftParIndex + 1);


        // Negation
        while (tempSubstr.rfind('~') != std::string::npos)
        {
            tempSubstr[tempSubstr.rfind('~') + 1] = ((tempSubstr[tempSubstr.rfind('~') + 1] == FALSE_SYMBOL) ? TRUE_SYMBOL : FALSE_SYMBOL);
            tempSubstr.erase(tempSubstr.rfind('~'), 1);
        }


        // Define logical operator in substr
        if (tempSubstr.find(LOGICAL_AND) != std::string::npos)
            res = logical_and(tempSubstr);
        else if (tempSubstr.find(LOGICAL_OR) != std::string::npos)
            res = logical_or(tempSubstr);
        else if (tempSubstr.find(LOGICAL_IF) != std::string::npos)
            res = logical_if(tempSubstr);
        else if (tempSubstr.find(LOGICAL_BICONDITIONAL) != std::string::npos)
            res = logical_biconditional(tempSubstr);
        else if (tempSubstr.find(LOGICAL_STRONG_OR) != std::string::npos)
            res = logical_strong_or(tempSubstr);
        else
            break;


        // swap these substr with its value
        formula.erase(leftParIndex, rightParIndex - leftParIndex + 1);
        formula.insert(leftParIndex, std::string(1, ((res) ? TRUE_SYMBOL : FALSE_SYMBOL)));
    }


    if (formula.find(LOGICAL_NOT) != std::string::npos)
    {
        for (auto ch : formula)
        {
            if (ch == LOGICAL_NOT)
                res = !(res);
        }
    }

    return res;
}


bool logical_and(const std::string& formula)
{
    bool firstArg = false;
    bool secondArg = false;


    // define the first argument
    for (int i = formula.find(LOGICAL_AND); i >= 0; --i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
        {
            firstArg = (formula[i] == TRUE_SYMBOL);
            break;
        }
    }

    // define the second one
    for (int i = formula.find(LOGICAL_AND); i < formula.length(); ++i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
        {
            secondArg = (formula[i] == TRUE_SYMBOL);
            break;
        }
    }

    return firstArg && secondArg;
}


bool logical_or(const std::string& formula)
{
    bool firstArg = false;
    bool secondArg = false;


    // define arguments
    for (int i = formula.find(LOGICAL_OR); i >= 0; --i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            firstArg = (formula[i] == TRUE_SYMBOL);
    }

    for (int i = formula.find(LOGICAL_OR); i < formula.length(); ++i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            secondArg = (formula[i] == TRUE_SYMBOL);
    }


    return firstArg || secondArg;
}


bool logical_if(const std::string& formula)
{
    bool firstArg = false;
    bool secondArg = false;


    // define arguments
    for (int i = formula.find(LOGICAL_IF); i >= 0; --i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            firstArg = (formula[i] == TRUE_SYMBOL);
    }

    for (int i = formula.find(LOGICAL_IF); i < formula.length(); ++i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            secondArg = (formula[i] == TRUE_SYMBOL);
    }

    return !firstArg || secondArg;
}


bool logical_biconditional(const std::string& formula)
{
    bool firstArg = false;
    bool secondArg = false;


    // define arguments
    for (int i = formula.find(LOGICAL_BICONDITIONAL); i >= 0; --i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            firstArg = (formula[i] == TRUE_SYMBOL);
    }

    for (int i = formula.find(LOGICAL_BICONDITIONAL); i < formula.length(); ++i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            secondArg = (formula[i] == TRUE_SYMBOL);
    }

    return firstArg == secondArg;
}


bool logical_strong_or(const std::string& formula)
{
    bool firstArg = false;
    bool secondArg = false;


    // define arguments
    for (int i = formula.find(LOGICAL_STRONG_OR); i >= 0; --i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            firstArg = (formula[i] == TRUE_SYMBOL);
    }

    for (int i = formula.find(LOGICAL_STRONG_OR); i < formula.length(); ++i)
    {
        if (formula[i] == TRUE_SYMBOL || formula[i] == FALSE_SYMBOL)
            secondArg = (formula[i] == TRUE_SYMBOL);
    }

    return firstArg != secondArg;
}