#ifndef IDXPhysicalEntity_H
#define IDXPhysicalEntity_H

#include "MOWCommon/MOWVector.h"
#include <iosfwd>

class IMOWPhysicalEntity
{
public:

    enum PHYSICAL_ENTITY_MOTION_TYPE
    {
        PEMT_NONE,
        PEMT_FIXED,
        PEMT_CHARACTER,
        PEMT_LAST

    };

    enum PHYSICAL_ENTITY_SHAPE
    {
        PES_NONE,
        PES_BOX,
        PES_SPHERE,
        PES_LAST

    };


    virtual void
        Clear(
            ) = 0;

    virtual void
        SerializeSave( 
            std::ofstream& fOut 
            )const = 0;


    virtual void
        LinearVelocity(
            const CMOWVector& velocity
            ) = 0;

    virtual CMOWVector
        LinearVelocity(
            )const = 0;

    virtual void
        Position(
            const CMOWVector& position
            ) = 0;

    virtual CMOWVector
        Position(
            )const = 0;

    virtual void
        Rotation(
            const CMOWVector& rotation
            ) = 0;

    virtual CMOWVector
        Rotation(
            )const = 0;

    virtual void
        AngularVelocity(
            const CMOWVector& velocity
            ) = 0;

    virtual CMOWVector
        AngularVelocity(
            )const = 0;

    virtual void
        AngularDamping(
            float damping
            ) = 0;

    virtual float
        AngularDamping(
            )const = 0;

    virtual void
        Mass(
            float mass
            ) = 0;

    virtual float
        Mass(
            )const = 0;

    virtual void
        MotionType(
            PHYSICAL_ENTITY_MOTION_TYPE motionType
            ) = 0;

    virtual PHYSICAL_ENTITY_MOTION_TYPE
        MotionType(
            )const = 0;

    virtual void
        AffectedByGravity(
            bool affected
            ) = 0;

    virtual bool
        AffectedByGravity(
            ) const = 0;


};

#endif