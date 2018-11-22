#pragma once

#include "tiki/generic_data_editor/generic_data_view.hpp"

namespace tiki
{
	class SpriteComponentView : public GenericDataView
	{
	public:

									SpriteComponentView( GenericDataTypeCollection& typeCollection, Transform2dComponentView& transformView );
		virtual						~SpriteComponentView();

		bool						getTextureName( DynamicString& targetTextureName, const GenericDataObject* pSpriteObject ) const;
		bool						getOffset( Vector2& targetOffset, const GenericDataObject* pSpriteObject ) const;
		bool						getLayerId( uint32& targetLayerId, const GenericDataObject* pSpriteObject ) const;

		virtual void				updateObject( GenericDataViewInfo& objectInfo, GenericDataViewInfo* pParentInfo ) TIKI_OVERRIDE_FINAL;
		virtual void				renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo ) TIKI_OVERRIDE_FINAL;

	private:

		Transform2dComponentView&	m_transformView;
	};
}