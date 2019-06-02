#include "MOWOctTreeNode.h"
#include "MOWBoundingBox.h"

static std::vector<DirectX::XMFLOAT3>  s_boundsOffsets = { {-0.5f,-0.5f,-0.5f},
                                                           { 0.5f,-0.5f,-0.5f},
                                                           {-0.5f, 0.5f,-0.5f},
                                                           { 0.5f, 0.5f,-0.5f},
                                                           {-0.5f,-0.5f, 0.5f},
                                                           { 0.5f,-0.5f, 0.5f},
                                                           {-0.5f, 0.5f, 0.5f},
                                                           { 0.5f, 0.5f, 0.5f}
                                                          };
using namespace DirectX;

//---------------------------------------------
CMOWOctTreeNode::CMOWOctTreeNode()
{
    m_boundingBox = 0;
    m_isRoot = false;
}
//---------------------------------------------
CMOWBoundingBox* CMOWOctTreeNode::BoundingBox() const
{
    return m_boundingBox;
}
//---------------------------------------------
void CMOWOctTreeNode::Initialize(
    CMOWBounds& bounds,
    model_collection& fixedModels,
    ID3D11Device* device
)
{
    m_boundingBox = CMOWBoundingBox::Create(bounds.m_size*2.0f, bounds.m_size*2.0f, bounds.m_size*2.0f);
    m_boundingBox->Initialize(device);
    m_boundingBox->Position(bounds.m_center.x, bounds.m_center.y, bounds.m_center.z);
    m_boundingBox->Update();
    AddModels(fixedModels);
    if (Models().size() >= 4)
    {

        CMOWBounds newBounds;
        newBounds.m_size = bounds.m_size * 0.5f;

        for (size_t i = 0; i < s_boundsOffsets.size(); i++)
        {
            float xOffset = s_boundsOffsets[i].x*bounds.m_size;
            float yOffset = s_boundsOffsets[i].y*bounds.m_size;
            float zOffset = s_boundsOffsets[i].z*bounds.m_size;

            newBounds.m_center.x = bounds.m_center.x + xOffset;
            newBounds.m_center.y = bounds.m_center.y + yOffset;
            newBounds.m_center.z = bounds.m_center.z + zOffset;
            CMOWOctTreeNode* child = new CMOWOctTreeNode();
            child->Initialize(newBounds, fixedModels, device);

            if (child->Models().size())
            {
                m_children.push_back(child);
            }
            else
            {
                delete child;
            }
        }

    }
    //RemoveNodeModelsFromCollection(fixedModels);
}
//---------------------------------------------
const CMOWOctTreeNode::model_collection& CMOWOctTreeNode::Models() const
{
    return m_models;
}
//---------------------------------------------
bool CMOWOctTreeNode::IsRoot() const
{
    return m_isRoot;
}
//---------------------------------------------
const CMOWOctTreeNode::children_collection& CMOWOctTreeNode::Children() const
{
    return m_children;
}
//---------------------------------------------
void CMOWOctTreeNode::AddModels(
    model_collection& allModels
    )
{
    for(CMOWModelPtr model : allModels)
    {
        CMOWBoundingBox* boundingBox = model->BoundingBox();

        if (m_boundingBox->IsInside(*boundingBox))
        {
            m_models.insert(model);
        }
    }
}
//---------------------------------------------
CMOWOctTreeNode::~CMOWOctTreeNode()
{
    children_collection::iterator itChild = m_children.begin();

    while (itChild != m_children.end())
    {
        CMOWOctTreeNode* child = (*itChild);
        delete child;
        itChild = m_children.erase(itChild);
    }
}
//---------------------------------------
void CMOWOctTreeNode::RemoveNodeModelsFromCollection(
    model_collection& allModels
    )
{
    auto itModelToBeRemoved = Models().begin();
    while (itModelToBeRemoved != Models().end())
    {
        auto itModel = allModels.find((*itModelToBeRemoved));
        if (itModel != allModels.end())
        {
            allModels.erase(itModel);
        }
        itModelToBeRemoved++;
    }
}
//---------------------------------------
void CMOWOctTreeNode::Render(
    ID3D11DeviceContext* context,
    const DirectX::XMFLOAT4X4& viewMatrix,
    const DirectX::XMFLOAT4X4& projectionMatrix,
    const DirectX::XMVECTOR& cameraPosition,
    int screenWidth,
    int screenHeight
)
{
    m_boundingBox->Render(context, viewMatrix, projectionMatrix, cameraPosition, screenWidth, screenHeight);

    auto itChild = m_children.begin();

    while (itChild != m_children.end())
    {
        CMOWOctTreeNode* child = (*itChild);
        child->Render(context, viewMatrix, projectionMatrix, cameraPosition, screenWidth, screenHeight);
        itChild++;
    }
}