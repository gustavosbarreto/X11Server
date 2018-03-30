#include "XKeymap.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "stringutil.h"

#define _DEBUG_VERBOSE

using namespace std;
using namespace XUtility;

namespace X
{
    /*
     *
     */
    XKeymap::XKeymap(XUtility::BinaryTree<std::string, KEYSYM> & syms) : m_symdef(syms)
    {
    }

    /*
     *
     */
    XKeymap::~XKeymap()
    {
    }
    
    /*
     *
     */
    bool XKeymap::AllocateTable(size_t count, size_t keysymsPerKeycode)
    {
        m_table.m_table = new KEYSYM[count * keysymsPerKeycode];
        if(!m_table.m_table) {
            cerr << "XKeymap::AllocateTable: Allocation failed" << endl;
            return false;
        }
        m_table.keysymsPerKeycode = keysymsPerKeycode;
        return true;
    }

    /* Function:        XKeymap::GetKeyboardMapping
     *
     */
    bool XKeymap::GetKeyboardMapping(int first, 
        int count, 
        int & keysymsPerKeycode, 
        LISTofKEYSYM & keysyms,
        ErrorStruct * error)
    {
        if(first < m_table.minKeycode || (first + count) > m_table.maxKeycode) {
            error->ErrorCode = ErrorStruct::ValueError;
            return false;
        }
        size_t base = first - m_table.minKeycode;
        // resize the vector
        keysyms.resize(count * m_table.keysymsPerKeycode);
        // and copy everything between to the vector.
        memcpy(&keysyms[0], &m_table.m_table[base * m_table.keysymsPerKeycode], count * m_table.keysymsPerKeycode * sizeof(KEYSYM));
        return true;
    }

    /* Function:        XKeymap::ChangeKeyboardMapping
     * Description:     Changes the keyboard mapping.
     */
    bool XKeymap::ChangeKeyboardMapping(int keycode, 
        int count, 
        int symsPerKeycode, 
        LISTofKEYSYM & keysyms, 
        ErrorStruct * error)
    {
        size_t base = 0, keysymIndex = 0; // TODO
        if(keycode < m_table.minKeycode || (keycode + count) > m_table.maxKeycode) {
            cerr << "ChangeKeyboardMapping failed, the specified range is invalid" << endl;
            return false;
        }
        if(symsPerKeycode == m_table.keysymsPerKeycode) {
            // solve everything with a memcpy?
            memcpy(&m_table.m_table[base * m_table.keysymsPerKeycode], 
                &keysyms[0], count * m_table.keysymsPerKeycode * sizeof(KEYSYM));
        } else if(symsPerKeycode < m_table.keysymsPerKeycode) {
            // fill the remaining characters with NoSymbol.
            for(size_t keycode = base; keycode < (base + count); ++keycode) {
                KEYSYM * keycodeBase = &m_table.m_table[keycode * m_table.keysymsPerKeycode];
                // for all the keysyms belonging to the keycode.
                for(size_t i = 0; i < m_table.keysymsPerKeycode;++i) 
                    keycodeBase[i] = (i >= symsPerKeycode ? NoSymbol : keysyms[keysymIndex++]);
            }
        } else {
            cerr << "ChangeKeyboardMapping failed, too many keysyms per keycode" << endl;
            return false;
        }
        return true;
    }

