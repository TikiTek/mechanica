#pragma once
#ifndef TIKI_GENERIC_DATA_RESOURCE_INL_INCLUDED
#define TIKI_GENERIC_DATA_RESOURCE_INL_INCLUDED

#include "tiki/resource/resource_file.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	template<typename TData>
	struct GenericDataInitData
	{
		ResRef< const TData > data;
	};

	template<typename TData, fourcc TFourCC>
	GenericDataResource<TData, TFourCC>::~GenericDataResource()
	{
	}

	template<typename TData, fourcc TFourCC>
	GenericDataResource<TData, TFourCC>::GenericDataResource()
	{
	}

	template<typename TData, fourcc TFourCC>
	void GenericDataResource<TData, TFourCC>::registerResourceType( ResourceManager& resourceManager )
	{
		static FactoryContextGenericBase< GenericDataResource< TData, TFourCC > > context = FactoryContextGenericBase< GenericDataResource< TData, TFourCC > >( FlagMask8< ResourceDefinitionFeature >() );
		resourceManager.registerResourceType( s_resourceType, context );
	}

	template<typename TData, fourcc TFourCC>
	void GenericDataResource<TData, TFourCC>::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	template<typename TData, fourcc TFourCC>
	const TData& GenericDataResource<TData, TFourCC>::getData() const
	{
		TIKI_ASSERT( m_pData != nullptr );
		return *m_pData;
	}

	template<typename TData, fourcc TFourCC>
	bool GenericDataResource<TData, TFourCC>::createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		if ( initData.size != sizeof( GenericDataInitData< TData > ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataResource::createInternal] Init data with wrong size.\n" );
			return false;
		}

		const GenericDataInitData< TData >* pInitData = static_cast< const GenericDataInitData< TData >* >( initData.pData );
		if ( pInitData )
		{
			m_pData = pInitData->data.getData();

			return m_pData != nullptr;
		}

		return false;
	}

	template<typename TData, fourcc TFourCC>
	void GenericDataResource<TData, TFourCC>::disposeInternal( const FactoryContext& factoryContext )
	{
		m_pData = nullptr;
	}
}

#endif // TIKI_GENERIC_DATA_RESOURCE_INL_INCLUDED
