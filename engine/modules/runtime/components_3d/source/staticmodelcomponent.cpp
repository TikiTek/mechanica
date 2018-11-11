#include "tiki/components3d/staticmodelcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components3d/transform3dcomponent.hpp"
#include "tiki/renderer/renderscene.hpp"

#include "components3d.hpp"

namespace tiki
{
	struct Transform3dComponentState;

	struct StaticModelComponentState : public ComponentState
	{
		const Transform3dComponentState*	pTransform;
		const Model*					pModel;
	};

	StaticModelComponent::StaticModelComponent()
	{
		m_pTransformComponent = nullptr;
	}

	StaticModelComponent::~StaticModelComponent()
	{
		TIKI_ASSERT( m_pTransformComponent == nullptr );
	}

	bool StaticModelComponent::create( Transform3dComponent& transformComponent )
	{
		m_pTransformComponent = &transformComponent;

		return true;
	}

	void StaticModelComponent::dispose()
	{
		m_pTransformComponent = nullptr;
	}

	void StaticModelComponent::render( RenderScene& scene ) const
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Matrix43 worldTransform;
			m_pTransformComponent->getWorldTransform( worldTransform, pState->pTransform );

			scene.queueModel( pState->pModel, &worldTransform, nullptr );
		}
	}

	crc32 StaticModelComponent::getTypeCrc() const
	{
		return crcString( "StaticModelComponent" );
	}

	uint32 StaticModelComponent::getStateSize() const
	{
		return sizeof( StaticModelComponentState );
	}

	const char* StaticModelComponent::getTypeName() const
	{
		return "StaticModelComponent";
	}

	bool StaticModelComponent::internalInitializeState( ComponentEntityIterator& componentIterator, StaticModelComponentState* pState, const StaticModelComponentInitData* pInitData )
	{
		pState->pTransform = (const Transform3dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		if ( pState->pTransform == nullptr )
		{
			return false;
		}

		pState->pModel = pInitData->model.getData();
		if ( pState->pModel == nullptr )
		{
			return false;
		}

		return true;
	}

	void StaticModelComponent::internalDisposeState( StaticModelComponentState* pState )
	{
		pState->pTransform	= nullptr;
		pState->pModel		= nullptr;
	}
}