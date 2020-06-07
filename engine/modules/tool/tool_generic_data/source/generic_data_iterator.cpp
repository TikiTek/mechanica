#include "tiki/tool_generic_data/generic_data_iterator.hpp"

namespace tiki
{
	GenericDataIterator::GenericDataIterator()
	{
	}

	GenericDataIterator::~GenericDataIterator()
	{
		dispose();
	}

	void GenericDataIterator::create( GenericDataObject* pObject )
	{
		dispose();

		Entry& entry = m_entries.pushBack();
		entry.entryType		= GenericDataIteratorEntryType::Object;
		entry.level			= GenericDataValueLevel::Self;
		entry.elementCount	= pObject->getFieldCount();
		entry.pType			= pObject->getType();
		entry.pObject		= pObject;
		entry.pConstObject	= pObject;
	}

	void GenericDataIterator::dispose()
	{
		m_entries.dispose();
	}

	void GenericDataIterator::openMemberByIndex( uintreg index )
	{
		const Entry& lastEntry = m_entries.getLast();

		GenericDataLevelValue value;
		const GenericDataType* pType = nullptr;
		switch( lastEntry.entryType )
		{
		case GenericDataIteratorEntryType::Object:
			{
				if( lastEntry.pObject != nullptr )
				{
					value = lastEntry.pObject->getFieldLevelValue( index );
				}
				else if( lastEntry.pConstObject != nullptr )
				{
					value = lastEntry.pConstObject->getFieldLevelValue( index );
				}
				else
				{
					TIKI_ASSERT( false );
				}

				pType = lastEntry.pConstObject->getFieldType( index );
			}
			break;

		case GenericDataIteratorEntryType::Array:
			{
				value.level = lastEntry.level;

				if( lastEntry.pArray != nullptr )
				{
					value.pValue		= lastEntry.pArray->getElement( index );
					value.pConstValue	= value.pValue;
				}
				else if( lastEntry.pConstArray != nullptr )
				{
					value.pValue		= nullptr;
					value.pConstValue	= lastEntry.pConstArray->getElement( index );
				}
				else
				{
					TIKI_ASSERT( false );
				}

				pType = lastEntry.pConstArray->getElementType();
			}
			break;

		case GenericDataIteratorEntryType::Invalid:
		case GenericDataIteratorEntryType::Value:
			TIKI_ASSERT( false );
			return;
		}

		TIKI_ASSERT( value.level != GenericDataValueLevel::Invalid );
		TIKI_ASSERT( value.pConstValue != nullptr );

		Entry& entry = m_entries.pushBack();
		entry.level			= value.level;
		entry.elementIndex	= index;
		entry.pType			= pType;
		entry.pValue		= value.pValue;
		entry.pConstValue	= value.pConstValue;

		if( value.pConstValue->getValueType() == GenericDataValueType_Object )
		{
			TIKI_VERIFY( value.pConstValue->getObject( entry.pConstObject ) );

			if( value.pValue != nullptr )
			{
				TIKI_VERIFY( value.pValue->getObject( entry.pObject ) );
			}

			entry.entryType		= GenericDataIteratorEntryType::Object;
			entry.elementCount	= entry.pConstObject->getFieldCount();
		}
		else if( value.pConstValue->getValueType() == GenericDataValueType_Pointer )
		{
			TIKI_VERIFY( value.pConstValue->getPointer( entry.pConstObject ) );

			if( value.pValue != nullptr )
			{
				TIKI_VERIFY( value.pValue->getPointer( entry.pObject ) );
			}

			entry.entryType		= GenericDataIteratorEntryType::Object;
			entry.elementCount	= entry.pConstObject->getFieldCount();
		}
		else if( value.pConstValue->getValueType() == GenericDataValueType_Array )
		{
			TIKI_VERIFY( value.pConstValue->getArray( entry.pConstArray ) );

			if( value.pValue != nullptr )
			{
				TIKI_VERIFY( value.pValue->getArray( entry.pArray ) );
			}

			entry.entryType		= GenericDataIteratorEntryType::Array;
			entry.elementCount	= entry.pConstArray->getCount();
		}
		else
		{
			entry.entryType		= GenericDataIteratorEntryType::Value;
		}

		TIKI_ASSERT( entry.pConstValue == nullptr || entry.pConstValue->getType() == pType );
	}

