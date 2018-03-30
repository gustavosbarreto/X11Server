#include "XServer.h"

namespace X
{
    /* Function:        XServer::RemoveResource
     * Description:     Removes a resource from the resource tree. Won't actually delete it.
     *                  The resource will be deleted when it's no longer referenced by any other
     *                  resource.
     */
    void XServer::RemoveResource(uint32 id)
    {
        m_resources.Remove(id);
    }

    /* Function:        XServer::isFree
     * Description:     Returns true if the supplied id is available.
     */
    bool XServer::isFree(uint32 id)
    {
        ResourcePtr ptr;
        if(!m_resources.Find(id, ptr))
            return true;
        return false;
    }

    /*
     *
     */
    bool XServer::GetResource(uint32 id, ResourcePtr & ptr)
    {
        if(!m_resources.Find(id, ptr))
            return false;
        return true;
    }
};