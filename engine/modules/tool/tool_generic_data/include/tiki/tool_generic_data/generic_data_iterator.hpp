#pragma once

#include "tiki/container/dynamic_array.hpp"
#include "tiki/tool_generic_data/generic_data_value.hpp"

namespace tiki
{
	enum class GenericDataIteratorEntryType
	{
		Invalid,
		Object,
		Array,
		Value
	};

	class GenericDataIterator
	{
	public:

											GenericDataIterator();
											~GenericDataIterator();

		void								create( GenericDataObject* pObject );
		void								dispose();

		void								openMemberByIndex( uintreg index );
		void								closeMember();

		GenericDataIteratorEntryType		getEntryType() const;
		void								getElementName( DynamicString& target, uintreg index ) const;
		uintreg								getElementCount() const;
		const GenericDataType*				getType() const;
		GenericDataValueLevel				getLevel() const;
		const GenericDataValue*				getValue() const;

		bool								addArrayElement();

		bool								setValueTagEnabled( bool enable );
		bool								setValueTagName( const DynamicString& name );
		bool								setValueTagContent( const DynamicString& content );

		bool								setObject( GenericDataObject* pObject );
		bool								setPointer( GenericDataObject* pObject );
		bool								setBoolean( bool value );
		bool								setSignedInteger( sint64 value );
		bool								setUnsignedInteger( uint64 value );
		bool								setFloatingPoint( float64 value );
		bool								setString( const DynamicString& value );

	private:

		struct Entry
		{
			GenericDataIteratorEntryType	entryType		= GenericDataIteratorEntryType::Invalid;
			GenericDataValueLevel			level			= GenericDataValueLevel::Invalid;
			uintreg							elementIndex	= 0u;
			uintreg							elementCount	= 0u;

			const GenericDataType*			pType			= nullptr;

			GenericDataObject*				pObject			= nullptr;
			const GenericDataObject*		pConstObject	= nullptr;
			GenericDataArray*				pArray			= nullptr;
			const GenericDataArray*			pConstArray		= nullptr;
			GenericDataValue*				pValue			= nullptr;
			const GenericDataValue*			pConstValue		= nullptr;
		};

		DynamicArray< Entry >				m_entries;

		GenericDataValue*					ensureCurrentValue();
		GenericDataValue*					ensureValue( uintreg maxIndex );
	};
}