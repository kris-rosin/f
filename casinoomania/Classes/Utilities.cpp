//
//  Utilities.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 31.10.16.
//
//

#include "Utilities.h"
#include <locale>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string FormatWithCommas(unsigned long long value)
{
    struct Numpunct: public std::numpunct<char>
    {
    protected:
        virtual char do_thousands_sep() const{return ',';}
        virtual std::string do_grouping() const{return "\03";}
    };
    
    std::stringstream ss;
    ss.imbue({std::locale(), new Numpunct});
    ss << std::setprecision(2) << std::fixed << value;
    return ss.str();
}
