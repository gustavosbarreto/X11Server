#ifndef _PROPERTYMANAGER_H_
#define _PROPERTYMANAGER_H_

#include "types.h"
#include <map>

namespace X
{
    /* Struct:          Property
     * Description:     A property,
     */
    class Property
    {
    public:
        enum PropertyMode {PropertyReplace = 0, PropertyPrepend, PropertyAppend};
        ATOM m_name, m_type;        // the name and type of the property
        size_t m_elementSize;       // 8, 16 or 32.
        std::vector<byte> m_data;
    };

    /* Class:           PropertyManager
     * Description:     Handles the properties of a window.
     */
    class PropertyManager
    {
    public: 
        // The data has to be byteswapped before.
        int ChangeProperty(ATOM name, ATOM type, int mode, const byte *, int num);
        int DeleteProperty(ATOM);
        int GetProperty(ATOM, ATOM, uint32 offset, uint32 length, bool, int & format,uint32 & remaining);
    protected:
        std::map<ATOM, Property> m_properties;
    };
};

#endif