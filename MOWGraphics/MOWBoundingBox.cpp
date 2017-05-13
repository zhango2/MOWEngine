
#include "MOWBoundingBox.h"
#include "MOWModelPart.h"
#include "MOWResourceManager.h"

CMOWBoundingBox::CMOWBoundingBox()
{

}
//---------------------------------------------
CMOWBoundingBox::~CMOWBoundingBox()
{

}
//---------------------------------------------
CMOWBoundingBox* CMOWBoundingBox::Create( 
    float width, 
    float height, 
    float depth 
    )
{
    CMOWBoundingBox* pThis = new CMOWBoundingBox;
    CMOWModelPart* part = pThis->CreateAndAddModelPart("Box",D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pThis->Width(width);
    pThis->Height(height);
    pThis->Depth(depth);
    pThis->Name("BoundingBox");
    part->MaterialName("");
    pThis->PopulateVerticesAndIndices();
    return pThis;
}
//---------------------------------------------
const CMOWModelPart* CMOWBoundingBox::ModelPart()
{
    return CMOWModel::ModelPart("Box");
}
//---------------------------------------------
void CMOWBoundingBox::UpdateBoundingVolumes()
{
    //Create new normals
}
//---------------------------------------------
bool CMOWBoundingBox::IsInside( 
    const CMOWBoundingBox& box 
    ) const
{

    
    float myMinX = Position().x - (Width() / 2.0f);
    float myMaxX = Position().x + (Width() / 2.0f);
    
    float myMinY = Position().y - (Height() / 2.0f);
    float myMaxY = Position().y + (Height() / 2.0f);

    float myMinZ = Position().z - (Depth() / 2.0f);
    float myMaxZ = Position().z + (Depth() / 2.0f);

    bool retVal = box.Position().x >= myMinX && box.Position().x <= myMaxX;

    retVal = retVal ? box.Position().y >= myMinY && box.Position().y <= myMaxY : retVal;
    retVal = retVal ? box.Position().z >= myMinZ && box.Position().z <= myMaxZ : retVal;

    return retVal;
}
//---------------------------------------
void CMOWBoundingBox::Render(
    ID3D11DeviceContext* context, 
    const DirectX::XMFLOAT4X4& viewMatrix, 
    const DirectX::XMFLOAT4X4& projectionMatrix, 
    const DirectX::XMFLOAT3& cameraPosition, 
    int screenWidth, 
    int screenHeight
    )const
{
    DirectX::XMFLOAT4X4 lightViewMatrix;
    DirectX::XMFLOAT4X4 lightProjectionMatrix;
    CMOWShader::LightBufferDefinition lightBuff;

    CMOWShader* shader = CMOWResourceManager::Instance()->GetShader("ColorPixelShader");
    ID3D11RasterizerState* wireFrame = CMOWResourceManager::Instance()->GetWireframeState();
    ID3D11RasterizerState* oldState = 0;
    
    context->RSGetState(&oldState);
    context->RSSetState(wireFrame);

    shader->Update(context,
        WorldMatrix(),
        viewMatrix,
        projectionMatrix,
        lightViewMatrix,
        lightProjectionMatrix,
        lightBuff,
        cameraPosition,
        screenWidth,
        screenHeight,
        FaceResources(),
        true
        );

    CMOWModel::ModelPart("Box")->Render(context);

    context->RSSetState(oldState);
    
}
//---------------------------------------
bool CMOWBoundingBox::Initialize(ID3D11Device* device)
{
    bool retVal = true;

    parts_map::iterator itParts = m_parts.begin();

    while( itParts != m_parts.end() )
    {
        itParts->second->CreateResources(device);
        itParts++;
    }
    FaceResources();

    return retVal;
}
//------------------------------------------------------
CMOWBoundingBox* CMOWBoundingBox::FromPb(
    const PbMOWGraphics::PbMOWBoundingBox& fromPb
    )
{
    CMOWBoundingBox* retVal = new CMOWBoundingBox;

    retVal->Name(fromPb.name().c_str());

    retVal->Width(fromPb.widthheightdepth().x());
    retVal->Height(fromPb.widthheightdepth().y());
    retVal->Depth(fromPb.widthheightdepth().z());

    CMOWModelPart* part = CMOWModelPart::FromPb(fromPb.part());

    retVal->AddModelPart(part);

    return retVal;
}
//------------------------------------------------------
void CMOWBoundingBox::ToPb(
    PbMOWGraphics::PbMOWBoundingBox& toPb
    ) const
{
    toPb.set_name(Name());
    toPb.mutable_widthheightdepth()->set_x(Width());
    toPb.mutable_widthheightdepth()->set_y(Height());
    toPb.mutable_widthheightdepth()->set_z(Depth());
    CMOWModel::ModelPart("Box")->ToPb(*toPb.mutable_part());
}
//---------------------------------------------


