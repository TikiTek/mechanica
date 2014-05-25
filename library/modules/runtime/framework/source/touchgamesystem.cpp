
#include "tiki/framework/touchgamesystem.hpp"

#include "tiki/graphics/texture.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	TouchGameSystem::TouchGameSystem()
	{
		m_pPadTexture		= nullptr;
		m_pPadPointTexture	= nullptr;
		m_pPointTexture		= nullptr;
	}

	TouchGameSystem::~TouchGameSystem()
	{
		TIKI_ASSERT( m_pPadTexture		== nullptr );
		TIKI_ASSERT( m_pPadPointTexture	== nullptr );
		TIKI_ASSERT( m_pPointTexture	== nullptr );
	}

	bool TouchGameSystem::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_pPadTexture		= resourceManager.loadResource< Texture >( "touch_dpad.texture" );
		m_pPadPointTexture	= resourceManager.loadResource< Texture >( "touch_dpad_point.texture" );
		m_pPointTexture		= resourceManager.loadResource< Texture >( "touch_point.texture" );

		if ( m_pPadTexture == nullptr || m_pPadPointTexture == nullptr || m_pPointTexture == nullptr )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}
		
		vector::set( m_pointSize, m_pPointTexture->getTextureData().getWidth(), m_pPointTexture->getTextureData().getHeight() );
		m_halfPointSize	= m_pointSize;
		vector::scale( m_halfPointSize, 0.5f );

		if ( !m_renderer.create( graphicsSystem, resourceManager ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		for (uint i = 0u; i < m_touchPoints.getCount(); ++i)
		{
			TouchPoint& point = m_touchPoints[ i ];

			point.isDown	= false;
			point.isOnPad	= false;
			point.scale		= 0.0f;
			vector::clear( point.position );
		}

		return true;
	}

	void TouchGameSystem::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_renderer.dispose( graphicsSystem, resourceManager );

		resourceManager.unloadResource( m_pPointTexture );
		resourceManager.unloadResource( m_pPadPointTexture );
		resourceManager.unloadResource( m_pPadTexture );
		m_pPadTexture		= nullptr;
		m_pPadPointTexture	= nullptr;
		m_pPointTexture		= nullptr;
	}

	void TouchGameSystem::update( float timeDelta )
	{
		static float time = 0.0f;
		time += timeDelta;

		const float halfTimeDelta = timeDelta / 1.0f;
		for (uint i = 0u; i < m_touchPoints.getCount(); ++i)
		{
			TouchPoint& point = m_touchPoints[ i ];

			if ( !point.isDown )
			{
				point.scale -= halfTimeDelta;
			}
			//else
			//{
			//	point.scale = 1.0f;
			//}

			//point.isDown	= !bool( int( time + i ) % 5 );
			//point.isOnPad	= bool( i & 2 );
			//point.position.x = 50.0f + ( i * 60.0f );
			//point.position.y = 50.0f + ( i * 60.0f );
		} 
	}

	void TouchGameSystem::render( GraphicsContext& graphicsContext ) const
	{
		m_renderer.beginRendering( graphicsContext );
		m_renderer.beginRenderPass();
		
		const float width = float( graphicsContext.getBackBuffer().getWidth() );
		const float height = float( graphicsContext.getBackBuffer().getHeight() );
		const float globalScale = width / 4096.0f;

		const float padWidth = float( m_pPadTexture->getTextureData().getWidth() );
		const float padHeight = float( m_pPadTexture->getTextureData().getHeight() );

		const Rectangle leftPadRect = Rectangle(
			padWidth * 0.5f * globalScale,
			height - ( padHeight * 1.5f * globalScale ),
			padWidth * globalScale,
			padHeight * globalScale
		);

		const Rectangle rightPadRect = Rectangle(
			width - ( padWidth * 1.5f * globalScale ),
			height - ( padHeight * 1.5f * globalScale ),
			padWidth * globalScale,
			padHeight * globalScale
		);

		m_renderer.drawTexturedRectangle( m_pPadTexture->getTextureData(), leftPadRect, TIKI_COLOR_WHITE );
		m_renderer.drawTexturedRectangle( m_pPadTexture->getTextureData(), rightPadRect, TIKI_COLOR_WHITE );

		for (uint i = 0u; i < m_touchPoints.getCount(); ++i)
		{
			const TouchPoint& point = m_touchPoints[ i ];

			if ( point.scale > 0.0f )
			{
				const float pointScale = globalScale * point.scale;

				const Rectangle destinationRectangle = Rectangle(
					point.position.x - ( m_halfPointSize.x * pointScale ),
					point.position.y - ( m_halfPointSize.y * pointScale ),
					m_pointSize.x * pointScale,
					m_pointSize.y * pointScale
				);

				const Color color = color::fromFloatRGBA( 1.0f, 1.0f, 1.0f, point.scale * point.scale );
				m_renderer.drawTexturedRectangle(
					( point.isOnPad ? m_pPadPointTexture : m_pPointTexture )->getTextureData(),
					destinationRectangle,
					color
				);
			}
		} 

		m_renderer.endRenderPass();
		m_renderer.endRendering();
	}

	bool TouchGameSystem::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.deviceType != InputDeviceType_Touch )
		{
			return false;
		}

		const InputEventTouchData& data = inputEvent.data.touch;
		vector::set( m_touchPoints[ data.pointIndex ].position, float( data.xState ), float( data.yState ) );

		switch ( inputEvent.eventType )
		{
		case InputEventType_Touch_PointDown:
			m_touchPoints[ data.pointIndex ].isDown = true;
			m_touchPoints[ data.pointIndex ].scale = 1.0f;
			break;

		case InputEventType_Touch_PointUp:
			m_touchPoints[ data.pointIndex ].isDown = false;
			break;

		case InputEventType_Touch_PointMove:
			break;
		}

		return false;
	}
}