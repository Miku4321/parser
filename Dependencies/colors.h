#ifndef COLORS_H_
#define COLORS_H_

#include <string>

namespace col
{

class RGB
{
    public:

    uint8_t r, g, b;
    std::string seq; // rgb value as an escape sequence

    RGB(
        uint8_t p_r,
        uint8_t p_g,
        uint8_t p_b)

        : r{p_r}
        , g{p_g}
        , b{p_b}
        , seq{"\033[38;2;" + std::to_string(p_r) + ';' + std::to_string(p_r) + ';' + std::to_string(p_r) + "m"} {
    }

    void change(int p_r, int p_g, int p_b)
    {
        r = p_r;
        g = p_g;
        b = p_b;

        seq = "\033[38;2;" +
        std::to_string(r) + ';' +
        std::to_string(g) + ';' +
        std::to_string(b) + "m";
    }
};

// Utilities
const std::string ERASE_SCREEN  = "\033[H";
const std::string ERASE_SCREEN2 = "\033[2J\033[H";
const std::string RESET = "\033[0m";
const char bell = '\7';

// Cursor
const std::string HIDE_CURSOR = "\x1b[?25l";
const std::string SHOW_CURSOR = "\x1b[?25h";

// colors
const std::string BLACK   = "\033[30m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";
const std::string BLUE    = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN    = "\033[36m";
const std::string WHITE   = "\033[37m";

const std::string BG_BLACK   = "\033[40m";
const std::string BG_RED     = "\033[41m";
const std::string BG_GREEN   = "\033[42m";
const std::string BG_YELLOW  = "\033[43m";
const std::string BG_BLUE    = "\033[44m";
const std::string BG_MAGENTA = "\033[45m";
const std::string BG_CYAN    = "\033[46m";
const std::string BG_WHITE   = "\033[47m";

// colors - Bright
const std::string B_BLACK   = "\033[90m";
const std::string B_RED     = "\033[91m";
const std::string B_GREEN   = "\033[92m";
const std::string B_YELLOW  = "\033[93m";
const std::string B_BLUE    = "\033[94m";
const std::string B_MAGENTA = "\033[95m";
const std::string B_CYAN    = "\033[96m";
const std::string B_WHITE   = "\033[97m";

void enableANSI() {
    #ifdef _WIN32
        system("cls");
    #endif
}

// Print custom color with parameters
// Must be paired with std::cout to change color
std::string fg_color(int r, int g, int b) {
    return "\033[38;2;" + std::to_string(r) + ';' + std::to_string(g) + ';' + std::to_string(b) + "m";
}

std::string bg_color(int r, int g, int b) {
    return "\033[48;2;" + std::to_string(r) + ';' + std::to_string(g) + ';' + std::to_string(b) + "m";
}

std::string setCursPos(int x, int y) {
    return "\033[" + std::to_string(y) + ';' + std::to_string(x) + 'H';
}

std::string cursUp(int rows) {
    return "\033[" + std::to_string(rows) + 'A';
}

std::string cursRight(int columns) {
    return "\033[" + std::to_string(columns) + 'C';
}

std::string fg_to_bg(std::string &color)
{
    size_t digit_pos = color.size() - 3;

    if (color[digit_pos] == '3')
    {
        color[digit_pos] = '4';
    }
    else if (color[digit_pos] == '9')
    {
        color[digit_pos] = '1';
        color.insert(digit_pos + 1, "0");
    }

    return color;
}

}

#endif

/*
template<typename... Args>
void print(Color color, const Args&... args)
{
    std::cout << rgb(color);
    (std::cout << ... << args);
    std::cout << RESET;
}
*/