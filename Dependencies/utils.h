#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <cmath>
#include <vector>

namespace ut
{

int fact(int n)
{
    if (n == 1) return n;

    return n * fact(n-1);
}

template<typename T>
double root(T base, T degree) {
    return pow(base, 1.0/degree);
}

template<typename T>
bool isInRange(T value, T a, T b)
{
    if (a > b) {
        std::swap(a, b);
    }

    return value >= a && value <= b;
}

template<typename T>
T getMinOrMax(const T &A, const T &B, bool getSmaller = true)
{
    if (getSmaller) return A <= B ?  A : B;
    else            return A >  B ?  A : B;
}

int random(int min, int max)
{
    if (min > max)
    {
        std::swap(min, max);
    }

    return min + (rand() % (max-min+1));
}

template<typename T>
void printVec(std::vector<T>& vec, std::string separator_str = ", ", std::string empty_msg = "Empty")
{
    if (vec.empty()) {
        std::cout << empty_msg;
        return;
    }

    size_t vec_size = vec.size();

    for (size_t i = 0; i < vec_size - 1; i++) {
        std::cout << vec[i] << separator_str;
    }
    std::cout << vec[vec_size-1];
}

template <typename T, typename Getter>
size_t find_element(std::vector<T>& vec, const std::string& desired_element, Getter get_name)
{
    size_t position = vec.size();

    if (isdigit(desired_element[0])) // If search by position
    {
        // - 1, because index starts from 0, but humans start from 1
        position = stoi(desired_element) - 1;

        if (position >= vec.size()) {
            std::cout << "Out of range!";
        }
    }
    else // If search by name
    {
        auto it = find_if(vec.begin(), vec.end(), 
            [&desired_element, &get_name](const auto& p) {return get_name(p) == desired_element; });

        position = std::distance(vec.begin(), it);

        if (position >= vec.size()) {
            std::cout << "Element not found!";
        }
    }

    return position;
}

};

#endif