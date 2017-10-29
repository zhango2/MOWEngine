#ifndef MOWAssimp_H
#define MOWAssimp_H

class CMOWModel;
namespace MOWAssimp
{
    CMOWModel*
        Load(
            const char* fileName
            );
}

#endif