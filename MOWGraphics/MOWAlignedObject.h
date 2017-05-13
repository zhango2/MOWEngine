#pragma once

class CMOWAlignedObject
{
public:


    void* operator 
        new(
            size_t count
            );

    void operator
        delete(
            void* data
            );


};
