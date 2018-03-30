#ifndef _XGCONTEXT_H_
#define _XGCONTEXT_H_

#include "XResource.h"
#include "types.h"
#include "XPoint.h"
#include "XRectangle.h"

namespace X
{
    class XWindow;

    /* Class:           XGContext
     * Description:     A graphics context.
     */
    class XGContext : public XResource
    {
    public:
        XGContext(XWindow * wnd);
        ResourceType GetType() const {return GraphicsContext;}
        
        PIXEL GetBackground() const {return m_bgcolor;}
        PIXEL GetForeground() const {return m_fgcolor;}

    protected:
        XPoint<uint32> m_clipOrigin;                    // the clip origin
        std::list<XRectangle> m_clipRectangles;     // the clip rectangles.
        byte m_depth;
        XWindow * m_root;
        PIXEL m_bgcolor, m_fgcolor;
    };
};

#endif