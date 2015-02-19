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

#include "tiki/animationconverter/animationconverter.hpp"
#include "tiki/fontconverter/fontconverter.hpp"
#include "tiki/materialconverter/materialconverter.hpp"
#include "tiki/modelconverter/modelconverter.hpp"
#include "tiki/shaderconverter/shaderconverter.hpp"
#include "tiki/textureconverter/textureconverter.hpp"
//#include "tiki/navmeshconverter/navmeshconverter.hpp"

namespace tiki
{
	class AssetConverter : public IAssetConverter
	{
	public:

		AssetConverter();
		~AssetConverter();

		virtual void	create( const AssetConverterParamter& parameters ) TIKI_OVERRIDE;
		virtual void	dispose() TIKI_OVERRIDE;

		virtual bool	convertAll() TIKI_OVERRIDE;

		virtual void	startWatch() TIKI_OVERRIDE;
		virtual void	stopWatch() TIKI_OVERRIDE;
		virtual bool	getChangedFiles( Array< string >& changedFiles ) TIKI_OVERRIDE;

		virtual void	lockConversion() TIKI_OVERRIDE;
		virtual void	unlockConversion() TIKI_OVERRIDE;

	private:

		string				m_sourcePath;

		ConverterManager	m_manager;
		
		Thread				m_watchThread;
		FileWatcher			m_fileWatcher;
		Mutex				m_converterMutex;
		string				m_currentFileName;
		List< string >		m_changedFiles;

		//NavMeshConverter	m_navmeshConverter;
		AnimationConverter	m_animationConverter;
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
