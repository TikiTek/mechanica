#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/list.hpp"
#include "tiki/graphics/shader_type.hpp"

namespace tiki
{
	struct ShaderVariant
	{
		crc32			bitMask;
		DynamicString	defineCode;
	};

	class ShaderPreprocessor
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ShaderPreprocessor );

	public:

		void					create( const DynamicString& shaderText, const List< DynamicString >& includePathes );
		void					dispose();

		bool					isTypeEnabled( ShaderType type ) const				{ return m_variants[ type ].getCount() != 0u; }
		uintreg					getVariantCount( ShaderType type ) const			{ return m_variants[ type ].getCount(); }
		const ShaderVariant&	getVariantByIndex( ShaderType type, uintreg index )	{ return m_variants[ type ][ index ]; }

		const DynamicString&	getSourceCode() const								{ return m_sourceCode; };

	private:

		DynamicString			m_sourceCode;

		Array< ShaderVariant >	m_variants[ ShaderType_Count ];
	};
}
