#pragma once
#ifndef TIKI_GENERIC_DATA_RESOURCE_HPP_INCLUDED
#define TIKI_GENERIC_DATA_RESOURCE_HPP_INCLUDED

#include "tiki/base/fourcc.hpp"
#include "tiki/base/types.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	template<typename TData, fourcc TFourCC>
	class GenericDataResource : public Resource
	{
		TIKI_DEFINE_RESOURCE( GenericDataResource, TFourCC );
		TIKI_NONCOPYABLE_CLASS( GenericDataResource );

	public:

		static void		registerResourceType( ResourceManager& resourceManager );
		static void		unregisterResourceType( ResourceManager& resourceManager );

		const TData&	getData() const;

	protected:

		virtual bool	createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext );
		virtual void	disposeInternal( const FactoryContext& factoryContext );

	private:

						GenericDataResource();
		virtual			~GenericDataResource();

		const TData*	m_pData;
	};
}

#include "../../../source/generic_data_resource.inl"

#endif // TIKI_GENERIC_DATA_RESOURCE_HPP_INCLUDED
