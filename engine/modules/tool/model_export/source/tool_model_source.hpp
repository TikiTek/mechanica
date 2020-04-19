#pragma once

#include "tiki/base/crc32.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/graphics/vertex_semantic.hpp"
#include "tiki/tool_base/tokenizer.hpp"

namespace tiki
{
	static void stringSourceToFloat( float& target, const Array< DynamicString >& source )
	{
		target = string_tools::parseFloat32( source[ 0u ].cStr() );
	}

	static void stringSourceToString( DynamicString& target, const Array< DynamicString >& source )
	{
		target = source[ 0u ];
	}

	static void stringSourceToMatrix( Matrix44& target, const Array< DynamicString >& source )
	{
		float* pMatrix = &target.x.x;
		for (uintreg i = 0u; i < 16u; ++i)
		{
			pMatrix[ i ] = string_tools::parseFloat32( source[ i ].cStr() );
		}
		matrix::transpose( target );
	}

	struct ToolModelSourceBase
	{
		struct Technique
		{
			DynamicString	name;
			DynamicString	type;
		};

		DynamicString				id;
		DynamicString				target;
		crc32				crcId;

		uintreg				count;
		uintreg				stride;
		uintreg				userData;
		VertexSementic		semantic;

		Array< Technique >	techniques;
	};

	template<typename T>
	struct ToolModelSource : public ToolModelSourceBase
	{
		typedef void(*ConvertFunc)(T&, const Array< DynamicString >&);

		Array< T >			data;

		bool				create( const XmlReader* pXml, const XmlElement* pNode, ConvertFunc pConvertFunc, uintreg elementCount )
		{
			DynamicString arrayId;
			DynamicString techniqueSource;

			const XmlAttribute* pIdAtt		= pXml->findAttributeByName( "id", pNode );

			if ( pIdAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "source has no id attribute." );
				return false;
			}

			id		= pIdAtt->content;
			crcId	= crcString( id );

			const XmlElement* pArrayNode	= pXml->findFirstChild( "float_array", pNode );
			const XmlElement* pTechnique	= pXml->findFirstChild( "technique_common", pNode );

			if ( pArrayNode == nullptr )
			{
				pArrayNode					= pXml->findFirstChild( "Name_array", pNode );
			}

			if ( pArrayNode == nullptr || pTechnique == nullptr )
			{
				TIKI_TRACE_ERROR( "create source failed.\n" );
				return false;
			}

			// technique
			{
				const XmlElement* pAccessorNode	= pXml->findFirstChild( "accessor", pTechnique );

				if ( pAccessorNode == nullptr )
				{
					TIKI_TRACE_ERROR( "accessor node not found." );
					return false;
				}

				const XmlAttribute* pCountAtt	= pXml->findAttributeByName( "count", pAccessorNode );
				const XmlAttribute* pStrideAtt	= pXml->findAttributeByName( "stride", pAccessorNode );
				const XmlAttribute* pSourceAtt	= pXml->findAttributeByName( "source", pAccessorNode );

				count			= string_tools::parseUInt32( pCountAtt->content );
				techniqueSource	= DynamicString( pSourceAtt->content ).subString( 1u );

				if ( pStrideAtt != nullptr )
				{
					stride	= string_tools::parseUInt32( pStrideAtt->content );
				}
				else
				{
					stride = 0;
				}

				techniques.create( count );

				uintreg techIndex = 0u;
				const XmlElement* pParam = pXml->findFirstChild( "param", pAccessorNode );
				while ( pParam )
				{
					const XmlAttribute* pNameAtt = pXml->findAttributeByName( "name", pParam );
					const XmlAttribute* pTypeAtt = pXml->findAttributeByName( "type", pParam );

					if ( pTypeAtt == nullptr )
					{
						TIKI_TRACE_ERROR( "technique has no type." );
						return false;
					}
					techniques[ techIndex ].type = pTypeAtt->content;

					if ( pNameAtt != nullptr )
					{
						DynamicString fieldName = DynamicString( pNameAtt->content ).toLower();

						if ( fieldName == "s" )
						{
							fieldName = "x";
						}
						else if ( fieldName == "t" )
						{
							fieldName = "y";
						}

						techniques[ techIndex ].name = fieldName;
					}

					techIndex++;

					pParam = pXml->findNext( "param", pParam );
				}
			}

			// float array
			{
				const XmlAttribute* pArrayIdAtt	= pXml->findAttributeByName( "id", pArrayNode );
				const XmlAttribute* pCountAtt	= pXml->findAttributeByName( "count", pArrayNode );

				arrayId = pArrayIdAtt->content;

				if ( techniqueSource != arrayId )
				{
					TIKI_TRACE_ERROR( "technique type not supported." );
					return false;
				}

				const uintreg arrayCount = string_tools::parseUInt32( pCountAtt->content );
				data.create( arrayCount );

				Tokenizer tokenizer;
				tokenizer.create( pArrayNode->content, " \t\n\r", true );

				Array< DynamicString > sourceValues;
				sourceValues.create( elementCount );

				uintreg i = 0u;
				uintreg c = 0u;
				uintreg stringIndex = 0;
				while ( stringIndex != (uintreg)-1 )
				{
					DynamicString value = tokenizer.findNext( &stringIndex );

					if ( !value.isEmpty() )
					{
						sourceValues[ c++ ] = value;
					}

					if ( c == elementCount )
					{
						T convertedValue;
						pConvertFunc( convertedValue, sourceValues );

						data[ i++ ] = convertedValue;
						c = 0u;
					}
				}

				sourceValues.dispose();

				if ( i * elementCount != arrayCount )
				{
					TIKI_TRACE_ERROR( "float_array node has a wrong count values(%i != %i)", i, count );
					return false;
				}
			}

			return true;
		}

		void			dispose()
		{
			techniques.dispose();
			data.dispose();
		}

		bool			isCreated() const
		{
			return !id.isEmpty() && techniques.getCount() != 0 && data.getCount() != 0;
		}
	};

	TIKI_FORCE_INLINE void parseMatrix( Matrix44& mtx, const char* str, float scale )
	{
		Tokenizer token;
		token.create( str, " \n\t\r", true );

		float* pValues = &mtx.x.x;

		uintreg j = 0u;
		uintreg stringIndex = 0;
		while ( stringIndex != (uintreg)-1 )
		{
			const DynamicString value = token.findNext( &stringIndex );

			if ( !value.isEmpty() )
			{
				const float fval = string_tools::parseFloat32( value.cStr() );

				pValues[ j++ ] = fval;
			}
			TIKI_ASSERT( j <= 16u );
		}

		matrix::transpose( mtx );

		// scale translation
		mtx.w.x *= scale;
		mtx.w.y *= scale;
		mtx.w.z *= scale;
	}

}
