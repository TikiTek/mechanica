
#include "tiki/graphics/staticbuffer.hpp"

namespace tiki
{
	template<uint Binding>
	tiki::StaticBuffer<Binding>::StaticBuffer()
	{
		m_pBuffer = nullptr;
	}

	template<uint TBinding>
	tiki::StaticBuffer<TBinding>::~StaticBuffer()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
	}

	template<uint TBinding>
	void tiki::StaticBuffer<TBinding>::create( const void* pData, size_t elementStride, size_t elementCount )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_BUFFER_DESC, desc );
		desc.Usage		= D3D11_USAGE_DEFAULT;
		desc.BindFlags	= TBinding;
		desc.ByteWidth	= elementStride * elementCount;

		TIKI_DECLARE_STACKANDZERO( D3D11_SUBRESOURCE_DATA, data );
		data.pSysMem = pData;

		m_elementSize	= elementStride;
		m_elementCount	= elementCount;

		ID3D11Device* pDevice = getHandles( framework::getGraphicsSystem() )->pDevice;

		HRESULT r = pDevice->CreateBuffer(
			&desc,
			&data,
			&m_pBuffer
		);
	}

	template<uint TBinding>
	void tiki::StaticBuffer<TBinding>::dispose()
	{
		safeRelease( &m_pBuffer );
		m_pBuffer = nullptr;
	}

	template<uint TBinding>
	void tiki::StaticBuffer<TBinding>::apply() const
	{
		ID3D11DeviceContext* pContext = getHandles( framework::getGraphicsSystem() )->pContext;

		if ( TBinding == D3D11_BIND_INDEX_BUFFER)
		{
			pContext->IASetIndexBuffer( m_pBuffer, DXGI_FORMAT_R32_UINT, 0u );
		}
		else if ( TBinding == D3D11_BIND_VERTEX_BUFFER )
		{
			uint32 offset = 0;
			uint32 stride = elementSize;

			pContext->IASetVertexBuffers( 0u, 1u, &m_pBuffer, &stride, &offset );
		}
	}

	//template<uint TBinding>
	//TGBuffer* tiki::StaticBuffer<TBinding>::getBuffer() const
	//{
	//	return m_pBuffer;
	//}
}