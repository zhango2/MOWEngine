#ifndef CMOWSerializer_H
#define CMOWSerializer_H

#include <string>
#include <iostream>
#include <fstream>
#include "D3D11.h"
#include "DirectXMath.h"

class CMOWSerializer
{
public:

    static void                 WriteString(
                                    std::ofstream& fOut,
                                    const std::string& str
                                    );

    static void                 WriteSizeT(
                                    std::ofstream& fOut,
                                    size_t val 
                                    );

    static void                 WriteBlob(
                                    std::ofstream& fOut,
                                    void* blob,
                                    size_t size
                                    );

    static void                 WriteVector3(
                                    std::ofstream& fOut,
                                    DirectX::XMFLOAT3& vector
                                    );

    static void                 WriteVector4(
                                    std::ofstream& fOut,
                                    DirectX::XMFLOAT4& vector
                                    );

    static void                 WriteMatrix(
                                    std::ofstream& fOut,
                                    DirectX::XMFLOAT4X4& matrix
                                    );

    static std::string          ReadString( std::ifstream& fIn );

    static size_t               ReadSizeT( std::ifstream& fIn );

    static void                 ReadBlob( 
                                    std::ifstream& fIn,
                                    void* blob,
                                    size_t size
                                    );

    static DirectX::XMFLOAT3    ReadVector3(std::ifstream& fIn);

    static DirectX::XMFLOAT4    ReadVector4(std::ifstream& fIn);

    static DirectX::XMFLOAT4X4  ReadMatrix(std::ifstream& fIn);
                                    
                                    
    
};

#endif