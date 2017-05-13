#include "MOWDepthShader.h"

//---------------------------------------------
CDXDepthShader::CDXDepthShader()
{
    m_vertexShaderDescription = "DepthVertexShader";
    m_pixelShaderDescription = "DepthPixelShader";
    m_vertexShaderFilename = "DepthVertexShader.cso";
    m_pixelShaderFilename = "DepthPixelShader.cso";
}
//---------------------------------------------
CDXDepthShader::~CDXDepthShader()
{

}
//---------------------------------------------
bool CDXDepthShader::UseResources() const
{
    return false;
}

