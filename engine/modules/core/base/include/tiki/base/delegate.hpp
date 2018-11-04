#pragma once
#ifndef TIKI_DELEGATE_HPP_INCLUDED
#define TIKI_DELEGATE_HPP_INCLUDED

namespace tiki
{
	template< class TReturn, class ... TParams >
	class Delegate
	{
	public:

		typedef TReturn( *Function )( TParams ... );

		template< class TInstance >
		using TInstanceMethod = TReturn( TInstance::* )( TParams ... );

							Delegate();
		explicit			Delegate( Function pFunction );
		template< class TInstance >
		explicit			Delegate( TInstance* pInstance, TInstanceMethod< TInstance > pMethod );

		void				setStaticFunction( Function pFunction );
		template< class TInstance >
		void				setInstanceMethod( TInstance* pInstance, TInstanceMethod< TInstance > pMethod );

		TReturn				invoke( TParams ... args ) const;

	private:

		struct Callback
		{
			virtual TReturn	invoke( TParams ... args ) const TIKI_PURE;
		};

		struct StaticFunctionCallback : public Callback
		{
			Function		pFunction;

			virtual TReturn	invoke( TParams ... args ) const TIKI_OVERRIDE_FINAL;
		};

		struct DefaultInstanceMethod
		{
			TReturn			defaultFunction( TParams ... args );
		};

		template< class TInstance >
		struct InstanceMethodCallback : public Callback
		{
			TInstance*						pInstance;
			TInstanceMethod< TInstance >	pMethod;

			virtual TReturn	invoke( TParams ... args ) const TIKI_OVERRIDE_FINAL;
		};

		enum
		{
			InternalDelegateSize = TIKI_MAX( sizeof( StaticFunctionCallback ), sizeof( InstanceMethodCallback< DefaultInstanceMethod > ) )
		};

		uint8				m_internalCallbackData[ InternalDelegateSize ];

		const Callback*		getCallback() const;
	};
}

#include "../../../source/delegate.inl"

#endif // TIKI_DELEGATE_HPP_INCLUDED
