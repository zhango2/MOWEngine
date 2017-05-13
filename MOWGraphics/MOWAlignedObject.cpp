#include "MOWAlignedObject.h"
#include <corecrt_malloc.h>

//------------------------------------------------------
void* CMOWAlignedObject::operator new(
    size_t count
    )
{
    return _aligned_malloc(count,16);
}
//------------------------------------------------------
void CMOWAlignedObject::operator delete(
    void* data
    )
{
    _aligned_free(data);
}