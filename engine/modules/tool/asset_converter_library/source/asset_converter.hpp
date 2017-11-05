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
	class AssetConverter : public AssetConverterInterface
	{
	public:

						AssetConverter();
		virtual			~AssetConverter();

		virtual bool	create( const AssetConverterParamter& parameters );
		virtual void	dispose();

		virtual bool	convertAll() TIKI_OVERRIDE_FINAL;

		virtual void	startWatch() TIKI_OVERRIDE_FINAL;
		virtual void	stopWatch() TIKI_OVERRIDE_FINAL;

		virtual bool	getChangedFiles( Array< string >& changedFiles ) TIKI_OVERRIDE_FINAL;

	private:

		PacketManager*			m_pPacketManager;
		string					m_sourcePath;

		ConverterManager		m_manager;

		Mutex					m_watcherMutex;
		Thread					m_watcherThread;
		FileWatcher				m_watcher;

		Mutex					m_changedFilesMutex;
		List< string >			m_changedFiles;

		void					watchThreadEntryPoint( const Thread& thread );
		static int				watchThreadStaticEntryPoint( const Thread& thread );
	};
}

#endif // TIKI_ASSET_CONVERTER_HPP
