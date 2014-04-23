
#include "tiki/base/reflection.hpp"

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
#include "trexpp.h"

namespace tiki
{
	namespace reflection
	{
		class TypeSystem
		{
			TIKI_NONCOPYABLE_WITHCTOR_CLASS( TypeSystem );

		public:

			void				initialize();
			void				shutdown();

			const ValueType*	registerValueType( const string& name, uint size, ValueTypeVariant variant );
			const StructType*	registerStructType( const string& name, const string& baseName, const string& code, TypeConstructor pFuncConstructor, TypeDestructor pFuncDestructor );

			const TypeBase*		getTypeByName( const string& name ) const;
			ValueType*			getValueTypeByName( const string& name ) const;
			StructType*			getStructTypeByName( const string& name ) const;

		private:

			List< const TypeBase* >	m_types;
			List< ValueType* >		m_valueTypes;
			List< StructType* >		m_structTypes;

			void					initializeValueTypes();

		};

		TypeSystem& getTypeSystem();
	}

	reflection::TypeSystem& reflection::getTypeSystem()
	{
		static reflection::TypeSystem s_typeSystem;
		return s_typeSystem;
	}

	void reflection::initialize()
	{
		reflection::getTypeSystem().initialize();		
	}

	void reflection::shutdown()
	{
		reflection::getTypeSystem().shutdown();
	}

	const reflection::ValueType* reflection::registerValueType( const string& name, uint size, ValueTypeVariant variant )
	{
		return reflection::getTypeSystem().registerValueType( name, size, variant );
	}

	const reflection::StructType* reflection::registerStructType( const string& name, const string& baseName, const string& code, TypeConstructor pFuncConstructor /*= nullptr*/, TypeDestructor pFuncDestructor /*= nullptr*/ )
	{
		return reflection::getTypeSystem().registerStructType( name, baseName, code, pFuncConstructor, pFuncDestructor );
	}

	const reflection::TypeBase* reflection::getTypeOf( const string& typeName )
	{
		return getTypeSystem().getTypeByName( typeName );
	}

	namespace reflection
	{
		//////////////////////////////////////////////////////////////////////////
		// TypeBase
		TypeBase::TypeBase( const string& name )
			: m_name( name )
		{
		}

		TypeBase::~TypeBase()
		{
		}

		//////////////////////////////////////////////////////////////////////////
		// MemberBase
		MemberBase::MemberBase( const string& name, const TypeBase* pContainingType )
			: m_name( name ), m_pContainingType( pContainingType )
		{
		}

		MemberBase::~MemberBase()
		{
		}

		//////////////////////////////////////////////////////////////////////////
		// FieldType
		FieldMember::FieldMember( const string& name, const TypeBase* pContainingType, const TypeMemberInfo& type, uint offset )
			: MemberBase( name, pContainingType ), m_type( type ), m_offset( offset )
		{
		}

		void FieldMember::setValue( void* pObject, const void* pValue ) const
		{
			void* pTarget = getValue( pObject );

			uint size = m_type.getType()->getSize();
			if ( m_type.isPointer() || m_type.isReference() )
			{
				size = sizeof( void* );
			}

			memory::copy( pTarget, pValue, size );
		}

		//////////////////////////////////////////////////////////////////////////
		// ValueType
		ValueType::ValueType( const string& name, uint size, ValueTypeVariant variant )
			: TypeBase( name ), m_size( size ), m_variant( variant )
		{			
		}

		//////////////////////////////////////////////////////////////////////////
		// StructType
		StructType::StructType( const string& name, const string& baseName, const string& code, TypeConstructor pFuncConstructor, TypeDestructor pFuncDestructor )
			: TypeBase( name ), m_pFuncConstructor( pFuncConstructor ), m_pFuncDestructor( pFuncDestructor )
		{
			m_isInitialized	= false;

			m_size			= 0u;
			m_alignment		= 0u;
			m_baseName		= baseName;
			m_code			= code;
			m_pBaseType		= nullptr;
		}

