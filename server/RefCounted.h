#ifndef _REFCOUNTED_H_
#define _REFCOUNTED_H_

#include <iostream>

namespace X
{
    /* Class:           RefCounted
     * Description:     Used for reference counting.
     */
    class RefCounted
    {
    public:
        RefCounted() {m_refs = 0;}
        virtual void increase() {++m_refs; std::cerr << "Increasing count=" << m_refs << std::endl;}
        virtual void decrease() {std::cerr << "Decreasing Count = " << m_refs << std::endl; if((--m_refs) <= 0) 
        {std::cerr << "Delete!!" << std::endl; delete this;}}
    protected:
        int m_refs;
    };
};

#endif