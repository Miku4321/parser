#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "parser_helper_funcs.h"
#include "../utils.h"
#define printVar(a) (std::cout << #a << " = " << a << '\n')

namespace par
{

struct functions_struct
{
    std::string name; // f
    std::vector<std::string> parameters; // x, a, b
    std::string formula; // a*x+b

    functions_struct(const std::string& n, const std::vector<std::string>& p, const std::string& f)
    : name(n), parameters(p), formula(f) {}
};

using vector_variable = std::vector<std::pair<std::string, double>>;
using vector_functions = std::vector<functions_struct>;

// function definition
double eval(std::string expression, vector_variable variables = {}, vector_functions functions = {}, bool show_err = true);

void insertVariables(std::string& expression, vector_variable variables)
{
    for (int i = variables.size()-1; i >= 0 ; i--)
    {
        std::string var_name  = variables[i].first;
        std::string var_value = std::to_string(variables[i].second);

        while (true)
        {
            size_t pos = expression.find(var_name);

            if (pos >= expression.size()) {
                break;
            }

            var_value.erase(var_value.find_last_not_of('0') + 1);
            var_value.erase(var_value.find_last_not_of('.') + 1);

            expression.erase(pos, var_name.size());
            expression.insert(pos, var_value);
        }
    }
}

void insertStandardFunctions(std::string& expression)
{
    std::vector<std::string> function_list{"asin", "sin", "acos", "cos", "atan", "tan", "actg", "ctg", "sqrt", "abs"};

    while (true)
    {
        size_t func_pos;
        int function_type;

        for (size_t i = 0; i < function_list.size(); i++)
        {
            func_pos = expression.find(function_list[i]);

            if (func_pos != std::string::npos)
            {
                function_type = i;
                break;
            }
        }

        if (func_pos == std::string::npos) {
            return;
        }

        std::string func_str = expression.substr(func_pos, expression.length());
        size_t opening_bracket_pos = func_str.find_first_of('(');
        size_t closing_bracket_pos = par_helper::findClosingBracket(func_str);
        func_str = func_str.substr(0, closing_bracket_pos+1);
        std::string func_in = func_str.substr(opening_bracket_pos+1, closing_bracket_pos - opening_bracket_pos - 1);

        double func_out = eval(func_in);
    
        // Tied to function_list
        switch (function_type)
        {
            case 0: func_out = asin(func_out);    break;
            case 1: func_out = sin(func_out);   break;

            case 2: func_out = acos(func_out);    break;
            case 3: func_out = cos(func_out);   break;

            case 4: func_out = atan(func_out);    break;
            case 5: func_out = tan(func_out);   break;

            case 6: func_out = 1/atan(func_out);  break;
            case 7: func_out = 1/tan(func_out); break;
            
            case 8: func_out = sqrt(func_out);   break;
            case 9: func_out = abs(func_out);    break;
        }
        
        expression.erase(func_pos, closing_bracket_pos+1);
        expression.insert(func_pos, std::to_string(func_out));
    }
}

void insertCustomFunctions(std::string& expression, vector_functions functions, vector_variable variables)
{
    if (functions.empty()) {
        return;
    }

    while (true)
    {
        size_t func_expr_pos; // where the function is in the expression
        size_t func_index;    // which function is in the expression

        for (size_t i = 0; i < functions.size(); i++)
        {
            std::string pattern = functions[i].name + "(";
            func_expr_pos = expression.rfind(pattern);

            if (func_expr_pos != std::string::npos) {
                func_index = i;
                break;
            }
        }
        
        if (func_expr_pos == std::string::npos) {
            return;
        }

        par::functions_struct func_copy = functions[func_index];

        std::string func_str = expression.substr(func_expr_pos, expression.length());
        size_t opening_bracket_pos = func_str.find_first_of('(');
        size_t closing_bracket_pos = par_helper::findClosingBracket(func_str);
        func_str = func_str.substr(0, closing_bracket_pos+1);
        std::string argument_list = func_str.substr(opening_bracket_pos + 1, closing_bracket_pos - opening_bracket_pos - 1);

        for (size_t i = 0; i < func_copy.parameters.size(); i++)
        {
            size_t comma_pos = argument_list.find(','); // 1

            if (comma_pos == std::string::npos) {
                comma_pos = argument_list.size();
            }

            std::string nth_argument = argument_list.substr(0, comma_pos);
            variables.push_back(std::make_pair(func_copy.parameters[i], eval(nth_argument, variables, functions)));

            if (comma_pos != argument_list.size()) {
                argument_list = argument_list.substr(comma_pos + 1, argument_list.size() - 1 - comma_pos);
            }
        }

        std::string func_out = '(' + std::to_string(eval(func_copy.formula, variables,functions)) + ')';
        variables.resize(variables.size() - func_copy.parameters.size());

        expression.erase(func_expr_pos, closing_bracket_pos+1);
        expression.insert(func_expr_pos, func_out);
    }
}

void operation(std::vector<double> &numbers, std::string &operators, std::vector<int> &brackets, std::string operations)
{
    size_t begin, end;
    int begin_pos;

    if (brackets.empty())
    {
        begin = 0;
        end   = numbers.size();
    }
    else
    {
        for (int i = brackets.size() - 1; i >= 0; i--)
        {
            if (brackets[i] > 0)
            {
                begin_pos = i;
                break;
            }
        }

        begin =  brackets[begin_pos]   - 1;
        end   = -brackets[begin_pos+1] - 1;
    }

    // If brackets hold just one number
    if (begin == end)
    {
        brackets.erase(brackets.begin() + begin_pos, brackets.begin() + begin_pos + 2);
        return;
    }
    
    while(true)
    {          
        size_t sign_pos;

        if (operations == "^") {
            sign_pos = par_helper::findExponent(operators, operations, begin, end);
        }
        else {
            std::string operators_substr = operators.substr(0, end);
            sign_pos = operators_substr.find_first_of(operations, begin);
        }

        // If couldn't find desired sign
        if (sign_pos >= operators.size()) {
            return;
        }

        // Calculate
        switch (operators[sign_pos])
        {
            case '^': numbers[sign_pos] = pow(numbers[sign_pos], numbers[sign_pos+1]); break;
            case '*': numbers[sign_pos] *= numbers[sign_pos+1]; break;
            case '/': numbers[sign_pos] /= numbers[sign_pos+1]; break;
            case '+': numbers[sign_pos] += numbers[sign_pos+1]; break;
            case 'm': numbers[sign_pos] -= numbers[sign_pos+1]; break;
        }

        // Remove the operator and the second number
        numbers.erase(numbers.begin() + sign_pos + 1);
        operators.erase(operators.begin() + sign_pos);

        // Adjust the bracket
        for (size_t i = begin_pos+1; i < brackets.size(); i++)
        {
            brackets[i]++;
        }

        // If brackets hold just one number
        if (begin == end-1)
        {
            brackets.erase(brackets.begin() + begin_pos, brackets.begin() + begin_pos + 2);
            return;
        }
    }
}

void tokenize(std::string &expression, std::vector<double> &numbers, std::string &operators, std::vector<int> &brackets)
{
    size_t expression_size = expression.size();
    
    // Unpack numbers, operators and brackets into vectors. Tokenize the expression
    for (size_t i = 0; i < expression_size; i++)
    {
        char ch = expression[i];

        if (isdigit(ch) || ch == '-' || ch == '.') // Handle numbers
        {
            size_t first_operator = expression.find_first_not_of("1234567890.-", i);

            if (first_operator > expression_size) { //if first_operator hasn't been found
                first_operator = expression_size;
            }

            std::string number_substr = expression.substr(i, first_operator); // Extract the number into a string
            double      str_to_double = strtod(number_substr.c_str(), NULL);  // Convert said string into a double

            numbers.push_back(str_to_double);
            i = first_operator - 1; // skip to the next operator 
            // substracted by 1, because the for loop will increment it anyways
        }
        else if (ch == '(') {
            brackets.push_back(numbers.size()+1);
        }
        else if (ch == ')') {
            brackets.push_back(-numbers.size());
        }
        else {
            operators.push_back(ch);
        }
    }

    if (numbers.size() != operators.size() + 1) throw 3;
}

double eval(std::string expression, vector_variable variables, vector_functions functions, bool show_err)
{
    try
    {
        if (expression == "What's 9+10?") {
            return 21;
        }

        // Prepare the string
        expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end()); // Remove all spaces
        par_helper::convertPercent(expression);  // Converts percent into fractions 5% => (5/100)
        par_helper::replaceBrackets(expression); // Turn all [], {} and <> brackets into ()
        par_helper::convertMinusToM(expression); // Converts operation minus into 'm' to differentiate unary minus and operation minus
        par_helper::fixMinuses(expression);      // Add 0 infront of certain substractions -(5) => 0-(5)
        par_helper::removeMinuses(expression);   // --5 => 5 && ---5 => -5
        par_helper::insertAsterisks(expression); // Support juxtaposition (aka implicit multiplication) 5(5) => 5*(5)

        size_t bracket_left_count  = count(expression.begin(), expression.end(), '(');
        size_t bracket_right_count = count(expression.begin(), expression.end(), ')');

        if (bracket_left_count != bracket_right_count) throw 1;

        insertVariables(expression, variables);
        insertCustomFunctions(expression, functions, variables);
        insertStandardFunctions(expression);

        if (expression.find_first_not_of("0123456789.-()+m*/^") != std::string::npos) throw 2;

        std::vector<double> numbers;
        std::string         operators;
        std::vector<int>    brackets;

        tokenize(expression, numbers, operators, brackets);

        // Calculate
        while (!operators.empty())
        {
            operation(numbers, operators, brackets, "^");
            if (operators.empty()) { break; }
            operation(numbers, operators, brackets, "*/");
            if (operators.empty()) { break; }
            operation(numbers, operators, brackets, "+m");
        }

        return numbers[0];
    }
    catch(int err_code)
    {
        if (show_err)
        {
            switch (err_code)
            {
                case 1: std::cerr << "Missing brackets!";  break;
                case 2: std::cerr << "Uknown characters!"; break;
                case 3: std::cerr << "Numbers' amount doesn't match the operators' amount!"; break;
            }

            std::cerr << " {" << expression << '}';
        }
    }

