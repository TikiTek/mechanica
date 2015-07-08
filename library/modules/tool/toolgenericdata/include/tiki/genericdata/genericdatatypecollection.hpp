#pragma once
#ifndef TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
#define TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__

#include "tiki/base/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/genericdata/genericdatatype.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	class GenericDataEnum;

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

		bool							exportCode( GenericDataTypeMode mode, const string& targetDir );

	private:
				
		LinkedList< GenericDataType >	m_types;

		const GenericDataEnum*			m_pModeEnum;
		
		void							registerDefaultTypes();

		void							findFiles( const string& path, List< string >& files, const string& ext ) const;

	};
}

#endif // TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
