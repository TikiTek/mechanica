#include "generic_data_file.hpp"

#include "tiki/base/string_tools.hpp"
#include "tiki/toolapplication/tool_ui.hpp"
#include "tiki/toolgenericdata/generic_data_array.hpp"
#include "tiki/toolgenericdata/generic_data_object.hpp"
#include "tiki/toolgenericdata/generic_data_tag.hpp"
#include "tiki/toolgenericdata/generic_data_type_array.hpp"
#include "tiki/toolgenericdata/generic_data_type_enum.hpp"

#include "res_generic_data_editor.hpp"

#include <imgui.h>

namespace tiki
{
	static const char* s_pPopupTagEditor = "Tag Editor";

	GenericDataFile::GenericDataFile( const Path& fileName, GenericDataEditor& genericDataEditor )
		: EditableFile( fileName, &genericDataEditor )
		, m_genericDataEditor( genericDataEditor )
		, m_document( genericDataEditor.getTypeCollection() )
		, m_tagsIcon( getGenericDataEditorResource( GenericDataEditorResources_TagEditor ) )
	{
	}

	GenericDataFile::~GenericDataFile()
	{
	}

	bool GenericDataFile::load()
	{
		return m_document.importFromFile( getFileName().getCompletePath() );
	}

	bool GenericDataFile::save()
	{
		return m_document.exportToFile( getFileName().getCompletePath() );
	}

	void GenericDataFile::doUi()
	{
		ImGui::Columns( 2 );

		GenericDataObject* pObject = m_document.getObject();
		doObjectUi( pObject, false );

		ImGui::Columns( 1 );
	}

	void GenericDataFile::doObjectUi( GenericDataObject* pObject, bool readOnly )
	{
		for( uint i = 0u; i < pObject->getFieldCount(); ++i )
		{
			GenericDataValue* pValue = pObject->getFieldValue( i, false );
			if( pValue == nullptr )
			{
				pValue		= const_cast< GenericDataValue* >( pObject->getFieldOrDefaultValue( i ) );
				readOnly	= true;
			}

			doElementUi( pObject->getFieldName( i ), pValue, pObject->getFieldType( i ), readOnly );
		}
	}

	void GenericDataFile::doArrayUi( GenericDataArray* pArray, bool readOnly )
	{
		for( uint i = 0u; i < pArray->getCount(); ++i )
		{
			const string name = formatDynamicString( "%d", i );
			doElementUi( name, pArray->getElement( i ), pArray->getType()->getBaseType(), readOnly );
		}
	}

