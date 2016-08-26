#   pragma once

#   include <stdint.h>

#	include "IntrusivePtr.h"

namespace GOAP
{
	template<class T>
	class IntrusivePtrBase
    {
    public:
		IntrusivePtrBase()
            : m_reference(0)
        {
        }

	public:
        inline static void intrusive_ptr_add_ref( T * _ptr );
        inline static void intrusive_ptr_dec_ref( T * _ptr );

	public:
		inline static void intrusive_ptr_destroy( T * _ptr );
			
    protected:
        uint32_t m_reference;
    };
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void IntrusivePtrSetup( T *& _ptr, T * _other )
	{
		_ptr = _other;

		if( _other != nullptr )
		{
			T::intrusive_ptr_add_ref( _other );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void IntrusivePtrSetup( T *& _ptr, const IntrusivePtr<T> & _other )
	{
		T * other_ptr = _other.get();
		
		GOAP::IntrusivePtrSetup( _ptr, other_ptr );
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void IntrusivePtrRelease( T *& _ptr )
	{
		if( _ptr != nullptr )
		{
			T::intrusive_ptr_dec_ref( _ptr );
			_ptr = nullptr;
		}
	}
    //////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void IntrusivePtrBase<T>::intrusive_ptr_add_ref( T * _ptr )
    {
        ++_ptr->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void IntrusivePtrBase<T>::intrusive_ptr_dec_ref( T * _ptr )
    {
        if( --_ptr->m_reference == 0 )
        {
			T::intrusive_ptr_destroy( _ptr );
        }
    }	
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void IntrusivePtrBase<T>::intrusive_ptr_destroy( T * _ptr )
	{
		delete _ptr;
		//Empty
	}
}