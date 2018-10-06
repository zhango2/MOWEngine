#ifndef CDXOctTreeNode_H
#define CDXOctTreeNode_H

#include <set>
#include <vector>
#include "MOWCommon.h"

class CMOWModel;
class CMOWBoundingBox;

class CMOWBounds
{
public:

    CMOWBounds()
    {
        m_size = 0.0f;
    }

    DirectX::XMFLOAT3 m_center;
    float m_size;
};
class CMOWOctTreeNode
{
public:

    typedef std::vector<CMOWOctTreeNode*> children_collection;
    typedef std::vector<CMOWBoundingBox*> bounding_boxes;
    typedef std::set<CMOWModel*> model_collection;


    CMOWOctTreeNode();
    ~CMOWOctTreeNode();

    void                        
        Initialize(
            CMOWBounds& bounds,
            model_collection& fixedModels,
            ID3D11Device* device
            );

    CMOWBoundingBox*             
        BoundingBox(
            )const;

    const model_collection&     
        Models(
            )const;
    bool                        
        IsRoot(
            )const;
    const children_collection&  
        Children(
            )const;

    void                        
        Render(
            ID3D11DeviceContext* context,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMVECTOR& cameraPosition,
            int screenWidth,
            int screenHeight
            );

private:

    void                            
        AddModels(
            model_collection& allModels
            );
    void
        RemoveNodeModelsFromCollection(
            model_collection& allModels
            );

    CMOWBoundingBox*                        
        m_boundingBox;

    model_collection                       
        m_models;

    children_collection                    
        m_children;

    bool                                   
        m_isRoot;

};

#endif;
