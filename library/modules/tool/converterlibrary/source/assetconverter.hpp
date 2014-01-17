#pragma once
#ifndef TIKI_ASSETCONVERTER_HPP
#define TIKI_ASSETCONVERTER_HPP

#include "tiki/toollibrarys/iassetconverter.hpp"

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/toolbase/list.hpp"

#include "tiki/fontconverter/fontconverter.hpp"
#include "tiki/materialconverter/materialconverter.hpp"
#include "tiki/modelconverter/modelconverter.hpp"
#include "tiki/shaderconverter/shaderconverter.hpp"
#include "tiki/textureconverter/textureconverter.hpp"
//#include "tiki/animationconverter/animationconverter.hpp"
//#include "tiki/navmeshconverter/navmeshconverter.hpp"

namespace tiki
{
	class AssetConverter : public IAssetConverter
	{
	public:

		AssetConverter();

		void				create( const AssetConverterParamter& parameters );
		void				dispose();

		int					run();

	private:

		string				m_sourcePath;

		ConverterManager	m_manager;

		//AnimationConverter	m_animationConverter;
		//NavMeshConverter	m_navmeshConverter;
		FontConverter		m_fontConverter;
		MaterialConverter	m_materialConverter;
		ModelConverter		m_modelConverter;
		ShaderConverter		m_shaderConverter;
		TextureConverter	m_textureConverter;

		void				resolveDependencies();
		void				findFiles( const string& path, List< string >& files, const string& ext ) const;

	};
}

#endif // TIKI_ASSETCONVERTER_HPP
