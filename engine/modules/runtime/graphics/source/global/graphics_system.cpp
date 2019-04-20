#include "tiki/graphics/graphics_system.hpp"

#include "tiki/base/crc32.hpp"

namespace tiki
{
	GraphicsSystem::GraphicsSystem()
	{
		TIKI_COMPILETIME_ASSERT( TIKI_OFFSETOF( GraphicsSystem, m_platformData ) == sizeof(uint) );

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
			m_blendStates.create( MaxBlendStateCount );
			m_depthStencilStates.create( MaxDepthStencilStateCount );
			m_rasterizerStates.create( MaxRasterizerStateCount );
			m_samplerStates.create( MaxSamplerStateCount );
			m_vertexFormats.create( MaxVertexFormatCount );
			m_vertexInputBindings.create( MaxVertexInputBindingCount );

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

		m_blendStates.dispose();
		m_depthStencilStates.dispose();
		m_rasterizerStates.dispose();
		m_samplerStates.dispose();
		m_vertexFormats.dispose();
		m_vertexInputBindings.dispose();

		disposePlatform();
	}

	const BlendState* GraphicsSystem::createBlendState( const BlendStateParamters& creationParameters )
	{
		BlendState* pState = m_blendStates.findOrAllocate( crcTemplate( creationParameters ) );
		if ( pState != nullptr && pState->isCreated() == false )
		{
			if ( pState->create( *this, creationParameters ) == false )
			{
				pState = nullptr;
			}
		}

		return pState;
	}

	const BlendState* GraphicsSystem::createBlendState( bool blendEnabled, Blend sourceBlend /* = Blend_One */, Blend destinationBlend /* = Blend_Zero */, BlendOperation operation /* = BlendOperation_Add */, ColorWriteMask colorWriteMask /* = ColorWriteMask_All */ )
	{
		BlendStateParamters creationParameters;
		creationParameters.blendEnabled		= blendEnabled;
		creationParameters.sourceBlend		= sourceBlend;
		creationParameters.destinationBlend	= destinationBlend;
		creationParameters.operation		= operation;
		creationParameters.colorWriteMask	= colorWriteMask;

		return createBlendState( creationParameters );
	}

	void GraphicsSystem::disposeBlendState( const BlendState*& pBlendState )
	{
		if ( pBlendState == nullptr )
		{
			return;
		}

		BlendState* pNonConstState = const_cast< BlendState* >( pBlendState );
		if ( pNonConstState->releaseRef() == false )
		{
			pNonConstState->dispose();
		}

		pBlendState = nullptr;
	}

	const DepthStencilState* GraphicsSystem::createDepthStencilState( const DepthStencilStateParamters& creationParameters )
	{
		DepthStencilState* pState = m_depthStencilStates.findOrAllocate( crcTemplate( creationParameters ) );
		if ( pState != nullptr && pState->isCreated() == false )
		{
			if ( pState->create( *this, creationParameters ) == false )
			{
				pState = nullptr;
			}
		}

		return pState;
	}

	const DepthStencilState* GraphicsSystem::createDepthStencilState( bool depthEnbaled, bool depthWriteEnabled )
	{
		DepthStencilStateParamters creationParameters;
		creationParameters.depthEnabled			= depthEnbaled;
		creationParameters.depthWriteEnabled	= depthWriteEnabled;

		return createDepthStencilState( creationParameters );
	}

	void GraphicsSystem::disposeDepthStencilState( const DepthStencilState*& pDepthStencilState )
	{
		if ( pDepthStencilState == nullptr )
		{
			return;
		}

		DepthStencilState* pNonConstState = const_cast< DepthStencilState* >( pDepthStencilState );
		if ( pNonConstState->releaseRef() == false )
		{
			pNonConstState->dispose( *this );
		}

		pDepthStencilState = nullptr;
	}

	const RasterizerState* GraphicsSystem::createRasterizerState( const RasterizerStateParamters& creationParameters )
	{
		RasterizerState* pState = m_rasterizerStates.findOrAllocate( crcTemplate( creationParameters ) );
		if ( pState != nullptr && pState->isCreated() == false )
		{
			if ( pState->create( *this, creationParameters ) == false )
			{
				pState = nullptr;
			}
		}

		return pState;
	}

