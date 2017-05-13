#ifndef CDXDepthShader_H
#define CDXDepthShader_H

#include "MOWShader.h"

class CDXDepthShader : public CMOWShader
{
    typedef CMOWShader BaseClass;

public:

    CDXDepthShader(
        );

    virtual ~CDXDepthShader(
                );


protected:

    virtual bool            
        UseResources(
            ) const;

};

#endif