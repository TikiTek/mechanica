#ifndef TIKI_MATERIAL_HPP__
#define TIKI_MATERIAL_HPP__

#include "tiki/resource/resource.hpp"

#include "tiki/base/array.hpp"

namespace tiki
{
	class Shader;
	class Texture;

	//class Material : public Resource
	//{
	//	TIKI_DEFINE_RESOURCE( MaterialFactory, TIKI_FOURCC( 'M', 'A', 'T', 'T' ) );
	//	friend class GraphicsContext;
	//public:

	//									Material();
	//	virtual							~Material();

	//	const Shader*					getVertexShader() const { return m_pVertexShader; }

	//private:

	//	bool							initialize( const void* pData );
	//	void							dispose();

	//	struct IATextureResource
	//	{
	//		IATextureResource() : m_pTexture( nullptr ), m_slot( 0u ) { }
	//		const Texture*				m_pTexture;
	//		uint						m_slot;
	//	};

	//	const Shader*					m_pVertexShader;
	//	const Shader*					m_pPixelShader;

	//	Array< IATextureResource >		m_textures;
	//};
}

#endif // TIKI_MATERIAL_HPP__
