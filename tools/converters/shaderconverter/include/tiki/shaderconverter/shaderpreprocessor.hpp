#pragma once
#ifndef TIKI_SHADERPREPROCESSOR_HPP__INCLUDED
#define TIKI_SHADERPREPROCESSOR_HPP__INCLUDED

#include "tiki/base/string.hpp"

namespace tiki
{
	struct ShaderVariant
	{
		crc32	variantHash;

		string	sourceCode;
	};

	class ShaderPreprocessor
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ShaderPreprocessor );

	public:

		void					create( const string& shaderText );
		void					dispose();

		size_t					getVariantCount() const				{ return m_variants.getCount(); }
		const ShaderVariant&	getVariantByIndex( size_t index )	{ return m_variants[ index ]; }

	private:

		Array< ShaderVariant >	m_variants;

	};
}

#endif // TIKI_SHADERPREPROCESSOR_HPP__INCLUDED
