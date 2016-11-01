#include "tiki/framework/touchgamesystem.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/math/axisalignedrectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_BOOL( s_convertMouseToTouchEvents, "ConvertMouseToTouchEvents", false );

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
		m_isEnabled = false;

		m_pGraphicsSystem = &graphicsSystem;

		m_pPadTexture		= resourceManager.loadResource< Texture >( "touch_dpad.texture" );
		m_pPadPointTexture	= resourceManager.loadResource< Texture >( "touch_dpad_point.texture" );
		m_pPointTexture		= resourceManager.loadResource< Texture >( "touch_point.texture" );

		if ( m_pPadTexture == nullptr || m_pPadPointTexture == nullptr || m_pPointTexture == nullptr )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		vector::set( m_pointSize, float( m_pPointTexture->getTextureData().getWidth() ), float( m_pPointTexture->getTextureData().getHeight() ) );
		m_halfPointSize	= m_pointSize;
		vector::scale( m_halfPointSize, 0.5f );

		vector::set( m_padSize, float( m_pPadTexture->getTextureData().getWidth() ), float( m_pPadTexture->getTextureData().getHeight() ) );
		m_halfPadSize = m_padSize;
		vector::scale( m_halfPadSize, 0.5f );

		for (uint i = 0u; i < m_touchPoints.getCount(); ++i)
		{
			TouchPoint& point = m_touchPoints[ i ];

			point.isDown		= false;
			point.isOnPad[ 0u ]	= false;
			point.isOnPad[ 1u ]	= false;
			point.scale			= 0.0f;
			vector::clear( point.position );
		}

		return true;
	}

	void TouchGameSystem::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		resourceManager.unloadResource( m_pPointTexture );
		resourceManager.unloadResource( m_pPadPointTexture );
		resourceManager.unloadResource( m_pPadTexture );
		m_pPadTexture		= nullptr;
		m_pPadPointTexture	= nullptr;
		m_pPointTexture		= nullptr;
	}

	void TouchGameSystem::update( float timeDelta )
	{
		m_inputEvents.clear();

		if ( !m_isEnabled && !s_convertMouseToTouchEvents )
		{
			return;
		}

		const float width = float( m_pGraphicsSystem->getBackBuffer().getWidth() );
		const float height = float( m_pGraphicsSystem->getBackBuffer().getHeight() );
		const float globalScale = width / 4096.0f;

		const Vector2 leftPadPos = { m_padSize.x * globalScale, height - ( m_padSize.y * globalScale ) };
		const Vector2 rightPadPos = { width - ( m_padSize.x * globalScale ), height - ( m_padSize.y * globalScale ) };

		const float halfTimeDelta = timeDelta / 1.0f;
		for (uint pointIndex = 0u; pointIndex < m_touchPoints.getCount(); ++pointIndex)
		{
			TouchPoint& point = m_touchPoints[ pointIndex ];

			bool needToReset = false;
			if ( !point.isDown )
			{
				point.scale -= halfTimeDelta;

				if ( point.isOnPad[ 0u ] || point.isOnPad[ 1u ] )
				{
					for (uint i = 0u; i < PadCount; ++i)
					{
						if ( m_inputEvents.isFull() )
						{
							continue;
						}

						InputEvent& inputEvent = m_inputEvents.push();
						inputEvent.eventType	= InputEventType_Controller_StickChanged;
						inputEvent.deviceType	= InputDeviceType_Controller;
						inputEvent.deviceId		= 0u;
						inputEvent.data.controllerStick.stickIndex		= (uint32)i;
						inputEvent.data.controllerStick.xState			= 0u;
						inputEvent.data.controllerStick.yState			= 0u;
					}
				}
			}
			else
			{
				Vector2 aDistances[] = { leftPadPos, rightPadPos };
				for (uint i = 0u; i < TIKI_COUNT( aDistances ); ++i)
				{
					if ( point.isOnPad[ i ] && !m_inputEvents.isFull() )
					{
						Vector2 distance = aDistances[ i ];
						vector::sub( distance, point.position );

						vector::scale( distance, 1.0f / globalScale );
						vector::div( distance, m_padSize );
						vector::clamp( distance, vector::create( -1.0f, -1.0f ), Vector2::one );
						TIKI_ASSERT( distance.x >= -1.0f && distance.x <= 1.0f );
						TIKI_ASSERT( distance.y >= -1.0f && distance.y <= 1.0f );

						InputEvent& inputEvent = m_inputEvents.push();
						inputEvent.eventType	= InputEventType_Controller_StickChanged;
						inputEvent.deviceType	= InputDeviceType_Controller;
						inputEvent.deviceId		= 0u;
						inputEvent.data.controllerStick.stickIndex		= (uint32)i;
						inputEvent.data.controllerStick.xState			= -distance.x;
						inputEvent.data.controllerStick.yState			= distance.y;
					}
				}
			}
		}
	}

	void TouchGameSystem::render( const ImmediateRenderer& renderer ) const
	{
		if ( !m_isEnabled && !s_convertMouseToTouchEvents )
		{
			return;
		}

		renderer.beginRenderPass();

		const float width = float( m_pGraphicsSystem->getBackBuffer().getWidth() );
		const float height = float( m_pGraphicsSystem->getBackBuffer().getHeight() );
		const float globalScale = width / 4096.0f;

		const AxisAlignedRectangle leftPadRect = createAxisAlignedRectangle(
			m_padSize.x * 0.5f * globalScale,
			height - ( m_padSize.y * 1.5f * globalScale ),
			m_padSize.x * globalScale,
			m_padSize.y * globalScale
		);

		const AxisAlignedRectangle rightPadRect = createAxisAlignedRectangle(
			width - ( m_padSize.x * 1.5f * globalScale ),
			height - ( m_padSize.y * 1.5f * globalScale ),
			m_padSize.x * globalScale,
			m_padSize.y * globalScale
		);

		renderer.drawTexturedRectangle( m_pPadTexture->getTextureData(), leftPadRect, TIKI_COLOR_WHITE );
		renderer.drawTexturedRectangle( m_pPadTexture->getTextureData(), rightPadRect, TIKI_COLOR_WHITE );

		for (uint i = 0u; i < m_touchPoints.getCount(); ++i)
		{
			const TouchPoint& point = m_touchPoints[ i ];

			if ( point.scale > 0.0f )
			{
				const float pointScale = globalScale * point.scale;

				const AxisAlignedRectangle destinationRectangle = createAxisAlignedRectangle(
					point.position.x - ( m_halfPointSize.x * pointScale ),
					point.position.y - ( m_halfPointSize.y * pointScale ),
					m_pointSize.x * pointScale,
					m_pointSize.y * pointScale
				);

				const Color color = color::fromFloatRGBA( 1.0f, 1.0f, 1.0f, point.scale * point.scale );
				renderer.drawTexturedRectangle(
					( point.isOnPad[ 0u ] || point.isOnPad[ 1u ] ? m_pPadPointTexture : m_pPointTexture )->getTextureData(),
					destinationRectangle,
					color
				);
			}
		}

		renderer.endRenderPass();
	}

	bool TouchGameSystem::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.deviceType != InputDeviceType_Touch )
		{
			if ( s_convertMouseToTouchEvents )
			{
				switch ( inputEvent.eventType )
				{
				case InputEventType_Mouse_ButtonDown:
					m_touchPoints[ 0u ].isDown			= true;
					m_touchPoints[ 0u ].scale			= 1.0f;
					m_touchPoints[ 0u ].isOnPad[ 0u ]	= false; // todo
					m_touchPoints[ 0u ].isOnPad[ 1u ]	= false; // todo
					break;

				case InputEventType_Mouse_ButtonUp:
					m_touchPoints[ 0u ].isDown = false;
					break;

				case InputEventType_Mouse_Moved:
					{
						const InputEventMouseMovedData& data = inputEvent.data.mouseMoved;
						vector::set( m_touchPoints[ 0u ].position, float( data.xState ), float( data.yState ) );
					}
					break;
				}
			}

			return false;
		}

		if ( inputEvent.eventType == InputEventType_Device_Connected || inputEvent.eventType == InputEventType_Device_Disconnected )
		{
			m_isEnabled = ( inputEvent.eventType == InputEventType_Device_Connected );

			return false;
		}

		if ( !m_isEnabled )
		{
			return false;
		}

		const InputEventTouchData& data = inputEvent.data.touch;
		vector::set( m_touchPoints[ data.pointIndex ].position, float( data.xState ), float( data.yState ) );

		switch ( inputEvent.eventType )
		{
		case InputEventType_Touch_PointDown:
			{
				TouchPoint& point = m_touchPoints[ data.pointIndex ];

				m_touchPoints[ data.pointIndex ].isDown = true;
				m_touchPoints[ data.pointIndex ].scale = 1.0f;

				const float width = float( m_pGraphicsSystem->getBackBuffer().getWidth() );
				const float height = float( m_pGraphicsSystem->getBackBuffer().getHeight() );
				const float globalScale = width / 4096.0f;

				const Vector2 leftPadPos = { m_padSize.x * globalScale, height - ( m_padSize.y * globalScale ) };
				const Vector2 rightPadPos = { width - ( m_padSize.x * globalScale ), height - ( m_padSize.y * globalScale ) };

				Vector2 aDistances[] = { leftPadPos, rightPadPos };
				for (uint i = 0u; i < TIKI_COUNT( aDistances ); ++i)
				{
					Vector2 distance = aDistances[ i ];
					vector::sub( distance, point.position );

					const bool isOnPad = ( vector::length( distance ) < m_halfPadSize.x * 1.25f * globalScale );
					point.isOnPad[ i ] = isOnPad;
				}
			}
			break;

		case InputEventType_Touch_PointUp:
			m_touchPoints[ data.pointIndex ].isDown = false;
			break;

		case InputEventType_Touch_PointMove:
			break;
		}

		return true;
	}
}
