#ifndef _ATOMLIST_H_
#define _ATOMLIST_H_

#include "types.h"
#include <map>
#include <string>

namespace X
{
    /* Class:           AtomList
     * Description:     Stores ATOM:s.
     */
    class AtomList
    {
    public:
        int LoadDefault(const wchar_t *);       // loads the default ATOM names from a string.
        ATOM InsertString(const std::wstring &);        // inserts a ATOM
        const std::wstring & GetName(ATOM);     // retrives the name of a ATOM.
        bool StringExists(const std::wstring &);        // returns true if the string already exists.
        bool AtomExists(ATOM);

    protected:
        // A waste of memory, a better implementation required later.
        std::vector<std::wstring> m_atomToStr;
        std::map<std::wstring, ATOM> m_strToAtom;
    };
};

#endif