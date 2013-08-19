
#include "tiki/graphics/linerenderer.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphicsbase/primitivetopologies.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	LineRenderer::LineRenderer( void )
		: m_pVertexShader( nullptr ), m_pPixelShader( nullptr ), m_pContext( nullptr ),
		  m_pVertexFormat( nullptr ), m_lineCount( 0u ), m_maxLines( 0u )
	{
	}

	LineRenderer::~LineRenderer( void )
	{
		TIKI_ASSERT( m_pVertexShader == nullptr );
		TIKI_ASSERT( m_pPixelShader == nullptr );
		TIKI_ASSERT( m_pVertexFormat == nullptr );


		TIKI_ASSERT( m_lines.getCount() == 0u );
		TIKI_ASSERT( m_vertexBuffer.getCount() == 0u );
	}

	bool LineRenderer::create( GpuContext* pContext, const uint maxLines /*= 150u*/ )
	{
		TIKI_ASSERT( pContext );
		TIKI_ASSERT( m_pVertexShader == nullptr );
		TIKI_ASSERT( m_pPixelShader == nullptr );
		TIKI_ASSERT( m_pVertexFormat == nullptr );
		TIKI_ASSERT( maxLines > 0u );

		m_pContext = pContext;

		ResourceManager& resourceManager = framework::getResourceManager();
		m_pVertexShader	= resourceManager.loadResource< Shader >("line.vs_5_0");
		m_pPixelShader	= resourceManager.loadResource< Shader >("line.ps_5_0");

		TIKI_ASSERT( m_pVertexShader != nullptr );
		TIKI_ASSERT( m_pPixelShader != nullptr );

		VertexAttribute attributes[ 2u ] =
		{
			{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
			{ VertexSementic_Color,		0u,	VertexAttributeFormat_x32y32z32w32_float,	0u, VertexInputType_PerVertex }
		};		

		VertexFormatParameters vertexParams;
		vertexParams.pAttributes	= attributes;
		vertexParams.attributeCount	= TIKI_COUNT( attributes );
		vertexParams.pShader		= m_pVertexShader;

		m_pVertexFormat = VertexFormat::getVertexFormat( vertexParams );

		m_maxLines = maxLines * 2u;
		m_lines.create( m_maxLines );

		return true;
	}

	void LineRenderer::dispose( void )
	{
		TIKI_ASSERT( m_pVertexShader != nullptr );
		TIKI_ASSERT( m_pPixelShader != nullptr );
		TIKI_ASSERT( m_pVertexFormat != nullptr );

		ResourceManager& resourceManager = framework::getResourceManager();

		resourceManager.unloadResource( m_pVertexShader );
		resourceManager.unloadResource( m_pPixelShader );

		VertexFormat::releaseVertexFormat( m_pVertexFormat );
		m_pVertexFormat	= nullptr;
		m_pVertexShader	= nullptr;
		m_pPixelShader	= nullptr;

		m_lines.dispose();
		m_vertexBuffer.dispose();

		m_lineCount = 0u;
	}

	void LineRenderer::drawLine( const Vector3& start, const Vector3& end, const Color& color )
	{
		if( m_maxLines == m_lineCount )
			flush();

		m_lines[ m_lineCount++ ] = VertexColor( start, color );
		m_lines[ m_lineCount++ ] = VertexColor( end, color );
	}

	void LineRenderer::flush( void )
	{
		if( m_lineCount == 0u )
			return;

		VertexColor* mapping = m_vertexBuffer.map( m_lineCount );
		memcpy( mapping, m_lines.getData(), sizeof( VertexColor ) * m_lineCount );
		m_vertexBuffer.unmap();

		m_pContext->setInputLayout( m_pVertexFormat );
		m_pContext->setVertexShader( m_pVertexShader );
		m_pContext->setPixelShader( m_pPixelShader );

		m_pContext->setVertexBuffer( m_vertexBuffer );
		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->draw( m_lineCount );

		m_lineCount = 0u;
	}

}