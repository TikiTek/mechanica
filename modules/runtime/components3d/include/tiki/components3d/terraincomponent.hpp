#pragma once
#ifndef TIKI_TERRAINCOMPONENT_HPP_INCLUDED
#define TIKI_TERRAINCOMPONENT_HPP_INCLUDED

#include "tiki/components/component.hpp"

namespace tiki
{
	class RenderScene;
	class Transform3dComponent;
	struct TerrainComponentInitData;
	struct TerrainComponentState;
	struct Vector2;

	class TerrainComponent : public Component< TerrainComponentState, TerrainComponentInitData >
	{
		TIKI_NONCOPYABLE_CLASS( TerrainComponent );

	public:

		explicit			TerrainComponent();
		virtual				~TerrainComponent();

		bool				create( Transform3dComponent& transformComponent );
		void				dispose();

		void				render( RenderScene& scene ) const;

		float				getHeightAtPosition( const TerrainComponentState* pComponentState, const Vector2& position ) const;

		virtual uint32		getStateSize() const TIKI_OVERRIDE;
		virtual const char*	getTypeName() const TIKI_OVERRIDE;

	protected:

		virtual bool		internalInitializeState( ComponentEntityIterator& componentIterator, TerrainComponentState* pComponentState, const TerrainComponentInitData* pComponentInitData ) TIKI_OVERRIDE;
		virtual void		internalDisposeState( TerrainComponentState* pComponentState ) TIKI_OVERRIDE;

	private:

		const Transform3dComponent*	m_pTransformComponent;

	};
}

#endif // TIKI_TERRAINCOMPONENT_HPP_INCLUDED