	const RasterizerState* GraphicsSystem::createRasterizerState( FillMode fillMode /* = FillMode_Solid */, CullMode cullMode /* = CullMode_Back */, WindingOrder windingOrder /* = WindingOrder_Clockwise */ )
	{
		RasterizerStateParamters creationParameters;
		creationParameters.fillMode		= fillMode;
		creationParameters.cullMode		= cullMode;
		creationParameters.windingOrder	= windingOrder;

		return createRasterizerState( creationParameters );
	}

	void GraphicsSystem::disposeRasterizerState( const RasterizerState*& pRasterizerState )
	{
		if ( pRasterizerState == nullptr )
		{
			return;
		}

		RasterizerState* pNonConstState = const_cast< RasterizerState* >( pRasterizerState );
		if ( pNonConstState->releaseRef() == false )
		{
			pNonConstState->dispose( *this );
		}

		pRasterizerState = nullptr;
	}

	const SamplerState* GraphicsSystem::createSamplerState( const SamplerStateParamters& creationParameters )
	{
		SamplerState* pState = m_samplerStates.findOrAllocate( crcTemplate( creationParameters ) );
		if ( pState != nullptr && pState->isCreated() == false )
		{
			if ( pState->create( *this, creationParameters ) == false )
			{
				pState = nullptr;
			}
		}

		return pState;
	}

	const SamplerState* GraphicsSystem::createSamplerState( AddressMode addressU /* = AddressMode_Clamp */, AddressMode addressV /* = AddressMode_Clamp */, AddressMode addressW /* = AddressMode_Clamp */, FilterMode magFilter /* = FilterMode_Linear */, FilterMode mipFilter /* = FilterMode_Linear */, size_t maxAnisotropy /* = 1 */, Color borderColor /* = TIKI_COLOR_BLACK */ )
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

	void GraphicsSystem::disposeSamplerState( const SamplerState*& pSamplerState )
	{
		if ( pSamplerState == nullptr )
		{
			return;
		}

		SamplerState* pNonConstState = const_cast< SamplerState* >( pSamplerState );
		if ( pNonConstState->releaseRef() == false )
		{
			pNonConstState->dispose( *this );
		}

		pSamplerState = nullptr;
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

	void GraphicsSystem::disposeVertexFormat( const VertexFormat*& pVertexFormat )
	{
		if ( pVertexFormat == nullptr )
		{
			return;
		}

		VertexFormat* pNonConst = const_cast< VertexFormat* >( pVertexFormat );
		if ( !pNonConst->releaseRef() )
		{
			pNonConst->dispose();
		}

		pVertexFormat = nullptr;
	}

	const VertexInputBinding* GraphicsSystem::createVertexInputBinding( const VertexInputBindingParameters& parameters )
	{
		if( parameters.pShader == nullptr || parameters.pVertexFormat == nullptr )
		{
			return nullptr;
		}

		const uint32 hashData[]	= { parameters.pShader->getShaderHash(), parameters.pVertexFormat->getHashValue() };
		const crc32 hashValue	= crcBytes( hashData, sizeof( hashData ) );

		VertexInputBinding* pVertexInputBinding = m_vertexInputBindings.findOrAllocate( hashValue );
		if ( pVertexInputBinding != nullptr && !pVertexInputBinding->isCreated() )
		{
			if ( !pVertexInputBinding->create( *this, parameters ) )
			{
				pVertexInputBinding = nullptr;
			}
		}

		return pVertexInputBinding;
	}

	const VertexInputBinding* GraphicsSystem::createVertexInputBinding( const Shader* pShader, const VertexFormat* pVertexFormat )
	{
		if( pShader == nullptr ||
			pVertexFormat != nullptr )
		{
			return nullptr;
		}

		VertexInputBindingParameters parameters;
		parameters.pShader			= pShader;
		parameters.pVertexFormat	= pVertexFormat;

		return createVertexInputBinding( parameters );
	}

	void GraphicsSystem::disposeVertexInputBinding( const VertexInputBinding*& pVertexInputBinding )
	{
		if ( pVertexInputBinding == nullptr )
		{
			return;
		}

		VertexInputBinding* pNonConst = const_cast< VertexInputBinding* >( pVertexInputBinding );
		if ( pNonConst->releaseRef() == false )
		{
			pNonConst->dispose( *this );
		}

		pVertexInputBinding = nullptr;
	}

	const VertexFormat* GraphicsSystem::getStockVertexFormat( StockVertexFormat format ) const
	{
		TIKI_ASSERT( format < StockVertexFormat_Count );
		return m_pStockVertexFormats[ format ];
	}
}