#include "tiki/tool_generic_data/generic_data_type_resource.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"

namespace tiki
{
	GenericDataTypeResource::GenericDataTypeResource( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataType* pBaseType, bool isGenericDataType )
		: GenericDataType( collection, name, filename, GenericDataTypeType_Resource, mode )
		, m_pBaseType( pBaseType )
		, m_isGenericData( isGenericDataType )
	{
	}

	GenericDataTypeResource::GenericDataTypeResource( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const string& postFix, fourcc fourCC )
		: GenericDataType( collection, name, filename, GenericDataTypeType_Resource, mode )
		, m_pBaseType( nullptr )
		, m_isGenericData( false )
		, m_postFix( postFix )
	{
		const uint8* pFourCC = (const uint8*)&fourCC;
		for (uint i = 0u; i < 4u; ++i )
		{
			m_fourCC[ i ] = pFourCC[ i ];
		}
	}

	GenericDataTypeResource::~GenericDataTypeResource()
	{
	}

	bool GenericDataTypeResource::loadFromXml( XmlElement* pTypeNode )
	{
		const XmlAttribute* pPostFixAtt = pTypeNode->findAttribute( "postfix" );
		const XmlAttribute* pFourCcAtt = pTypeNode->findAttribute( "fourcc" );

		if ( pPostFixAtt == nullptr || pFourCcAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataTypeResource::loadFromXml] Resource types needs 'postfix' and 'fourcc' attributes. Typename: %s\n", getName().cStr() );
			return false;
		}

		m_postFix = pPostFixAtt->getValue();

		uint i = 0u;
		const char* pFourccText = pFourCcAtt->getValue();
		const uint charCount = TIKI_MIN( 4u, getStringLength( pFourccText ) );
		for (; i < charCount; ++i)
		{
			m_fourCC[ i ] = pFourccText[ i ];
		}

		for (; i < 4u; ++i)
		{
			m_fourCC[ i ] = ' ';
		}

		return true;
	}

	bool GenericDataTypeResource::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		static const char* s_pBaseFormat =	"\n"
											"\ttypedef GenericDataResource< %s, TIKI_FOURCC( '%c', '%c', '%c', '%c' ) > %sGenericDataResource;\n";

		targetData.containsResource = true;

		targetData.code += formatDynamicString(
			s_pBaseFormat,
			m_pBaseType->getCodeExportName().cStr(),
			m_fourCC[ 0u ], m_fourCC[ 1u ], m_fourCC[ 2u ], m_fourCC[ 3u ],
			getName().cStr()
		);

		return true;
	}

	uint GenericDataTypeResource::getAlignment() const
	{
		return 8u;
	}

	uint GenericDataTypeResource::getSize() const
	{
		return 8u;
	}

	string GenericDataTypeResource::getCodeExportName() const
	{
		if( m_isGenericData )
		{
			return formatDynamicString( "ResRef< %sGenericDataResource >", getName().cStr() );
		}

		return formatDynamicString( "ResRef< %s >", getName().cStr() );
	}

	crc32 GenericDataTypeResource::getTypeCrc() const
	{
		if ( m_pBaseType )
		{
			return m_pBaseType->getTypeCrc();
		}
		else
		{
			return crcString( getName() );
		}
	}

	const GenericDataType* GenericDataTypeResource::getBaseType() const
	{
		return m_pBaseType;
	}

	const string& GenericDataTypeResource::getPostFix() const
	{
		return m_postFix;
	}

	fourcc GenericDataTypeResource::getFourCC() const
	{
		return TIKI_FOURCC( m_fourCC[ 0u ], m_fourCC[ 1u ], m_fourCC[ 2u ], m_fourCC[ 3u ] );
	}
}