#ifndef MOWAssimp_H
#define MOWAssimp_H

#include <string>
#include <map>
#include <vector>

class CMOWPhysics;
class CMOWModel;
struct aiScene;
struct aiMesh;
namespace MOWAssimp
{
    void
        LoadModelsIntoResourceManager(
            const char* fileName,
            const char* modelName,
            const std::string& texturePath,
            CMOWPhysics& physics,
            bool loadAllMeshesIntoASingleModel
            );

    void
        LoadMaterialsAndTextures(
            const aiScene* aiScene,
            const char* fileName,
            const std::string& texturePath,
            std::map< std::string, std::vector<std::string> >& textureNamesByMaterialName,
            std::map< unsigned int, std::string >& materialNameByIndex
            );

    void
        LoadModel(
            const aiMesh* mesh,
            const char* modelName,
            CMOWPhysics& physics,
            const std::map< std::string, std::vector<std::string> >& textureNamesByMaterialName,
            const std::map< unsigned int, std::string >& materialNameByIndex,
            CMOWModel* target
            );
}

#endif