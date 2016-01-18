#pragma once
#ifndef TIKI_GRAPHICSCONTEXT_INL_INCLUDED__
#define TIKI_GRAPHICSCONTEXT_INL_INCLUDED__

namespace tiki
{
	template<typename T>
	T* GraphicsContext::mapBuffer( const BaseBuffer& buffer )
	{
		TIKI_ASSERT( buffer.getSize() >= sizeof( T ) );
		return static_cast< T* >( mapBuffer( buffer ) );
	}

	template<typename T>
	void GraphicsContext::beginImmediateGeometry( StaticArray< T >& targetArray, uint vertexCount )
	{
		void* pData = beginImmediateGeometry( sizeof( T ), vertexCount );
		targetArray.create( static_cast< T* >( pData ), vertexCount );
	}
	
	template<typename T>
	void GraphicsContext::endImmediateGeometry( StaticArray< T >& targetArray )
	{
		targetArray.dispose();
		endImmediateGeometry();
	}
}

#endif // TIKI_GRAPHICSCONTEXT_INL_INCLUDED__
