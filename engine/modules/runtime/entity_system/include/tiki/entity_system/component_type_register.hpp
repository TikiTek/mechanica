#pragma once
#ifndef __TIKI_TYPEREGISTER_HPP_INCLUDED__
#define __TIKI_TYPEREGISTER_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/components/component_types.hpp"
#include "tiki/container/array.hpp"

namespace tiki
{
	class ComponentBase;

	class ComponentTypeRegister
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ComponentTypeRegister );

	public:

		bool				create( uint maxTypeCount );
		void				dispose();

		uint				getMaxTypeCount() const { return m_types.getCount(); }

		ComponentTypeId		registerType( ComponentBase* pComponent );
		void				unregisterType( ComponentTypeId typeId );

		bool				isTypeRegistred( ComponentTypeId typeId ) const;
		bool				isComponentRegistred( const ComponentBase* pComponent ) const;

		ComponentBase*		getTypeComponent( ComponentTypeId typeId ) const;
		crc32				getTypeCrc( ComponentTypeId typeId ) const;
		uint32				getTypeStateSize( ComponentTypeId typeId ) const;
		const char*			getTypeName( ComponentTypeId typeId ) const;

	private:

		struct RegisterType
		{
			ComponentBase*	pComponent;
			uint32			stateSize;
			crc32			typeCrc;
			const char*		pName;
		};

		Array< RegisterType >	m_types;
	};
}

#endif // __TIKI_TYPEREGISTER_HPP_INCLUDED__
