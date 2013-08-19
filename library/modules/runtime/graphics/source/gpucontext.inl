namespace tiki
{
	template<typename Buffer>
	TIKI_FORCE_INLINE void GpuContext::setVertexBuffer( const Buffer& buffer, const uint slot /* = 0u */ )
	{
		TIKI_ASSERT( buffer.getType() == D3D11_BIND_VERTEX_BUFFER );
		uint offset = 0;
		uint stride = buffer.getStride();
		m_pHandles->pContext->IASetVertexBuffers( slot, 1, &buffer.m_pBuffer, &stride, &offset );
	}
	
	template<typename Buffer>
	TIKI_FORCE_INLINE void GpuContext::setIndexBuffer( const Buffer& buffer )
	{
		TIKI_ASSERT( buffer.getType() == D3D11_BIND_INDEX_BUFFER );
		m_pHandles->pContext->IASetIndexBuffer( buffer.m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
	}

	template<typename Buffer>
	TIKI_FORCE_INLINE void GpuContext::setConstantBuffer( const Buffer& buffer, const uint slot /*= 0u*/ )
	{
		TIKI_ASSERT( buffer.getType() == D3D11_BIND_CONSTANT_BUFFER );
		m_pHandles->pContext->PSSetConstantBuffers( slot, 1, &buffer.m_pBuffer );
		m_pHandles->pContext->VSSetConstantBuffers( slot, 1, &buffer.m_pBuffer );
	}
}