#include "MOWCamera.h"

using namespace DirectX;

CMOWCamera::CMOWCamera(
    float fieldOfView,
    float aspectRatio,
    float nearPlane,
    float farPlane
    ):
    BaseClass(fieldOfView, aspectRatio, nearPlane, farPlane)
{
    m_oldMouseX = 0;
    m_oldMouseY = 0;
    m_mouseXDelta = 0;
    m_mouseYDelta = 0;
    m_xVelPart = -1.0f;
    m_yVelPart = -1.0f;
    m_zVelPart = -1.0f;
    m_velFactor = 5.0f;
}
//---------------------------------------------
CMOWCamera::~CMOWCamera()
{

}
//---------------------------------------------
CMOWRenderableObject* CMOWCamera::RenderableObject()
{
    return this;
}
//---------------------------------------------
CMOWCameraPtr CMOWCamera::Create(
    const CMOWPhysics& physics,
    float fieldOfView,
    float aspectRatio,
    float nearPlane,
    float farPlane
    )
{
    CMOWCameraPtr cam =CMOWCameraPtr( new CMOWCamera(
        fieldOfView,
        aspectRatio,
        nearPlane,
        farPlane))
    ;
    cam->Init(physics);

    return cam;
}
//---------------------------------------------
CMOWModelPtr CMOWCamera::MutableModel()
{
    return shared_from_this();
}
//---------------------------------------------
CMOWModelPtrC CMOWCamera::Model() const
{
    return shared_from_this();
}
//---------------------------------------------
void CMOWCamera::Update()
{
    
    CMOWSphere::Update();
        
}
//---------------------------------------------
void CMOWCamera::MouseUpdated( 
    int x,
    int y 
    )
{
    m_mouseXDelta += x;
    m_mouseYDelta += y;

    if( x || y )
    {
        float yRad = XMConvertToRadians((float)m_mouseXDelta*0.05f);

        m_xVelPart = sin(yRad);
        m_zVelPart = cos(yRad);
    }
    
   //Mouse x is rotation on the y axis and mouse y is rotation on the x axis
    Rotation(m_mouseYDelta*-0.05f,m_mouseXDelta*0.05f,0.0f);
   //GeometryObject()->SetAngularVelocity((float)m_mouseYDelta*-0.05f,(float)m_mouseXDelta*0.05f,0.0f);
}
//---------------------------------------------
void CMOWCamera::KeyboardUpdated( 
    const std::vector<char>& pressedKeys 
    )
{
    float zVelPart = m_zVelPart;
    float xVelPart = m_xVelPart;
    

    
    xVelPart = m_mouseXDelta == 0 ? 0.0f : m_xVelPart;
    zVelPart = m_mouseXDelta == 0 ? 1.0f : m_zVelPart;

    if( (pressedKeys[DIK_W] & 0x80) || (pressedKeys[DIK_S] & 0x80)  )
    {
        

        if( pressedKeys[DIK_W] & 0x80 )
        {
            LinearVelocity(xVelPart*m_velFactor,0.0f,zVelPart*m_velFactor);
        }
        else
        {
            LinearVelocity(-1.0f*xVelPart*m_velFactor,0.0f,-1.0f*zVelPart*m_velFactor);
        }
    }
    else if( (pressedKeys[DIK_D] & 0x80) || (pressedKeys[DIK_A] & 0x80) )
    {
        

        if( pressedKeys[DIK_D] & 0x80 )
        {
            LinearVelocity(zVelPart*m_velFactor,0.0f,xVelPart*m_velFactor*-1.0f);
        }
        else if( pressedKeys[DIK_A] & 0x80)
        {
            LinearVelocity(-1.0f*zVelPart*m_velFactor,0.0f,xVelPart*m_velFactor);
        }
    }
    else if( (pressedKeys[DIK_Q] & 0x80) || (pressedKeys[DIK_Z] & 0x80) )
    {
        if( pressedKeys[DIK_Q] & 0x80 )
        {
            LinearVelocity(0.0f,5.0f,0.0f);
        }
        else
        {
            LinearVelocity(0.0f,-5.0f,0.0f);
        }
    }
    else
    {
        LinearVelocity(0.0f,0.0f,0.0f);
    }

}
//---------------------------------------
void CMOWCamera::Init(
    const CMOWPhysics& physics
    )
{
	SphereConstruct(1.3f,0,0,physics,false,5,500.0f);
}
//------------------------------------------------------
const CMOWObject* CMOWCamera::GeometryObject() const
{
    return this;
}
//------------------------------------------------------
CMOWObject* CMOWCamera::MutableGeometryObject()
{
    return this;
}
//------------------------------------------------------
CMOWCameraPtr CMOWCamera::FromPb(
    const PbMOWGraphics::PbMOWModel& fromPb, 
    const CMOWPhysics& physics
    )
{
    return nullptr;
    /*CMOWCameraPtr retVal = new CMOWCamera;
    CMOWModel* base = reinterpret_cast<CMOWModel*>(retVal);
    base->FromPb(fromPb,physics);
    return retVal;*/
}