		StructType::~StructType()
		{
			for (uint i = 0u; i < m_fields.getCount(); ++i)
			{
				TIKI_DEL m_fields[ i ];
			}

			m_isInitialized = false;
		}
	
		void StructType::initialize()
		{
			TIKI_ASSERT( m_isInitialized == false );
			m_isInitialized = true;

			string code = m_code;
			code = code.replace( '\n', ' ' );
			code = code.replace( '\t', ' ' );
			code = code.replace( '\r', ' ' );

			while ( code.countSubstring( "  " ) > 0u )
			{
				code = code.replace( "  ", " " );
			}

			StructType* pBaseType = reflection::getTypeSystem().getStructTypeByName( m_baseName );
			if ( pBaseType != nullptr && pBaseType->isInitialized() == false )
			{
				pBaseType->initialize();
			}
			m_pBaseType = pBaseType;

			const string regexString = formatString( "%s\\(\\)( +)\\{[( a-zA-Z0-9=\\-_;,.&<>*)]+\\}", getName().cStr() );

			TRexpp regex;
			regex.Compile( regexString.cStr() );

			const char* pSearchBegin;
			const char* pSearchEnd;
			if ( regex.Search( code.cStr(), &pSearchBegin, &pSearchEnd ) )
			{
				//remove constructor
				code = code.replace( string( pSearchBegin, pSearchEnd - pSearchBegin ), "" );
			}

			Array< string > fields;
			code.split( fields, ";" );

			for (uint i = 0u; i < fields.getCount(); ++i)
			{
				string field = fields[ i ].trim();

				if ( field.isEmpty() )
				{
					continue;
				}

				int lastSpaceIndex		= field.lastIndexOf( ' ' );

				uint fieldSize		= 0u;
				uint fieldAlign		= 0u;

				uint32 flags			= TypeMemberFlag_Public;
				string fieldName		= field.substring( lastSpaceIndex + 1u );
				string typeName			= field.substring( 0u, lastSpaceIndex );

				bool found = true;
				while ( found )
				{
					if ( typeName.startsWith( "const " ) )
					{
						flags |= TypeMemberFlag_Const;
					}
					else if ( typeName.startsWith( "volatile " ) )
					{
						flags |= TypeMemberFlag_Volatile;
					}
					else if ( typeName.startsWith( "mutable " ) )
					{
						flags |= TypeMemberFlag_Mutable;
					}
					else
					{
						found = false;
					}

					if ( found == true )
					{
						int firstSpaceIndex = typeName.indexOf( ' ' );
						typeName = typeName.substring( firstSpaceIndex );
					}
				}
				typeName = typeName.replace( " ", "" );

				if ( typeName.endsWith( '*' ) )
				{
					flags |= TypeMemberFlag_Pointer;
				}
				else if ( typeName.endsWith( '&' ) )
				{
					flags |= TypeMemberFlag_Reference;
				}

				if ( isBitSet( flags, TypeMemberFlag_Pointer ) || isBitSet( flags, TypeMemberFlag_Reference ) )
				{
					typeName = typeName.substring( 0u, typeName.getLength() - 1u );

					fieldSize	= sizeof( void* );
					fieldAlign	= sizeof( void* );
				}

				const TypeBase* pType	= getTypeSystem().getTypeByName( typeName );
				TIKI_ASSERT( pType != nullptr );

				if ( !( isBitSet( flags, TypeMemberFlag_Pointer ) || isBitSet( flags, TypeMemberFlag_Reference ) ) )
				{
					fieldSize	= pType->getSize();
					fieldAlign	= pType->getAlignment();
				}

				m_size		 = alignValue( m_size, fieldAlign );

				m_fields.add( TIKI_NEW FieldMember( fieldName, this, TypeMemberInfo( pType, (TypeMemberFlag)flags ), getSize() ) );

				m_size		+= fieldSize;
				m_alignment	 = TIKI_MAX( m_alignment, fieldAlign );
			}

			fields.dispose();
		}

