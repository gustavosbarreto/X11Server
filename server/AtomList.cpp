#include "AtomList.h"

using namespace std;

namespace X
{
    /* Function:        AtomList::StringExists
     * Description:     Returns true if the string exists.
     */
    bool AtomList::StringExists(const std::wstring & str)
    {
        std::map<wstring, ATOM>::iterator it = m_strToAtom.find(str);
        return (it != m_strToAtom.end());
    }

    /* Function:        AtomList::InsertString
     * Description:     Inserts a string and returns the associated ATOM.
     */
    ATOM AtomList::InsertString(const wstring & str)
    {
        ATOM atom = (ATOM) m_atomToStr.size();
        m_atomToStr[atom] = str;
        m_strToAtom[str] = atom;
        return atom;
    }

    /* Function:        AtomList::GetAtom
     * Description:     Returns the string associated with the ATOM, or a empty string if the
     *                  ATOM dosen't exist.
     */
    const std::wstring & AtomList::GetName(ATOM atom)
    {
        if(atom >= m_atomToStr.size()) 
            return wstring(L"");
        return m_atomToStr[atom];
    }

    /* Function:        AtomList::AtomExists
     * Description:     Returns true if the ATOM exists.
     */
    bool AtomList::AtomExists(ATOM atom)
    {
        return (atom < m_atomToStr.size());
    }
};