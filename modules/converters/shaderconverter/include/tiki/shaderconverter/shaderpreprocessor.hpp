#pragma once
#ifndef TIKI_SHADERPREPROCESSOR_HPP__INCLUDED
#define TIKI_SHADERPREPROCESSOR_HPP__INCLUDED

#include "tiki/base/string.hpp"
#include "tiki/graphics/shadertype.hpp"

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

		void					create( const string& shaderText );
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

#endif // TIKI_SHADERPREPROCESSOR_HPP__INCLUDED
