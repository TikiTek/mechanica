
#include "tiki/genericdata/genericdatatypecollection.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/genericdata/genericdataenum.hpp"
#include "tiki/genericdata/genericdatavaluetype.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	GenericDataTypeCollection::GenericDataTypeCollection()
	{
		m_pModeEnum = nullptr;
	}

	GenericDataTypeCollection::~GenericDataTypeCollection()
	{
	}

	void GenericDataTypeCollection::create( const char* pFolderName, bool recursive, GenericDataTypeMode mode )
	{
		registerDefaultTypes();

		// todo
	}

	void GenericDataTypeCollection::dispose()
	{
		m_pModeEnum = nullptr;

		List< GenericDataType* > types;
		{
			types.reserve( m_types.getCount() );
			for ( GenericDataType& type : m_types )
			{
				types.add( &type );
			}
			m_types.clear();
		}

		for (uint i = 0u; i < types.getCount(); ++i)
		{
			TIKI_MEMORY_DELETE_OBJECT( types[ i ] );
		}
		types.dispose();
	}

	void GenericDataTypeCollection::addType( GenericDataType& type )
	{
		TIKI_ASSERT( getTypeByName( type.getName() ) == nullptr );

		m_types.push( type );
	}

	const GenericDataType* GenericDataTypeCollection::getTypeByName( const string& name ) const
	{
		for (const GenericDataType& type : m_types)
		{
			if ( type.getName() == name )
			{
				return &type;
			}
		}

		return nullptr;

	}

	GenericDataTypeMode GenericDataTypeCollection::getModeByName( const string& name ) const
	{
		const sint64* pValue = m_pModeEnum->getValueByName( name );
		if ( pValue != nullptr )
		{
			return (GenericDataTypeMode)*pValue;
		}

		return GenericDataTypeMode_Invalid;
	}

	bool GenericDataTypeCollection::exportCode( GenericDataTypeMode mode )
	{
		return false;
	}

	void GenericDataTypeCollection::registerDefaultTypes()
	{
		GenericDataValueType* pSInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint8",  GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger8 );
		GenericDataValueType* pSInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint16", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataValueType* pSInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint32", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataValueType* pSInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint64", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );
		GenericDataValueType* pUInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint8",  GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger8 );
		GenericDataValueType* pUInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint16", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataValueType* pUInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint32", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataValueType* pUInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint64", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );
		GenericDataValueType* pFloat16	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "half",   GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataValueType* pFloat32	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "float",  GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataValueType* pFloat64	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "double", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );

		addType( *pSInt8 );
		addType( *pSInt16 );
		addType( *pSInt32 );
		addType( *pSInt64 );
		addType( *pUInt8 );
		addType( *pUInt16 );
		addType( *pUInt32 );
		addType( *pUInt64 );
		addType( *pFloat16 );
		addType( *pFloat32 );
		addType( *pFloat64 );

		GenericDataEnum* pGenericDataTypeMode = TIKI_MEMORY_NEW_OBJECT( GenericDataEnum )( *this, "GenericDataTypeMode", GenericDataTypeMode_ToolOnly, *pUInt8 );
		pGenericDataTypeMode->addValue( "Invalid",			0, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "RuntimeOnly",		1, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolOnly",			2, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolAndRuntime",	3, GenericDataTypeMode_ToolOnly );
		m_pModeEnum = pGenericDataTypeMode;
	}
}
