#include "XWindow.h"
#include "types.h"
#include <list>
#include "XServer.h"
#include "common.h"

namespace X
{
    using namespace std;

    /* Function:        XWindow::GetInterestedButtonPressClient
     * Description:     Retrives the client that will get the pointer grab.
     */
    /*XClient * XWindow::GetInterestedButtonPressClient()
    {
        if(m_buttonPress != NULL) return m_buttonPress;
        WindowPtr current = m_parent;
        while(current.valid())
        {
            if(current->m_buttonPress)
                return current->m_buttonPress;
        }
        return NULL;
    }*/

    /* Function:        XWindow::FindMatchingPassiveGrab
     * Description:     Finds any matching passive keygrab.
     */
    bool XWindow::FindMatchingKeyGrab(KEYCODE keycode,      // (in) the key pressed 
        SETofKEYMASK modifiers,                         // (in) the current state of the modifier keys
        KeyGrabStruct & ks,                                 // (out) Information about the grab
        XClient * client)                                   // (out) the client the grab belongs to.
    {
        WindowLst wndList;
        GetAncestors(wndList);
        
        // for all the ancestors, beginning with the root
        for(WindowLst::reverse_iterator it = wndList.rbegin();
            it != wndList.rend();
            it++)
        {
            // for all the grabs in this window
            for(list<PassiveGrab>::iterator grabIt = (*it)->m_grabs.begin();
                grabIt != (*it)->m_grabs.end();
                grabIt++)
            {
                if(grabIt->m_type != PassiveGrab::KeyGrab) continue;
                if(grabIt->keygrab.key == 0 || grabIt->keygrab.key == keycode)
                {
                    if(grabIt->keygrab.modifiers == 0) {
                        // found a matching grab
                        ks = grabIt->keygrab;
                        client = grabIt->m_client;
                        return true;
                    } else 
                    {
                        if((grabIt->keygrab.modifiers & modifiers) == grabIt->keygrab.modifiers)
                        {
                            // all the specified modifier keys are set. found a matching grab
                            ks = grabIt->keygrab;
                            client = grabIt->m_client;
                            return true;
                        }
                    }
                }
            }
        }
        // did not find a matching passive keygrabs
        return false;
    }

    /* Function:        XWindow::RemoveClientGrabs
     * Description:     Removes all the grabs set by a client.
     */
    void XWindow::RemoveClientGrabs(const XClient * client)
    {
        for(std::list<PassiveGrab>::iterator it = m_grabs.begin();
            it != m_grabs.end();
            it++)
        {
            if(it->m_client == client)
            {
                it = m_grabs.erase(it);
                if(it == m_grabs.end())
                    break;
            }
        }
    }

    /* Function:        XWindow::UngrabKey
     * Description:     Performs a ungrab.
     */
    int XWindow::UngrabKey(XClient * client, KEYCODE key, DWORD modifiers)
    {
        for(std::list<PassiveGrab>::iterator it = m_grabs.begin();
            it != m_grabs.end();
            it++)
        {
            if(it->m_type == PassiveGrab::KeyGrab &&
                (key == 0 || key == it->keygrab.key) &&
                (modifiers == 0 || modifiers == it->keygrab.modifiers))
            {
                if(it->m_client == client) {
                    it = m_grabs.erase(it);
                    if(it == m_grabs.end())
                        break;
                }
            }
        }
        return REQUEST_SUCCESS;
    }
};