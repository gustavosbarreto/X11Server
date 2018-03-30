#ifndef _XRECTANGLE_H_
#define _XRECTANGLE_H_



namespace X 
{
    class XRectangle
    {
    public:
        XRectangle();       // default constructor
        XRectangle(int,int,int, int);
        // returns true if the rectangle overlaps with this one.
        bool overlaps(const XRectangle & rect, XRectangle & overlap) const;
        bool IsInside(int x, int y);
        XRectangle Translate(int x, int y) const;

        int m_x, m_y, m_width, m_height;
    };
};

#endif