#include "generic_data_file.hpp"

#include "tiki/base/string_tools.hpp"
#include "tiki/input/input_event.hpp"
#include "tiki/tool_application/tool_ui.hpp"
#include "tiki/tool_generic_data/generic_data_array.hpp"
#include "tiki/tool_generic_data/generic_data_iterator.hpp"
#include "tiki/tool_generic_data/generic_data_object.hpp"
#include "tiki/tool_generic_data/generic_data_tag.hpp"
#include "tiki/tool_generic_data/generic_data_type_array.hpp"
#include "tiki/tool_generic_data/generic_data_type_enum.hpp"
#include "tiki/tool_generic_data/generic_data_type_pointer.hpp"
#include "tiki/tool_generic_data/generic_data_type_struct.hpp"

#include "res_generic_data_editor.hpp"

#include <imgui.h>

namespace tiki
{
	static const char* s_pPopupTagEditor = "Tag Editor";

	GenericDataFile::GenericDataFile( const Path& fileName, GenericDataEditor& genericDataEditor, GenericDataRenderer& renderer )
		: EditableFile( fileName, genericDataEditor )
		, m_genericDataEditor( genericDataEditor )
		, m_renderer( renderer )
		, m_document( genericDataEditor.getDocumentCollection() )
		, m_tagsIcon( getGenericDataEditorResource( GenericDataEditorResources_TagEditor ) )
		, m_valueCreateIcon( getGenericDataEditorResource( GenericDataEditorResources_ValueCreate ) )
		, m_valueRemoveIcon( getGenericDataEditorResource( GenericDataEditorResources_ValueRemove ) )
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
		if( m_document.exportToFile( getFileName().getCompletePath() ) )
		{
			m_isDirty = false;
			return true;
		}

