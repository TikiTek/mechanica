#pragma once

namespace tiki
{
	template< class TReturn, class ... TParams >
	class Delegate
	{
	public:

		using Function = TReturn( * )( TParams ... );

		template< class TInstance >
		using InstanceMethod = TReturn( TInstance::* )( TParams ... );

										Delegate();
		explicit						Delegate( Function pFunction );
		template< class TInstance >
		explicit						Delegate( TInstance* pInstance, InstanceMethod< TInstance > pMethod );

		void							setNullFunction();
		void							setStaticFunction( Function pFunction );
		template< class TInstance >
		void							setInstanceMethod( TInstance* pInstance, InstanceMethod< TInstance > pMethod );

		TReturn							invoke( TParams ... args ) const;

	private:

		struct Callback
		{
			virtual TReturn				invoke( TParams ... args ) const = 0;
		};

		struct NullCallback : public Callback
		{
			virtual TReturn				invoke( TParams ... args ) const TIKI_OVERRIDE_FINAL;
		};

		struct StaticFunctionCallback : public Callback
		{
			Function					pFunction = nullptr;

			virtual TReturn				invoke( TParams ... args ) const TIKI_OVERRIDE_FINAL;
		};

		struct DefaultInstance
		{
		};

		template< class TInstance >
		struct InstanceMethodCallback : public Callback
		{
			TInstance*					pInstance	= nullptr;
			InstanceMethod< TInstance >	pMethod		= nullptr;

			virtual TReturn				invoke( TParams ... args ) const TIKI_OVERRIDE_FINAL;
		};

		static constexpr uintreg		InternalDelegateSize		= TIKI_MAX( sizeof( StaticFunctionCallback ), sizeof( InstanceMethodCallback< DefaultInstance > ) );
		static constexpr uintreg		InternalDelegateAlignment	= TIKI_MAX( TIKI_ALIGNOF( StaticFunctionCallback ), TIKI_ALIGNOF( InstanceMethodCallback< DefaultInstance > ) );

		alignas( InternalDelegateAlignment )
		uint8							m_internalCallbackData[ InternalDelegateSize ];

		const Callback*					getCallback() const;
	};
}

#include "../../../source/delegate.inl"