    /* Function:        XKeymap::LoadKeyboardLayout
     * Description:     Loads the keyboard layout.
     */
    int XKeymap::LoadKeyboardLayout(const char * filename)
    {
        ifstream file(filename);
        if(!file) {
            cerr << "Could not open the keyboard layout file" << endl;
            return -1;
        }
        // allocate the table
        if(!AllocateTable(255 - 8, 4)) return -1;
        m_table.minKeycode = 8;
        m_table.maxKeycode = 255;
        // fill it with NoSymbol.
        for(size_t i = 0;i<(247 * 4);++i)
            m_table.m_table[i] = NoSymbol;

        string line, idstr, syms;
        int id;
        int lastId = 0;
        while(getline(file, line)) {
            // now parse the line.
            size_t i = line.find_first_not_of(" \t");
            if(line.empty() || i == string::npos || line.at(i) == '#') continue;
            size_t divider = line.find_first_of('=');
            if(divider == string::npos || divider == 0 || divider == (line.length() - 1)) {
                cerr << "Failed to parse the keyboard layout file" << endl;
                return -1;
            }
            // split the string.
            idstr = line.substr(0, divider);
            syms = line.substr(divider+1);
            id = atoi(idstr.c_str());
            if(id < 8 || id > 255 || id <= lastId) {
                // invalid value for keycode
                cerr << "Failed to parse the keyboard layout file, invalid id" << endl;
                return -1;
            }
            lastId = id;
            // split the right side into keysyms.
            KEYSYM * symentry = &m_table.m_table[(id - 8) * m_table.keysymsPerKeycode];
            if(!SplitToKeysyms(syms,symentry, m_table.keysymsPerKeycode)) {
                cerr << "Failed to split the string into keysyms" << endl;
                return -1;
            }
        }
        return 0;
    }

    /*
     *
     */
    bool XKeymap::SplitToKeysyms(const std::string & symstr, KEYSYM * symentry, size_t w)
    {
        if(w == 0) return false;
        vector<string> keysyms;
        string str = trim(symstr);
        size_t delim = str.find_first_of(" \t");
        while(delim != string::npos)
        {
            string sub = str.substr(0, delim);
            keysyms.push_back(sub);
            str = str.substr(delim, string::npos);
            str = trim(str);
            delim = str.find_first_of(" \t");
        }
        if(str.length() > 0) keysyms.push_back(str);
        if(keysyms.size() > w) {
            cerr << "XKeymap::SplitToKeysyms: To many keysym supplied, will ignore the rest" << endl;
            return false;
        }
        for(size_t i = 0;i<w;i++)
            symentry[i] = NoSymbol; // fill the entry with the empty symbol
        for(size_t i = 0;i<min(keysyms.size(),w);i++) {
            if(!NameToKeysym(keysyms[i], symentry[i])) {
                cerr << "XKeymap:SplitToKeysyms: Unknown name for a keysym, ignoring it" << endl;
            }
        }
        return true;
    }

    /* Function:        XKeymap::ChangeModifier
     * Description:     Changes the keycode mapping for a modifier.
     */
    bool XKeymap::ChangeModifier(int mod, KEYCODE * keys, size_t num)
    {
        if(mod > 7 || mod < 0) {
            cerr << "XKeymap::ChangeModifier: Invalid modifier specified" << endl;
            return false;
        }
        if(num > 4)
        {
            cerr << "XKeymap::ChangeModifier: Invalid number of keycodes for a modifier" << endl;
            return false;
        }
#ifdef _DEBUG_VERBOSE
        cerr << "Modifier key #" << mod << " change to: ";
        for(size_t i = 0;i < num;i++) {
            cerr << keys[i] << ((i == (num-1)) ? "\n" : ", ");
        }
#endif
        // fill in the correct values.
        for(size_t i = 0;i < 4; i++) {
            m_table.modifiers[mod][i] = (i > num ? 0 : keys[i]);
        }
        return true;
    }

    /* Function:        XKeymap::GetModifierKeys
     *
     */
    bool XKeymap::GetModifierKeys(int modifier, KEYCODE * keycodes, size_t & num)
    {
        if(modifier < 0 || modifier > 7) {
            cerr << "XKeymap::GetModifierKeys: Invalid modifier key" << endl;
            return false;
        }
        // copy the keycodes.
        num = (num < 4 ? num : 4);
        memcpy(keycodes, &m_table.modifiers[modifier], num);
        return true;
    }

    /*
     *
     */
    bool XKeymap::NameToKeysym(const std::string & name, KEYSYM & value)
    {
        return m_symdef.Find(name, value);
    }

};