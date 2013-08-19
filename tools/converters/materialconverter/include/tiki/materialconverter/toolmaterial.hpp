#ifndef TIKI_TOOLMATERIAL_HPP__
#define TIKI_TOOLMATERIAL_HPP__

#include "tiki/base/list.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/tikixml.hpp"

namespace tiki
{
	struct  TextureAsset
	{
		string	fileName;
		uint	slot;
	};

	class ToolMaterial
	{
	public:
								ToolMaterial();
								~ToolMaterial();

		bool					create( const string& fileName );
		void					dispose();

		List< TextureAsset >&	getAssets() { return m_assets; }

		const string&			getVertexShader() const { return m_vertexShader; }
		const string&			getPixelShader() const { return m_pixelShader; }

	private:

		TikiXml					m_xml;

		string					m_vertexShader;
		string					m_pixelShader;

		List< TextureAsset >	m_assets;
	};
}

#endif // TIKI_TOOLMATERIAL_HPP__
