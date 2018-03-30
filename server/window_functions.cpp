#include "XServer.h"
#include "types.h"
#include "common.h"
#include "XDefines.h"
#include <iostream>
namespace X
{

    using namespace std;
    using namespace XUtility;

    /* Function:        XServer::GetWindowResource  
     * Description:     Retrives the Window Resource associated with the ID, if any.
     */
    bool XServer::GetWindowResource(WINDOW wId, SmartPointer<XWindow> & wnd, ErrorStruct * error)
    {
        SmartPointer<XResource> res;
        if(!m_resources.Find(wId, res)) {
            // The parent window does not exist.
            error->ErrorCode = ErrorStruct::WindowError;
            return false;
        }
        // found the resource with the id, but is it really a window?
        if(res->GetType() != XResource::WindowResource) {
            // The identifier for the parent does not belong to a valid window.
            error->ErrorCode = ErrorStruct::WindowError;
            return false;
        }
        wnd = (XWindow *) *res;
        return true;
    }

    /* Function:        XServer::XFindWindow
     * Description:     Finds the requested window if it exists.
     */
    bool XServer::XFindWindow(X::WINDOW wId, X::WindowPtr & wnd, ErrorStruct * error)
    {
        return GetWindowResource(wId, wnd, error);
    }

    /* Function:        XServer::XCreateWindow
     * Description:     Create a window.
     */
    int XServer::XCreateWindow(X::WINDOW wId, WindowPtr & wnd, WindowPtr & parent)
    {
        ResourcePtr ptr;
        if(m_resources.Find(wId, ptr)) {
            cerr << "A window with the requested identifier already exists" << endl;
            return -1;
        }
        // create the window.
        wnd = new (std::nothrow) XWindow(this, wId, parent, parent->GetScreen());
        // now insert the window
        m_resources.Insert(wId, ResourcePtr(*wnd));
        return 0;
    }

    /* Function:        XServer::XDestroyWindow
     * Description:     Destroys a window.
     */
    int XServer::XDestroyWindow(WINDOW wId, ErrorStruct * error)
    {
        // first get the window
        WindowPtr wndPtr;
        if(!XFindWindow(wId, wndPtr, error)) {
            // the window does not exist.
            return -1;
        }
        // is it a root window?
        if(wndPtr->isRoot()) {
            // yes, but you can't destroy the root window.
            cerr << "XServer::XDestroyWindow: Client tried to destroy the root window" << endl;
            error->ErrorCode = ErrorStruct::AccessError;
            return -1;
        }
        // Destroy the window
        wndPtr->DestroyWindow();
        // now remove the window from the parents list
        WindowPtr parent = wndPtr->getParent();
        if(!parent.valid()) {
            // does not have a parent?
        }
        parent->RemoveChild(wndPtr);
        // now remove the resource
        RemoveResource(wndPtr->GetId());
        return 0;
    }

    /*
     *
     */
    bool XServer::ConfinePointerTo(WindowPtr ptr)
    {
        return false;
    }
};
