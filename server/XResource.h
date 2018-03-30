#ifndef _XRESOURCE_H_
#define _XRESOURCE_H_

#include "types.h"
#include "RefCounted.h"
#include "SmartPointer.h"

namespace X
{
    /* Class:           XResource
     * Description:     Baseclass for the X resourceses like windows, pixmaps, fonts etc.
     */
    class XResource : public RefCounted     // all resources are reference counted.
    {
    public: 
        XResource(uint32 id) : m_id(id) {
        }

        virtual ~XResource() {}

        enum ResourceType {WindowResource = 1, PixmapResource, GraphicsContext};
        virtual ResourceType GetType() const = 0;       // returns the resource type.   
        uint32 GetId() {return m_id;}
    protected:
        uint32      m_id;       // the resource id.
    };

    typedef XUtility::SmartPointer<XResource> ResourcePtr;

};

#endif