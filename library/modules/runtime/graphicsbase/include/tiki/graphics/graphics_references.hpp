#pragma once
#ifndef __TIKI_GRAPHICS_REFERENCES_HPP_INCLUDED__
#define __TIKI_GRAPHICS_REFERENCES_HPP_INCLUDED__

#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	class Texture;
	class Model;
	class Font;
	class ShaderSet;

	typedef ResRef< Texture >		TextureReference;
	typedef ResRef< Model >			ModelReference;
	typedef ResRef< Font >			FontReference;
	typedef ResRef< ShaderSet >		ShaderSetReference;
}

#endif // __TIKI_GRAPHICS_REFERENCES_HPP_INCLUDED__
