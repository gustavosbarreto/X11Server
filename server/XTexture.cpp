#include "XTexture.h"
#include "tga.h"
#include <glew.h>
#include <gl/gl.h>
#include <wglew.h>
#include <GL/glaux.h>
#include <iostream>
#pragma comment(lib, "glaux.lib")

namespace X
{
    using namespace std;
    /*
     *
     */
    XTexture::XTexture()
    {
        m_id = 0;
    }

    /*
     *
     */
    XTexture::~XTexture()
    {
        if(m_id)
            glDeleteTextures(1, &m_id);
    }

    /* Function:        XTexture::LoadTexture
     * Description:     Loads the texture from a file.
     */
    bool XTexture::LoadTexture(const char * filename)
    {
        if(strstr(filename, ".tga"))
        {
            tga_file tga;
            if(!load_tga(filename , &tga))
                return false;
            glGenTextures(1, &m_id);    
            glBindTexture(GL_TEXTURE_2D,m_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.hdr.width, tga.hdr.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tga.pdata);
            // now some cleanup
            free_tga(&tga);
            m_w = tga.hdr.width;
            m_h = tga.hdr.height;
            return true;
        } else if(strstr(filename, ".bmp"))
        {
            AUX_RGBImageRec *pTextureImage = auxDIBImageLoad( L"d:\\test.bmp" );
            unsigned int g_testTextureID;
            if( pTextureImage != NULL )
            {
                glGenTextures( 1, &g_testTextureID );
                glBindTexture(GL_TEXTURE_2D, g_testTextureID);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexImage2D( GL_TEXTURE_2D, 0, 3,pTextureImage->sizeX,pTextureImage->sizeY, 0,
                    GL_RGB, GL_UNSIGNED_BYTE,pTextureImage->data );
            } else {
                cerr << "XTexture::LoadTexture: Failed to load the bitmap file" << endl;
                return false;
            }
            m_w = pTextureImage->sizeX;
            m_h = pTextureImage->sizeY;
            return true;
        }
        return false;
    }

    /*
     *
     */
    void XTexture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void XTexture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};