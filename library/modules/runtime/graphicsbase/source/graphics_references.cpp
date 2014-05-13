
#include "tiki/graphics/graphics_references.hpp"

#include "tiki/base/reflection.hpp"

namespace tiki
{
	TIKI_REFLECTION_VALUE_TYPE( TextureReference,	8u, ValueTypeVariant_UnsignedInteger );
	TIKI_REFLECTION_VALUE_TYPE( ModelReference,		8u, ValueTypeVariant_UnsignedInteger );
	TIKI_REFLECTION_VALUE_TYPE( FontReference,		8u, ValueTypeVariant_UnsignedInteger );
	TIKI_REFLECTION_VALUE_TYPE( ShaderSetReference,	8u, ValueTypeVariant_UnsignedInteger );
}