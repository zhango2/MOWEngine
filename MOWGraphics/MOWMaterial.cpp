#include "MOWMaterial.h"
#include "MOWCommon/MOWVector.h"

using namespace DirectX;
//---------------------------------------------
CMOWMaterial::CMOWMaterial(
    const char* name,
    const char* texture, 
    XMFLOAT4 Ka, 
    XMFLOAT4 Kd, 
    XMFLOAT4 Ks, 
    XMFLOAT4 Ke,
    float specularPower
    )
{
    m_name = name;
    m_texture = texture;
    m_Ka = Ka;
    m_Kd = Kd;
    m_Ks = Ks;
    m_Ke = Ke;
    m_specularPower = specularPower;
}
//---------------------------------------------
CMOWMaterial::CMOWMaterial()
{
    m_specularPower = 0.0f;
}
//---------------------------------------------
CMOWMaterial::~CMOWMaterial()
{

}
//---------------------------------------------
const XMFLOAT4& CMOWMaterial::Ka() const
{
    return m_Ka;
}
//---------------------------------------------
void CMOWMaterial::Ka( 
    XMFLOAT4 Ka 
    )
{
    m_Ka = Ka;
}

//---------------------------------------------
const XMFLOAT4& CMOWMaterial::Kd() const
{
    return m_Kd;
}
//---------------------------------------------
void CMOWMaterial::Kd( 
    XMFLOAT4 Kd 
    )
{
    m_Kd = Kd;
}

//---------------------------------------------
const XMFLOAT4& CMOWMaterial::Ks() const
{
    return m_Ks;
}
//---------------------------------------------
void CMOWMaterial::Ks( 
    XMFLOAT4 Ks 
    )
{
    m_Ks = Ks;
}

//---------------------------------------------
const XMFLOAT4& CMOWMaterial::Ke() const
{
    return m_Ke;
}
//---------------------------------------------
void CMOWMaterial::Ke( 
    XMFLOAT4 Ke 
    )
{
    m_Ke = Ke;
}

//---------------------------------------------
const char* CMOWMaterial::TextureFileName() const
{
    return m_texture.c_str();
}
//---------------------------------------------
void CMOWMaterial::TextureFileName( 
    const char* textureFileName 
    )
{
    m_texture = textureFileName;
}

//---------------------------------------------
const char* CMOWMaterial::Name() const
{
    return m_name.c_str();
}
//---------------------------------------------
void CMOWMaterial::Name( 
    const char* name 
    )
{
    m_name = name;
}

//---------------------------------------------
float CMOWMaterial::SpecularPower() const
{
    return m_specularPower;
}
//---------------------------------------------
void CMOWMaterial::SpecularPower( 
    float specularPower 
    )
{
    m_specularPower = specularPower;
}

