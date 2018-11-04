#pragma once
#ifndef TIKI_DELEGATE_INL_INCLUDED
#define TIKI_DELEGATE_INL_INCLUDED

#include "tiki/base/assert.hpp"

namespace tiki
{
	template< class TReturn, class ... TParams >
	Delegate< TReturn, TParams... >::Delegate()
	{
		setStaticFunction( nullptr );
	}

	template< class TReturn, class ... TParams >
	Delegate< TReturn, TParams... >::Delegate( Function pFunction )
	{
		setStaticFunction( pFunction );
	}

	template< class TReturn, class ... TParams >
	template< class TInstance >
	Delegate< TReturn, TParams... >::Delegate( TInstance* pInstance, TInstanceMethod< TInstance > pMethod )
	{
		setInstanceMethod< TInstance >( pInstance, pMethod );
	}

	template< class TReturn, class ... TParams >
	void Delegate< TReturn, TParams... >::setStaticFunction( Function pFunction )
	{
		StaticFunctionCallback* pInternalCallback = new(m_internalCallbackData) StaticFunctionCallback();
		pInternalCallback->pFunction = pFunction;
	}

	template< class TReturn, class ... TParams >
	template< class TInstance >
	void Delegate< TReturn, TParams... >::setInstanceMethod( TInstance* pInstance, TInstanceMethod< TInstance > pMethod )
	{
		InstanceMethodCallback< TInstance >* pInternalCallback = new( m_internalCallbackData ) InstanceMethodCallback< TInstance >();
		pInternalCallback->pInstance	= pInstance;
		pInternalCallback->pMethod		= pMethod;
	}

	template< class TReturn, class ... TParams >
	TReturn Delegate< TReturn, TParams... >::invoke( TParams ... args ) const
	{
		getCallback()->invoke( args ... );
	}

	template< class TReturn, class ... TParams >
	const typename Delegate< TReturn, TParams... >::Callback* Delegate< TReturn, TParams... >::getCallback() const
	{
		return (const Callback*)m_internalCallbackData;
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

#endif // TIKI_DELEGATE_INL_INCLUDED
