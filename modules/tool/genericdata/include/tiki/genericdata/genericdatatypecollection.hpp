#pragma once
#ifndef TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
#define TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__

#include "tiki/base/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/genericdata/genericdatatype.hpp"

namespace tiki
{
	class GenericDataEnum;

	class GenericDataTypeCollection
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeCollection );

	public:

										GenericDataTypeCollection();
										~GenericDataTypeCollection();

		void							create( const char* pFolderName, bool recursive, GenericDataTypeMode mode );
		void							dispose();

		void							addType( GenericDataType& type );

		const GenericDataType*			getTypeByName( const string& name ) const;
		GenericDataTypeMode				getModeByName( const string& name ) const;

		bool							exportCode( GenericDataTypeMode mode );

	private:
				
		LinkedList< GenericDataType >	m_types;

		const GenericDataEnum*			m_pModeEnum;
		
		void							registerDefaultTypes();

	};
}

#endif // TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
