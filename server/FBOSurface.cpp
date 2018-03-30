#include "FBOSurface.h"
#include <iostream>

using namespace std;

namespace X
{
    /*
     *
     */
    FBOSurface::FBOSurface()
    {
        m_fbo = m_texture = 0;
    }

    /*
     *
     */
    FBOSurface::~FBOSurface()
    {
        if(m_fbo)
            glDeleteFramebuffersEXT(1, &m_fbo);
        if(m_texture)
            glDeleteTextures(1, &m_texture);
    }

    /* Function:        FBOSurface::Init
     * Description:     Initalizes the FBO
     */
    bool FBOSurface::Init(unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;
        // Create the FBO
        
        glEnable(GL_TEXTURE_2D);
        // Create the texture
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        // bind the texture as a render target to the FBO
        if(!glFramebufferTexture2DEXT) {
            return false;
        }

        
        glGenFramebuffersEXT(1, &m_fbo);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

        GLuint depthbuffer;
        glGenRenderbuffersEXT(1, &depthbuffer);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);

        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);
    
        
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture, 0);
        GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        switch(status)
        {
            case GL_FRAMEBUFFER_COMPLETE_EXT:
#if defined(_DEBUG_VERBOSE)
                cerr << "FBOSurface::Init: The Frame Buffer Object was created successfully" << endl;
#endif
                return true;
            case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
                cerr << "FBOSurface::Init: The requested FBO format is not supported" << endl;
                return false;
            default:
                cerr << "FBOSurface::Init: Unknown error, could not initalize the FBO" << endl;
                return false;
        }
    }

    /*
     *
     */
    bool FBOSurface::BindTarget()
    {
        if(!m_fbo || !m_texture) {
            cerr << "FBOSurface::BindTarget: Can't bind the FBO, invalid fbo/texture value" << endl;
            return false;
        }
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
        glViewport(0, 0, m_width, m_height);
        return true;
    }

    /*
     *
     */
    bool FBOSurface::UnbindTarget()
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        return true;
    }

    /*
     *
     */
    bool FBOSurface::BindTexture()
    {
        if(!m_texture) {
            cerr <<"FBOSurface::BindTexture: Invalid texture" << endl;
            return false;
        }
        glBindTexture(GL_TEXTURE_2D, m_texture);
        return true;
    }

    /*
     *
     */
    bool FBOSurface::UnbindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }
};