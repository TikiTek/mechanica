#pragma once
#ifndef TIKI_TOOLMODELSOURCE_HPP
#define TIKI_TOOLMODELSOURCE_HPP

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/vertexsemantic.hpp"
#include "tiki/toolbase/tokenizer.hpp"

namespace tiki
{
	static void stringSourceToFloat( float& target, const Array< string >& source )
	{
		target = ParseString::parseSingle( source[ 0u ] );
	}

	static void stringSourceToString( string& target, const Array< string >& source )
	{
		target = source[ 0u ];
	}

	static void stringSourceToMatrix( Matrix44& target, const Array< string >& source )
	{
		float* pMatrix = &target.x.x;
		for (size_t i = 0u; i < 16u; ++i)
		{
			pMatrix[ i ] = ParseString::parseSingle( source[ i ] );
		}
		matrix::transpose( target );
	}

	struct ToolModelSourceBase
	{
		struct Technique
		{
			string	name;
			string	type;
		};

		string				id;
		string				target;
		crc32				crcId;

		size_t				count;
		size_t				stride;
		size_t				userData;
		VertexSementic		semantic;

		Array< Technique >	techniques;
	};

	template<typename T>
	struct ToolModelSource : public ToolModelSourceBase
	{
		typedef void(*ConvertFunc)(T&, const Array< string >&);

		Array< T >			data;

		bool			create( const TikiXml* pXml, const XmlElement* pNode, ConvertFunc pConvertFunc, size_t elementCount )
		{
			string arrayId;
			string techniqueSource;

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

				count			= ParseString::parseUInt32( pCountAtt->content );
				techniqueSource	= string( pSourceAtt->content ).subString( 1u );

				if ( pStrideAtt != nullptr )
				{
					stride	= ParseString::parseUInt32( pStrideAtt->content );
				}
				else
				{
					stride = 0;
				}

				techniques.create( count );

				size_t techIndex = 0u;
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
						string fieldName = string( pNameAtt->content ).toLower();

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
				const XmlAttribute* pIdAtt	= pXml->findAttributeByName( "id", pArrayNode );
				const XmlAttribute* pCountAtt	= pXml->findAttributeByName( "count", pArrayNode );

				arrayId = pIdAtt->content;

				if ( techniqueSource != arrayId )
				{
					TIKI_TRACE_ERROR( "technique type not supported." );
					return false;
				}

				const size_t arrayCount = ParseString::parseUInt32( pCountAtt->content );
				data.create( arrayCount );

				Tokenizer tokenizer;
				tokenizer.create( pArrayNode->content, " \t\n\r", true );

				Array< string > sourceValues;
				sourceValues.create( elementCount );

				size_t i = 0u;
				size_t c = 0u;
				size_t stringIndex = 0; 
				while ( stringIndex != (size_t)-1 )
				{
					string value = tokenizer.findNext( &stringIndex );

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

	TIKI_FORCE_INLINE void parseMatrix( Matrix44& mtx, cstring str, float scale )
	{
		Tokenizer token;
		token.create( str, " \n\t\r", true );

		float* pValues = &mtx.x.x;

		size_t j = 0u;
		size_t stringIndex = 0; 
		while ( stringIndex != (size_t)-1 )
		{
			const string value = token.findNext( &stringIndex );

			if ( !value.isEmpty() )
			{
				const float fval = ParseString::parseSingle( value );

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

#endif // TIKI_TOOLMODELSOURCE_HPP
