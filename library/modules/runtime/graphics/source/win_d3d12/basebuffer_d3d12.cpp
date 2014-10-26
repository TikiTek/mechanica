
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	D3D11_BIND_FLAG getD3dBinding( GraphicsBufferType binding )
	{
		switch ( binding )
		{
		case GraphicsBufferType_ConstantBuffer:
			return D3D11_BIND_CONSTANT_BUFFER;
		case GraphicsBufferType_IndexBuffer:
			return D3D11_BIND_INDEX_BUFFER;
		case GraphicsBufferType_VertexBuffer:
			return D3D11_BIND_VERTEX_BUFFER;
		}

		TIKI_BREAK( "[graphics] wrong GraphicsBufferType.\n" );
		return D3D11_BIND_UNORDERED_ACCESS;
	}

	BaseBuffer::BaseBuffer()
	{
		m_pBuffer = nullptr;
	}

	BaseBuffer::~BaseBuffer()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, GraphicsBufferType binding, const void* pInitData /*= nullptr*/ )
	{
		TIKI_ASSERT( m_pBuffer == nullptr );

		m_dynamic = dynamic;

		D3D11_BUFFER_DESC desc;
		desc.Usage					= ( dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT );
		desc.CPUAccessFlags			= ( dynamic ? D3D11_CPU_ACCESS_WRITE : 0u );
		desc.StructureByteStride	= 0;
		desc.BindFlags				= getD3dBinding( binding );
		desc.ByteWidth				= UINT( size );
		desc.MiscFlags				= 0;

		TIKI_DECLARE_STACKANDZERO( D3D11_SUBRESOURCE_DATA, initData );
		initData.pSysMem = pInitData;
		const D3D11_SUBRESOURCE_DATA* pD3dInitData = ( pInitData != nullptr ? &initData : nullptr );

		const HRESULT result = graphics::getDevice( graphicsSystem )->CreateBuffer( &desc, pD3dInitData, &m_pBuffer );
		if ( FAILED( result ) )
		{
			dispose( graphicsSystem );
			return false;
		}

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
}