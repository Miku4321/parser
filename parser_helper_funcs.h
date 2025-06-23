#ifndef PARSER_HELPER_FUNCS_H_
#define PARSER_HELPER_FUNCS_H_

#define printVar(a) (std::cout << #a << " = " << a << '\n')

namespace par_helper
{

int fact(int n)
{
    if (n == 1) return n;

    return n * fact(n-1);
}

void convertPercent(std::string& expression)
{
    for (size_t i = 1; i < expression.size(); i++)
    {
        if (expression[i] == '%')
        {
            std::string expr_substr = expression.substr(0, i);
            size_t last_operator = expr_substr.find_last_of("()+-/*");
            std::string fraction = '(' + expr_substr.substr(last_operator + 1, i - last_operator - 1) + "/100)";

            expression.erase(last_operator + 1, i - last_operator);
            expression.insert(last_operator + 1, fraction);

            i = last_operator + fraction.size();
        }
    }
}

void replaceBrackets(std::string& expression)
{
    std::replace(expression.begin(), expression.end(), '{', '(');
    std::replace(expression.begin(), expression.end(), '<', '(');
    std::replace(expression.begin(), expression.end(), '[', '(');
    std::replace(expression.begin(), expression.end(), '}', ')');
    std::replace(expression.begin(), expression.end(), ']', ')');
    std::replace(expression.begin(), expression.end(), '>', ')');
}

void convertMinusToM(std::string& expression)
{
    for (size_t i = 0; i < expression.size(); i++)
    {
        if (expression[i] == '-' && (isdigit(expression[i-1]) || expression[i+1] == '(' || expression[i-1] == ')'))
        {
            expression[i] = 'm';
        }
    }
}

void fixMinuses(std::string& expression)
{
    for (size_t i = 0; i < expression.size(); i++)
    {
        if (expression[i] == 'm' && expression[i+1] == '(' && !isdigit(expression[i-1]))
        {
            expression.insert(expression.begin() + i, '0');
            i++;
        }
    }
}

void removeMinuses(std::string& expression)
{
    for (size_t i = 0; i < expression.size(); i++)
    {
        if (expression[i] == '-' && expression[i+1] == '-')
        {
            expression.erase(i, 2);
            i-=1;
        }
    }
}

void insertAsterisks(std::string& expression)
{
    for (size_t i = 1; i < expression.size(); i++)
    {
        if (expression[i] == '(' && (isdigit(expression[i-1]) || expression[i-1] == ')'))
        {
            expression.insert(expression.begin() + i, '*');
            i++;
        }
        else if (expression[i] == ')' && isdigit(expression[i+1])) {
            expression.insert(expression.begin() + i + 1, '*');
            i++;
        }
    }
}

size_t findClosingBracket(std::string& func_str)
{
    int depth = 0;
    size_t i = func_str.find_first_of('(');

    while (i < func_str.size())
    {
                if (func_str[i] == '(') depth++;
        else if (func_str[i] == ')') depth--;
        
        if (depth == 0) return i;

        i++;
    }

    std::cerr << "Mismatched brackets in a function!";
    return 0;
}

size_t findExponent(const std::string &operators, const std::string &operations, size_t &begin, size_t &end)
{
    for (int i = end - 1; i >= (int)begin; i--) {
        if (operators[i] == operations[0]) {
            return i;
        }
    }

    return operators.size();
}

};

#endif