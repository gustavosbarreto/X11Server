#include "PropertyManager.h"

using namespace std;

namespace X
{
    /*
     *
     */
    int PropertyManager::ChangeProperty(ATOM name, ATOM type, int mode, const byte * src, int num)
    {
        map<ATOM, Property>::iterator it = m_properties.find(name);

        Property & prop = it->second;

        if(mode == Property::PropertyReplace) {
            // resize the vector
            prop.m_data.resize(num);
            // copy the data
            memcpy(&prop.m_data[0], src, num);
        } 
        else if(mode == Property::PropertyPrepend) {
            
        } 
        else if(mode == Property::PropertyAppend) {
            size_t currentSize = prop.m_data.size();
            prop.m_data.resize(currentSize + num);
            memcpy(&prop.m_data[currentSize], src, num);
        }

        return 0;
    }

    /*
     *
     */
    int PropertyManager::DeleteProperty(ATOM atom)
    {
        map<ATOM, Property>::iterator it = m_properties.find(atom);
        if(it == m_properties.end()) {
            // does not exist.
        }
        // delete the property
        m_properties.erase(it);
        return 0;
    }   
};