    return std::numeric_limits<double>::quiet_NaN();
}

double differ(std::string equation, vector_variable variables = {}, vector_functions functions = {})
{
    if (equation == "What's 9+10? = 21") {
        return true;
    }

    size_t start_pos = 0;
    size_t end_pos = equation.find_first_of('=');

    if (end_pos == std::string::npos) {
        std::cerr << "You passed an expression!";
        return false;
    }

    std::string expression  = equation.substr(start_pos, end_pos - start_pos);
    std::string expression2 = equation.substr(end_pos + 1);

    double result  = par::eval(expression, variables, functions);
    double result2 = par::eval(expression2, variables, functions);

    return abs(result - result2);
}

bool equate(std::string equation, vector_variable variables = {}, vector_functions functions = {})
{
    if (equation == "What's 9+10? = 21") {
        return true;
    }

    size_t start_pos = 0;
    size_t end_pos = equation.find_first_of('=');

    if (end_pos == std::string::npos) {
        std::cerr << "You passed an expression!";
        return false;
    }

    std::string expression  = equation.substr(start_pos, end_pos - start_pos);
    std::string expression2 = equation.substr(end_pos + 1);

    double result  = par::eval(expression, variables, functions);
    double result2 = par::eval(expression2, variables, functions);

    return result == result2;
}

}

