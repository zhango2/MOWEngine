#ifndef DXCommon_H
#define DXCommon_H

#include <vector>
#include <memory.h>
#include "D3D11.h"
#include "DirectXMath.h"

#ifndef SCREEN_DEPTH
    #define SCREEN_DEPTH 500.0f
#endif
#ifndef SCREEN_NEAR
    #define SCREEN_NEAR 1.0f
#endif

#ifndef SHADOWMAP_DEPTH
    #define SHADOWMAP_DEPTH 500.0f
#endif
#ifndef SHADOWMAP_NEAR
    #define SHADOWMAP_NEAR 1.0f
#endif
#ifndef DIRECTINPUT_VERSION
    #define DIRECTINPUT_VERSION 0x8000
#endif

struct Vertex
{
    Vertex()
    {
        DirectX::XMStoreFloat4(&m_color,DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f));
    }

    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT2 m_texture;
    DirectX::XMFLOAT3 m_normal;
    DirectX::XMFLOAT3 m_tangent;
    DirectX::XMFLOAT3 m_biTangent;
    DirectX::XMFLOAT4 m_color;
};

struct ShaderMaterial
{
    ShaderMaterial()
    {
        DirectX::XMStoreFloat4(&m_ambient,DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f));
        DirectX::XMStoreFloat4(&m_diffuse,DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f));
        DirectX::XMStoreFloat4(&m_specular,DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f));
        DirectX::XMStoreFloat4(&m_reflection,DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f));
    }

    DirectX::XMFLOAT4 m_ambient;
    DirectX::XMFLOAT4 m_diffuse;
    DirectX::XMFLOAT4 m_specular; //specular power in w
    DirectX::XMFLOAT4 m_reflection;
};

enum ShaderLightType
{
    SHADER_LIGHT_TYPE_NONE,
    SHADER_LIGHT_TYPE_DIRECTIONAL,
    SHADER_LIGHT_TYPE_POINT,
    SHADER_LIGHT_TYPE_SPOT,
    SHADER_LIGHT_TYPE_LAST
};

struct ShaderLight
{
    ShaderLight()
    {
        DirectX::XMStoreFloat4(&m_ambient,DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f));
        DirectX::XMStoreFloat4(&m_diffuse,DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f));
        DirectX::XMStoreFloat4(&m_specular,DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f));
        DirectX::XMStoreFloat3(&m_position,DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f));
        m_range = 0.0f;
        DirectX::XMStoreFloat3(&m_direction,DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f));
        m_spot = 0.0f;
        DirectX::XMStoreFloat3(&m_attenuation,DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f));
        m_lightType = SHADER_LIGHT_TYPE_NONE;
    }

    DirectX::XMFLOAT4 m_ambient;
    DirectX::XMFLOAT4 m_diffuse;
    DirectX::XMFLOAT4 m_specular;
    DirectX::XMFLOAT3 m_position;
    float m_range;
    DirectX::XMFLOAT3 m_direction;
    float m_spot;
    DirectX::XMFLOAT3 m_attenuation;
    ShaderLightType m_lightType;


};

static bool IsPositionEqual(
    const Vertex& vertex1,
    const Vertex& vertex2
    )
{
    return vertex1.m_position.x == vertex2.m_position.x &&
           vertex1.m_position.y == vertex2.m_position.y &&
           vertex1.m_position.z == vertex2.m_position.z;
}

typedef std::vector<Vertex> vertex_vector;
typedef std::vector<long> index_vector;

#endif