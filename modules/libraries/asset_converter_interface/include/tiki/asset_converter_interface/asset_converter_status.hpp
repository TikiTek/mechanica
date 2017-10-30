#pragma once
#ifndef TIKI_ASSET_CONVERTER_STATUS_HPP_INCLUDED
#define TIKI_ASSET_CONVERTER_STATUS_HPP_INCLUDED

#include "tiki/asset_converter_interface/asset_converter_types.hpp"
#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	enum AssetStage
	{
		AssetStage_Pending,
		AssetStage_InProgress,
		AssetStage_Failed,
		AssetStage_Succeeded
	};

	struct AssetLog
	{
		TraceLevel	level;
		string		message;
	};

	struct AssetStatus
	{
		uint32				assetId;
		string				assetName;
		Path				inputFilepath;

		AssetStage			stage;
		List< AssetLog >	logs;
	};

	class AssetConverterStatus
	{
		TIKI_NONCOPYABLE_CLASS( AssetConverterStatus );

	public:

		uint32						getBuildId() const { return m_buildId; }
		const List< AssetStatus >&	getAssets() const { return m_assets;  }

	protected:

		uint				m_buildId;
		List< AssetStatus >	m_assets;

							AssetConverterStatus() { }
		virtual				~AssetConverterStatus() { }
	};
}

#endif // TIKI_ASSET_CONVERTER_STATUS_HPP_INCLUDED
