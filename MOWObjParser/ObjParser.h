#ifndef _OBJPARSER_H
#define _OBJPARSER_H

#include <vector>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

class CObjParser
{
public:

    struct ObjMaterial
    {
        ObjMaterial()
        {
            Reset();
        }

        bool IsComplete()
        {
            bool isComplete = m_name.size() && 
                              m_sR != -1.0 && 
                              m_sG != -1.0 &&
                              m_sB != -1.0 &&
                              m_dR != -1.0 && 
                              m_dG != -1.0 &&
                              m_dB != -1.0 &&
                              m_aR != -1.0 && 
                              m_aG != -1.0 &&
                              m_aB != -1.0 &&
                              m_specularPower != -1.0f;

            isComplete = isComplete && 
                         (m_aTexture.size() || 
                          m_dTexture.size() ||
                          m_sTexture.size());

            return isComplete;

        }

        void Reset()
        {
            m_sR = -1.0;
            m_sG = -1.0;
            m_sB = -1.0;

            m_dR = -1.0;
            m_dG = -1.0;
            m_dB = -1.0;

            m_aR = -1.0;
            m_aG = -1.0;
            m_aB = -1.0;
            m_name = "";
            m_aTexture = "";
            m_dTexture = "";
            m_sTexture = "";
            m_specularPower = -1.0f;
        }

        std::string m_name;
        std::string m_aTexture;
        std::string m_dTexture;
        std::string m_sTexture;
        float m_sR;
        float m_sG;
        float m_sB;

        float m_dR;
        float m_dG;
        float m_dB;

        float m_aR;
        float m_aG;
        float m_aB;

        float m_eR;
        float m_eG;
        float m_eB;

        float m_specularPower;
    };

    struct ObjVertex
    {
        ObjVertex()
        {
            m_x = 0.0;
            m_y = 0.0;
            m_z = 0.0;
        }

        float m_x;
        float m_y;
        float m_z;

    };

    struct ObjTexture
    {
        float m_u;
        float m_v;
        float m_w;
    };

    struct ObjNormal
    {
        float m_nX;
        float m_nY;
        float m_nZ;
    };

    struct ObjFace
    {
        std::vector<long> m_textures;
        std::vector<long> m_normals;
        std::vector<long> m_vertices;
        std::string m_material;
    };

    struct ObjObject
    {
        std::vector<long> m_indecies;
        std::vector<ObjTexture> m_textureCoordinates;
        std::vector<ObjNormal> m_normals;
        std::vector<ObjVertex> m_vertices;
        std::vector<ObjFace> m_faces;
        std::string m_name;
        std::string m_materialName;
    };

    

    static std::vector<ObjObject> Parse(
        const char* fileName,
        std::map<std::string,ObjMaterial>& materials
        );
private:

    static void PopulateVertexCoordinates(
        ObjVertex& vertex,
        std::stringstream& stream
        );

    static void PopulateTextureCoordinates(
        ObjTexture& texture,
        std::stringstream& stream
        );

    static void PopulateNormalCoordinates(
        ObjNormal& normals,
        std::stringstream& stream
        );

    static void StepStream(
        int bytes,
        std::stringstream& stream
        );

    static void ParseMaterials(
        const char* fileName,
        std::map<std::string, ObjMaterial>& materials
        );

    static bool
        PopulateObjects(
            std::stringstream& stream,
            std::stringstream& sourceStream,
            std::vector<ObjObject>& objects,
            std::string& materialLib
            );
};

#endif