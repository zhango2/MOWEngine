#ifndef IDXPhysicsWorld_H
#define IDXPhysicsWorld_H

#include "IMOWPhysicalEntity.h"
#include "MOWCommon/MOWVector.h"
#include <string>

class IMOWPhysicsWorld
{
public:

    virtual void
        Initialize(
            ) = 0;

    virtual void
        Gravity(
            const CMOWVector& gravity
            ) = 0;

    virtual CMOWVector
        Gravity(
            )const = 0;

    virtual void
        AddEntity(
            IMOWPhysicalEntity* entity
            ) = 0;

    virtual void
        RemoveEntity(
            IMOWPhysicalEntity* entity
            ) = 0;

    virtual void
        Step(
            float time
            ) = 0;

    virtual IMOWPhysicalEntity*
        ConstructBox(
            bool collidable,
            bool fixed,
            float width,
            float height,
            float depth,
            float mass
            ) const = 0;

    virtual IMOWPhysicalEntity*
        ConstructSphere(
            bool collidable,
            bool fixed,
            float radius,
            float mass
            ) const = 0;

    virtual std::string
        SaveEntity(
            IMOWPhysicalEntity* entity
            ) const = 0;

    virtual IMOWPhysicalEntity*
        LoadEntity(
            const std::string& data
            )const = 0;

};

#endif