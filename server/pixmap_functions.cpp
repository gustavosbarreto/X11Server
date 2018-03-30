#include "XServer.h"
#include "XPixmap.h"
#include <iostream>
#include "tga.h"
#include "common.h"
#include "GLRenderer.h"
#include "XTexture.h"

using namespace std;
namespace X
{
    /* Function:        XServer::CreatePixmap
     * Description:     Creates a pixmap. The internal pixmap representation is always 32 bits.
     */
    int XServer::CreatePixmap(PIXMAP pId, byte depth, int w , int h, XScreen * screen, ErrorStruct * error)
    {
        // verify that the chosen identifier is free
        if(!isFree(pId)) {
            cerr << "XServer::CreatePixmap: The chosen ID for the resource is not free" << endl;
            error->ErrorCode = ErrorStruct::IDChoiceError;
            return -1;
        }
        if(depth != 1 && depth != 4 && depth != 8 && depth != 16 && depth != 24 && depth != 32) {
            cerr << "XServer::CreatePixmap: Invalid depth value '" << (unsigned int) depth <<"'." << endl;
            error->ErrorCode = ErrorStruct::ValueError;
            return -1;
        }
        PixmapPtr ptr = new (std::nothrow) XPixmap(pId, screen, this);
        if(!ptr.valid()) {
            cerr << "XServer::CreatePixmap: Failed to allocate the pixmap" << endl;
            error->ErrorCode = ErrorStruct::AllocError;
            return -1;
        }
        // now initalize the pixmap
        if(ptr->Init(w, h, depth) != 0) {
            error->ErrorCode = ErrorStruct::ValueError;
            return -1;
        }
        
        ResourcePtr rptr = *ptr;
        // now insert the resource
        if(!m_resources.Insert(pId, rptr)) {
            cerr << "XServer::CreatePixmap: Failed to insert the resource" << endl;
            error->ErrorCode = ErrorStruct::AllocError;
            return -1;
        }
        return 0;
    }

    /* Function:        XServer::FindPixmap
     *
     */
    int XServer::FindPixmap(PIXMAP pId, PixmapPtr & ptr, ErrorStruct * error)
    {
        ResourcePtr resource;
        if(!GetResource(pId, resource)) {
            cerr << "XServer::FindPixmap: The identifier does not identify a valid pixmap" << endl;
            error->ErrorCode = ErrorStruct::PixmapError;
            return -1;
        }
        if(resource->GetType() != XResource::PixmapResource)
        {
            cerr << "XServer::FindPixmap: The identifier does not identify a valid pixmap" << endl;
            error->ErrorCode = ErrorStruct::PixmapError;
            return -1;
        }
        ptr = (XPixmap *) *resource;
        return 0;
    }

    /* Function:        XServer::FreePixmap
     * Description:     Removes the primary reference to the resource. The actual pixmap data will
     *                  be destroyed when no other resource references it.
     */
    int XServer::FreePixmap(PIXMAP pId, ErrorStruct * error)
    {
        PixmapPtr ptr;
        if(FindPixmap(pId, ptr, error) != 0)
            return -1;
        // now free the resource. does not actually deallocate it.
        DestroyResource(pId);
        return 0;
    }

    /* Function:        XServer::CreatePixmapFromFile
     * Description:     Loads a pixmap from a file.
     */
    int XServer::CreatePixmapFromFile(const char * filename, PixmapPtr & pixmap)
    {
        XTexture texture;
        if(!texture.LoadTexture(filename))
        {
            cerr << "XServer::CreatePixmapFromFile: Failed to load the texture" << endl;
            return -1;
        }
        unsigned int w, h;
        texture.GetSize(w,h);
        // the texture was created, now create the pixmap.
        ErrorStruct error;
        PIXMAP pid = 256;
        if(CreatePixmap(pid, 24, w,h, m_activeScreen, &error) != 0)
        {
            cerr << "XServer::CreatePixmapFromFile: Failed to create the pixmap" << endl;
            return -1;
        }
        FindPixmap(pid, pixmap, &error);
        // now bind the pixmap as the target.
        pixmap->BindTarget();
        GLRenderer::Ortho2d(0, w, 0, h);
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        // now bind the texture
        texture.Bind();
        GLRenderer::DrawTexturedRect(0,0,w, h);
        texture.Unbind();
        // add the pixmap as a resource
        m_resources.Insert(pid, ResourcePtr(*pixmap));
        return 0;
    }

};