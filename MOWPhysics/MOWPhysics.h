#ifndef CMOWPhysics_H
#define CMOWPhysics_H

#include "MOWCommon/MOWTimer.h"
#include <string>
#include "MOWCommon/MOWTransform.h"
#include "MOWCommon/MOWSharedPtr.h"

class IMOWPhysicalEntity;
class IMOWPhysicsWorld;

DECLARE_SHARED_PTR(CMOWModel)

class CMOWPhysics
{
public:
    CMOWPhysics(
        IMOWPhysicsWorld* world
        );

    ~CMOWPhysics();

    void        
        Initialize(
            );

    void        
        AddModel(
            CMOWModelPtr model,
            bool fixedObject = false,
            bool character = false
            );

    void        
        RemoveModel(
            CMOWModelPtr model
            );

    void        
        Update(
            );

    void        
        Gravity(
            float x,
            float y,
            float z
            );

    void        
        Unlock(
            );

    IMOWPhysicalEntity*
        CreatePhysicalBox(
            bool collidable,
            bool fixed,
            float width,
            float height,
            float depth,
            float mass
            ) const;

    IMOWPhysicalEntity*
        CreatePhysicalSphere(
            bool collidable,
            bool fixed,
            float radius,
            float mass
            )const;

    IMOWPhysicalEntity*
        CreatePhysicalEntityFromModel(
            CMOWModelPtrC model,
            bool fixed,
            bool collidable,
            const CMOWTransform& transform = CMOWTransform()
            ) const;

    IMOWPhysicalEntity*
        LoadEntity(
            const std::string& data
            )const;

    std::string
        SaveEntity(
            IMOWPhysicalEntity* entity
            )const;

private:

    IMOWPhysicsWorld*       
        m_world;

    CMOWTimer
        m_timer;
    
};

#endif