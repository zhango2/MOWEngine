#include "MOWColorShader.h"
//---------------------------------------------
CMOWColorShader::CMOWColorShader()
{
   m_vertexShaderDescription = "ColorVertexShader";
   m_pixelShaderDescription = "ColorPixelShader";
   m_vertexShaderFilename = "ColorVertexShader.cso";
   m_pixelShaderFilename = "ColorPixelShader.cso";
   m_fxFileName = "Color.fxo";
   m_fxTechName = "ColorTech";
}
//---------------------------------------------
CMOWColorShader::~CMOWColorShader()
{

}
//---------------------------------------------
/*void CDXColorShader::SetupElementDescriptions( std::vector<D3D11_INPUT_ELEMENT_DESC>& descriptions )
{
    
    D3D11_INPUT_ELEMENT_DESC desc1;
    D3D11_INPUT_ELEMENT_DESC desc2;
    // This setup needs to match the VertexType structure in the ModelClass and in the shader.
    desc1.SemanticName = "POSITION";
    desc1.SemanticIndex = 0;
    desc1.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    desc1.InputSlot = 0;
    desc1.AlignedByteOffset = 0;
    desc1.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc1.InstanceDataStepRate = 0;

    desc2.SemanticName = "COLOR";
    desc2.SemanticIndex = 0;
    desc2.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc2.InputSlot = 0;
    desc2.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    desc2.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc2.InstanceDataStepRate = 0;

    descriptions.push_back(desc1);
    descriptions.push_back(desc2);
}*/


