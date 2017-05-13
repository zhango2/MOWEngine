#ifndef CMOWStringConverter_H
#define CMOWStringConverter_H

#include <string>

class CMOWStringConverter
{
public:

    static std::wstring 
        CharToWchar(
            const char* source
            );
};

#endif