	void GenericDataIterator::closeMember()
	{
		m_entries.popBack();
	}

	GenericDataIteratorEntryType GenericDataIterator::getEntryType() const
	{
		const Entry& lastEntry = m_entries.getLast();
		return lastEntry.entryType;
	}

	void GenericDataIterator::getElementName( DynamicString& target, uintreg index ) const
	{
		const Entry& lastEntry = m_entries.getLast();
		if( lastEntry.entryType == GenericDataIteratorEntryType::Object )
		{
			target = lastEntry.pConstObject->getFieldName( index );
		}
		else if( lastEntry.entryType == GenericDataIteratorEntryType::Array )
		{
			target = formatDynamicString( "[%d]", index );
		}
	}

	uintreg GenericDataIterator::getElementCount() const
	{
		const Entry& lastEntry = m_entries.getLast();
		return lastEntry.elementCount;
	}

	const GenericDataType* GenericDataIterator::getType() const
	{
		const Entry& lastEntry = m_entries.getLast();
		return lastEntry.pType;
	}

	GenericDataValueLevel GenericDataIterator::getLevel() const
	{
		const Entry& lastEntry = m_entries.getLast();
		return lastEntry.level;
	}

	const GenericDataValue* GenericDataIterator::getValue() const
	{
		const Entry& lastEntry = m_entries.getLast();
		return lastEntry.pConstValue;
	}

	bool GenericDataIterator::addArrayElement()
	{
		GenericDataValue* pValue = ensureValue( m_entries.getCount() - 1u );
		if( pValue == nullptr )
		{
			return false;
		}

		GenericDataArray* pArray = nullptr;
		if( !pValue->getArray( pArray ) || pArray == nullptr )
		{
			return false;
		}

		return pArray->addElement( true ) != nullptr;
	}

	bool GenericDataIterator::setValueTagEnabled( bool enable )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		GenericDataTag* pValueTag = pValue->getValueTag();
		if( enable && pValueTag == nullptr )
		{
			pValueTag = new GenericDataTag();
		}
		else if( !enable && pValueTag != nullptr )
		{
			pValueTag = nullptr;
		}

		pValue->setValueTag( pValueTag );
		return true;
	}

	bool GenericDataIterator::setValueTagName( const DynamicString& name )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		GenericDataTag* pValueTag = pValue->getValueTag();
		if( pValueTag == nullptr )
		{
			return false;
		}

		pValueTag->setTag( name );
		return true;
	}

	bool GenericDataIterator::setValueTagContent( const DynamicString& content )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		GenericDataTag* pValueTag = pValue->getValueTag();
		if( pValueTag == nullptr )
		{
			return false;
		}

		pValueTag->setContent( content );
		return true;
	}

	bool GenericDataIterator::setObject( GenericDataObject* pObject )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		return pValue->setObject( pObject );
	}

	bool GenericDataIterator::setPointer( GenericDataObject* pObject )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		return pValue->setPointer( pObject );
	}

	bool GenericDataIterator::setBoolean( bool value )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		return pValue->setBoolean( value );
	}

	bool GenericDataIterator::setSignedInteger( sint64 value )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		return pValue->setSignedInteger( value );
	}

	bool GenericDataIterator::setUnsignedInteger( uint64 value )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		return pValue->setUnsignedInteger( value );
	}

	bool GenericDataIterator::setFloatingPoint( float64 value )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		return pValue->setFloatingPoint( value );
	}

	bool GenericDataIterator::setString( const DynamicString& value )
	{
		GenericDataValue* pValue = ensureCurrentValue();
		if( pValue == nullptr )
		{
			return false;
		}

		return pValue->setString( value );
	}

	GenericDataValue* GenericDataIterator::ensureCurrentValue()
	{
		return ensureValue( m_entries.getCount() );
	}

	GenericDataValue* GenericDataIterator::ensureValue( uintreg maxIndex )
	{
		return nullptr;
	}
}