	bool GenericDataFile::doElementUi( const string& name, GenericDataValue* pValue, const GenericDataType* pType, bool readOnly )
	{
		TIKI_ASSERT( pValue == nullptr || pValue->getType() == pType );
		const bool canBeOpen = pType->getType() == GenericDataTypeType_Array || pType->getType() == GenericDataTypeType_Struct || pType->getType() == GenericDataTypeType_Pointer;

		ImGui::PushID( pValue );
		ImGui::AlignTextToFramePadding();
		bool nodeOpen = false;
		if( canBeOpen )
		{
			nodeOpen = ImGui::TreeNode( name.cStr() );
		}
		else
		{
			ImGui::TreeNodeEx( name.cStr(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
		}
		ImGui::NextColumn();

		ImGui::AlignTextToFramePadding();
		if( canBeOpen )
		{
			ImGui::Text( " " );
		}
		else if( pValue == nullptr )
		{
			ImGui::Text( "null" );
		}
		else
		{
			doValueUi( pValue, readOnly );
		}
		ImGui::NextColumn();

		if( nodeOpen && pValue != nullptr )
		{
			if( pType->getType() == GenericDataTypeType_Array )
			{
				GenericDataArray* pArray = nullptr;
				if( pValue->getArray( pArray ) )
				{
					doArrayUi( pArray, readOnly );
				}
			}
			else if( pType->getType() == GenericDataTypeType_Struct || pType->getType() == GenericDataTypeType_Pointer )
			{
				GenericDataObject* pSubObject = nullptr;
				if( pValue->getObject( pSubObject ) ||
					pValue->getPointer( pSubObject ) )
				{
					doObjectUi( pSubObject, readOnly );
				}
			}

			ImGui::TreePop();
		}
		else if( nodeOpen )
		{
			ImGui::TreePop();
		}
		ImGui::PopID();

		return nodeOpen;
	}

	void GenericDataFile::doValueUi( GenericDataValue* pValue, bool readOnly )
	{
		const GenericDataType* pType = pValue->getType();
		switch( pType->getType() )
		{
		case GenericDataTypeType_Enum:
			doEnumUi( pValue, readOnly );
			break;

		case GenericDataTypeType_ValueType:
			doValueTypeUi( pValue, readOnly );
			break;

		case GenericDataTypeType_Resource:
			doResourceUi( pValue, readOnly );
			break;

		case GenericDataTypeType_Reference:
			ImGui::Text( "reference" );
			break;

		default:
			ImGui::Text( "ready only" );
			break;
		}
	}

	void GenericDataFile::doEnumUi( GenericDataValue* pValue, bool readOnly )
	{
		const GenericDataTypeEnum* pEnumType = (const GenericDataTypeEnum*)pValue->getType();

		string enumName;
		if( !pValue->getEnum( enumName ) )
		{
			ImGui::Text( "invalid enum" );
			return;
		}

		if( ImGui::BeginCombo( "", enumName.cStr() ) )
		{
			for( const GenericDataEnumValue& enumValue : pEnumType->getValues() )
			{
				bool isSelected = (enumValue.name == enumName);
				if( ImGui::Selectable( enumValue.name.cStr(), &isSelected ) )
				{
					TIKI_VERIFY( pValue->setEnum( enumValue.name, pEnumType ) );
					m_isDirty = true;
				}

				if( isSelected )
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	void GenericDataFile::doValueTypeUi( GenericDataValue* pValue, bool readOnly )
	{
		const GenericDataTypeValueType* pValueTypeType = (const GenericDataTypeValueType*)pValue->getType();

		GenericDataTag* pValueTag = pValue->getValueTag();

		bool wasSelected = false;
		if( pValueTag != nullptr )
		{
			ImGui::PushStyleColor( ImGuiCol_Button, ImGui::GetStyle().Colors[ ImGuiCol_ButtonHovered ] );
			wasSelected = true;
		}
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 4.0f, 3.5f ) );

		if( ImGui::ImageButton( ImGui::Tex( m_tagsIcon ), ImGui::Vec2( m_tagsIcon.getSize() ) ) && !readOnly )
		{
			if( pValueTag == nullptr )
			{
				pValueTag = TIKI_NEW( GenericDataTag )();
			}
			else
			{
				pValueTag = nullptr;
			}

			pValue->setValueTag( pValueTag );
		}

		ImGui::PopStyleVar();
		if( wasSelected )
		{
			ImGui::PopStyleColor();
		}

		ImGui::SameLine();

		if( pValueTag != nullptr )
		{
			doValueTagUi( pValueTag, readOnly );
			return;
		}

		bool result = false;
		string valueText = "";
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
		switch( pValueTypeType->getValueType() )
		{
		case GenericDataTypeValueTypeType_Boolean:
			{
				bool b = false;
				if( !pValue->getBoolean( b ) )
				{
					ImGui::Text( "invalid boolean" );
					return;
				}

				if( ImGui::Checkbox( "", &b ) && !readOnly )
				{
					TIKI_VERIFY( pValue->setBoolean( b, pValueTypeType ) );
					m_isDirty = true;
				}

				return;
			}
			break;

		case GenericDataTypeValueTypeType_SingedInteger8:
		case GenericDataTypeValueTypeType_SingedInteger16:
		case GenericDataTypeValueTypeType_SingedInteger32:
		case GenericDataTypeValueTypeType_SingedInteger64:
			{
				flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank;

				sint64 s64 = 0;
				result = pValue->getSignedValue( s64 );
				valueText = string_tools::toString( s64 );
			}
			break;

		case GenericDataTypeValueTypeType_UnsingedInteger8:
		case GenericDataTypeValueTypeType_UnsingedInteger16:
		case GenericDataTypeValueTypeType_UnsingedInteger32:
		case GenericDataTypeValueTypeType_UnsingedInteger64:
			{
				flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank;

				uint64 u64 = 0u;
				result = pValue->getUnsignedValue( u64 );
				valueText = string_tools::toString( u64 );
			}
			break;

		case GenericDataTypeValueTypeType_FloatingPoint16:
		case GenericDataTypeValueTypeType_FloatingPoint32:
		case GenericDataTypeValueTypeType_FloatingPoint64:
			{
				flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank;

				float64 f64 = 0.0;
				result = pValue->getFloatingPoint( f64 );
				valueText = string_tools::toString( f64 );
			}
			break;

		case GenericDataTypeValueTypeType_String:
			result = pValue->getString( valueText );
			break;

		default:
			break;
		}

		if( !result )
		{
			ImGui::Text( "invalid value" );
			return;
		}

		char buffer[ 256u ];
		copyString( buffer, sizeof( buffer ), valueText.cStr() );
		if( ImGui::InputText( "", buffer, sizeof( buffer ), flags ) && !readOnly )
		{
			switch( pValueTypeType->getValueType() )
			{
			case GenericDataTypeValueTypeType_SingedInteger8:
			case GenericDataTypeValueTypeType_SingedInteger16:
			case GenericDataTypeValueTypeType_SingedInteger32:
			case GenericDataTypeValueTypeType_SingedInteger64:
				{
					sint64 s64 = 0;
					if( string_tools::tryParseSInt64( s64, buffer ) )
					{
						result = pValue->setSignedValue( s64, pValueTypeType );
						m_isDirty = true;
					}
					else
					{
						result = false;
					}
				}
				break;

			case GenericDataTypeValueTypeType_UnsingedInteger8:
			case GenericDataTypeValueTypeType_UnsingedInteger16:
			case GenericDataTypeValueTypeType_UnsingedInteger32:
			case GenericDataTypeValueTypeType_UnsingedInteger64:
				{
					uint64 u64 = 0;
					if( string_tools::tryParseUInt64( u64, buffer ) )
					{
						result = pValue->setUnsignedValue( u64, pValueTypeType );
						m_isDirty = true;
					}
					else
					{
						result = false;
					}
				}
				break;

			case GenericDataTypeValueTypeType_FloatingPoint16:
			case GenericDataTypeValueTypeType_FloatingPoint32:
			case GenericDataTypeValueTypeType_FloatingPoint64:
				{
					float64 f64 = 0;
					if( string_tools::tryParseFloat64( f64, buffer ) )
					{
						result = pValue->setFloatingPoint( f64, pValueTypeType );
						m_isDirty = true;
					}
					else
					{
						result = false;
					}
				}
				break;

			case GenericDataTypeValueTypeType_String:
				result = pValue->setString( buffer );
				m_isDirty = true;
				break;

			default:
				break;
			}
		}

		if( !result )
		{
			// what to do?
		}
	}

	void GenericDataFile::doValueTagUi( GenericDataTag* pTag, bool readOnly )
	{
		GenericDataTagHandler& tagHandler = m_genericDataEditor.getTypeCollection().getTagHandler();

		ImGui::PushItemWidth( 100.0f );
		if( ImGui::BeginCombo( "\n", pTag->getTag().cStr(), readOnly ? ImGuiComboFlags_NoArrowButton : 0 ) )
		{
			for( int i = 0; i < GenericDataValueTag_Count; i++ )
			{
				const GenericDataValueTag tag = (GenericDataValueTag)i;
				const char* pTagName = tagHandler.getValueTag( tag );

				bool selected = (pTag->getTag() == pTagName);
				if( ImGui::Selectable( pTagName, &selected ) )
				{
					pTag->setTag( pTagName );
				}

				if( selected )
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		char buffer[ 1024 ];
		copyString( buffer, sizeof( buffer ), pTag->getContent().cStr() );
		if( ImGui::InputText( "", buffer, sizeof( buffer ) ) )
		{
			pTag->setContent( buffer );
		}
	}

	void GenericDataFile::doResourceUi( GenericDataValue* pValue, bool readOnly )
	{
		ImGui::Text( "resource" );
	}
}