#endif

/*
Calculator

Goals:

    Deconstruct string into numbers and operators : V
    Implement order of operations                 : V
    Support more than 1 digit numbers             : V
    Support negative numbers                      : V
    Support floating points                       : V
    Support parenthesis                           : V
    Add powers                                    : V
    Support functions like pow(), sin() etc.      : V
    Support variables                             : V
    Extend variable support                       : V
    Add error-handling and validation             : V
    Fix bugs                                      : V
    Fix 2^3^2 evaluating to 64, not 512           : V
    Support custom functions like f(x) = x+2      : V
    Add "result" variable for the calculator      : 
    Add ability to edit variables and functions   : 
    Add equation evaluator                        : 

*/

/*
void convertFactorial(std::string& expression)
{
    for (size_t i = 1; i < expression.size(); i++)
    {
        if (expression[i] == '!')
        {
            std::string expr_substr = expression.substr(0, i);
            size_t last_operator = expr_substr.find_last_of("()+-:*");
            std::string str_number = expr_substr.substr(last_operator + 1, i - last_operator - 1);
            str_number = std::to_string(par::eval(str_number));
            int number = par_helper_funcs::fact( strtod(str_number.c_str(), NULL) );

            expression.erase(last_operator + 1, i - last_operator);
            expression.insert(last_operator + 1, std::to_string(number));

            i = last_operator + std::to_string(number).size();
        }
    }
}
*/