#include "XPixmap.h"
#include "common.h"
#include <iostream>
#include "SmartPointer.h"

namespace X
{
    using namespace std;
    using namespace XUtility;

    /* Function:        XPixmap::XPixmap
     * Description:     Constructor
     */
    XPixmap::XPixmap(PIXMAP pId, XScreen * screen, XServer * server) : XDrawable(pId, screen, server)
    {
    }

    /*
     *
     */
    XPixmap::~XPixmap()
    {
        // TODO
    }

    /* Function:        XPixmap::Init
     * Description:     Initalizes the underlying p-buffer. The internal format is always 32 bits.
     */
    

    

    
    /* Function:        XPixmap::Init
     * Description:     Initalizes the pixmap.
     */
    int XPixmap::Init(unsigned int w, unsigned int h, byte depth)
    {
        if(!AllocSurface(w, h)) 
            return -1;
        m_rect.m_width = w;
        m_rect.m_height = h;
        return 0;
    }
}