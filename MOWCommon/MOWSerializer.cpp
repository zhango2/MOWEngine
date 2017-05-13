#include "MOWSerializer.h"

//------------------------------------------------------
void CMOWSerializer::WriteString(
    std::ofstream& fOut, 
    const std::string& str
    )
{
    
    WriteSizeT(fOut,str.length());
    
    const char* actualString = str.c_str();

    WriteBlob(fOut,(void*)(actualString),sizeof(char)*str.length());
}
//------------------------------------------------------
void CMOWSerializer::WriteSizeT(
    std::ofstream& fOut, 
    size_t val
    )
{
    WriteBlob(fOut,reinterpret_cast<void*>(&val),sizeof(size_t));
}
//------------------------------------------------------
void CMOWSerializer::WriteBlob(
    std::ofstream& fOut,
    void* blob, 
    size_t size
    )
{
    fOut.write(reinterpret_cast<const char*>(blob),size);
}
//------------------------------------------------------
void CMOWSerializer::WriteVector3(
    std::ofstream& fOut,
    DirectX::XMFLOAT3& vector
    )
{
    WriteBlob(fOut,&vector,sizeof(DirectX::XMFLOAT3));
}
//------------------------------------------------------
void CMOWSerializer::WriteVector4(
    std::ofstream& fOut,
    DirectX::XMFLOAT4& vector
    )
{
    WriteBlob(fOut,&vector,sizeof(DirectX::XMFLOAT4));
}
//------------------------------------------------------
std::string CMOWSerializer::ReadString(std::ifstream& fIn)
{
    std::string retVal = "";
    size_t length = ReadSizeT(fIn);

    retVal.resize(length);
    fIn.read(&retVal[0],length);

    return retVal;
}
//------------------------------------------------------
size_t CMOWSerializer::ReadSizeT(std::ifstream& fIn)
{
    size_t retVal = 0;
    fIn.read(reinterpret_cast<char*>(&retVal),sizeof(size_t));
    return retVal;
}
//------------------------------------------------------
void CMOWSerializer::ReadBlob(
    std::ifstream& fIn, 
    void* blob, 
    size_t size
    )
{
    fIn.read(reinterpret_cast<char*>(blob),size);
}
//------------------------------------------------------
DirectX::XMFLOAT3 CMOWSerializer::ReadVector3(std::ifstream& fIn)
{
    DirectX::XMFLOAT3 vector;
    ReadBlob(fIn,&vector,sizeof(DirectX::XMFLOAT3));

    return vector;
}
//------------------------------------------------------
DirectX::XMFLOAT4 CMOWSerializer::ReadVector4(std::ifstream& fIn)
{
    DirectX::XMFLOAT4 vector;
    ReadBlob(fIn,&vector,sizeof(DirectX::XMFLOAT4));

    return vector;
}
//------------------------------------------------------
void CMOWSerializer::WriteMatrix(
    std::ofstream& fOut, 
    DirectX::XMFLOAT4X4& matrix
    )
{
    WriteBlob(fOut,&matrix,sizeof(DirectX::XMFLOAT4X4));
}
//------------------------------------------------------
DirectX::XMFLOAT4X4 CMOWSerializer::ReadMatrix(std::ifstream& fIn)
{
    DirectX::XMFLOAT4X4 matrix;
    ReadBlob(fIn,&matrix,sizeof(DirectX::XMFLOAT4X4));

    return matrix;
}