#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <functional>
#include "../../Headers/colors.h"
#include "../../Headers/fio.h"
#include "../../Headers//Parser/parser.h"
#include "../../Headers/utils.h"

#define allowed_var_chars "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_"

void addVariable(par::vector_variable& variables)
{
    std::string name;
    std::string value;

    std::cout << "Enter name : ";
    fio::promptStr(name);

    if (name.find_first_not_of(allowed_var_chars) != std::string::npos)
    {
        std::cout << "Name can only have alpha numeric characters or underscore!";
        return;
    }

    if (isdigit(name[0]))
    {
        std::cout << "Variable can't start with a number or be made of only numbers!";
        return;
    }

    auto it = find_if(variables.begin(), variables.end(), 
        [&name](const auto& p) { return p.first == name; });

    if (it != variables.end())
    {
        std::cout << "Variable must have an unique name!";
        return;
    }

    std::cout << "Enter value: ";
    fio::promptStr(value);

    variables.push_back(std::make_pair(name, par::eval(value, variables)));
    std::sort(variables.begin(), variables.end());
}

void addFunction(par::vector_functions& functions)
{
    std::string name;
    std::string formula;
    std::vector<std::string> parameters;

    std::cout << "Enter name: ";
    fio::promptStr(name);

    auto it = find_if(functions.begin(), functions.end(), 
        [&name](const auto& p) { return p.name == name; });

    if (it != functions.end())
    {
        std::cout << "Function must have an unique name!";
        return;
    }

    std::cout << "Enter formula (e.g x+2): ";
    fio::promptStr(formula);

    std::cout << "\nEnter parameter names (give empty input to stop)\n";
    for (size_t i = 0; true; i++)
    {
        std::string parameter_name;

        std::cout << "Paramater " << i+1 << ": ";
        if (fio::promptStr(parameter_name, true)) {
            break;
        }

        auto it = find_if(parameters.begin(), parameters.end(), 
            [parameter_name](const auto& p) { return p == parameter_name; });

        if (it != parameters.end())
        {
            std::cout << "Parameter must have a unique name!";
            return;
        }

        parameters.push_back(parameter_name);
    }

    functions.emplace_back(name, parameters, formula);

    std::sort(functions.begin(), functions.end(),
        [](par::functions_struct a, par::functions_struct b) { return a.name.size() > b.name.size(); });
}

void removeVariable(par::vector_variable& variables)
{
    std::string desired_variable;

    std::cout << "Enter position of the variable or its name: ";
    fio::promptStr(desired_variable);

    size_t position = ut::find_element(variables, desired_variable,
        [](const std::pair<std::string, double>& p) { return p.first; });

    if (position < variables.size()) {
        variables.erase(variables.begin() + position);
    }
}

void removeFunction(par::vector_functions &functions)
{
    std::string desired_function;

    std::cout << "Enter position of the function or its name: ";
    fio::promptStr(desired_function);

    size_t position = ut::find_element(functions, desired_function,
        [](const par::functions_struct& p) { return p.name; });

    if (position < functions.size()) {
        functions.erase(functions.begin() + position);
    }
}

void calcExpression(par::vector_variable& variables, par::vector_functions& functions)
{
    while (true)
    {
        std::string expression;

        std::cout << "> ";
        if (fio::promptStr(expression, true)) {
            return;
        }
        std::cout << col::cursUp(1) << col::cursRight(expression.size()+2);
        std::cout << " = ";

        double result = par::eval(expression, variables, functions);

        if (!std::isnan(result))
        {
            std::cout << result;
        }
        std::cout << '\n';
    }
}

int main()
{
    par::vector_variable variables  (1, std::make_pair("PI", 3.14159));
    par::vector_functions functions =
    {
        par::functions_struct ("delta", {"a", "b", "c"}, "b^2 - 4*a*c"),
        par::functions_struct ("condition", {"n", "bool"}, "(n*2)*bool + (n)*(1-bool)"),
        par::functions_struct ("dist", {"Xa", "Ya", "Xb", "Yb"}, "sqrt((Xa-Xb)^2+(Ya-Yb)^2)"),
        par::functions_struct ("pyth", {"a", "b"}, "sqrt(a^2 + b^2)"),
        par::functions_struct ("summarize", {"a", "b", "c"}, "a+b+c"),
        par::functions_struct ("rad", {"d"}, "d*PI/180"),
        par::functions_struct ("deg", {"r"}, "r*180/PI"),
        par::functions_struct ("circle_area", {"r"}, "PI*r^2"),
        par::functions_struct ("circle_diam", {"r"}, "2*PI*r"),
        par::functions_struct ("BMI", {"kg", "m"}, "kg/m^2")
    };

    std::sort(functions.begin(), functions.end(),
        [](par::functions_struct a, par::functions_struct b) { return a.name.size() > b.name.size(); });

    col::enableANSI();

    while (true)
    {
        std::cout << "Variables:\n"; 
        for (size_t i = 0; i < variables.size(); i++)
        {
            std::cout << i+1 << ". " << variables[i].first << " = " << variables[i].second << '\n';
        }

        std::cout << "\nFunctions:\n"; 
        for (size_t i = 0; i < functions.size(); i++)
        {
            std::cout << i+1 << ". " << functions[i].name << '('; 
            ut::printVec(functions[i].parameters, ", ", "");
            std::cout << ") = " << functions[i].formula << '\n';
        }
        
        std::cout << '\n'
        << "1. Add a variable\n"
        << "2. Add a function\n"
        << "3. Remove a variable\n"
        << "4. Remove a function\n"
        << "5. Calculate expression\n"
        << "6. Exit\n"
        << "Choose action: ";
        int choice = fio::getRangeChar('1', '6') - 48;
        std::cout << "\n\n";

        switch (choice)
        {
            case 1: addVariable(variables);               break;
            case 2: addFunction(functions);               break;
            case 3: removeVariable(variables);            break;
            case 4: removeFunction(functions);            break;
            case 5: calcExpression(variables, functions); break;

            case 6: return 0;
        }

        std::cout << "\n";
        fio::pressProceed();
        std::cout << col::ERASE_SCREEN2;
    }

    return 0;
}