#ifndef CMOWPhysics_H
#define CMOWPhysics_H

#include "MOWCommon/MOWTimer.h"
#include <string>

class CMOWModel;
class IMOWPhysicalEntity;
class IMOWPhysicsWorld;


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
            CMOWModel* model,
            bool fixedObject = false,
            bool character = false
            );

    void        
        RemoveModel(
            CMOWModel* model
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