#ifndef _FBOSURFACE_H_
#define _FBOSURFACE_H_

#include "IRenderSurface.h"

namespace X
{
    /* Class:       FBOSurface      
     *
     */
    class FBOSurface : public IRenderSurface
    {
    public:
        FBOSurface();
        virtual ~FBOSurface();
        bool BindTarget();
        bool UnbindTarget();
        bool BindTexture();
        bool UnbindTexture();
        bool Init(unsigned int, unsigned int);

    protected:
        unsigned int m_fbo,     // the frame buffer object
            m_texture;          // the texture

    };
};

#endif