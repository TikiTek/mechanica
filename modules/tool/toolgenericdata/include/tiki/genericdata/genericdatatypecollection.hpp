#pragma once
#ifndef TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
#define TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__

#include "tiki/base/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/genericdata/genericdatatype.hpp"
#include "tiki/toolbase/list.hpp"
#include "tiki/toolbase/map.hpp"

namespace tiki
{
	class GenericDataTypeArray;
	class GenericDataTypeEnum;

	class GenericDataTypeCollection
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeCollection );

	public:

										GenericDataTypeCollection();
										~GenericDataTypeCollection();

		void							create( const string& contentFolder, bool recursive );
		void							dispose();

		bool							addType( GenericDataType& type );

		const GenericDataType*			findTypeByName( const string& name ) const;
		GenericDataTypeMode				findModeByName( const string& name ) const;

		const GenericDataTypeArray*		makeArrayType( const GenericDataType* pType );

		bool							exportCode( GenericDataTypeMode mode, const string& targetDir );

	private:

		typedef Map< const GenericDataType*, const GenericDataTypeArray* > TypeArrayMap;
				
		LinkedList< GenericDataType >	m_types;
		TypeArrayMap					m_arrays;

		const GenericDataTypeEnum*		m_pModeEnum;
		
		void							registerDefaultTypes();
		
		void							findFiles( const string& path, List< string >& files, const string& ext ) const;

	};
}

#endif // TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
