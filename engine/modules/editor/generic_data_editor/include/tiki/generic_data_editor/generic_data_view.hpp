#pragma once

#include "tiki/base/types.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"

namespace tiki
{
	class GenericDataObject;
	class GenericDataTypeCollection;
	class GenericDataTypeStruct;
	class GenericDataView;
	class Renderer2d;
	struct InputEvent;

	struct GenericDataViewInfo
	{
		GenericDataView*			pView;
		GenericDataObject*			pObject;

		GenericDataObject*			pParent;
		List< GenericDataObject* >	childObjects;

		bool						isActive;
		uint32						focusLayer;
		AxisAlignedRectangle		rectangle;
	};

	class GenericDataView
	{
	public:

		const GenericDataTypeStruct*	getObjectType() const { return m_pType; }

		virtual void					updateObject( GenericDataViewInfo& objectInfo, GenericDataViewInfo* pParentInfo ) = 0;
		virtual void					renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo ) = 0;

		//virtual bool					handleInputEvent( const InputEvent& inputEvent, GenericDataViewState* pState ) = 0;

	protected:

										GenericDataView( GenericDataTypeCollection& typeCollection, const char* pTypeName );
		virtual							~GenericDataView() { }

		GenericDataTypeCollection&		m_typeCollection;
		const GenericDataTypeStruct*	m_pType;
	};
}