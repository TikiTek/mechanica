
#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/crc32.hpp"

namespace tiki
{
	const SamplerState* GraphicsSystem::createSamplerState( const SamplerStateParamters& creationParameters )
	{
		SamplerState* pSampler = m_samplerStates.findOrAllocate( crcT( &creationParameters ) );
		if ( pSampler != nullptr && pSampler->icCreated() == false )
		{
			pSampler->create( *this, creationParameters );
		}

		return pSampler;
	}

	const SamplerState* GraphicsSystem::createSamplerState( AddressMode addressU, AddressMode addressV, AddressMode addressW, FilterMode magFilter, FilterMode mipFilter, size_t maxAnisotropy /*= 1*/, Color borderColor /*= TIKI_COLOR_BLACK */ )
	{
		SamplerStateParamters creationParameters;
		creationParameters.addressU			= addressU;
		creationParameters.addressV			= addressV;
		creationParameters.addressW			= addressW;
		creationParameters.magFilter		= magFilter;
		creationParameters.mipFilter		= mipFilter;
		creationParameters.maxAnisotropy	= maxAnisotropy;
		creationParameters.borderColor		= borderColor;

		return createSamplerState( creationParameters );
	}

	void GraphicsSystem::disposeSamplerState( const SamplerState* pSamplerState )
	{
		SamplerState* pNonConstState = const_cast< SamplerState* >( pSamplerState );
		if ( pNonConstState->releaseRef() == true )
		{
			pNonConstState->dispose();
		}
	}

	const VertexFormat* GraphicsSystem::createVertexFormat( const VertexFormatParameters& creationParameters )
	{
		const crc32 vertexFormatCrc = crcBytes( creationParameters.pAttributes, sizeof( VertexAttribute ) * creationParameters.attributeCount );

		VertexFormat* pVertexFormat = m_vertexFormats.findOrAllocate( vertexFormatCrc );
		if ( pVertexFormat != nullptr && pVertexFormat->isCreated() == false )
		{
			pVertexFormat->create( creationParameters );
		}

		return pVertexFormat;
	}

	const VertexFormat* GraphicsSystem::createVertexFormat( const VertexAttribute* pVertexAttributes, uint vertexAttrubuteCount )
	{
		VertexFormatParameters creationParameters;
		creationParameters.pAttributes		= pVertexAttributes;
		creationParameters.attributeCount	= vertexAttrubuteCount;

		return createVertexFormat( creationParameters );
	}

	void GraphicsSystem::disposeVertexFormat( const VertexFormat* pVertexFormat )
	{
		VertexFormat* pNonConst = const_cast< VertexFormat* >( pVertexFormat );
		if ( pNonConst->releaseRef() == true )
		{
			pNonConst->dispose();
		}
	}
}