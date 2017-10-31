#pragma once
#ifndef TIKI_DELEGATE_INL_INCLUDED
#define TIKI_DELEGATE_INL_INCLUDED

#include "tiki/base/assert.hpp"

namespace tiki
{
	template< class TReturn, class TParam0 >
	Delegate<TReturn, TParam0>::Delegate()
	{
		setStaticFunction( nullptr );
	}

	template< class TReturn, class TParam0 >
	Delegate< TReturn, TParam0 >::Delegate( Function pFunction )
	{
		setStaticFunction( nullptr );
	}

	template< class TReturn, class TParam0 >
	template< class TInstance, typename TMethod >
	Delegate< TReturn, TParam0 >::Delegate( TInstance* pInstance, TMethod pMethod )
	{
		setInstanceMethod< TInstance, TMethod >( pInstance );
	}

	template< class TReturn, class TParam0 >
	void Delegate< TReturn, TParam0 >::setStaticFunction( Function pFunction )
	{
		StaticFunctionCallback* pInternalCallback = new(m_internalCallbackData) StaticFunctionCallback();
		pInternalCallback->pFunction = pFunction;
	}

	template< class TReturn, class TParam0 >
	template< class TInstance, TReturn( TInstance::*TMethod )(TParam0) >
	void Delegate< TReturn, TParam0 >::setInstanceMethod( TInstance* pInstance )
	{
		InstanceMethodCallback< TInstance, TMethod >* pInternalCallback = new( m_internalCallbackData ) InstanceMethodCallback< TInstance, TMethod >();
		pInternalCallback->pInstance = pInstance;
	}

	template< class TReturn, class TParam0 >
	TReturn Delegate<TReturn, TParam0>::invoke( TParam0 param0 ) const
	{
		getCallback()->invoke( param0 );
	}

	template< class TReturn, class TParam0 >
	const typename Delegate< TReturn, TParam0 >::Callback* Delegate<TReturn, TParam0>::getCallback() const
	{
		return (const Callback*)m_internalCallbackData;
	}

	template< class TReturn, class TParam0 >
	TReturn Delegate< TReturn, TParam0 >::StaticFunctionCallback::invoke( TParam0 param0 ) const
	{
		TIKI_ASSERT( pFunction != nullptr );
		return pFunction( param0 );
	}

	template< class TReturn, class TParam0 >
	template< class TInstance, TReturn( TInstance::*TMethod )( TParam0 ) >
	TReturn Delegate< TReturn, TParam0 >::InstanceMethodCallback< TInstance, TMethod >::invoke( TParam0 param0 ) const
	{
		return (pInstance->*TMethod)( param0 );
	}
}

#endif // TIKI_DELEGATE_INL_INCLUDED
