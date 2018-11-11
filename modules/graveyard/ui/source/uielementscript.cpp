#include "tiki/ui/uielementscript.hpp"

#include "tiki/script/scriptcontext.hpp"
#include "tiki/ui/uielement.hpp"
#include "tiki/ui/uisystem.hpp"

namespace tiki
{
	struct UiElementScriptData
	{
		UiSystem*	pUiSystem;
		UiElement*	pUiElement;
	};

	UiElementScript::UiElementScript()
	{
		m_pUiSystem = nullptr;
	}

	UiElementScript::~UiElementScript()
	{
		TIKI_ASSERT( m_pUiSystem == nullptr );
	}

	bool UiElementScript::create( ScriptContext& scriptContext, UiSystem& uiSystem )
	{
		m_pUiSystem = &uiSystem;

		static const ScriptMethod s_aMethods[] =
		{
			{ "setPosition",	&ScriptContext::scriptWrapperFunction< UiElementScript::setPosition > },
			{ "setWidth",		&ScriptContext::scriptWrapperFunction< UiElementScript::setWidth > },
			{ "setHeight",		&ScriptContext::scriptWrapperFunction< UiElementScript::setHeight > }
		};

		if( !ScriptClass::create( scriptContext, "UiElement", s_aMethods, TIKI_COUNT( s_aMethods ) ) )
		{
			return false;
		}

		return true;
	}

	void UiElementScript::dispose()
	{
		ScriptClass::dispose();

		m_pUiSystem = nullptr;
	}

	ScriptValue UiElementScript::createScriptElement( const ScriptValue& parentValue )
	{
		UiElement* pParent = nullptr;
		if( parentValue.isValid() )
		{
			const UiElementScriptData& scriptData = *(const UiElementScriptData*)parentValue.getObjectInstanceData();
			pParent = scriptData.pUiElement;
		}

		UiElement* pElement = m_pUiSystem->addElement( pParent );

		UiElementScriptData scriptData;
		scriptData.pUiSystem	= m_pUiSystem;
		scriptData.pUiElement	= pElement;

		return registerInstance( &scriptData, sizeof( scriptData ) );
	}

	void UiElementScript::disposeScriptElement( const ScriptValue& elementValue )
	{
		const UiElementScriptData& scriptData = *(const UiElementScriptData*)elementValue.getObjectInstanceData();
		scriptData.pUiSystem->removeElement( scriptData.pUiElement );
	}

	/*static*/ void UiElementScript::setPosition( const ScriptCall& call )
	{

	}

	/*static*/ void UiElementScript::setWidth( const ScriptCall& call )
	{
		const UiElementScriptData& scriptData = *(const UiElementScriptData*)call.getInstanceData();

		const float value = (float)call.getArgument( 0u ).getFloatValue();
		scriptData.pUiElement->setWidth( UiSize( value ) );
	}

	/*static*/ void UiElementScript::setHeight( const ScriptCall& call )
	{
		const UiElementScriptData& scriptData = *(const UiElementScriptData*)call.getInstanceData();

		const float value = (float)call.getArgument( 0u ).getFloatValue();
		scriptData.pUiElement->setHeight( UiSize( value ) );
	}
}