//---------------------------------------------
bool CMOWMaterial::Serialize( 
    std::ofstream& fOut 
    )
{
    size_t nameLength = m_name.length();
    fOut.write(reinterpret_cast<const char*>(&nameLength),sizeof(size_t));
    fOut.write(reinterpret_cast<const char*>(m_name.c_str()),sizeof(char)*nameLength);

    size_t textureLength = m_texture.length();
    fOut.write(reinterpret_cast<const char*>(&textureLength),sizeof(size_t));
    fOut.write(reinterpret_cast<const char*>(m_texture.c_str()),sizeof(char)*textureLength);

    fOut.write(reinterpret_cast<const char*>(&m_Ka),sizeof(XMFLOAT4));
    fOut.write(reinterpret_cast<const char*>(&m_Kd),sizeof(XMFLOAT4));
    fOut.write(reinterpret_cast<const char*>(&m_Ks),sizeof(XMFLOAT4));
    fOut.write(reinterpret_cast<const char*>(&m_Ke),sizeof(XMFLOAT4));
    fOut.write(reinterpret_cast<const char*>(&m_specularPower),sizeof(float));

    return true;

    
}
//---------------------------------------------
bool CMOWMaterial::Serialize( 
    std::ifstream& fIn 
    )
{
    size_t nameLength;
    fIn.read(reinterpret_cast<char*>(&nameLength),sizeof(size_t));

    m_name.resize(nameLength);

    fIn.read(reinterpret_cast<char*>(&m_name[0]),sizeof(char)*nameLength);

    size_t textureLength;
    fIn.read(reinterpret_cast<char*>(&textureLength),sizeof(size_t));

    m_texture.resize(textureLength);

    fIn.read(reinterpret_cast<char*>(&m_texture[0]),sizeof(char)*textureLength);

    fIn.read(reinterpret_cast<char*>(&m_Ka),sizeof(XMFLOAT4));
    fIn.read(reinterpret_cast<char*>(&m_Kd),sizeof(XMFLOAT4));
    fIn.read(reinterpret_cast<char*>(&m_Ks),sizeof(XMFLOAT4));
    fIn.read(reinterpret_cast<char*>(&m_Ke),sizeof(XMFLOAT4));
    fIn.read(reinterpret_cast<char*>(&m_specularPower),sizeof(float));

    return true;
}
//---------------------------------------
ShaderMaterial CMOWMaterial::AsShaderMaterial() const
{
    ShaderMaterial retVal;

    retVal.m_ambient = m_Ka;
    retVal.m_diffuse = m_Kd;
    retVal.m_specular = m_Ks;
    retVal.m_specular.w = m_specularPower/255.0f;

    return retVal;
}
//------------------------------------------------------
std::string CMOWMaterial::ClassName()
{
    return "CDXMaterial";
}
//------------------------------------------------------
void CMOWMaterial::InitializeMOWClass()
{
    std::vector<std::string> names;

    names.push_back("Name");
    CMOWClass::RegisterMOWClass("CDXMaterial");
    //CDXClass::RegisterAttribute("CDXMaterial","Name","Name",DXAttributeType_String,names);
    
    names.clear();
    names.push_back("Red");
    names.push_back("Green");
    names.push_back("Blue");
    /*CDXClass::RegisterAttribute("CDXMaterial","Ka","Ambient Color",DXAttributeType_Vector);
    CDXClass::RegisterAttribute("CDXMaterial","Kd","Diffuse Color",DXAttributeType_Vector);*/
    //CDXClass::RegisterAttribute("CDXMaterial","Ks","Specular Color",DXAttributeType_Vector,names);
    //CDXClass::RegisterAttribute("CDXMaterial","Ke","Emissive Color",DXAttributeType_Vector);

    names.clear();
    names.push_back("Specular Power");
    //CDXClass::RegisterAttribute("CDXMaterial","SpecularPower","Specular Power",DXAttributeType_Float,names);

    names.clear();
    names.push_back("Texture Filename");

    //CDXClass::RegisterAttribute("CDXMaterial","TextureFileName","Texture Filename",DXAttributeType_String,names);
    
}
//------------------------------------------------------
CMOWVariant* CMOWMaterial::DoGetAttributeValue(
    const char* attrName
    )
{
    CMOWVariant* retVariant = 0;
    std::string attrString = attrName;

    if( "Name" == attrString )
    {
        std::string* retVal = new std::string(Name());
        retVariant = new CMOWVariant(retVal);
    }
    else if( "Ka" == attrString )
    {
        CMOWVector* color = new CMOWVector(Ka().x,Ka().y,Ka().z,Ka().w);
        retVariant = new CMOWVariant(color);
    }
    else if( "Kd" == attrString )
    {
        CMOWVector* color = new CMOWVector(Kd().x,Kd().y,Kd().z,Kd().w);
        retVariant = new CMOWVariant(color);
    }
    else if( "Ks" == attrString )
    {
        CMOWVector* color = new CMOWVector(Ks().x,Ks().y,Ks().z,Ks().w);
        retVariant = new CMOWVariant(color);
    }
    else if( "SpecularPower" == attrName )
    {
        retVariant = new CMOWVariant(SpecularPower());
    }
    else if( "TextureFileName" == attrName )
    {
        std::string* retVal = new std::string(TextureFileName());
        retVariant = new CMOWVariant(retVal);
    }

    return retVariant;
}
//------------------------------------------------------
void CMOWMaterial::DoSetAttributeValue(
    const char* attrName,
    CMOWVariant* val
    )
{
    std::string name = attrName;
    
    if( name == "Ka" )
    {
        Ka(XMFLOAT4(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W()));
    }
    else if( name == "Kd" )
    {
        Kd(XMFLOAT4(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W()));
    }
    else if( name == "Ks" )
    {
        Ks(XMFLOAT4(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W()));
    }
    else if( name == "Ke" )
    {
        Ke(XMFLOAT4(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W()));
    }
    else if( name == "SpecularPower" )
    {
        SpecularPower(val->AsFloat());
    }
}

