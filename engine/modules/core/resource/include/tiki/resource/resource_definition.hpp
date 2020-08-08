#pragma once

#include "tiki/base/flag_mask.hpp"
#include "tiki/base/platform.hpp"

namespace tiki
{
	enum ResourceDefinitionFeature
	{
		ResourceDefinitionFeature_Platform,
		ResourceDefinitionFeature_GraphicsApi,
		ResourceDefinitionFeature_SoundApi
	};

	enum ResourceDefinitionMask
	{
		ResourceDefinitionMask_Platform_FirstBit	= 0u,
		ResourceDefinitionMask_Platform_BitCount	= 4u,
		ResourceDefinitionMask_Platform_BitMask		= ((1u << ResourceDefinitionMask_Platform_BitCount) - 1u) << ResourceDefinitionMask_Platform_FirstBit,
		ResourceDefinitionMask_GraphicsApi_FirstBit	= ResourceDefinitionMask_Platform_FirstBit + ResourceDefinitionMask_Platform_BitCount + 1u,
		ResourceDefinitionMask_GraphicsApi_BitCount	= 3u,
		ResourceDefinitionMask_GraphicsApi_BitMask	= ((1u << ResourceDefinitionMask_GraphicsApi_BitCount) - 1u) << ResourceDefinitionMask_GraphicsApi_FirstBit,
		ResourceDefinitionMask_SoundApi_FirstBit	= ResourceDefinitionMask_GraphicsApi_FirstBit + ResourceDefinitionMask_GraphicsApi_BitCount + 1u,
		ResourceDefinitionMask_SoundApi_BitCount	= 2u,
		ResourceDefinitionMask_SoundApi_BitMask		= ((1u << ResourceDefinitionMask_SoundApi_BitCount) - 1u) << ResourceDefinitionMask_SoundApi_FirstBit,
	};
	TIKI_COMPILETIME_ASSERT( 1u << ResourceDefinitionMask_Platform_BitCount >= PlatformType_Count );
	TIKI_COMPILETIME_ASSERT( 1u << ResourceDefinitionMask_GraphicsApi_BitCount >= GraphicsApi_Count );
	TIKI_COMPILETIME_ASSERT( 1u << ResourceDefinitionMask_SoundApi_BitCount >= SoundApi_Count );

	class ResourceDefinition
	{
	public:

		TIKI_INLINE ResourceDefinition()
		{
			m_definitionMask = 0u;
		}

		TIKI_INLINE ResourceDefinition( uint32 definitionMask )
		{
			m_definitionMask = definitionMask;
		}

		TIKI_INLINE void applyHostValues()
		{
			setPlatformType( platform::getHostPlatform() );
			setGraphicsApi( platform::getHostGraphicsApi() );
			setSoundApi( platform::getHostSoundApi() );
		}

		TIKI_INLINE bool isCompatibleWithHost( FlagMask8< ResourceDefinitionFeature > features ) const
		{
			bool compatible = true;

			if( features.isFlagSet( ResourceDefinitionFeature_Platform ) )
			{
				const PlatformType platformType = getPlatformType();
				compatible &= (platformType == PlatformType_Invalid || platformType == platform::getHostPlatform());
			}

			if( features.isFlagSet( ResourceDefinitionFeature_GraphicsApi ) )
			{
				const GraphicsApi graphicsApi = getGraphicsApi();
				compatible &= (graphicsApi == GraphicsApi_Invalid || graphicsApi == platform::getHostGraphicsApi());
			}

			if( features.isFlagSet( ResourceDefinitionFeature_SoundApi ) )
			{
				const SoundApi soundApi = getSoundApi();
				compatible &= (soundApi == SoundApi_Invalid || soundApi == platform::getHostSoundApi());
			}

			return compatible;
		}

		TIKI_INLINE PlatformType getPlatformType() const
		{
			return (PlatformType)((m_definitionMask & ResourceDefinitionMask_Platform_BitMask) >> ResourceDefinitionMask_Platform_FirstBit);
		}

		TIKI_INLINE void setPlatformType(PlatformType val)
		{
			m_definitionMask = m_definitionMask & ~ResourceDefinitionMask_Platform_BitMask;
			m_definitionMask |= val << ResourceDefinitionMask_Platform_FirstBit;
		}

		TIKI_INLINE GraphicsApi getGraphicsApi() const
		{
			return (GraphicsApi)((m_definitionMask & ResourceDefinitionMask_GraphicsApi_BitMask) >> ResourceDefinitionMask_GraphicsApi_FirstBit);
		}

		TIKI_INLINE void setGraphicsApi(GraphicsApi val)
		{
			m_definitionMask = m_definitionMask & ~ResourceDefinitionMask_GraphicsApi_BitMask;
			m_definitionMask |= val << ResourceDefinitionMask_GraphicsApi_FirstBit;
		}

		TIKI_INLINE SoundApi getSoundApi() const
		{
			return (SoundApi)((m_definitionMask & ResourceDefinitionMask_SoundApi_BitMask) >> ResourceDefinitionMask_SoundApi_FirstBit);
		}

		TIKI_INLINE void setSoundApi( SoundApi val )
		{
			m_definitionMask = m_definitionMask & ~ResourceDefinitionMask_SoundApi_BitMask;
			m_definitionMask |= val << ResourceDefinitionMask_SoundApi_FirstBit;
		}

		TIKI_INLINE bool operator==( const ResourceDefinition& rhs ) const
		{
			return m_definitionMask == rhs.m_definitionMask;
		}

	private:

		uint32 m_definitionMask;
	};
}
