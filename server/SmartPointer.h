#ifndef _SMARTPOINTER_H_
#define _SMARTPOINTER_H_

#include <iostream>

namespace XUtility
{
    /* Class:           SmartPointer
     * Description:     A smart pointer class.
     */
    template <class T>
    class SmartPointer
    {
    public:
        /*SmartPointer(T * obj) {
            m_obj = obj;
            m_obj->increase();
        }*/

        SmartPointer(const SmartPointer<T> & ptr) {
            m_obj = ptr.m_obj;
            if(m_obj) m_obj->increase();
        }

        SmartPointer(T * obj)
        {
            m_obj = obj;
            if(m_obj) m_obj->increase();
        }

        SmartPointer()
        {
            m_obj = 0;
        }

        ~SmartPointer()
        {
            std::cerr << "~SmartPointer" << std::endl;
            if(m_obj) {
                std::cerr << "Decreasing" << std::endl;
                m_obj->decrease();
                m_obj = 0;
            }
        }

        SmartPointer<T> & operator = (T * obj)
        {
            if(m_obj) {
                m_obj->decrease();
                m_obj = 0;
            }
            m_obj = obj;
            if(m_obj) m_obj->increase();
            return *this;
        }

        SmartPointer<T> & operator = (const SmartPointer<T> & rhs)
        {
            if(m_obj) {
                m_obj->decrease();
                m_obj = 0;
            }
            m_obj = rhs.m_obj;
            if(m_obj) m_obj->increase();
            return *this;
        }

        T * operator *() 
        {
            return m_obj;
        }

        bool operator == (const T * rhs) const
        {
            return rhs == m_obj;
        }

        bool operator == (const SmartPointer<T> & rhs) const
        {
            return m_obj == rhs.m_obj;
        }

        bool operator !() const
        {
            return m_obj == 0;
        }

        bool valid() const
        {
            return m_obj != 0;
        }

        T * operator -> () const
        {
            return m_obj;
        }

        T * ptr() {
            return m_obj;
        }
    protected:
        T * m_obj;
    };
};

#endif