#ifndef CMOWTransform_H
#define CMOWTransform_H

#include "MOWVector.h"

class CMOWTransform
{
public:

    CMOWTransform();

    CMOWTransform(
        const CMOWVector& position,
        const CMOWVector& rotationQuaternion
        );

    const CMOWVector&
        Position(
            )const;

    const CMOWVector&
        RotationQuaternion(
            )const;

private:

    CMOWVector
        m_position;

    CMOWVector
        m_rotationQuaternion;

};

#endif
