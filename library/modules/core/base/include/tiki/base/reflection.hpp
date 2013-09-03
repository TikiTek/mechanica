#ifndef TIKI_REFLECTION_HPP__
#define TIKI_REFLECTION_HPP__

#include "tiki/base/types.hpp"

#if TIKI_BUILD_TOOLS
#	include "tiki/base/array.hpp"
#	include "tiki/base/functions.hpp"
#	include "tiki/base/list.hpp"
#	include "tiki/base/string.hpp"

#	define TIKI_REFLECTION_STRUCT( name, ... ) struct name\
	{ \
		__VA_ARGS__ \
		\
		const ::tiki::reflection::StructType* getType() const { return &s_typeDefinition; } \
		static const ::tiki::reflection::StructType* getStaticType() { return &s_typeDefinition; } \
		\
	private:\
		\
		struct TypeDefinition : ::tiki::reflection::StructType\
		{ \
			TypeDefinition() \
				: StructType( #name, TIKI_REFLECTION_STR( __VA_ARGS__ ) ) \
			{ \
			} \
		}; \
		static const TypeDefinition s_typeDefinition;\
	}
#	define TIKI_REFLECTION_STR( str ) #str
#	define TIKI_REFLECTION_FIELD( type, name ) type name;
#	define TIKI_REFLECTION_CPPDECLARE( type ) const type :: TypeDefinition type :: s_typeDefinition
#else
#	define TIKI_REFLECTION_STRUCT( name, ... ) struct name \
	{ \
		__VA_ARGS__ \
	};
#	define TIKI_REFLECTION_FIELD( type, name ) type name;
#	define TIKI_REFLECTION_CPPDECLARE( type )
#endif

#if TIKI_BUILD_TOOLS
namespace tiki
{
	namespace reflection
	{
		enum FieldFlags
		{
			FieldFlags_None			= 0u,
			FieldFlags_Public		= 1u,
			FieldFlags_Const		= 2u,
			FieldFlags_Pointer		= 4u,
			FieldFlags_Reference	= 8u
		};

		enum ValueTypeVariants
		{
			ValueTypeVariants_Void,
			ValueTypeVariants_Bool,
			ValueTypeVariants_FloatingPoint,
			ValueTypeVariants_SignedInteger,
			ValueTypeVariants_UnsignedInteger
		};

		enum TypeBaseLeaf
		{
			TypeBaseLeaf_ValueType,
			TypeBaseLeaf_FieldType,
			TypeBaseLeaf_StructType
		};

		class TypeBase
		{
		public:

			TypeBase( const string& name, const TypeBase* pBaseType );

			const string&				getName() const { return m_name; }
			const TypeBase*				getBaseType() const { return m_pBaseType; }

			virtual TypeBaseLeaf		getLeaf() const = 0;

			virtual size_t				getAlignment() const = 0;
			virtual size_t				getSize() const = 0;

		protected:

			static void					addType( const TypeBase* pType );
			static size_t				getTypeCount();

			static const TypeBase*		getTypeByName( const string& name );

		private:

			string							m_name;
			const TypeBase*					m_pBaseType;

			static List< const TypeBase* >	m_types;

		};

		class ValueType : public TypeBase
		{
		public:

			ValueType( const string& name, size_t size, ValueTypeVariants variant );

			static void					initializeTypes();

			virtual TypeBaseLeaf		getLeaf() const			{ return TypeBaseLeaf_ValueType; }
			virtual size_t				getAlignment() const	{ return m_size; }
			virtual size_t				getSize() const			{ return m_size; }
			
		private:

			size_t				m_size;
			ValueTypeVariants	m_variant;

		};

		class FieldType : public TypeBase
		{
		public:

			FieldType();
			FieldType( const string& name, const TypeBase* pType, size_t offset, FieldFlags flags );

			bool					isConst() const							{ return isBitSet( m_flags, FieldFlags_Const ); }
			bool					isPointer() const						{ return isBitSet( m_flags, FieldFlags_Pointer ); }
			bool					isPublic() const						{ return isBitSet( m_flags, FieldFlags_Public ); }
			bool					isReference() const						{ return isBitSet( m_flags, FieldFlags_Reference ); }

			const TypeBase*			getType() const							{ return getBaseType(); }
			size_t					getOffset() const						{ return m_offset; }

			void*					getValue( void* pObject ) const			{ return (((uint8*)pObject) + m_offset); }
			const void*				getValue( const void* pObject ) const	{ return (((const uint8*)pObject) + m_offset); }

			virtual TypeBaseLeaf	getLeaf() const							{ return TypeBaseLeaf_FieldType; }
			virtual size_t			getAlignment() const					{ return getBaseType()->getAlignment(); }
			virtual size_t			getSize() const							{ return getBaseType()->getSize(); }
			
			void					setValue( void* pObject, const void* pValue ) const;

		private:

			size_t					m_offset;

			FieldFlags				m_flags;

		};

		class StructType : public TypeBase
		{
		public:

			StructType( cstring name, cstring structString );

			const FieldType*			getFieldByName( const string& name ) const;
			void						findFieldRecursve( List< const FieldType* >& wayToField, const string& name ) const;

			virtual TypeBaseLeaf		getLeaf() const			{ return TypeBaseLeaf_StructType; }
			virtual size_t				getAlignment() const	{ return m_alignment; }
			virtual size_t				getSize() const			{ return m_size; }

		private:

			string						m_structString;

			size_t						m_size;
			size_t						m_alignment;

			List< FieldType >			m_fields;

		};
	}
}
#endif

#endif // TIKI_REFLECTION_HPP__
