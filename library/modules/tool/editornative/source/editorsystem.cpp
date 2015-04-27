
#include "tiki/editornative/editorsystem.hpp"

namespace tiki
{
	EditorSystem::EditorSystem()
	{
		m_pGraphicsSystem = nullptr;
		m_pResourceManager = nullptr;
	}

	bool EditorSystem::create( EditorParameters^ parameters )
	{
		return false;
	}

	void EditorSystem::dispose()
	{

	}
}