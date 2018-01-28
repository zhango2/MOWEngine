#ifndef MOWAssimp_H
#define MOWAssimp_H

#include "MOWGraphics/MOWMaterial.h"

class CMOWModel;
namespace MOWAssimp
{
    CMOWModel*
        Load(
            const char* fileName
            );

    void
        AppendTextureFileNameToMaterial(
            CMOWMaterial::TEXTURE_TYPE textureType,
            const std::string& objectName,
            CMOWMaterial*& material
            );
}

#endif