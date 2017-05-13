#include "MOWStringConverter.h"

//---------------------------------------
std::wstring CMOWStringConverter::CharToWchar(
    const char* source
    )
{
    std::wstring resultString;
    size_t size = strlen(source)+1;
    size_t retVal = 0;
    resultString.resize(size);
    mbstowcs_s(&retVal,&resultString[0],size,source,size);

    return resultString;
}

