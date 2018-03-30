#ifndef _XWINDOW_H_
#define _XWINDOW_H_

#include "XDrawable.h"
#include "XScreen.h"
#include <list>
#include "types.h"
#include "XClient.h"
#include "PropertyManager.h"
#include "XPixmap.h"
#include "ErrorStruct.h"
#include "SmartPointer.h"
#include "XRectangle.h"
#include "PassiveGrab.h"
#include "AttributeStruct.h"
#include "GrabInfo.h"

namespace X
{
    class XScreen;
    class XServer; 
    class XWindow;

    enum WindowClass {InputOutput, InputOnly, CopyFromParent};
    enum BitGravity {BitForget = 0, BitStatic, BitNorthWest, BitNorth, BitNorthEast, BitWest, BitCenter,BitEast,BitSouthWest, BitSouth, BitSouthEast};
    enum WinGravity {WinUnmap = 0, WinStatic, WinNorthWest, WinNorth, WinNorthEast, WinWest, WinCenter,WinEast, WinSouthWest, WinSouth, WinSouthEast};

    enum MapType {WhenMapped = 0, Always};

    /* Class:           XWindow
     * Description:
     */
    class XWindow : public XDrawable
    {
    public:

        XWindow(XServer *, WINDOW, WindowPtr &, XScreen *);     // constructor
        ~XWindow();                                             // destructor.

        int Initalize(CreateWindowStruct *,byte depth, XClient *, ErrorStruct *);

        XResource::ResourceType GetType() const {return XResource::WindowResource;}
        WindowClass GetClass() const            {return m_class;}
        XScreen * GetScreen() const             {return m_screen;}
        bool isDrawable() const                 {return m_class == InputOutput;}
        bool isRoot() const                     {return m_parent == NULL;}
        bool isParent(const WindowPtr & wnd)    {return m_parent == wnd;}
        bool isVisible() const;
        WindowPtr getParent()                   {return m_parent;}
        void RemoveChild(WindowPtr);
        void AddChild(WindowPtr);
        void SetBackground(PixmapPtr &);
        
        // hierarchy functions.
        bool isAncestor(const WindowPtr &);
        bool isChild(const WindowPtr &);
        void GetAncestors(std::list<WindowPtr> &);
        WindowPtr GetNearestAncestorNotCreatedByClient(XClient *);
        void ParentChild(WindowPtr &);
        WindowPtr GetLeastCommonAncestor(const WindowPtr & , const WindowPtr &) const;

        // Map functions.
        int MapWindow(XClient *);               // maps the window
        int MapSubWindows(XClient *);
        int UnmapWindow(bool fromConfigure = false);    
        int UnmapSubWindows();


        // Destroy functions.
        void DestroySubWindows();
        void DestroyWindow();

        // Attribute functions
        void GetAttributes(AttributeStruct *,const XClient *) const;
        bool ChangeWindowAttributes(BITMASK, const LISTofVALUE &, XClient *, ErrorStruct *);
        bool ConfigureWindow(BITMASK, const LISTofVALUE &, ErrorStruct *);
        bool SetAttributes(BITMASK bitmask,LISTofVALUE values, ErrorStruct *, XClient *);

        // visibility functions.
        int GetVisibilityStatus(std::list<XRectangle> &);

        // Retrives the visible window at the specified location.
        bool GetWindowAt(int x, int y, WindowPtr &);
        // returns true if the point is inside the (outer) rectangle of the window.
        bool IsInside(int x, int y);

        // Grab functions
        void RemoveClientGrabs(const XClient *);        // remove all the grabs set by the client.
        bool FindMatchingKeyGrab(KEYCODE, SETofKEYMASK, KeyGrabStruct &, XClient *);
        bool FindMatchingButtonGrab(BUTTON, SETofKEYMASK, ButtonGrabStruct *, XClient **);

        BITMASK GetClientInterests(const XClient *) const;


        /* TOFIX */

        
        // resizes the window.
        void ReSize(uint16 width, uint16 height);
        
        VISUALID GetVisualType() {return m_visualId;}
    

        // The window becomes the parent to a window. The child is placed on top of the 
        // other siblings in the stack order.
        
            bool FindMatchingPassiveGrab(const XEvent &);
        // Returns the property manager for this window.
        PropertyManager & GetPropertyManager() {return m_properties;}
        
        std::list<PassiveGrab> & GetGrabs() {return m_grabs;}

        
        // removes one or more passive grabs set by the client.
        int UngrabKey(XClient *, KEYCODE, DWORD);

        
         void GetGeometry(XRectangle & rect, WINDOW &, uint32 &, byte &);

         // retrives a list of visibles ares.
         bool GetVisibleAreas(std::list<XRectangle> &) const;

         void DrawWindow(int,int,int,int);

         void PropagatePointerEvent(XEvent &, WINDOW srcId = 0, WINDOW callId = 0);
    protected:

        std::list<XClient *> GetInterestedClients(BITMASK);
        void NowVisible();

        void SetVisibility(int);
        void ChildMapped(WINDOW);
        void ChildUnmapped(WINDOW);

        // called when the window is destroyed by it's parent.
        void WindowDestroyed(); 

        int Resize(uint32 width, uint32 height);
        
        void ChildDestroyed(const XWindow *);
        void SetEventMask(XClient *, BITMASK);
        bool ChildMapped(XWindow *);
        void ParentMappedAndVisible();
        void ParentUnmapped();
        void Reposition(int16, int16);

        void ReportIfClientInterested(XEvent & ev);

        /*
         * Pixmaps
         */
        XUtility::SmartPointer<XPixmap> m_bgPixmap,             // the background pixmap
                                        m_borderPixmap,         // the pixmap used for the borders
                                        m_pixmap;               // the surface of the window.
        /*
         * Windows.
         */
        WindowPtr   m_parent;
        std::list<WindowPtr> m_children;
        /*
         * Client related data
         */
        XClient *                                   m_creator;              // the client that created the window.
        std::list<std::pair<BITMASK, XClient *> >   m_clientInterests;  

        /*
         * Grab related stuff.
         */
        XClient * m_substructureRedirect, * m_resizeRedirect, * m_buttonPress;
        std::list<PassiveGrab>  m_grabs;        // the passive grabs.
                        
        /*
         * Misc data
         */
        XRectangle              m_insideRect,
                                m_borderRect;
        WindowClass             m_class;                            // the window class.
        VISUALID                m_visualId;                         // the visual id of the window.
        bool                    m_mapped;   
        PropertyManager         m_properties;                       // the properties stored in the window.
        BITMASK                 m_doNotPropagateMask;
        bool                    m_overrideRedirect;
        unsigned int            m_WinGravity, 
                                m_BitGravity;
        int                     m_borderWidth;
        MapType                 m_mapType;
    };
};

#endif