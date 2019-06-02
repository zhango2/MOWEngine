#ifndef CDXModelPart_H
#define CDXModelPart_H

#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include "MOWCommon.h"


class CMOWFace;
class CMOWVector;

DECLARE_SHARED_PTR(CMOWBone)
DECLARE_SHARED_PTR(CMOWFace)
DECLARE_SHARED_PTR(CMOWModelPart)

typedef std::vector<CMOWFacePtr> face_vector;
typedef std::vector<ID3D11ShaderResourceView*> texture_vector;

namespace PbMOWGraphics
{
    class PbMOWModelPart;
}

class CMOWModelPart
{
public:
    CMOWModelPart(
        D3D11_PRIMITIVE_TOPOLOGY topology
        );

    CMOWModelPart(
        CMOWModelPart& orig
        );

    ~CMOWModelPart();

    const vertex_vector&        
        Vertices(
            ) const;

    const index_vector&         
        Indices(
            ) const;

    const index_vector&               
        IndicesWithAdjacencies(
            )const;

    index_vector&
        MutableIndicesWithAdjacencies(
            );

    const face_vector&          
        Faces(
            ) const;

    bool                        
        CreateResources(
            ID3D11Device* device
            );

    bool                        
        Render(
            ID3D11DeviceContext* context,
            bool useAdjacentIndices = false
            );

    D3D11_PRIMITIVE_TOPOLOGY    
        Topology(
            ) const;

    void                        
        AddVertexAndIndex(
            const Vertex& vert,
            long index
            );

    void                        
        AddVertex(
            const Vertex& vert
            );

    void                        
        AddFace(
            CMOWFacePtr face
            );

    void
        AddBone(
            CMOWBonePtr bone
            );
    
    const std::vector<ID3D11ShaderResourceView*>&
        Resources(
            ) const;

    bool                        
        Serialize(
            std::ofstream& fOut
            );
    bool                        
        Serialize(
            std::ifstream& fIn
            );
    
    const char*                 
        MaterialName(
            ) const;
    
    void                        
        MaterialName(
            const char* materialName
            );

    ID3D11Buffer*               
        GetVertexBuffer(
            );
    
    ID3D11Buffer*               
        GetIndexBuffer(
            );
    
    const char*                 
        Name(
            )const;
    
    void                        
        Name(
            const char* name
            );

    void                        
        ToPb(
            PbMOWGraphics::PbMOWModelPart& toPb
            ) const;

    static CMOWModelPartPtr
        FromPb(
            const PbMOWGraphics::PbMOWModelPart& fromPb
            );



private:


    void                        
        Clear(
            );

    ID3D11Buffer*               
        m_vertexBuffer;
    ID3D11Buffer*               
        m_indexBuffer;
    ID3D11Buffer*               
        m_adjacentIndexBuffer;
    vertex_vector               
        m_vertices;
    face_vector                 
        m_faces;
    index_vector                
        m_indices;
    index_vector                
        m_indicesWithAdjacencies;
    D3D11_PRIMITIVE_TOPOLOGY    
        m_topology;
    texture_vector              
        m_resources;
    std::string                 
        m_materialName;
    std::string                 
        m_name;
    std::map<std::string, CMOWBonePtr>
        m_bones;
};


#endif