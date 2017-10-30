#pragma once
#ifndef TIKI_ASSET_CONVERTER_HPP
#define TIKI_ASSET_CONVERTER_HPP

#include "tiki/asset_converter_interface/asset_converter_interface.hpp"

#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/converterbase/converter_manager.hpp"
#include "tiki/io/filewatcher.hpp"
#include "tiki/threading/thread.hpp"
#include "tiki/container/list.hpp"


namespace tiki
{
	class AssetConverter : public IAssetConverter
	{
	public:

		AssetConverter();
		~AssetConverter();

		virtual bool	create( const AssetConverterParamter& parameters ) TIKI_OVERRIDE;
		virtual void	dispose() TIKI_OVERRIDE;

		virtual bool	convertAll() TIKI_OVERRIDE;

		virtual void	startWatch() TIKI_OVERRIDE;
		virtual void	stopWatch() TIKI_OVERRIDE;
		virtual bool	getChangedFiles( Array< string >& changedFiles ) TIKI_OVERRIDE;

		virtual void	lockConversion() TIKI_OVERRIDE;
		virtual void	unlockConversion() TIKI_OVERRIDE;

	private:

		string					m_sourcePath;

		ConverterManager		m_manager;

		Thread					m_watchThread;
		FileWatcher				m_fileWatcher;
		Mutex					m_converterMutex;
		string					m_currentFileName;
		List< string >			m_changedFiles;

		void					watchThreadEntryPoint( const Thread& thread );
		static int				watchThreadStaticEntryPoint( const Thread& thread );
	};
}

#endif // TIKI_ASSET_CONVERTER_HPP
