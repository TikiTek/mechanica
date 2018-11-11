#pragma once
#ifndef TIKI_GENERIC_DATA_TYPE_TYPE_HPP_INCLUDED
#define TIKI_GENERIC_DATA_TYPE_TYPE_HPP_INCLUDED

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

#endif // TIKI_GENERIC_DATA_TYPE_TYPE_HPP_INCLUDED
