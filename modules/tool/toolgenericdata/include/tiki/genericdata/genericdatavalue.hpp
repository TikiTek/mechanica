#pragma once
#ifndef TIKI_GENERICDATAVALUE_HPP_INCLUDED__
#define TIKI_GENERICDATAVALUE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/basicstring.hpp"

namespace tiki
{
	class GenericDataObject;
	class GenericDataType;

	class GenericDataValue
	{
	public:

		GenericDataValue( const GenericDataType* pType );
		~GenericDataValue();

		void		setUint8();

	private:

		union Values
		{
			uint8	u8;
			uint16	u16;
			uint32	u32;
			uint64	u64;

			sint8	s8;
			sint16	s16;
			sint32	s32;
			sint64	s64;

			float	float32;
			double	float64;

			GenericDataObject*	pObject;
		};

		const GenericDataType*	m_pType;

		Values					m_value;
		string					m_text;

	};
}

#endif // TIKI_GENERICDATAVALUE_HPP_INCLUDED__
