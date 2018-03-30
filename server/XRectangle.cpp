#include "XRectangle.h"
#include <algorithm>

using namespace std;
namespace X
{
    /* Function:        XRectangle::XRectangle
     * Description:     Default constructor.
     */
    XRectangle::XRectangle()
        : m_x(0), m_y(0), m_width(0), m_height(0)
    {
    }
    /* Function:        XRectangle::XRectangle
     * Description:     Constructor.
     */
    XRectangle::XRectangle(int x, int y, int w, int h)
        : m_x(x), m_y(y), m_width(w), m_height(h)
    {
    }
    /* Function:        XRectangle::overlaps
     * Description:     Returns true if the rectangles overlaps
     */
    bool XRectangle::overlaps(const XRectangle & rect, XRectangle & overlap) const
    {
        overlap.m_x = max(m_x, rect.m_x);
        overlap.m_width = min((rect.m_x + rect.m_width), (m_x + m_width)) - overlap.m_x;
        if(overlap.m_width <= 0) return false;
        overlap.m_y = max(m_y, rect.m_y);
        overlap.m_height = min((rect.m_y + rect.m_height), (m_y + m_height)) - overlap.m_y;
        if(overlap.m_height <= 0) return false;
        return true;
    }

    /* Function:        XRectangle::IsInside
     * Description:     Returns true if the point is inside the rectangle.
     */
    bool XRectangle::IsInside(int x, int y)
    {
        return ((x >= m_x) && (x <= (m_x + m_width)) && (y >= m_y) && (y <= (m_y + m_height)));
    }
};