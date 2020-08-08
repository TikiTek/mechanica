#pragma once

#include "tiki/asset_converter_interface/asset_converter_types.hpp"
#include "tiki/base/delegate.hpp"
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
		TraceLevel					level;
		DynamicString				message;
	};

	struct AssetStatus
	{
		uint32						assetId;
		DynamicString				assetName;
		Path						inputFilepath;

		AssetStage					stage;
		List< AssetLog >			logs;
	};

	class AssetConverterStatus
	{
		TIKI_NONCOPYABLE_CLASS( AssetConverterStatus );

	public:

		typedef Delegate< void, int > ChangedDelegate;

		uint32						getBuildId() const { return m_buildId; }
		const List< AssetStatus >&	getAssets() const { return m_assets;  }

		void						addChangedListener( const ChangedDelegate& listener );
		void						removeChangedListener( const ChangedDelegate& listener );

	protected:

		uint32						m_buildId = 0u;
		List< AssetStatus >			m_assets;

		List< ChangedDelegate >		m_changedListener;

									AssetConverterStatus() { }
		virtual						~AssetConverterStatus() { }

		void						notifyChangedListner();
	};
}
