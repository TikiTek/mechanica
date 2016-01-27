#include "tiki/ui/uielementscript.hpp"

#include "tiki/script/scriptcontext.hpp"
#include "tiki/ui/uielement.hpp"

namespace tiki
{
	bool UiElementScript::create( ScriptContext& scriptContext )
	{
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
	}

	void UiElementScript::setPosition( const ScriptCall& call )
	{

	}

	void UiElementScript::setWidth( const ScriptCall& call )
	{
		UiElement* pElement = (UiElement*)call.getInstance();

		const float value = (float)call.getArgument( 0 ).getFloatValue();
		pElement->setWidth( UiSize( value ) );
	}

	void UiElementScript::setHeight( const ScriptCall& call )
	{
		UiElement* pElement = (UiElement*)call.getInstance();

		const float value = (float)call.getArgument( 0 ).getFloatValue();
		pElement->setHeight( UiSize( value ) );
	}
}