
#include "tiki/toolgenericdata/genericdatatyperesource.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/io/xmlreader.hpp"

namespace tiki
{
	GenericDataTypeResource::GenericDataTypeResource( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataType* pBaseType )
		: GenericDataType( collection, name, mode )
		, m_pBaseType( pBaseType )
	{
	}

	GenericDataTypeResource::GenericDataTypeResource( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const string& postFix, fourcc fourCC )
		: GenericDataType( collection, name, mode )
	{
		m_pBaseType = nullptr;
		m_postFix	= postFix;

		const uint8* pFourCC = (const uint8*)&fourCC;
		for (uint i = 0u; i < 4u; ++i )
		{
			m_fourCC[ i ] = pFourCC[ i ];
		}
	}

	GenericDataTypeResource::~GenericDataTypeResource()
	{
	}

	bool GenericDataTypeResource::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		const XmlAttribute* pPostFixAtt = reader.findAttributeByName( "postfix", pTypeRoot );
		const XmlAttribute* pFourCcAtt = reader.findAttributeByName( "fourcc", pTypeRoot );

		if ( pPostFixAtt == nullptr || pFourCcAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataTypeResource::loadFromXml] Resource types needs 'postfix' and 'fourcc' attributes. Typename: %s\n", getName().cStr() );
			return false;
		}

		m_postFix = pPostFixAtt->content;

		const string fourccText = string( pFourCcAtt->content );

		uint i = 0u;
		const uint charCount = TIKI_MIN( 4u, fourccText.getLength() );
		for (; i < charCount; ++i)
		{
			m_fourCC[ i ] = fourccText[ i ];
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

		targetData.code += formatString(
			s_pBaseFormat,
			m_pBaseType->getExportName().cStr(),
			m_fourCC[ 0u ], m_fourCC[ 1u ], m_fourCC[ 2u ], m_fourCC[ 3u ],
			getName().cStr()
		);

		return true;
	}

	GenericDataTypeType GenericDataTypeResource::getType() const
	{
		return GenericDataTypeType_Resource;
	}

	uint GenericDataTypeResource::getAlignment() const
	{
		return 8u;
	}

	uint GenericDataTypeResource::getSize() const
	{
		return 8u;
	}

	string GenericDataTypeResource::getExportName() const
	{
		return formatString( "ResRef< %s >", getName().cStr() );
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