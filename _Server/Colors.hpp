#pragma once

#include <iostream>

class Color {
public:
    enum Code {
        BLACK = 30,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        PURPLE,
        CYAN,
        WHITE,
        BOLD = 1
    };
    Color(Code code) : code_(code) {}
    friend std::ostream& operator<<(std::ostream& os, const Color& color) {
        return os << "\033[" << color.code_ << "m";
    }
private:
    Code code_;
};