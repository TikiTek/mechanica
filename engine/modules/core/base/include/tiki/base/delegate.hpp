#pragma once
#ifndef TIKI_DELEGATE_HPP_INCLUDED
#define TIKI_DELEGATE_HPP_INCLUDED

namespace tiki
{
	template< class TReturn, class TParam0 >
	class Delegate
	{
	public:

		typedef TReturn( *Function )( TParam0 );

							Delegate();
		explicit			Delegate( Function pFunction );
		template< class TInstance, typename TMethod >
		explicit			Delegate( TInstance* pInstance, TMethod pMethod );

		void				setStaticFunction( Function pFunction );
		template< class TInstance, TReturn( TInstance::*TMethod )( TParam0 ) >
		void				setInstanceMethod( TInstance* pInstance );

		TReturn				invoke( TParam0 param0 ) const;

	private:

		struct Callback
		{
			virtual TReturn	invoke( TParam0 param0 ) const TIKI_PURE;
		};

		struct StaticFunctionCallback : public Callback
		{
			Function		pFunction;

			virtual TReturn	invoke( TParam0 param0 ) const TIKI_OVERRIDE_FINAL;
		};

		struct DefaultInstanceMethod
		{
			TReturn			defaultFunction( TParam0 param0 );
		};

		template< class TInstance, TReturn( TInstance::*TMethod )( TParam0 ) >
		struct InstanceMethodCallback : public Callback
		{
			TInstance*		pInstance;

			virtual TReturn	invoke( TParam0 param0 ) const TIKI_OVERRIDE_FINAL;
		};

		enum
		{
			InternalDelegateSize = TIKI_MAX( sizeof( StaticFunctionCallback ), sizeof( InstanceMethodCallback< DefaultInstanceMethod, &DefaultInstanceMethod::defaultFunction > ) )
		};

		uint8				m_internalCallbackData[ InternalDelegateSize ];

		const Callback*		getCallback() const;
	};
}

#include "../../../source/delegate.inl"

#endif // TIKI_DELEGATE_HPP_INCLUDED
