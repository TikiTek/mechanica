
#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/vertexformat.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	static const char* s_apValidationLayers[] =
	{
		"VK_LAYER_LUNARG_standard_validation"
	};

	bool GraphicsSystem::createPlatform( const GraphicsSystemParameters& params )
	{
		// create instance
		{
			static const char* s_apInstanceExtensions[] =
			{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN)
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
				VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#endif
			};

			TIKI_DECLARE_STACKANDZERO( VkApplicationInfo, applicationInfo );
			applicationInfo.sType						= VK_STRUCTURE_TYPE_APPLICATION_INFO;
			applicationInfo.pApplicationName			= "TikiEninge 3.0";
			applicationInfo.pEngineName					= "TikiEninge 3.0";
			applicationInfo.apiVersion					= VK_API_VERSION_1_0;

			TIKI_DECLARE_STACKANDZERO( VkInstanceCreateInfo, instanceCreateInfo );
			instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceCreateInfo.pApplicationInfo			= &applicationInfo;
			instanceCreateInfo.enabledExtensionCount	= TIKI_COUNT( s_apInstanceExtensions );
			instanceCreateInfo.ppEnabledExtensionNames	= s_apInstanceExtensions;

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
			instanceCreateInfo.enabledLayerCount		= TIKI_COUNT( s_apValidationLayers );
			instanceCreateInfo.ppEnabledLayerNames		= s_apValidationLayers;
#endif

			if( vkCreateInstance( &instanceCreateInfo, nullptr, &m_platformData.instance ) != VK_SUCCESS )
			{
				return false;
			}
		}

		// find physical device
		uint32 graphicsQueueIndex = 0u;
		VkPhysicalDevice physicalDevice;
		{
			uint32 gpuCount = 0u;
			if( vkEnumeratePhysicalDevices( m_platformData.instance, &gpuCount, nullptr ) != VK_SUCCESS || gpuCount == 0u )
			{
				disposePlatform();
				return false;
			}

			FixedArray< VkPhysicalDevice, 32u > physicalDevices;
			if( vkEnumeratePhysicalDevices( m_platformData.instance, &gpuCount, physicalDevices.getBegin() ) != VK_SUCCESS )
			{
				disposePlatform();
				return false;
			}

			physicalDevice = physicalDevices.getFirst();

			uint32 queueCount;
			vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueCount, nullptr );
			if( queueCount < 1u )
			{
				disposePlatform();
				return false;
			}

			FixedArray< VkQueueFamilyProperties, 32u > queueProps;
			vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueCount, queueProps.getBegin() );

			for( graphicsQueueIndex = 0; graphicsQueueIndex < queueCount; graphicsQueueIndex++ )
			{
				if( isBitSet( queueProps[ graphicsQueueIndex ].queueFlags, VK_QUEUE_GRAPHICS_BIT ) )
				{
					break;
				}
			}

			if( graphicsQueueIndex == queueCount )
			{
				disposePlatform();
				return false;
			}
		}

		// create device
		{
			static const float s_aQueueProperties[] =
			{
				0.0f 
			};

			static const char* s_apDeviceExtensions[] =
			{ 
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			VkDeviceQueueCreateInfo queueCreateInfo ={ };
			queueCreateInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex			= graphicsQueueIndex;
			queueCreateInfo.queueCount					= TIKI_COUNT( s_aQueueProperties );
			queueCreateInfo.pQueuePriorities			= s_aQueueProperties;

			TIKI_DECLARE_STACKANDZERO( VkDeviceCreateInfo, deviceCreateInfo );
			deviceCreateInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceCreateInfo.queueCreateInfoCount		= 1u;
			deviceCreateInfo.pQueueCreateInfos			= &queueCreateInfo;
			deviceCreateInfo.enabledExtensionCount		= TIKI_COUNT( s_apDeviceExtensions );
			deviceCreateInfo.ppEnabledExtensionNames	= s_apDeviceExtensions;

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
			deviceCreateInfo.enabledLayerCount			= TIKI_COUNT( s_apValidationLayers );
			deviceCreateInfo.ppEnabledLayerNames		= s_apValidationLayers;
#endif

			if( vkCreateDevice( physicalDevice, &deviceCreateInfo, nullptr, &m_platformData.device ) != VK_SUCCESS )
			{
				disposePlatform();
				return false;
			}
		}

		uint2 backBufferSize;
		backBufferSize.x = (uint32)params.backBufferWidth;
		backBufferSize.y = (uint32)params.backBufferHeight;

		if ( params.fullScreen )
		{
			RECT rect;
			HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &rect);
			
			backBufferSize.x = (rect.left - rect.right);
			backBufferSize.y = (rect.top - rect.bottom);
		}

		//if( !graphics::initOpenGL( m_platformData, params ) )
		//{
		//	TIKI_TRACE_ERROR( "[graphics] Could not create OpenGL.\n" );
		//	return false;
		//}

		//if( !graphics::initDepthStencilBuffer( m_platformData, backBufferSize ) )
		//{
		//	TIKI_TRACE_ERROR( "[graphics] Could not create DepthStencilBuffer.\n" );
		//	return false;
		//}

		//glViewport( 0, 0, backBufferSize.x, backBufferSize.y );

		// create back buffer target
		{
			m_backBufferTarget.m_width						= backBufferSize.x;
			m_backBufferTarget.m_height						= backBufferSize.y;

			m_backBufferTarget.m_colorBuffers[ 0u ].format		= PixelFormat_R8G8B8A8_Gamma;
			m_backBufferTarget.m_colorBuffers[ 0u ].pDataBuffer = nullptr;
			m_backBufferTarget.m_colorBufferCount = 1u;

			m_backBufferTarget.m_depthBuffer.format			= PixelFormat_Depth24Stencil8;
			m_backBufferTarget.m_depthBuffer.pDataBuffer	= nullptr;
		}

		if ( !m_commandBuffer.create( *this ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create CommandBuffer.\n" );
			return false;
		}

		return true;
	}

	void GraphicsSystem::disposePlatform()
	{
		m_commandBuffer.dispose( *this );
	}

	bool GraphicsSystem::resize( uint width, uint height )
	{
		if ( width == 0u || height == 0u )
		{
			return false;
		}

		//m_platformData.pContext->OMSetRenderTargets( 0u, nullptr, nullptr );

		//safeRelease( &m_platformData.pDepthStencilView );
		//safeRelease( &m_platformData.pDepthStencilBuffer );
		//safeRelease( &m_platformData.pBackBufferTargetView);

		//HRESULT result = m_platformData.pSwapChain->ResizeBuffers( 0, UINT( width ), UINT( height ), DXGI_FORMAT_UNKNOWN, 0 );
		//if ( FAILED( result ) )
		//{
		//	dispose();
		//	return false;
		//}

		//uint2 backBufferSize = { uint32( width ), uint32( height ) };
		//graphics::initBackBuffer( m_platformData );
		//graphics::initDepthStencilBuffer( m_platformData, backBufferSize );

		//m_backBufferTarget.m_width	= width;
		//m_backBufferTarget.m_height	= height;
		//m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= m_platformData.pBackBufferTargetView;
		//m_backBufferTarget.m_platformData.pDepthView		= m_platformData.pDepthStencilView;

		return true;
	}

	GraphicsContext& GraphicsSystem::beginFrame()
	{	
		m_frameNumber++;

		return m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		//graphics::resetDeviceState( m_platformData.pContext );
	}
}