#include "MOWPhysics.h"
#include "MOWGraphics/MOWModel.h"
#include "IMOWPhysicsWorld.h"

static DWORD s_dwTimeStart = 0;
//---------------------------------------------
CMOWPhysics::CMOWPhysics(
    IMOWPhysicsWorld* world
    )
{
    m_world = world;
}
//---------------------------------------------
CMOWPhysics::~CMOWPhysics()
{
    
}
//---------------------------------------------
void CMOWPhysics::Initialize()
{
    m_world->Initialize();
}
//---------------------------------------------
void CMOWPhysics::AddModel( 
    CMOWModel* model,
    bool fixedObject,
    bool character
    )
{
    IMOWPhysicalEntity* entity = model->PhysicalBody();
    if( entity )
    {
        if( fixedObject )
        {
            entity->MotionType(IMOWPhysicalEntity::PEMT_FIXED);
        }
        else if( character )
        {
            entity->MotionType(IMOWPhysicalEntity::PEMT_CHARACTER);
        }
        m_world->AddEntity(entity);
    }
    
    
}
//---------------------------------------------
void CMOWPhysics::Update()
{
    m_timer.Stop();
    m_world->Step(static_cast<float>(m_timer.ElapsedTimeSeconds()));
    m_timer.Start();
}
//---------------------------------------------
void CMOWPhysics::Gravity( 
    float x,
    float y,
    float z 
    )
{
    m_world->Gravity(CMOWVector(x,y,z));
}
//---------------------------------------------
void CMOWPhysics::RemoveModel( 
    CMOWModel* model 
    )
{
    m_world->RemoveEntity(model->PhysicalBody());
}
//---------------------------------------------
void CMOWPhysics::Unlock()
{
    //m_world->unlock();
}
//------------------------------------------------------
IMOWPhysicalEntity* CMOWPhysics::CreatePhysicalBox(
    bool collidable,
    bool fixed,
    float width, 
    float height, 
    float depth, 
    float mass
    ) const
{
    return m_world->ConstructBox(collidable, fixed, width, height, depth, mass);
}
//------------------------------------------------------
IMOWPhysicalEntity* CMOWPhysics::CreatePhysicalSphere(
    bool collidable,
    bool fixed,
    float radius, 
    float mass
    )const
{
    return m_world->ConstructSphere(collidable, fixed, radius, mass);
}
//------------------------------------------------------
IMOWPhysicalEntity* CMOWPhysics::CreatePhysicalEntityFromModel(
    const CMOWModel* model,
    bool fixed,
    bool collidable,
    const CMOWTransform& rotationQuaternion
    ) const
{

    return m_world->ConstructPhysicalEntityFromModel(model, fixed, collidable, rotationQuaternion);
}
//------------------------------------------------------
IMOWPhysicalEntity* CMOWPhysics::LoadEntity(
    const std::string& data
    )const
{
    return m_world->LoadEntity(data);
}
//------------------------------------------------------
std::string CMOWPhysics::SaveEntity(
    IMOWPhysicalEntity* entity
    ) const
{
    return m_world->SaveEntity(entity);
}