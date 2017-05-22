#pragma once
#ifndef TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
#define TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/fixedarray.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/container/list.hpp"
#include "tiki/container/map.hpp"
#include "tiki/toolgenericdata/generic_data_tag_handler.hpp"
#include "tiki/toolgenericdata/genericdatatype.hpp"
#include "tiki/toolgenericdata/genericdatatypetype.hpp"
#include "tiki/toolgenericdata/genericdatatypevaluetype.hpp"

namespace tiki
{
	class GenericDataTag;
	class GenericDataTypeArray;
	class GenericDataTypeEnum;
	class GenericDataTypePointer;
	class GenericDataTypeReference;
	class GenericDataTypeResource;
	class GenericDataTypeStruct;
	class GenericDataValue;
	struct GenericDataModuleData;

	class GenericDataTypeCollection
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeCollection );

	public:

		typedef LinkedList< GenericDataType > TypeList;

										GenericDataTypeCollection();
										~GenericDataTypeCollection();

		bool							create( const string& contentFolder, bool recursive );
		void							dispose();

		GenericDataTagHandler&			getTagHandler();

		bool							addType( GenericDataType& type );
		bool							removeType( GenericDataType& type );

		const TypeList&					getTypes() const;

		const GenericDataType*			findTypeByName( const string& name ) const;
		void							findTypesByType( List< const GenericDataType* >& types, GenericDataTypeType typeType ) const;

		GenericDataTypeMode				findModeByName( const string& name ) const;

		const GenericDataTypeValueType*	getEnumDefaultBaseType() const;

		const GenericDataTypeArray*		makeArrayType( const GenericDataType* pBaseType );
		const GenericDataTypeReference*	makeReferenceType( const GenericDataTypeResource* pBaseType );
		const GenericDataTypePointer*	makePointerType( const GenericDataTypeStruct* pBaseType );

		const GenericDataType*			parseType( const string& typeString );
		bool							parseValue( GenericDataValue* pTargetValue, const string& valueString, const GenericDataType* pType, const GenericDataType* pParentType );

		bool							exportCode( GenericDataTypeMode mode, const string& targetDir );

	private:

		typedef Map< string, GenericDataModuleData > ModuleMap;
		typedef Map< const GenericDataType*, const GenericDataTypeArray* > TypeArrayMap;
		typedef Map< const GenericDataTypeResource*, const GenericDataTypeReference* > TypeReferenceMap;
		typedef Map< const GenericDataTypeStruct*, const GenericDataTypePointer* > TypePointerMap;

		TypeList						m_types;
		TypeArrayMap					m_arrays;
		TypeReferenceMap				m_references;
		TypePointerMap					m_pointers;

		GenericDataTagHandler			m_tagHandler;

		const GenericDataTypeEnum*		m_pModeEnum;
		const GenericDataTypeValueType*	m_pEnumDefaultType;

		ModuleMap						m_modules;

		bool							registerDefaultValueTypes();
		bool							registerDefaultResourceTypes();
		void							findFiles( const string& path, List< string >& files, const string& ext ) const;
		bool							parseFile( XmlReader& reader, const _XmlElement* pRootNode, const string& fileName, const string& moduleName );

		void							writeToFileIfNotEquals( const string& fileName, const string& content );

	};
}

#endif // TIKI_GENERICDATATYPECOLLECTION_HPP_INCLUDED__
