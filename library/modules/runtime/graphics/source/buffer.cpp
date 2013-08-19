
#include "tiki/graphics/buffer.hpp"

#include "tiki/framework/framework.hpp"

#include "graphicshandles.hpp"

#include <dxgi.h>
#include <d3d11.h>

namespace tiki
{
	void BaseBuffer::create( const size_t size, uint binding )
	{
		dispose();

		ID3D11Device* device = getHandles( framework::getGraphicsSystem() )->pDevice;
		TIKI_ASSERT( device != nullptr );

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.StructureByteStride = 0;
		desc.BindFlags = binding;
		desc.ByteWidth = size;
		desc.MiscFlags = 0;

		HRESULT result = device->CreateBuffer( &desc, 0, &m_pBuffer );
		TIKI_ASSERT( SUCCEEDED(result) );
	}

	void BaseBuffer::dispose()
	{
		if ( m_pBuffer != nullptr )
		{
			m_pBuffer->Release();
			m_pBuffer = nullptr;
		}
	}

	void BaseBuffer::apply( const size_t index, const size_t stride, const size_t size, const uint binding ) const
	{
		ID3D11DeviceContext* context = getHandles( framework::getGraphicsSystem() )->pContext;

		if( binding == D3D11_BIND_VERTEX_BUFFER )
		{
			const uint offset = 0u;
			context->IASetVertexBuffers( index, 1, &m_pBuffer, &stride, &offset );
		}
		else if( binding == D3D11_BIND_INDEX_BUFFER )
		{
			context->IASetIndexBuffer( m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
		}
		else if( binding == D3D11_BIND_CONSTANT_BUFFER )
		{
			context->PSSetConstantBuffers( index, 1, &m_pBuffer );
			context->VSSetConstantBuffers( index, 1, &m_pBuffer );
		}
	}

	void* BaseBuffer::map()
	{
		D3D11_MAPPED_SUBRESOURCE mapped;

		ID3D11DeviceContext* context = getHandles( framework::getGraphicsSystem() )->pContext;

		HRESULT result = context->Map( m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
		TIKI_ASSERT( !FAILED( result ) );

		return mapped.pData;
	}

	void BaseBuffer::unmap()
	{
		if( m_pBuffer )
		{
			ID3D11DeviceContext* context = getHandles( framework::getGraphicsSystem() )->pContext;
			context->Unmap( m_pBuffer, 0 );
		}
	}

}