#include "XServer.h"

namespace X
{
    /*
     *
     */
    bool XServer::XGetAtomName(ATOM atom, std::wstring & name)
    {
        if(m_atoms.AtomExists(atom)) {
            // The ATOM exists
            name = m_atoms.GetName(atom);
            return true;
        } else {
            // The ATOM does not exist
            return false;
        }
    }

    /* Function:        XServer::InternAtom
     * Description:     Retrives the name of the ATOM, or creates it if onlyIfExist = false.
     */
    bool XServer::XInternAtom(const std::wstring & name, ATOM & atom, bool onlyIfExist)
    {
        if(!m_atoms.StringExists(name)) {
            // does not exist
            if(onlyIfExist == false) {
                // does not exist, so create it.
                atom = m_atoms.InsertString(name);
                // return succss
            }
            // return failure
        } else {
            atom = m_atoms.InsertString(name);
            // return success.
        }
        return true;
    }
};