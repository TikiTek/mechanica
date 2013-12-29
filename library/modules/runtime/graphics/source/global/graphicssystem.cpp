
#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/crc32.hpp"

namespace tiki
{
	GraphicsSystem::GraphicsSystem()
	{
		for (uint i = 0u; i < TIKI_COUNT( m_pStockVertexFormats ); ++i)
		{
			m_pStockVertexFormats[ i ] = nullptr;
		}
	}

	GraphicsSystem::~GraphicsSystem()
	{
	}

	bool GraphicsSystem::create( const GraphicsSystemParameters& params )
	{
		bool result = createPlatform( params );

		if ( result )
		{
			m_shaders.create( 1024u );

			m_samplerStates.create( 32u );
			m_vertexFormats.create( 32u );
			m_vertexInputBindings.create( 32u );

			{
				VertexAttribute attributes_pos2[] =
				{
					{ VertexSementic_Position, 0u, VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex }
				};

				VertexAttribute attributes_pos2tex2[] =
				{
					{ VertexSementic_Position, 0u, VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex },
					{ VertexSementic_TexCoord, 0u, VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex }
				};

				VertexAttribute attributes_pos3[] =
				{
					{ VertexSementic_Position, 0u, VertexAttributeFormat_x32y32z32_float,	0u, VertexInputType_PerVertex }
				};

				VertexAttribute attributes_pos3tex2[] =
				{
					{ VertexSementic_Position, 0u, VertexAttributeFormat_x32y32z32_float,	0u, VertexInputType_PerVertex },
					{ VertexSementic_TexCoord, 0u, VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex }
				};

				m_pStockVertexFormats[ StockVertexFormat_Pos2 ]		= createVertexFormat( attributes_pos2,		TIKI_COUNT( attributes_pos2 ) );
				m_pStockVertexFormats[ StockVertexFormat_Pos2Tex2 ]	= createVertexFormat( attributes_pos2tex2,	TIKI_COUNT( attributes_pos2tex2 ) );
				m_pStockVertexFormats[ StockVertexFormat_Pos3 ]		= createVertexFormat( attributes_pos3,		TIKI_COUNT( attributes_pos3 ) );
				m_pStockVertexFormats[ StockVertexFormat_Pos3Tex2 ]	= createVertexFormat( attributes_pos3tex2,	TIKI_COUNT( attributes_pos3tex2 ) );
			}
		}

		return result;
	}

	void GraphicsSystem::dispose()
	{
		for (uint i = 0u; i < TIKI_COUNT( m_pStockVertexFormats ); ++i)
		{
			if ( m_pStockVertexFormats[ i ] != nullptr )
			{
				disposeVertexFormat( m_pStockVertexFormats[ i ] );
				m_pStockVertexFormats[ i ] = nullptr;
			}
		} 

		m_samplerStates.dispose();
		m_vertexFormats.dispose();
		m_vertexInputBindings.dispose();

		m_shaders.dispose();

		disposePlatform();
	}

	const Shader* GraphicsSystem::createShader( ShaderType type, const void* pData, uint dataSize )
	{
		Shader* pShader = m_shaders.allocate();

		if ( pShader != nullptr && pShader->create( *this, type, pData, dataSize ) == false )
		{
			m_shaders.free( pShader );
			pShader = nullptr;
		}

		return pShader;
	}

	void GraphicsSystem::disposeShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );

		Shader* pNonConstShader = const_cast< Shader* >( pShader );
		pNonConstShader->dispose();		
		m_shaders.free( pNonConstShader );
	}

	const SamplerState* GraphicsSystem::createSamplerState( const SamplerStateParamters& creationParameters )
	{
		SamplerState* pSampler = m_samplerStates.findOrAllocate( crcT( &creationParameters ) );
		if ( pSampler != nullptr && pSampler->isCreated() == false )
		{
			if ( pSampler->create( *this, creationParameters ) == false )
			{
				pSampler = nullptr;
			}
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
		if ( pNonConstState->releaseRef() == false )
		{
			pNonConstState->dispose();
		}
	}

	const VertexFormat* GraphicsSystem::createVertexFormat( const VertexFormatParameters& creationParameters )
	{
		TIKI_ASSERT( creationParameters.pAttributes != 0u );
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
		if ( pNonConst->releaseRef() == false )
		{
			pNonConst->dispose();
		}
	}

	const VertexInputBinding* GraphicsSystem::createVertexInputBinding( const VertexInputBindingParameters& parameters )
	{
		TIKI_ASSERT( parameters.pShader != nullptr );
		TIKI_ASSERT( parameters.pVertexFormat != nullptr );

		const uint32 hashData[]	= { parameters.pShader->getShaderHash(), parameters.pVertexFormat->getHashValue() };
		const crc32 hashValue	= crcBytes( hashData, sizeof( hashData ) );

		VertexInputBinding* pVertexInputBinding = m_vertexInputBindings.findOrAllocate( hashValue );
		if ( pVertexInputBinding != nullptr && pVertexInputBinding->isCreated() == false )
		{
			if ( pVertexInputBinding->create( parameters ) == false )
			{
				pVertexInputBinding = nullptr;
			}
		}

		return pVertexInputBinding;
	}

	const VertexInputBinding* GraphicsSystem::createVertexInputBinding( const Shader* pShader, const VertexFormat* pVertexFormat )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pVertexFormat != nullptr );

		VertexInputBindingParameters parameters;
		parameters.pShader			= pShader;
		parameters.pVertexFormat	= pVertexFormat;

		return createVertexInputBinding( parameters );
	}

	void GraphicsSystem::disposeVertexInputBinding( const VertexInputBinding* pVertexInputBinding )
	{
		VertexInputBinding* pNonConst = const_cast< VertexInputBinding* >( pVertexInputBinding );
		if ( pNonConst->releaseRef() == false )
		{
			pNonConst->dispose();
		}
	}

	const VertexFormat* GraphicsSystem::getStockVertexFormat( StockVertexFormat format ) const
	{
		TIKI_ASSERT( format < StockVertexFormat_Count );
		return m_pStockVertexFormats[ format ];
	}
}