#ifndef TIKI_REFLECTION_HPP__
#define TIKI_REFLECTION_HPP__

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
#	include "tiki/base/array.hpp"
#	include "tiki/base/functions.hpp"
#	include "tiki/base/string.hpp"
#	include "tiki/toolbase/list.hpp"

#	define TIKI_REFLECTION_STRUCT( name, ... ) struct name													\
	{																										\
		__VA_ARGS__																							\
		const ::tiki::reflection::StructType*			getType() const { return s_typeDefinition.pType; }	\
		static const ::tiki::reflection::StructType*	getStaticType()	{ return s_typeDefinition.pType; }	\
	private:																								\
		static struct Definition																			\
		{																									\
			Definition() 																					\
			{																								\
				pType = ::tiki::reflection::registerStructType(												\
					#name,																					\
					nullptr,																				\
					TIKI_STRING( __VA_ARGS__ )																\
				);																							\
			}																								\
			const ::tiki::reflection::StructType* pType;													\
		} s_typeDefinition;																					\
	}

#	define TIKI_REFLECTION_FIELD( type, name ) type name;
#	define TIKI_REFLECTION_CPPDECLARE( type_name ) type_name ::Definition type_name :: s_typeDefinition

#else

#	define TIKI_REFLECTION_STRUCT( name, ... ) struct name	\
	{														\
		__VA_ARGS__											\
	};

#	define TIKI_REFLECTION_FIELD( type, name ) type name;
#	define TIKI_REFLECTION_CPPDECLARE( type )

#endif

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
namespace tiki
{
	namespace reflection
	{
		enum TypeMemberFlag
		{
			TypeMemberFlag_None			= 0u,

			TypeMemberFlag_Public		= 1u << 0u,
			TypeMemberFlag_Const		= 1u << 1u,
			TypeMemberFlag_Pointer		= 1u << 2u,
			TypeMemberFlag_Reference	= 1u << 3u
		};

		enum ValueTypeVariant
		{
			ValueTypeVariant_Void,
			ValueTypeVariant_Bool,
			ValueTypeVariant_FloatingPoint,
			ValueTypeVariant_SignedInteger,
			ValueTypeVariant_UnsignedInteger
		};

		enum TypeBaseLeaf
		{
			TypeBaseLeaf_ValueType,
			TypeBaseLeaf_StructType
		};

		enum MemberType
		{
			MemberType_Field,
			MemberType_Method,
			//MemberType_Constructor,
			//MemberType_Destructor,
			//MemberType_Operator
		};

		struct TypeDescription
		{
			TypeDescription( cstring _name, cstring _baseName, cstring _code )
				: name( _name ), baseName( _baseName ), code( _code )
			{
			}

			cstring		name;
			cstring		baseName;
			cstring		code;
		};

		class TypeBase
		{
		public:

										TypeBase( const string& name, const TypeBase* pBaseType );
			virtual						~TypeBase();

			const string&				getName() const		{ return m_name; }
			const TypeBase*				getBaseType() const	{ return m_pBaseType; }

			virtual TypeBaseLeaf		getLeaf() const = 0;

			virtual uint				getAlignment() const = 0;
			virtual uint				getSize() const = 0;

		protected:

			void						setBaseType( const TypeBase* pBaseType ) { m_pBaseType = pBaseType; }

		private:

			string				m_name;
			const TypeBase*		m_pBaseType;

		};

		class ValueType : public TypeBase
		{
		public:

			ValueType( const string& name, uint size, ValueTypeVariant variant );

			virtual TypeBaseLeaf		getLeaf() const			{ return TypeBaseLeaf_ValueType; }
			virtual uint				getAlignment() const	{ return m_size; }
			virtual uint				getSize() const			{ return m_size; }
			ValueTypeVariant			getTypeVariant() const	{ return m_variant; }
			
		private:

			uint				m_size;
			ValueTypeVariant	m_variant;

		};

		class TypeMemberInfo
		{
		public:

			TypeMemberInfo( const TypeBase* pType, TypeMemberFlag flags )
				: m_pType( pType ), m_flags( flags )
			{
			}

			const TypeBase*		getType() const			{ return m_pType; }

			bool				isConst() const			{ return isBitSet( m_flags, TypeMemberFlag_Const ); }
			bool				isPointer() const		{ return isBitSet( m_flags, TypeMemberFlag_Pointer ); }
			bool				isPublic() const		{ return isBitSet( m_flags, TypeMemberFlag_Public ); }
			bool				isReference() const		{ return isBitSet( m_flags, TypeMemberFlag_Reference ); }

		private:

			const TypeBase*		m_pType;
			TypeMemberFlag		m_flags;

		};

		class MemberBase
		{
		public:

										MemberBase( const string& name, const TypeBase* pContainingType );
			virtual						~MemberBase();

			const string&				getName() const				{ return m_name; }
			const TypeBase*				getContainingType() const	{ return m_pContainingType; }

			virtual MemberType			getMemberType() const = 0;

		private:

			string			m_name;
			const TypeBase*	m_pContainingType;

		};

		class FieldMember : public MemberBase
		{
		public:

			FieldMember( const string& name, const TypeBase* pContainingType, const TypeMemberInfo& type, uint offset );
			
			const TypeMemberInfo&	getTypeInfo() const						{ return m_type; }
			uint					getOffset() const						{ return m_offset; }
			virtual MemberType		getMemberType() const					{ return MemberType_Field; }

			void*					getValue( void* pObject ) const			{ return addPtr( pObject, m_offset ); }
			const void*				getValue( const void* pObject ) const	{ return addPtr( pObject, m_offset ); }
			
			void					setValue( void* pObject, const void* pValue ) const;

		private:

			TypeMemberInfo	m_type;
			uint			m_offset;
		};

		class MethodMember : public MemberBase
		{
		public:

			MethodMember( const string& name, const TypeBase* pContainingType, const TypeMemberInfo* returnType );

			virtual MemberType	getMemberType() const	{ return MemberType_Method; }

			const TypeBase*		getReturnValue() const	{ return m_pReturnValue; }

		private:

			const TypeBase*		m_pReturnValue;

		};

		class StructType : public TypeBase
		{
		public:

			StructType( const string& name, const string& baseName, const string& code );
			~StructType();

			void						initialize();

			const FieldMember*			getFieldByName( const string& name ) const;
			void						findFieldRecursve( List< const FieldMember* >& wayToField, const string& name ) const;

			virtual TypeBaseLeaf		getLeaf() const			{ return TypeBaseLeaf_StructType; }
			virtual uint				getAlignment() const	{ return m_alignment; }
			virtual uint				getSize() const			{ return m_size; }

		private:

			string						m_baseName;
			string						m_code;

			uint						m_size;
			uint						m_alignment;

			List< FieldMember* >		m_fields;

		};

		void				initialize();
		void				shutdown();

		const ValueType*	registerValueType( const string& name, uint size, ValueTypeVariant variant );
		const StructType*	registerStructType( const string& name, const string& baseName, const string& code );

		const TypeBase*		getTypeOf( const string& typeName );
	}
}
#endif

#endif // TIKI_REFLECTION_HPP__
