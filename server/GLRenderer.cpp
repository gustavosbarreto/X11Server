#include "GLRenderer.h"
#include <iostream>

namespace X
{
    using namespace std;
    /*
     *
     */
    bool GLRenderer::TileArea(IRenderSurface * surface, int x, int y, int w,int h)
    {
        glEnable(GL_TEXTURE_2D);
        surface->BindTexture();
        // render a quad with the texture
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
            glTexCoord2f(0,0);                                          
            glVertex2i(x, y);
            glTexCoord2f(4,0);  
            //glTexCoord2f(float(h) / rect.m_height,0);                 
            glVertex2i(x, y + h);
            //glTexCoord2f(float(h) / rect.m_height, float(w) / rect.m_width);  
            glTexCoord2f(4,4);  
            glVertex2i(x+w, y + h);
            glTexCoord2f(0,4);  
            //glTexCoord2f(0, w / rect.m_width);                    
            glVertex2i(x+w, y);
        glEnd();
        // undbind the texture
        surface->UnbindTexture();
        glDisable(GL_TEXTURE_2D);
        return true;
    }

    /*
     *
     */
    void GLRenderer::SwapBuffers(IRenderTarget * target)
    {
        target->SwapBuffers();
    }
    
    /*
     *
     */
    bool GLRenderer::Draw(IRenderSurface * surface, const XRectangle & dst)
    {
        surface->BindTexture();
        unsigned int w, h;
        surface->GetDimensions(w,h);
        // bind as a texture
        // draw the quad.
        glBegin(GL_QUADS);
            glVertex2i(dst.m_x, dst.m_y);
            glVertex2i(dst.m_x, dst.m_y + h);
            glVertex2i(dst.m_x + w, dst.m_y + h);
            glVertex2i(dst.m_x + w, dst.m_y);
        glEnd();
        // unbind it.
        surface->UnbindTexture();
        return true;
    }



    /*
     *
     */
    void GLRenderer::Ortho2d(int left, int right, int bottom, int top)  
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(left, bottom, right, top);
        gluOrtho2D(double(left), double(right), double(top), double(bottom));
        glMatrixMode(GL_MODELVIEW);
    }

    /*
     *
     */
    void GLRenderer::LoadIdentity()
    {
        glLoadIdentity();
    }

    void GLRenderer::DrawTexturedRect(int x, int y, int w, int h)
    {
        glBegin(GL_QUADS);
        //glColor3f(1,0,0);
            glTexCoord2f(0,0);                                          
            glVertex2i(x, y);
            glTexCoord2f(1,0);  
            //glTexCoord2f(float(h) / rect.m_height,0);                 
            glVertex2i(x, y + h);
            //glTexCoord2f(float(h) / rect.m_height, float(w) / rect.m_width);  
            glTexCoord2f(1,1);  
            glVertex2i(x+w, y + h);
            glTexCoord2f(0,1);  
            //glTexCoord2f(0, w / rect.m_width);        
        //  glColor3f(0,0,1);
            glVertex2i(x+w, y);
        glEnd();
    }
};