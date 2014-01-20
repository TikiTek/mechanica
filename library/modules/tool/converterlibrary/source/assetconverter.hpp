#pragma once
#ifndef TIKI_ASSETCONVERTER_HPP
#define TIKI_ASSETCONVERTER_HPP

#include "tiki/toollibrarys/iassetconverter.hpp"

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/io/filewatcher.hpp"
#include "tiki/threading/thread.hpp"
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

		virtual void	create( const AssetConverterParamter& parameters );
		virtual void	dispose();

		virtual int		convertAll();

		virtual void	startWatch();
		virtual void	stopWatch();
		virtual bool	getChangedFiles( Array< string >& changedFiles );

		virtual void	lockConversion();
		virtual void	unlockConversion();

	private:

		string				m_sourcePath;

		ConverterManager	m_manager;
		
		Thread				m_watchThread;
		FileWatcher			m_fileWatcher;
		Mutex				m_converterMutex;
		string				m_currentFileName;
		List< string >		m_changedFiles;

		//AnimationConverter	m_animationConverter;
		//NavMeshConverter	m_navmeshConverter;
		FontConverter		m_fontConverter;
		MaterialConverter	m_materialConverter;
		ModelConverter		m_modelConverter;
		ShaderConverter		m_shaderConverter;
		TextureConverter	m_textureConverter;

		void				findFiles( const string& path, List< string >& files, const string& ext ) const;

		void				watchThreadEntryPoint( const Thread& thread );
		static int			watchThreadStaticEntryPoint( const Thread& thread );

	};
}

#endif // TIKI_ASSETCONVERTER_HPP
