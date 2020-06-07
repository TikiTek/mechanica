#pragma once

#include "tiki/base/assert.hpp"

namespace tiki
{
	template< class TReturn, class ... TParams >
	Delegate< TReturn, TParams... >::Delegate()
	{
		setNullFunction();
	}

	template< class TReturn, class ... TParams >
	Delegate< TReturn, TParams... >::Delegate( Function pFunction )
	{
		setStaticFunction( pFunction );
	}

	template< class TReturn, class ... TParams >
	template< class TInstance >
	Delegate< TReturn, TParams... >::Delegate( TInstance* pInstance, InstanceMethod< TInstance > pMethod )
	{
		setInstanceMethod< TInstance >( pInstance, pMethod );
	}

	template< class TReturn, class ... TParams >
	void Delegate< TReturn, TParams... >::setNullFunction()
	{
		new( m_internalCallbackData ) NullCallback();
	}

	template< class TReturn, class ... TParams >
	void Delegate< TReturn, TParams... >::setStaticFunction( Function pFunction )
	{
		StaticFunctionCallback* pInternalCallback = new( m_internalCallbackData ) StaticFunctionCallback();
		TIKI_ASSUME( pInternalCallback != nullptr );

		pInternalCallback->pFunction = pFunction;
	}

	template< class TReturn, class ... TParams >
	template< class TInstance >
	void Delegate< TReturn, TParams... >::setInstanceMethod( TInstance* pInstance, InstanceMethod< TInstance > pMethod )
	{
		InstanceMethodCallback< TInstance >* pInternalCallback = new( m_internalCallbackData ) InstanceMethodCallback< TInstance >();
		TIKI_ASSUME( pInternalCallback != nullptr );

		pInternalCallback->pInstance	= pInstance;
		pInternalCallback->pMethod		= pMethod;
	}

	template< class TReturn, class ... TParams >
	TReturn Delegate< TReturn, TParams... >::invoke( TParams ... args ) const
	{
		return getCallback()->invoke( args ... );
	}

	template< class TReturn, class ... TParams >
	const typename Delegate< TReturn, TParams... >::Callback* Delegate< TReturn, TParams... >::getCallback() const
	{
		return (const Callback*)m_internalCallbackData;
	}

	template< class TReturn, class ... TParams >
	TReturn Delegate< TReturn, TParams... >::NullCallback::invoke( TParams ... args ) const
	{
		return TReturn();
	}

	template< class TReturn, class ... TParams >
	TReturn Delegate< TReturn, TParams... >::StaticFunctionCallback::invoke( TParams ... args ) const
	{
		TIKI_ASSERT( pFunction != nullptr );
		return pFunction( args ... );
	}

	template< class TReturn, class ... TParams >
	template< class TInstance >
	TReturn Delegate< TReturn, TParams... >::InstanceMethodCallback< TInstance >::invoke( TParams ... args ) const
	{
		return (pInstance->*pMethod)( args ... );
	}
}
