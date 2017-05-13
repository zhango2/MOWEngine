#include "ObjParser.h"
#include <iostream>
#include <string>

#define OBJECT "# object "
#define VERTEX "v "
#define TEXTURE "vt"
#define NORMAL "vn"
#define FACE "f "
#define USEMATERIAL "usemtl "
#define MATERIALLIB "mtllib "
#define NEWMATERIAL "newmtl "
#define AMBIENT "\tKa "
#define SPECULAR "\tKs "
#define DIFFUSE "\tKd "
#define EMISSIVE "\tKe "
#define SPECULARPOWER "\tNs"
#define AMBIENTMAP "\tmap_Ka "
#define DIFFUSEMAP "\tmap_Kd "
#define SPECULARMAP "\tmap_Ks "

std::vector<CObjParser::ObjObject> CObjParser::Parse(
    const char* fileName,
    std::map<std::string,ObjMaterial>& materials
    )
{
    bool retVal = false;

    std::ifstream fin;
    std::string materialName = "";
    std::string currentLine = "";
    std::stringstream stream;
    std::string materialLib = fileName;
    std::stringstream test;
    std::vector<ObjObject> objects;

    materialLib = materialLib.substr(0,materialLib.find_last_of('\\')+1);

    fin.open(fileName);

    if( !fin.fail() )
    {
        char readByte = 0;
        
        test << fin.rdbuf();
        
        long vertexIndex = 0;
        int objCount = 0;
        while( !test.eof() )
        {
            char buff[256];
            
            test.getline(buff,256);
            currentLine = buff;
            ObjObject nextObject;

            if( currentLine.find(OBJECT) == 0 )
            {
                stream << currentLine.substr(strlen(OBJECT)).c_str();
                PopulateObjects(stream,test,objects,materialLib);
            }
            else if(currentLine.find(MATERIALLIB) == 0)
            {
                materialLib += currentLine.substr(strlen(MATERIALLIB));
            }
        }

        retVal = true;
    }
    fin.close();
    if( retVal && materialLib.size() )
    {
        ParseMaterials(materialLib.c_str(),materials);
    }

    return objects;
}

void CObjParser::ParseMaterials(
    const char* fileName,
    std::map<std::string, ObjMaterial>& materials
    )
{
    std::ifstream fin;
    std::string currentLine = "";
    std::string materialName = "";
    std::stringstream stream;
    std::stringstream test;
    fin.open(fileName);

    if( !fin.fail() )
    {
        ObjMaterial material;
        test << fin.rdbuf();
        while( !test.eof() )
        {
            
            char buff[256];
            test.getline(buff,256);

            currentLine = buff;

            std::stringstream stream;
            if( currentLine.find(NEWMATERIAL) == 0 )
            {
                material.m_name = currentLine.substr(strlen(NEWMATERIAL));
            }
            else if( currentLine.find(AMBIENT) == 0 )
            {
                stream << currentLine.substr(strlen(AMBIENT)).c_str();

                stream >> material.m_aB; 
                stream >> material.m_aR; 
                stream >> material.m_aG; 

                stream.clear();

            }
            else if( currentLine.find(SPECULAR) == 0 )
            {
                stream << currentLine.substr(strlen(SPECULAR)).c_str();

                stream >> material.m_sB; 
                stream >> material.m_sR; 
                stream >> material.m_sG; 

                stream.clear();

            }
            else if( currentLine.find(DIFFUSE) == 0 )
            {
                stream << currentLine.substr(strlen(DIFFUSE)).c_str();

                stream >> material.m_dB; 
                stream >> material.m_dR; 
                stream >> material.m_dG; 

                stream.clear();

            }
            else if( currentLine.find(EMISSIVE) == 0 )
            {
                stream << currentLine.substr(strlen(EMISSIVE)).c_str();

                stream >> material.m_eB; 
                stream >> material.m_eR; 
                stream >> material.m_eG; 

                stream.clear();

            }
            else if( currentLine.find(SPECULARPOWER) == 0 )
            {
                stream << currentLine.substr(strlen(SPECULARPOWER)).c_str();

                stream >> material.m_specularPower; 
                
                stream.clear();

            }
            else if( currentLine.find(AMBIENTMAP) == 0 )
            {
                stream << currentLine.substr(strlen(AMBIENTMAP)).c_str();
                material.m_aTexture = stream.str();
                stream.clear();
                int count = 0;
                while(test.getline(buff,256) && count < 2)
                {
                    currentLine = buff;

                    if( currentLine.find(DIFFUSEMAP) == 0 )
                    {
                        stream << currentLine.substr(strlen(DIFFUSEMAP)).c_str();
                        material.m_dTexture = stream.str();
                        stream.clear();
                    }
                    else if( currentLine.find(SPECULARMAP) == 0 )
                    {
                        stream << currentLine.substr(strlen(SPECULARMAP)).c_str();
                        stream >> material.m_sTexture;
                        stream.clear();
                    }
                    else
                    {
                        break;
                    }
                    count++;
                }
            }
            
            
            if( material.IsComplete() )
            {
                
                materials[material.m_name] = material;
                material.Reset();
            }

        }
    }

}

 void CObjParser::PopulateVertexCoordinates(
    ObjVertex& vertex,
    std::stringstream& stream
    )
 {
     stream >> vertex.m_x;
     stream >> vertex.m_y;
     stream >> vertex.m_z;
 }

 void CObjParser::PopulateTextureCoordinates(
    ObjTexture& texture,
    std::stringstream& stream
    )
 {
     
     stream >> texture.m_u;
     stream >> texture.m_v;
     stream >> texture.m_w;
 }

 void CObjParser::PopulateNormalCoordinates(
    ObjNormal& normal,
    std::stringstream& stream
    )
 {
     stream >> normal.m_nX;
     stream >> normal.m_nY;
     stream >> normal.m_nZ;
 }

 void CObjParser::StepStream(
    int bytes,
    std::stringstream& stream
    )
 {
    int count = 0;
    char temp;
    while( count != bytes && !stream.eof() )
    {
        stream >> temp;
        count++;
    }
 }

