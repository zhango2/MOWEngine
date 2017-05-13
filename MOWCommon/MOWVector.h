#ifndef CMOWVector_H
#define CMOWVector_H

class CMOWVector
{
public:
    CMOWVector(
        );

    CMOWVector( 
        float x,
        float y,
        float z,
        float w = 1.0f
        );

    float                   
        X(
            )const;
    void                    
        X(
            float x
            );

    float                   
        Y(
            )const;
    void                    
        Y(
        float y
            );

    float                   
        Z(
            )const;
    void                    
        Z(
            float z
            );

    float                   
        W(
            )const;
    void                    
        W(
            float w
            );

private:

    float                   
        m_x;

    float                   
        m_y;

    float                   
        m_z;

    float                   
        m_w;
};

#endif