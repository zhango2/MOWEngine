#ifndef CDXFace_H
#define CDXFace_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "MOWCommon.h"

namespace PbMOWGraphics
{
    class PbMOWFace;
}

class CMOWFace
{
public:
    CMOWFace(
        );

    ~CMOWFace(
        );

    static CMOWFace*         
        Create(
            long index1,
            long index2,
            long index3
            );

    const std::vector<long>&      
        Indices(
            ) const;

    const std::vector<long>&      
        AdjecentIndices(
            ) const;

    bool                    
        Serialize(
            std::ofstream& fOut
            );

    bool                    
        Serialize(
            std::ifstream& fIn
            );

    void                    
        AddIndex(
            long index
            );

    void                    
        AddAdjecentIndex(
            long index
            );

    void                    
        CreateAdjecentEdgesAndNormals(
            const vertex_vector& vertices,
            DirectX::XMFLOAT4X4 worldMatrix
            );    
    
    
    DirectX::XMFLOAT3 
        Normal(
            ) const;

    const std::vector<CMOWFace>& 
        NeighborFaces( 
            ) const;

    void                    
        IncreaseAdjacentCount(
            );

    int                     
        AdjacentCount(
            ) const;

    void                    
        ToPb( 
            PbMOWGraphics::PbMOWFace& toPb 
            )const;

    static CMOWFace*
        FromPb(
            const PbMOWGraphics::PbMOWFace& fromPb 
            );

private:

    CMOWFace(
        long index1,
        long index2,
        long index3
        );

    DirectX::XMFLOAT3       
        CalculateNormal(
            DirectX::XMFLOAT3 p1,
            DirectX::XMFLOAT3 p2
            )const;

    DirectX::XMFLOAT4       
        Float3ToFloat4(
            DirectX::XMFLOAT3 src
            )const;

    DirectX::XMFLOAT3       
        CreateEdge(
            DirectX::XMFLOAT3 p1, 
            DirectX::XMFLOAT3 p2, 
            DirectX::XMFLOAT4X4 worldMatrix
            )const;

    std::vector<long>       
        m_indices;

    std::vector<long>       
        m_adjIndices;

    std::vector<DirectX::XMFLOAT3>      
        m_adjEdges;

    std::vector<CMOWFace>                
        m_adjFaces;

    DirectX::XMFLOAT3                   
        m_normal;

    int                                 
        m_adjCount;

};

class CMOWEdge
{
public:

    CMOWEdge(
        long index1,
        long index2
        );

    ~CMOWEdge(
        );


    long                    
        m_index1;

    long                    
        m_index2;
};

class CMOWCompareEdges
{
public:

    bool                    
        operator()(
            const CMOWEdge& edge1, 
            const CMOWEdge& edge2
            );
};

class CMOWCompareXMFLOAT3
{
public:

    bool                    
        operator()(
            const DirectX::XMFLOAT3& vec1, 
            const DirectX::XMFLOAT3& vec2
            );
};

class CMOWNeighbors
{
public:

    CMOWNeighbors(
        );

    ~CMOWNeighbors(
        );
    
    long                    
        GetOther(
            long me
            )const;

    void                    
        AddNeighbor(
            long index
            );

private:

    long                    
        m_index1;

    long                    
        m_index2;
};

#endif