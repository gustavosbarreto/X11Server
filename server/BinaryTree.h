#ifndef _BINARYTREE_H_
#define _BINARYTREE_H_

#include <hash_map>

namespace XUtility
{
    /* Class:           BinaryTree
     * Description:     A binary tree.
     */
    template<class key_type, class data_type>
    class BinaryTree
    {
    public:
        bool Find(key_type key, data_type & data)
        {
            stdext::hash_map<key_type, data_type>::iterator it = m_map.find(key);
            if(it == m_map.end())
                return false;
            data = it->second;
            return true;
        }

        bool Insert(key_type key, data_type & data)
        {
            m_map[key] = data;
            return true;
        }
        
        void Remove(key_type key)
        {
            m_map.erase(key);
        }
    protected:

        stdext::hash_map<key_type, data_type> m_map;
    };
};

#endif