		void* StructType::createInstance() const
		{
			void* pData = TIKI_MEMORY_ALLOC( getSize() );

			if ( m_pFuncConstructor != nullptr )
			{
				m_pFuncConstructor( pData );
			}

			return pData;
		}
		
		void StructType::disposeInstance( void* pObject ) const
		{
			if ( pObject != nullptr )
			{
				if ( m_pFuncDestructor != nullptr )
				{
					m_pFuncDestructor( pObject );
				}

				memory::freeAlign( pObject );
			}
		}

		const FieldMember* StructType::getFieldByName( const string& name ) const
		{
			for (uint i = 0u; i < m_fields.getCount(); ++i)
			{
				if ( m_fields[ i ]->getName() == name )
				{
					return m_fields[ i ];
				}
			}

			if ( m_pBaseType != nullptr )
			{
				return m_pBaseType->getFieldByName( name );
			}

			return nullptr;
		}

		const FieldMember* StructType::getFieldByIndex( uint index ) const
		{			
			if ( m_pBaseType != nullptr )
			{
				const uint localIndex = index - m_pBaseType->getFieldCount();

				if ( localIndex < m_fields.getCount() )
				{
					return m_fields[ localIndex ];
				}
				else
				{
					return m_pBaseType->getFieldByIndex( index );
				}
			}

			return m_fields[ index ];
		}

		uint StructType::getFieldCount() const
		{
			uint count = m_fields.getCount();
			if ( m_pBaseType != nullptr )
			{
				count += m_pBaseType->getFieldCount();
			}
			return count;
		}

		uint StructType::getAlignment() const
		{
			if ( m_pBaseType != nullptr )
			{
				return TIKI_MAX( m_alignment, m_pBaseType->getAlignment() );
			}
			return m_alignment;
		}

		uint StructType::getSize() const
		{
			if ( m_pBaseType != nullptr )
			{
				return m_size + m_pBaseType->getSize();
			}
			return m_size;
		}
		
		void StructType::findFieldRecursve( List< const FieldMember* >& wayToField, const string& name ) const
		{
			bool lastField	= false;
			uint index	= TIKI_MIN( (uint)name.indexOf( '.' ), (uint)name.indexOf( "->" ) );

			if ( index == -1 )
			{
				index		= name.getLength();
				lastField	= true;
			}

			string localField = name.substring( 0u, index );

			const FieldMember* pLocalField = getFieldByName( localField );
			TIKI_ASSERT( pLocalField );
			TIKI_ASSERT( lastField || pLocalField->getTypeInfo().getType()->getLeaf() == TypeBaseLeaf_StructType );

			wayToField.add( pLocalField );

			if ( lastField )
			{
				return;
			}

			const StructType* pStructType = static_cast< const StructType* >( pLocalField->getTypeInfo().getType() );
			pStructType->findFieldRecursve( wayToField, name.substring( index + 1u ) );
		}

		//////////////////////////////////////////////////////////////////////////
		// TypeSystem
		void TypeSystem::initialize()
		{
			initializeValueTypes();

			for (uint i = 0u; i < m_structTypes.getCount(); ++i)
			{
				if ( m_structTypes[ i ]->isInitialized() == false )
				{
					m_structTypes[ i ]->initialize();
				}
			} 
		}

		void TypeSystem::shutdown()
		{
			for (uint i = 0u; i < m_valueTypes.getCount(); ++i)
			{
				TIKI_DEL( m_valueTypes[ i ] );
			} 

			for (uint i = 0u; i < m_structTypes.getCount(); ++i)
			{
				TIKI_DEL( m_structTypes[ i ] );
			} 

			m_types.dispose();
			m_valueTypes.dispose();
			m_structTypes.dispose();
		}

