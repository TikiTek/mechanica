#pragma once

#include "tiki/editor_interface/editor_ribbon.hpp"

namespace tiki
{
	class PackageEditorRibbon : public EditorRibbon
	{
		TIKI_NONCOPYABLE_CLASS( PackageEditorRibbon );

	public:

								PackageEditorRibbon();
		virtual					~PackageEditorRibbon();

		virtual void			doUi() TIKI_OVERRIDE_FINAL;

	private:

	};
}
