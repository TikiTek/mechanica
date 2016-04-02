
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/graphicssystem.hpp"

#include <d3d11.h>

namespace tiki
{
	D3D11_BIND_FLAG getD3dBinding( BaseBufferTypes binding )
	{
		switch ( binding )
		{
		case BaseBufferTypes_ConstantBuffer:
			return D3D11_BIND_CONSTANT_BUFFER;
		case BaseBufferTypes_IndexBuffer:
			return D3D11_BIND_INDEX_BUFFER;
		case BaseBufferTypes_VertexBuffer:
			return D3D11_BIND_VERTEX_BUFFER;
		}

		TIKI_BREAK( "[graphics] wrong BaseBufferTypes.\n" );
		return D3D11_BIND_UNORDERED_ACCESS;
	}

	BaseBuffer::BaseBuffer()
	{
		m_pBuffer	= nullptr;
		m_size		= 0u;
	}

	BaseBuffer::~BaseBuffer()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, BaseBufferTypes binding, const void* pInitData, const char* pDebugName )
	{
		TIKI_ASSERT( m_pBuffer == nullptr );

		m_size		= size;
		m_dynamic	= dynamic;

		D3D11_BUFFER_DESC desc;
		desc.Usage					= ( dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT );
		desc.CPUAccessFlags			= ( dynamic ? D3D11_CPU_ACCESS_WRITE : 0u );
		desc.StructureByteStride	= 0;
		desc.BindFlags				= getD3dBinding( binding );
		desc.ByteWidth				= UINT( alignValue< uint >( size, 16u ) );
		desc.MiscFlags				= 0;

		TIKI_DECLARE_STACKANDZERO( D3D11_SUBRESOURCE_DATA, initData );
		initData.pSysMem = pInitData;
		const D3D11_SUBRESOURCE_DATA* pD3dInitData = ( pInitData != nullptr ? &initData : nullptr );

		const HRESULT result = GraphicsSystemPlatform::getDevice( graphicsSystem )->CreateBuffer( &desc, pD3dInitData, &m_pBuffer );
		if ( FAILED( result ) )
		{
			dispose( graphicsSystem );
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( m_pBuffer, pDebugName );

		return true;
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_pBuffer != nullptr )
		{
			m_pBuffer->Release();
			m_pBuffer = nullptr;
		}
	}

	uint BaseBuffer::getSize() const
	{
		return m_size;
	}
}