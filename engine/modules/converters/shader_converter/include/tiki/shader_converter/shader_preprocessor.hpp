#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/list.hpp"
#include "tiki/graphics/shader_type.hpp"

namespace tiki
{
	struct ShaderVariant
	{
		crc32	bitMask;
		string	defineCode;
	};

	class ShaderPreprocessor
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ShaderPreprocessor );

	public:

		void					create( const string& shaderText, const List< string >& includePathes );
		void					dispose();

		bool					isTypeEnabled( ShaderType type ) const				{ return m_variants[ type ].getCount() != 0u; }
		size_t					getVariantCount( ShaderType type ) const			{ return m_variants[ type ].getCount(); }
		const ShaderVariant&	getVariantByIndex( ShaderType type, size_t index )	{ return m_variants[ type ][ index ]; }

		const string&			getSourceCode() const								{ return m_sourceCode; };

	private:

		string					m_sourceCode;

		Array< ShaderVariant >	m_variants[ ShaderType_Count ];
	};
}
