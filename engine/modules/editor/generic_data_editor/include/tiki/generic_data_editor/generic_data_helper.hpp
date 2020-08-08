#pragma once

namespace tiki
{
	class DynamicString;
	class GenericDataObject;
	struct Vector2;

	namespace generic_data
	{
		bool	getMemberVector2( Vector2& target, const GenericDataObject* pObject, const DynamicString& memberName );
	}
}
