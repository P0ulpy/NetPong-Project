//
// Created by Flo on 17/01/2022.
//

#ifndef NETPONG_PROJECT_CONSOLECOLOR_HPP
#define NETPONG_PROJECT_CONSOLECOLOR_HPP

#include <iostream>

#ifdef WIN32

#include <Windows.h>

namespace ConsoleColor
{
    inline std::ostream& blue(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
                                         |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& red(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_RED|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& green(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& yellow(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& white(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
        return s;
    }

    struct color {
        color(WORD attribute):m_color(attribute){};
        WORD m_color;
    };

    template <class _Elem, class _Traits>
    std::basic_ostream<_Elem,_Traits>&
    operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
    {
        HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,c.m_color);
        return i;
    }
}

#endif

#ifdef LINUX

namespace ConsoleColor
{
    inline std::ostream& blue(std::ostream &s)
    {
        return s;
    }

    inline std::ostream& red(std::ostream &s)
    {
        return s;
    }

    inline std::ostream& green(std::ostream &s)
    {
        return s;
    }

    inline std::ostream& yellow(std::ostream &s)
    {
        return s;
    }

    inline std::ostream& white(std::ostream &s)
    {
        return s;
    }

    /*struct color {
        color(WORD attribute):m_color(attribute){};
        WORD m_color;
    };

    template <class _Elem, class _Traits>
    std::basic_ostream<_Elem,_Traits>&
    operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
    {
        HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,c.m_color);
        return i;
    }*/
}

#endif

#endif //NETPONG_PROJECT_CONSOLECOLOR_HPP
