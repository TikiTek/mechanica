#pragma once
#ifndef TIKI_SCRIPTCALL_HPP_INCLUDED
#define TIKI_SCRIPTCALL_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/script/scriptvalue.hpp"

namespace tiki
{
	class ScriptContext;

	class ScriptCall
	{
		TIKI_NONCOPYABLE_CLASS( ScriptCall );

	public:

							ScriptCall();
							~ScriptCall();

		bool				create( ScriptContext& context, bool hasInstance );
		void				dispose();

		void*				getInstanceData() const;
		ScriptValue			getArgument( uint index ) const;
		void				setReturnValue( const ScriptValue& value ) const;

		int					pushReturnValue();

	private:

		ScriptContext*			m_pContext;
		void*					m_pInstanceData;
		mutable ScriptValue		m_returnValue;

	};
}

#endif // TIKI_SCRIPTCALL_HPP_INCLUDED