bool CObjParser::PopulateObjects(
    std::stringstream& stream,
    std::stringstream& sourceStream,
    std::vector<ObjObject>& objects,
    std::string& materialLib
    )
{
    bool retVal = false;
    std::vector<ObjVertex> vertices;
    std::vector<ObjFace> faces;
    std::vector<ObjTexture> textures;
    std::vector<ObjNormal> normals;
    std::vector<long> indices;
    std::string materialName = "";
    ObjObject obj;
    bool objectAdded = false;

    stream >> obj.m_name;

    while( !sourceStream.eof() )
    {
        char buff[256];

        sourceStream.getline(buff, 256);
        std::string currentLine = buff;

        if( currentLine.find(OBJECT) == 0 )
        {
            //Just one object at a time!
            
            obj.m_faces = faces;
            obj.m_indecies = indices;
            obj.m_normals = normals;
            obj.m_textureCoordinates = textures;
            obj.m_vertices = vertices;
            obj.m_materialName = materialName;
            objects.push_back(obj);

            objectAdded = true;

            stream << currentLine.substr(strlen(OBJECT)).c_str();
            PopulateObjects(stream,sourceStream,objects,materialLib);
            break;
        }
        else if(currentLine.find(VERTEX) == 0)
        {
            stream << currentLine.substr(strlen(VERTEX)).c_str();
            ObjVertex vertex;
            PopulateVertexCoordinates(vertex, stream);
            vertices.push_back(vertex);
            stream.clear();
        }
        else if(currentLine.find(TEXTURE) == 0)
        {
            stream << currentLine.substr(strlen(TEXTURE)).c_str();
            ObjTexture texture;
            PopulateTextureCoordinates(texture, stream);
            textures.push_back(texture);
            stream.clear();
        }
        else if(currentLine.find(NORMAL) == 0)
        {
            stream << currentLine.substr(strlen(NORMAL)).c_str();
            ObjNormal normal;
            PopulateNormalCoordinates(normal, stream);
            normals.push_back(normal);
            stream.clear();
        }
        else if(currentLine.find(FACE) == 0)
        {
            stream << currentLine.substr(strlen(FACE)).c_str();

            ObjFace face;
            long index = 0;
            long textureIndex = 0;
            long normalIndex = 0;
            for(int i = 0; i < 3; i++)
            {
                stream >> index;
                indices.push_back(index - 1);

                face.m_vertices.push_back(index);

                StepStream(1, stream);
                stream >> textureIndex;
                face.m_textures.push_back(textureIndex);

                StepStream(1, stream);
                stream >> normalIndex;
                face.m_normals.push_back(normalIndex);

                face.m_material = materialName;
            }
            faces.push_back(face);
            stream.clear();

        }
        else if(currentLine.find(USEMATERIAL) == 0)
        {
            materialName = currentLine.substr(strlen(USEMATERIAL));
        }
        
    }
    
    if( !objectAdded )
    {
        obj.m_faces = faces;
        obj.m_indecies = indices;
        obj.m_normals = normals;
        obj.m_textureCoordinates = textures;
        obj.m_vertices = vertices;
        obj.m_materialName = materialName;
        objects.push_back(obj);
    }
    
    return true;
 }
