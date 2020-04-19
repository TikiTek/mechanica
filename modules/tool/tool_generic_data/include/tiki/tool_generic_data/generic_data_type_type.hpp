#pragma once

namespace tiki
{
	enum GenericDataTypeType
	{
		GenericDataTypeType_Invalid = -1,

		GenericDataTypeType_Enum,
		GenericDataTypeType_Struct,
		GenericDataTypeType_ValueType,
		GenericDataTypeType_Resource,
		GenericDataTypeType_Array,
		GenericDataTypeType_Reference,
		GenericDataTypeType_Pointer,

		GenericDataTypeType_Count,
	};
}
