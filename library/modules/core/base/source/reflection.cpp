
#include "tiki/base/reflection.hpp"

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
namespace tiki
{
	namespace reflection
	{
		//////////////////////////////////////////////////////////////////////////
		// TypeBase
		List< const TypeBase* > TypeBase::m_types;

		TypeBase::TypeBase( const string& name, const TypeBase* pBaseType )
		{
			m_name		= name;
			m_pBaseType	= pBaseType;
		}

		void TypeBase::addType( const TypeBase* pType )
		{
			m_types.add( pType );
		}

		size_t TypeBase::getTypeCount()
		{
			return m_types.getCount();
		}

		const TypeBase* TypeBase::getTypeByName( const string& name )
		{
			for (size_t i = 0u; i < m_types.getCount(); ++i)
			{
				if ( m_types[ i ]->m_name == name )
				{
					return m_types[ i ];
				}
			}

			return nullptr;
		}


		//////////////////////////////////////////////////////////////////////////
		// ValueType
		ValueType::ValueType( const string& name, size_t size, ValueTypeVariants variant )
			: TypeBase( name, nullptr )
		{
			m_size		= size;
			m_variant	= variant;
		}

		void ValueType::initializeTypes()
		{
			static ValueType valueTypes[] =
			{
				ValueType( "void",   0u, ValueTypeVariants_Void ),
				ValueType( "bool",   1u, ValueTypeVariants_Bool ),
				ValueType( "uint8",  1u, ValueTypeVariants_UnsignedInteger ),
				ValueType( "uint16", 2u, ValueTypeVariants_UnsignedInteger ),
				ValueType( "uint32", 4u, ValueTypeVariants_UnsignedInteger ),
				ValueType( "uint64", 8u, ValueTypeVariants_UnsignedInteger ),
				ValueType( "sint8",	 1u, ValueTypeVariants_SignedInteger ),
				ValueType( "sint16", 2u, ValueTypeVariants_SignedInteger ),
				ValueType( "sint32", 4u, ValueTypeVariants_SignedInteger ),
				ValueType( "sint64", 8u, ValueTypeVariants_SignedInteger ),
				ValueType( "float",  4u, ValueTypeVariants_FloatingPoint ),
				ValueType( "double", 8u, ValueTypeVariants_FloatingPoint ),
#if TIKI_BUILD_32BIT
				ValueType( "size_t", 4u, ValueTypeVariants_UnsignedInteger ),
				ValueType( "int",	 4u, ValueTypeVariants_SignedInteger ),
				ValueType( "uint",	 4u, ValueTypeVariants_UnsignedInteger ),
#elif TIKI_BUILD_64BIT
				ValueType( "size_t", 8u, ValueTypeVariants_UnsignedInteger ),
				ValueType( "int",	 8u, ValueTypeVariants_SignedInteger ),
				ValueType( "uint",	 8u, ValueTypeVariants_UnsignedInteger ),
#else
#	error Not implemented
#endif
			};

			if ( TypeBase::getTypeCount() != 0u )
			{
				return;
			}

			for (size_t i = 0u; i < TIKI_COUNT( valueTypes ); ++i)
			{
				TypeBase::addType( &valueTypes[ i ] );
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// FieldType
		FieldType::FieldType()
			: TypeBase( "", nullptr )
		{
		}

		FieldType::FieldType( const string& name, const TypeBase* pType, size_t offset, FieldFlags flags )
			: TypeBase( name, pType )
		{
			m_flags		= flags;
			m_offset	= offset;
		}

		void FieldType::setValue( void* pObject, const void* pValue ) const
		{
			void* pTarget = getValue( pObject );

			size_t size = getSize();

			if ( isBitSet( m_flags, FieldFlags_Pointer ) || isBitSet( m_flags, FieldFlags_Reference ) )
			{
				size = sizeof( void* );
			}

			memory::copy( pTarget, pValue, size );
		}


		//////////////////////////////////////////////////////////////////////////
		// StructType
		StructType::StructType( cstring name, cstring structString )
			: TypeBase( name, nullptr )
		{
			ValueType::initializeTypes();

			m_size			= 0u;
			m_alignment		= 0u;
			m_structString	= structString;

			Array< string > fields;
			m_structString.split( fields, ";" );

			for (size_t i = 0u; i < fields.getCount(); ++i)
			{
				string field = fields[ i ].replace( '\t', ' ' ).trim();

				if ( field.isEmpty() )
				{
					continue;
				}

				int firstSpaceIndex		= field.indexOf( ' ' );
				int lastSpaceIndex		= field.lastIndexOf( ' ' );

				if ( firstSpaceIndex == lastSpaceIndex )
				{
					firstSpaceIndex = 0u;
				}

				size_t fieldSize		= 0u;
				size_t fieldAlign		= 0u;

				uint32 flags			= FieldFlags_Public;
				string fieldName		= field.substring( lastSpaceIndex + 1u );
				string typeName			= field.substring( firstSpaceIndex, lastSpaceIndex - firstSpaceIndex ).replace( " ", "" );

				if ( typeName.endsWith( '*' ) )
				{
					flags |= FieldFlags_Pointer;
				}

				if ( typeName.endsWith( '&' ) )
				{
					flags |= FieldFlags_Reference;
				}

				if ( isBitSet( flags, FieldFlags_Pointer ) || isBitSet( flags, FieldFlags_Reference ) )
				{
					typeName = typeName.substring( 0u, typeName.length() - 1u );

					fieldSize	= sizeof( void* );
					fieldAlign	= sizeof( void* );
				}

				const TypeBase* pType	= TypeBase::getTypeByName( typeName );
				TIKI_ASSERT( pType != nullptr );

				if ( !( isBitSet( flags, FieldFlags_Pointer ) || isBitSet( flags, FieldFlags_Reference ) ) )
				{
					fieldSize	= pType->getSize();
					fieldAlign	= pType->getAlignment();
				}

				if ( firstSpaceIndex != 0u )
				{
					string typeModifier	= field.substring( 0u, firstSpaceIndex ).replace( " ", "" );

					if ( typeModifier == "const" )
					{
						flags |= FieldFlags_Const;
					}
					else
					{
						TIKI_BREAK( "only const supported." );
					}
				}

				m_size		 = alignValue( m_size, fieldAlign );

				m_fields.add( FieldType( fieldName, pType, m_size, (FieldFlags)flags ) );

				m_size		+= fieldSize;
				m_alignment	 = TIKI_MAX( m_alignment, fieldAlign );
			}

			fields.dispose();

			TypeBase::addType( this );
		}

		const FieldType* StructType::getFieldByName( const string& name ) const
		{
			for (size_t i = 0u; i < m_fields.getCount(); ++i)
			{
				if ( m_fields[ i ].getName() == name )
				{
					return &m_fields[ i ];
				}
			}

			return nullptr;
		}

		void StructType::findFieldRecursve( List< const FieldType* >& wayToField, const string& name ) const
		{
			bool lastField	= false;
			size_t index	= TIKI_MIN( (size_t)name.indexOf( '.' ), (size_t)name.indexOf( "->" ) );

			if ( index == -1 )
			{
				index		= name.length();
				lastField	= true;
			}

			string localField = name.substring( 0u, index );

			const FieldType* pLocalField = getFieldByName( localField );
			TIKI_ASSERT( pLocalField );
			TIKI_ASSERT( lastField || pLocalField->getType()->getLeaf() == TypeBaseLeaf_StructType );

			wayToField.add( pLocalField );

			if ( lastField )
			{
				return;
			}

			const StructType* pStructType = (const StructType*)pLocalField->getType();
			pStructType->findFieldRecursve( wayToField, name.substring( index + 1u ) );
		}
	}
}
#endif