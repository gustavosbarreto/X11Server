#ifndef _XKEYMAP_H_
#define _XKEYMAP_H_

#include "ErrorStruct.h"
#include "KeyboardMapping.h"
#include "types.h"
#include <map>
#include <string>
#include "BinaryTree.h"

#define NoSymbol        0

namespace X
{
    /* Class:           XKeymap
     * Description:     Represents a keymap
     */
    class XKeymap
    {
    public:
        XKeymap(XUtility::BinaryTree<std::string, KEYSYM> & syms);
        ~XKeymap();

        // loads the keyboard layout to use.
        int LoadKeyboardLayout(const char *);
        // changes the keyboard mapping
        bool ChangeKeyboardMapping(int keycode, int count, int symsPerKeycode, LISTofKEYSYM &, ErrorStruct *); 
        // retrives the mapping for the keys in the interval.
        bool GetKeyboardMapping(int first, int count, int & symsPerKeycode, LISTofKEYSYM &,ErrorStruct *);
        // Retrives the keycodes for the specified modifier.
        bool GetModifierKeys(int, KEYCODE *, size_t &);
        // changes the keys for the specified modifier.
        bool ChangeModifier(int, KEYCODE *, size_t);
        // loads the keysym definitions
        bool LoadKeysymDefs(const char *);
    protected:
        bool AllocateTable(size_t, size_t);
        bool NameToKeysym(const std::string &, KEYSYM &);
        bool SplitToKeysyms(const std::string &, KEYSYM *, size_t);
        XUtility::BinaryTree<std::string, KEYSYM> m_symdef;
        KeyboardMapping m_table;
    };
};


#endif