		const TypeBase* TypeSystem::getTypeByName( const string& name ) const
		{
			for (uint i = 0u; i < m_types.getCount(); ++i)
			{
				if ( m_types[ i ]->getName() == name )
				{
					return m_types[ i ];
				}
			}

			return nullptr;
		}

		ValueType* TypeSystem::getValueTypeByName( const string& name ) const
		{
			for (uint i = 0u; i < m_valueTypes.getCount(); ++i)
			{
				if ( m_valueTypes[ i ]->getName() == name )
				{
					return m_valueTypes[ i ];
				}
			}

			return nullptr;
		}

		StructType* TypeSystem::getStructTypeByName( const string& name ) const
		{
			for (uint i = 0u; i < m_structTypes.getCount(); ++i)
			{
				if ( m_structTypes[ i ]->getName() == name )
				{
					return m_structTypes[ i ];
				}
			}

			return nullptr;
		}

		const ValueType* TypeSystem::registerValueType( const string& name, uint size, ValueTypeVariant variant )
		{
			ValueType* pType = TIKI_NEW ValueType( name, size, variant );

			m_types.add( pType );
			m_valueTypes.add( pType );

			return pType;
		}

		const StructType* TypeSystem::registerStructType( const string& name, const string& baseName, const string& code, TypeConstructor pFuncConstructor, TypeDestructor pFuncDestructor )
		{
			StructType* pType = TIKI_NEW StructType( name, baseName, code, pFuncConstructor, pFuncDestructor );

			m_types.add( pType );
			m_structTypes.add( pType );

			return pType;
		}

		void TypeSystem::initializeValueTypes()
		{
			const ValueType valueTypes[] =
			{
				ValueType( "void",		0u, ValueTypeVariant_Void ),
				ValueType( "bool",		1u, ValueTypeVariant_Bool ),
				ValueType( "uint8",		1u, ValueTypeVariant_UnsignedInteger ),
				ValueType( "uint16",	2u, ValueTypeVariant_UnsignedInteger ),
				ValueType( "uint32",	4u, ValueTypeVariant_UnsignedInteger ),
				ValueType( "uint64",	8u, ValueTypeVariant_UnsignedInteger ),
				ValueType( "sint8",		1u, ValueTypeVariant_SignedInteger ),
				ValueType( "sint16",	2u, ValueTypeVariant_SignedInteger ),
				ValueType( "sint32",	4u, ValueTypeVariant_SignedInteger ),
				ValueType( "sint64",	8u, ValueTypeVariant_SignedInteger ),
				ValueType( "float16",	2u, ValueTypeVariant_FloatingPoint ),
				ValueType( "float32",	4u, ValueTypeVariant_FloatingPoint ),
				ValueType( "float64",	8u, ValueTypeVariant_FloatingPoint ),
				ValueType( "float",		4u, ValueTypeVariant_FloatingPoint ),
				ValueType( "double",	8u, ValueTypeVariant_FloatingPoint ),
#if TIKI_ENABLED( TIKI_BUILD_32BIT )
				ValueType( "size_t",	4u, ValueTypeVariant_UnsignedInteger ),
				ValueType( "int",		4u, ValueTypeVariant_SignedInteger ),
				ValueType( "uint",		4u, ValueTypeVariant_UnsignedInteger ),
#elif TIKI_ENABLED( TIKI_BUILD_64BIT )
				ValueType( "size_t",	8u, ValueTypeVariant_UnsignedInteger ),
				ValueType( "int",		8u, ValueTypeVariant_SignedInteger ),
				ValueType( "uint",		8u, ValueTypeVariant_UnsignedInteger ),
#else
#	error Not implemented
#endif
			};

			for (uint i = 0u; i < TIKI_COUNT( valueTypes ); ++i)
			{
				const ValueType& type = valueTypes[ i ];
				registerValueType( type.getName(), type.getSize(), type.getTypeVariant() );
			} 
		}
	}
}
#endif