		return false;
	}

	void GenericDataFile::update( float deltaTime )
	{
		m_renderer.updateState( m_rendererState, deltaTime );
	}

	void GenericDataFile::render( GraphicsContext& graphicsContext )
	{
		m_renderer.renderState( m_rendererState, graphicsContext );
	}

	void GenericDataFile::doUi()
	{
		const bool canBerendered = !m_rendererState.objectInfos.isEmpty();
		if( canBerendered )
		{
			m_renderer.setBaseObject( m_rendererState, m_document.getObject() );

			ImGui::Columns( 2 );

			if( ImGui::BeginChild( "View", ImVec2( 0.0f, 0.0f ), false, ImGuiWindowFlags_NoInputs ) )
			{
				const ImVec2 position = ImGui::GetWindowPos();
				const ImVec2 size = ImGui::GetContentRegionAvail();
				TIKI_VERIFY( m_renderer.resize( uint16( size.x ), uint16( size.y ) ) );
				m_rendererRectangle = AxisAlignedRectangle::create( ImGui::Vec2( position ), ImGui::Vec2( size ) );

				ImGui::Image( ImGui::Tex( m_renderer.getOutput() ), size );

				ImGui::EndChild();
			}
			ImGui::NextColumn();
		}

		if( ImGui::BeginChild( "Data" ) )
		{
			ImGui::Columns( 2 );
			ImGui::SetColumnWidth( 0, 250.0f );

			GenericDataObject* pObject = m_document.getObject();
			if( pObject->getParentObject() != nullptr )
			{
				//m_pSelectedObject
			}

			GenericDataIterator iterator;
			iterator.create( pObject );

			doContainerUi( iterator );

			ImGui::EndChild();
		}

		if( canBerendered )
		{
			ImGui::NextColumn();

			ImGui::Columns( 1 );
		}
	}

	bool GenericDataFile::processToolInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			const Vector2 position = vector::create( inputEvent.data.mouseMoved.xState, inputEvent.data.mouseMoved.yState );
			m_hasRendererFocus = m_rendererRectangle.contains( position );
		}

		if( !m_hasRendererFocus )
		{
			return false;
		}

		return m_renderer.processStateInputEvent( m_rendererState, inputEvent );
	}

	void GenericDataFile::doContainerUi( GenericDataIterator& iterator )
	{
		DynamicString elementName;
		for( uintreg i = 0u; i < iterator.getElementCount(); ++i )
		{
			iterator.getElementName( elementName, i );
			iterator.openMemberByIndex( i );

			doElementUi( iterator, elementName );

			iterator.closeMember();

			//bool readOnlyValue = readOnly;
			//GenericDataValue* pValue = pObject->getFieldValue( i, !readOnly );
			//GenericDataValue* pNewValue = doElementUi( pObject->getFieldName( i ), pValue, pObject->getFieldType( i ), readOnly );
			//if( pNewValue != pValue && !readOnly )
			//{
			//	pObject->setFieldValue( i, pNewValue );
			//	m_isDirty = true;
			//}
			//else if( pNewValue != pValue && readOnly )
			//{
			//	TIKI_DELETE( pNewValue );
			//}
		}

		if( iterator.getEntryType() == GenericDataIteratorEntryType::Array )
		{
			ImGui::TreeNodeEx( " ", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
			ImGui::NextColumn();

			if( ImGui::Button( "Add Element" ) )
			{
				if( iterator.addArrayElement() )
				{
					m_isDirty = true;
				}
				else
				{
					getEditor().getInterface().showMessageBox( "Failed to add Array element." );
				}
			}
			ImGui::NextColumn();
		}
	}

	//void GenericDataFile::doArrayUi( GenericDataArray* pArray, bool readOnly )
	//{
	//	for( uintreg i = 0u; i < pArray->getCount(); ++i )
	//	{
	//		const DynamicString name = formatDynamicString( "%d", i );
	//		GenericDataValue* pNewValue = doElementUi( name, pArray->getElement( i ), pArray->getType()->getBaseType(), readOnly );
	//		if( pNewValue == nullptr )
	//		{
	//			pArray->removeElement( i );
	//			m_isDirty = true;
	//		}
	//	}

	//	if( !readOnly )
	//	{
	//		ImGui::TreeNodeEx( " ", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
	//		ImGui::NextColumn();

	//		if( ImGui::Button( "Add Element" ) )
	//		{
	//			pArray->addElement( true );
	//			m_isDirty = true;
	//		}
	//		ImGui::NextColumn();
	//	}
	//}

	void GenericDataFile::doElementUi( GenericDataIterator& iterator, const DynamicString& name )
	{
		const GenericDataValue* pValue	= iterator.getValue();
		const GenericDataType* pType	= iterator.getType();

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

		//ImGui::AlignTextToFramePadding();
		//ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 4.0f, 3.5f ) );
		//if( readOnly && ImGui::ImageButton( ImGui::Tex( m_valueCreateIcon ), ImGui::Vec2( m_valueCreateIcon.getSize() ) ) )
		//{
		//	GenericDataValue* pNewValue = TIKI_NEW( GenericDataValue )( pValue->getType() );
		//	if( !pNewValue->setCopyFromValue( m_genericDataEditor.getTypeCollection(), pValue ) )
		//	{
		//		TIKI_DELETE( pNewValue );
		//	}
		//	else
		//	{
		//		pValue = pNewValue;
		//	}
		//}
		//else if( !readOnly && ImGui::ImageButton( ImGui::Tex( m_valueRemoveIcon ), ImGui::Vec2( m_valueRemoveIcon.getSize() ) ) )
		//{
		//	pValue = nullptr;
		//}
		//ImGui::PopStyleVar();
		//ImGui::SameLine();

		if( pValue == nullptr )
		{
			ImGui::Text( "null" );
		}
		else if( canBeOpen )
		{
			const GenericDataObject* pSubObject = nullptr;
			if( pValue->getPointer( pSubObject ) && pSubObject == nullptr )
			{
				doPointerUi( iterator, pValue, pType );
			}
			else
			{
				ImGui::Text( getValuePreview( pValue ).cStr() );
			}
		}
		else
		{
			switch( pType->getType() )
			{
			case GenericDataTypeType_Enum:
				doEnumUi( iterator, pValue, pType );
				break;

			case GenericDataTypeType_ValueType:
				doValueUi( iterator, pValue, pType );
				break;

			case GenericDataTypeType_Reference:
				doReferenceUi( iterator, pValue, pType );
				break;

			default:
				ImGui::Text( "not supported" );
				break;
			}
		}
		ImGui::NextColumn();

		if( nodeOpen /*&& pValue != nullptr*/ )
		{
			doContainerUi( iterator );

			//if( pType->getType() == GenericDataTypeType_Array )
			//{
			//	GenericDataArray* pArray = nullptr;
			//	if( pValue->getArray( pArray ) )
			//	{
			//		doArrayUi( pArray, readOnly );
			//	}
			//}
			//else if( pType->getType() == GenericDataTypeType_Struct || pType->getType() == GenericDataTypeType_Pointer )
			//{
			//	GenericDataObject* pSubObject = nullptr;
			//	if( (pValue->getObject( pSubObject ) || pValue->getPointer( pSubObject )) && pSubObject != nullptr )
			//	{
			//		doObjectUi( pSubObject, readOnly );
			//	}
			//}

			ImGui::TreePop();
		}
		//else if( nodeOpen )
		//{
		//	ImGui::TreePop();
		//}
		ImGui::PopID();
	}

	void GenericDataFile::doEnumUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType )
	{
		const GenericDataTypeEnum* pEnumType = (const GenericDataTypeEnum*)pValue->getType();

		//if( readOnly )
		//{
		//	DynamicString enumName;
		//	if( !pValue->getEnum( enumName ) )
		//	{
		//		ImGui::Text( "invalid enum" );
		//		return;
		//	}

		//	DynamicString text = pValue->getType()->getName() + "." + enumName;
		//	ImGui::Text( text.cStr() );
		//	return;
		//}

		GenericDataTag dummyTag;
		const GenericDataTag* pValueTag = pValue->getValueTag();
		if( pValueTag == nullptr )
		{
			dummyTag.setTag( "enum" );
			pValueTag = &dummyTag;

			//pValueTag = TIKI_NEW( GenericDataTag )();
			//pValueTag->setTag( "enum" );
			//
			//pValue->setValueTag( pValueTag );
		}

		doValueTagUi( iterator, pValueTag, pValue->getType(), true );
	}

	void GenericDataFile::doValueUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType )
	{
		const GenericDataTypeValueType* pValueTypeType = (const GenericDataTypeValueType*)pType;

		const GenericDataTag* pValueTag = pValue->getValueTag();

		bool wasSelected = false;
		if( pValueTag != nullptr )
		{
			ImGui::PushStyleColor( ImGuiCol_Button, ImGui::GetStyle().Colors[ ImGuiCol_ButtonHovered ] );
			wasSelected = true;
		}
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 4.0f, 3.5f ) );

		if( ImGui::ImageButton( ImGui::Tex( m_tagsIcon ), ImGui::Vec2( m_tagsIcon.getSize() ) ) )
		{
			if( iterator.setValueTagEnabled( pValueTag == nullptr ) )
			{
				m_isDirty = true;
			}
			else
			{
				getEditor().getInterface().showMessageBox( "Failed to toggle value Tag." );
			}

			//if( pValueTag == nullptr )
			//{
			//	pValueTag = TIKI_NEW( GenericDataTag )();
			//}
			//else
			//{
			//	pValueTag = nullptr;
			//}

			//pValue->setValueTag( pValueTag );
		}

		ImGui::PopStyleVar();
		if( wasSelected )
		{
			ImGui::PopStyleColor();
		}

		ImGui::SameLine();

		if( pValueTag != nullptr )
		{
			doValueTagUi( iterator, pValueTag, pValueTypeType, false );
			return;
		}

		bool result = false;
		bool boolValue = false;
		DynamicString valueText = "";
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
		switch( pValueTypeType->getValueType() )
		{
		case GenericDataTypeValueTypeType_Boolean:
			{
				result = pValue->getBoolean( boolValue );
			}
			break;

		case GenericDataTypeValueTypeType_SingedInteger8:
		case GenericDataTypeValueTypeType_SingedInteger16:
		case GenericDataTypeValueTypeType_SingedInteger32:
		case GenericDataTypeValueTypeType_SingedInteger64:
			{
				flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank;

				sint64 s64 = 0;
				result = pValue->getSignedInteger( s64 );
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
				result = pValue->getUnsignedInteger( u64 );
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
		if( ImGui::InputText( "", buffer, sizeof( buffer ), flags ) )
		{
			switch( pValueTypeType->getValueType() )
			{
			case GenericDataTypeValueTypeType_Boolean:
				{
					if( ImGui::Checkbox( "", &boolValue ) )
					{
						result = iterator.setBoolean( boolValue );
						m_isDirty = true;
					}
				}
				break;

			case GenericDataTypeValueTypeType_SingedInteger8:
			case GenericDataTypeValueTypeType_SingedInteger16:
			case GenericDataTypeValueTypeType_SingedInteger32:
			case GenericDataTypeValueTypeType_SingedInteger64:
				{
					sint64 s64 = 0;
					if( string_tools::tryParseSInt64( s64, buffer ) )
					{
						result = iterator.setSignedInteger( s64 );
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
						result = iterator.setUnsignedInteger( u64 );
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
						result = iterator.setFloatingPoint( f64 );
						m_isDirty = true;
					}
					else
					{
						result = false;
					}
				}
				break;

			case GenericDataTypeValueTypeType_String:
				result = iterator.setString( buffer );
				m_isDirty = true;
				break;

			default:
				break;
			}
		}

		if( !result )
		{
			getEditor().getInterface().showMessageBox( "Failed to set Value." );
		}
	}

	void GenericDataFile::doValueTagUi( GenericDataIterator& iterator, const GenericDataTag* pTag, const GenericDataType* pTargetType, bool fixedTag )
	{
		GenericDataTagHandler& tagHandler = m_genericDataEditor.getTypeCollection().getTagHandler();

		GenericDataValueTag currentValueTag = GenericDataValueTag_Count;
		ImGui::PushItemWidth( 100.0f );
		if( !fixedTag && ImGui::BeginCombo( "\n", pTag->getTag().cStr() ) )
		{
			for( int i = 0; i < GenericDataValueTag_Count; i++ )
			{
				const GenericDataValueTag tag = (GenericDataValueTag)i;
				const char* pTagName = tagHandler.getValueTag( tag );

				bool selected = (pTag->getTag() == pTagName);
				if( ImGui::Selectable( pTagName, &selected ) )
				{
					currentValueTag = tag;
					iterator.setValueTagName( pTagName );
				}

				if( selected )
				{
					currentValueTag = tag;
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		else
		{
			for( int i = 0; i < GenericDataValueTag_Count; i++ )
			{
				const GenericDataValueTag tag = (GenericDataValueTag)i;
				const char* pTagName = tagHandler.getValueTag( tag );

				if( pTag->getTag() == pTagName )
				{
					currentValueTag = tag;
				}
			}
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		switch( currentValueTag )
		{
		case GenericDataValueTag_Enum:
			{
				bool hasChanged = false;
				const GenericDataTypeEnum* pEnumType = nullptr;
				const GenericDataEnumValue* pEnumValue = nullptr;
				tagHandler.parseEnum( &pEnumType, &pEnumValue, pTag->getContent() );

				if( pTargetType->getType() != GenericDataTypeType_Enum )
				{
					DynamicString enumTypeName = "";
					if( pEnumType != nullptr )
					{
						enumTypeName = pEnumType->getName();
					}

					ImGui::PushID( currentValueTag );
					ImGui::PushItemWidth( 250.0f );
					if( ImGui::BeginCombo( "\n", enumTypeName.cStr() ) )
					{
						for( const GenericDataType& type : m_genericDataEditor.getTypeCollection().getTypes() )
						{
							if( type.getType() != GenericDataTypeType_Enum )
							{
								continue;
							}

							bool selected = (&type == pEnumType);
							if( ImGui::Selectable( type.getName().cStr(), &selected ) )
							{
								pEnumType = static_cast< const GenericDataTypeEnum* >( &type );
								pEnumValue = pEnumType->getValues().getBegin();
								hasChanged = true;
							}

							if( selected )
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
					ImGui::SameLine();
				}

				if( pEnumType == nullptr )
				{
					ImGui::Text( "Invalid enum" );
					return;
				}

				DynamicString enumValueName = "";
				if( pEnumValue != nullptr )
				{
					enumValueName = pEnumValue->name;
				}

				ImGui::PushID( pEnumType );
				if( ImGui::BeginCombo( "\n", enumValueName.cStr() ) )
				{
					for( const GenericDataEnumValue& value : pEnumType->getValues() )
					{
						bool selected = (value.name == enumValueName);
						if( ImGui::Selectable( value.name.cStr(), &selected ) )
						{
							pEnumValue = &value;
							hasChanged = true;
						}

						if( selected )
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopID();

				if( hasChanged && pEnumValue != nullptr )
				{
					iterator.setValueTagContent( tagHandler.encodeEnum( pEnumType, *pEnumValue ) );
					m_isDirty = true;
				}
			}
			break;

		//case GenericDataValueTag_Reference:
		//	break;

		case GenericDataValueTag_Bit:
			{
				char buffer[ 32u ];
				copyString( buffer, sizeof( buffer ), pTag->getContent().cStr() );

				if( ImGui::InputText( "", buffer, sizeof( buffer ), ImGuiInputTextFlags_CharsHexadecimal ) )
				{
					iterator.setValueTagContent( buffer );
					m_isDirty = true;
				}
			}
			break;

		//case GenericDataValueTag_Offset:
		//	break;

		case GenericDataValueTag_Crc:
			{
				char buffer[ 1024u ];
				copyString( buffer, sizeof( buffer ), pTag->getContent().cStr() );

				if( ImGui::InputText( "", buffer, sizeof( buffer ) ) )
				{
					iterator.setValueTagContent( buffer );
					m_isDirty = true;
				}
			}
			break;

		default:
			ImGui::Text( "Unsupported Tag" );
			break;
		}

	}

	void GenericDataFile::doPointerUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType2 )
	{
		const GenericDataTypeStruct* pType = nullptr;
		if( pValue->getValueType() == GenericDataValueType_Pointer )
		{
			const GenericDataTypePointer* pPointerType = static_cast<const GenericDataTypePointer*>(pValue->getType());
			const GenericDataTypeStruct* pPointerStructType = static_cast<const GenericDataTypeStruct*>(pPointerType->getBaseType());
			pType = pPointerStructType;

			ImGuiStorage* pStorage = ImGui::GetStateStorage();

			const ImGuiID typeId = ImGui::GetID( "type" );
			void* pVoidType = pStorage->GetVoidPtr( typeId );
			if( pVoidType != nullptr )
			{
				pType = (const GenericDataTypeStruct*)pVoidType;
			}

			if( ImGui::BeginCombo( "\n", pType->getName().cStr() ) )
			{
				const GenericDataTypeCollection::TypeList& types = m_genericDataEditor.getTypeCollection().getTypes();
				for( const GenericDataType& type : types )
				{
					if( type.getType() != GenericDataTypeType_Struct )
					{
						continue;
					}

					const GenericDataTypeStruct* pStructType = static_cast<const GenericDataTypeStruct*>(&type);
					if( !pStructType->isDerivedType( pPointerStructType ) )
					{
						continue;
					}

					if( ImGui::Selectable( pStructType->getName().cStr(), pStructType == pType ) )
					{
						pType = pStructType;
						pStorage->SetVoidPtr( typeId, (void*)pStructType );
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
		}
		else
		{
			pType = static_cast<const GenericDataTypeStruct*>(pValue->getType());
		}

		if( ImGui::Button( "Create" ) )
		{
			GenericDataObject* pObject = TIKI_NEW( GenericDataObject )( m_genericDataEditor.getTypeCollection() );
			if( !pObject->create( pType, nullptr ) )
			{
				TIKI_DELETE( pObject );
				return;
			}

			if( pValue->getValueType() == GenericDataValueType_Object )
			{
				TIKI_VERIFY( iterator.setObject( pObject ) );
			}
			else
			{
				TIKI_VERIFY( iterator.setPointer( pObject ) );
			}
		}
	}

	void GenericDataFile::doReferenceUi( GenericDataIterator& iterator, const GenericDataValue* pValue, const GenericDataType* pType )
	{
		ImGui::Text( "ref not implemented" );
	}

	DynamicString GenericDataFile::getValuePreview( const GenericDataValue* pValue )
	{
		if( pValue->getValueType() == GenericDataValueType_Object ||
			pValue->getValueType() == GenericDataValueType_Pointer )
		{
			const GenericDataObject* pObject = nullptr;
			if( (pValue->getObject( pObject ) || pValue->getPointer( pObject )) && pObject != nullptr )
			{
				DynamicString fieldText;
				for( uintreg i = 0u; i < pObject->getFieldCount(); ++i )
				{
					const GenericDataLevelValue fieldValue = pObject->getFieldLevelValue( i );
					if( fieldValue.level != GenericDataValueLevel::Invalid )
					{
						if( !fieldText.isEmpty() )
						{
							fieldText += ", ";
						}

						fieldText += pObject->getFieldName( i ) + "=" + getValuePreview( fieldValue.pConstValue );

						if( fieldText.getLength() > 32u )
						{
							fieldText += ", ...";
							break;
						}
					}
				}

				if( !fieldText.isEmpty() )
				{
					return  "{" + fieldText + "}";
				}

				return pObject->getType()->getName();
			}
		}
		else if( pValue->getValueType() == GenericDataValueType_Array )
		{
			const GenericDataArray* pArray = nullptr;
			if( pValue->getArray( pArray ) && pArray != nullptr )
			{
				return pArray->getType()->getBaseType()->getName() + "[" + string_tools::toString( pArray->getCount() ) + "]";
			}
		}

		return pValue->toString();
	}
}
