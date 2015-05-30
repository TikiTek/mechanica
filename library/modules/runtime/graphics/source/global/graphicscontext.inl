#pragma once
#ifndef TIKI_GRAPHICSCONTEXT_INL_INCLUDED__
#define TIKI_GRAPHICSCONTEXT_INL_INCLUDED__

namespace tiki
{
	template<typename T>
	T* GraphicsContext::mapBuffer( const BaseBuffer& buffer )
	{
		return static_cast< T* >( mapBuffer( buffer ) );
	}

	template<typename T>
	void tiki::GraphicsContext::beginImmediateGeometry( StaticArray< T >& targetArray, uint vertexCount )
	{
		void* pData = beginImmediateGeometry( sizeof( T ), vertexCount );
		targetArray.create( static_cast< T* >( pData ), vertexCount );
	}
}

#endif // TIKI_GRAPHICSCONTEXT_INL_INCLUDED__
