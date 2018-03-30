#include "XKeyboard.h"
#include <iostream>
#include <fstream>
#include "stringutil.h"
#include <sstream>
#include <iomanip>

namespace X
{
    using namespace std;
    using namespace XUtility;

    /*
     *
     */
    XKeyboard::XKeyboard()
    {
        memset(m_keys, 0, 256);
        keymap = NULL;
    }

    /* Function:        XKeyboard::~XKeyboard
     * Description:     Performs the required cleanup
     */
    XKeyboard::~XKeyboard()
    {
        delete keymap;
    }

    /* Function:        XKeyboard::KeyPressed
     * Description:     Sets the logical state of a key to pressed
     */
    void XKeyboard::KeyPressed(byte keycode)
    {
        m_keys[keycode] = 1;
    }

    /* Function:        XKeyboard::KeyReleased
     * Description:     Sets the logical state of a key to released.
     */
    void XKeyboard::KeyReleased(byte keycode)
    {
        m_keys[keycode] = 0;
    }

    /* Function:        XKeyboard::IsKeyPressed
     * Description:     Returns true if the key is pressed.
     */
    bool XKeyboard::IsKeyPressed(byte keycode)
    {
        return m_keys[keycode] == 1;
    }

    /* Function:        XKeyboard::GetKeyStates
     * Description:     Returns the logical state of all the keys.
     */
    void XKeyboard::GetKeyStates(byte * dst)
    {
        for(size_t b = 0;b<32;++b) 
        {
            dst[b] = 0;
            for(size_t bit = 0;bit < 8;++bit)
            {
                dst[b] |= (m_keys[(b<<3) + bit] == 0 ? 0 : (1 << bit));
            }
        }
    }

    /* Function:        XKeyboard::LoadKeymap
     *
     */
    bool XKeyboard::LoadKeymap(const char * keys)
    {
        if(keymap) delete keymap;
        if(!(keymap = new (std::nothrow) XKeymap(m_keysyms)))
        {
            cerr << "XKeyboard::LoadKeymap: Failed to allocate the XKeymap instance" << endl;
            return false;
        }
        if(keymap->LoadKeyboardLayout(keys) != 0)
        {
            cerr << "XKeyboard::LoadKeymap: LoadKeyboardLayout() failed" << endl;
            return false;
        }
        return true;
    }

    /* Function:        XKeyboard::GetModifierState
     * Description:     Retrives the state of the modifiers.
     */
    SETofKEYBUTMASK XKeyboard::GetModifierState()
    {
        SETofKEYBUTMASK bitmask = 0;
        KEYCODE syms[4];
        size_t numSyms;
        // for all the 8 modifiers
        for(int i = 0; i < 8;++i)
        {
            // get the actual keys for the modifier.
            keymap->GetModifierKeys(i, syms, numSyms);
            if(numSyms == 0) continue;      // the modifier is not defined.
            // is any of the keysyms set?
            for(int k = 0;k<numSyms;++k)
            {
                if(m_keys[syms[k]]) {
                    bitmask |= (1 << i);
                    break;
                }
            }
        }
        return bitmask;
    }

    /* Function:        XKeyboard::LoadKeysyms
     * Description:     Loads the file containing the keysyms definition. Each definition 
     *                  maps a name to a value, e.g. BACKSPACE -> 0xFF08. This simplifies the task
     *                  of writing the keymap files.
     */
    bool XKeyboard::LoadKeysyms(const char * filename)
    {
        ifstream file(filename);
        if(!file) {
            cerr << "XKeymap::LoadKeysymDefs: Failed to open the definition file" << endl;
            return false;
        }
        size_t count=1;
        string line;
        while(getline(file, line))
        {
            // trim the line
            line = trim(line);
            size_t i = line.find_first_not_of(" \t");
            if(line.empty() || i == string::npos || line.at(i) == '#')
                continue;
            // split the line.
            size_t delim = line.find_first_of(" \t");
            if(delim == 0 || delim == (line.length() - 1) || delim == string::npos)
            {
                cerr << "Error while parsing line " << count << " in the keysym-definition file" << endl;
                return false;
            }
            string name = trim(line.substr(0, delim));
            string val = trim(line.substr(delim));
            stringstream ss;
            ss << std::setbase(16) << val;
            KEYSYM ival;
            if(!(ss >> ival))
            {
                cerr << "Error while converting the value on line " << count << ", expected a hexadecimal number" << endl;
                return false;
            }
            cerr << name << " = " << val << endl;
            ++count;
            // insert the value
            m_keysyms.Insert(name, ival);
        }
        return true;
    }
};