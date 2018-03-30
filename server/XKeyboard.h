#ifndef _XKEYBOARD_H_
#define _XKEYBOARD_H_

#include "XKeymap.h"

namespace X
{
    /* Class:           XKeyboard
     * Description:     
     */
    class XKeyboard
    {
    public:
        XKeyboard();
        ~XKeyboard();
        
        // This should be or:ed by the SETofKEYUTMASK from the pointer device.
        SETofKEYBUTMASK GetModifierState();

        bool LoadKeymap(const char *);          // loads the keymap.
        void KeyPressed(byte);
        void KeyReleased(byte);
        void GetKeyStates(byte *);
        bool IsKeyPressed(byte);
        bool LoadKeysyms(const char *);
        XKeymap * GetKeyMap() {return keymap;}      // returns the keymap
    protected:
        byte m_keys[256];                       // the logical state of all the keys.
        XKeymap * keymap;                       // the keymap
        XUtility::BinaryTree<std::string, KEYSYM> m_keysyms;        
    };
};

#endif