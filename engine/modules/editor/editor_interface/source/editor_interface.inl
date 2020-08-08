#pragma once

namespace tiki
{
	template< class T >
	EditorResourceResult EditorInterface::getResource( const T** ppResource, const DynamicString& resourceName )
	{
		const int seperatorIndex = resourceName.indexOf( ':' );
		if( seperatorIndex != -1 )
		{
			return getResource( (const Resource**)ppResource, resourceName.cStr() + seperatorIndex + 1u, T::getResourceType() );
		}
		else
		{
			return getResource( (const Resource**)ppResource, resourceName.cStr(), T::getResourceType() );
		}
	}
}
