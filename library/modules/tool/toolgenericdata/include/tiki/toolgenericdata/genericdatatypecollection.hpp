#pragma once
#ifndef TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
#define TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__

#include "tiki/base/fixedarray.hpp"
#include "tiki/base/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"
#include "tiki/toolbase/map.hpp"
#include "tiki/toolgenericdata/genericdatatype.hpp"
#include "tiki/toolgenericdata/genericdatatypetype.hpp"
#include "tiki/toolgenericdata/genericdatatypevaluetype.hpp"

namespace tiki
{
	class GenericDataTypeArray;
	class GenericDataTypeEnum;
	class GenericDataTypeReference;
	class GenericDataTypeResource;
	class GenericDataValue;
	struct GenericDataModuleData;

	class GenericDataTypeCollection
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeCollection );

	public:

										GenericDataTypeCollection();
										~GenericDataTypeCollection();

		bool							create( const string& contentFolder, bool recursive );
		void							dispose();

		bool							addType( GenericDataType& type );

		const GenericDataType*			findTypeByName( const string& name ) const;
		void							findTypesByType( List< const GenericDataType* >& types, GenericDataTypeType typeType ) const;

		GenericDataTypeMode				findModeByName( const string& name ) const;

		const GenericDataTypeArray*		makeArrayType( const GenericDataType* pType );
		const GenericDataTypeReference*	makeReferenceType( const GenericDataTypeResource* pBaseType );

		const GenericDataType*			parseType( const string& typeString );
		bool							parseValue( GenericDataValue& outValue, const string& valueString, const GenericDataType* pType );

		bool							exportCode( GenericDataTypeMode mode, const string& targetDir );

	private:

		struct ModifierDescription
		{
			string	modifier;
			string	content;
		};

		typedef Map< string, GenericDataModuleData > ModuleMap;
		typedef Map< const GenericDataType*, const GenericDataTypeArray* > TypeArrayMap;
		typedef Map< const GenericDataTypeResource*, const GenericDataTypeReference* > TypeReferenceMap;

		LinkedList< GenericDataType >	m_types;
		TypeArrayMap					m_arrays;
		TypeReferenceMap				m_references;

		const GenericDataTypeEnum*		m_pModeEnum;

		ModuleMap						m_modules;
		
		bool							registerDefaultValueTypes();
		bool							registerDefaultResourceTypes();
		void							findFiles( const string& path, List< string >& files, const string& ext ) const;
		bool							parseFile( XmlReader& reader, const _XmlElement* pRootNode, const string& fileName, const string& moduleName );

		bool							parseToken( ModifierDescription& outModifier, const string& text );

	};
}

#endif // TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
