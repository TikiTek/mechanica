#ifndef TIKI_PRIMITIVEFACTORY_HPP__
#define TIKI_PRIMITIVEFACTORY_HPP__

#include "tiki/base/types.hpp"
#include "tiki/base/array.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/graphics/vertexcolor.hpp"

namespace tiki
{
	class Mesh;

	class Primitive
	{
	public:
		static void	createCube( Mesh* pMesh );
		static void	createPlane( Mesh* pMesh );
		static void createCheckerBoard( Mesh* pMesh, 
										const float scaleX = 1.0f,
										const float scaleY = 1.0f,
										const uint subDivX = 1u, 
										const uint subDivY = 1u,
										const Color& color1 = Color::white, 
										const Color& color2 = Color::tikiBlue);
	};
}

#endif // TIKI_PRIMITIVEFACTORY_HPP__
