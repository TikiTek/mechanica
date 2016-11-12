#include "tiki/mechanica_components/breakablecomponent.hpp"

#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/polygoncomponent.hpp"
#include "tiki/components2d/spritecomponent.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/container/fixedarray.hpp"
#include "tiki/entitysystem/entitysystem.hpp"
#include "tiki/physics2d/physics2dbody.hpp"
#include "tiki/physics2d/physics2djoint.hpp"
#include "tiki/physics2d/physics2dnoneshape.hpp"
#include "tiki/physics2dcomponents/physics2dbodycomponent.hpp"

#include "entities.hpp"
#include "components2d.hpp"
#include "physics2dcomponents.hpp"
#include "mechanica_components.hpp"

namespace tiki
{
	struct BreakableComponentState : public ComponentState
	{
		const ResArray< BreakableFragment >*	pFragments;

		float									destructionForce;
		float									breakAfterSeconds;
		bool									enableBreakTimer;
		uint32									fragmentMaterialId;
	};
	TIKI_COMPONENT_STATE_CONSTRUCT_FUNCTIONS( BreakableComponentState );

	BreakableComponent::BreakableComponent()
		: Component( MechanicaComponentType_Breakable, "BreakableComponent", sizeof( BreakableComponentState ), true )
	{
		m_pEntitySystem			= nullptr;
		m_pPhysicsWorld			= nullptr;
		m_pTransformComponent	= nullptr;
		m_pBodyComponent		= nullptr;
		m_pSpriteComponent		= nullptr;
		m_pPolgonComponent		= nullptr;
	}

	BreakableComponent::~BreakableComponent()
	{
		TIKI_ASSERT( m_pEntitySystem		== nullptr );
	}

	bool BreakableComponent::create( EntitySystem& entitySystem, Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent, const Physics2dBodyComponent& bodyComponent, const SpriteComponent& spriteComponent, const PolygonComponent& polygonComponent )
	{
		m_pEntitySystem			= &entitySystem;
		m_pPhysicsWorld			= &physicsWorld;
		m_pTransformComponent	= &transformComponent;
		m_pBodyComponent		= &bodyComponent;
		m_pSpriteComponent		= &spriteComponent;
		m_pPolgonComponent		= &polygonComponent;

		return true;
	}

	void BreakableComponent::dispose()
	{
		m_pEntitySystem			= nullptr;
		m_pPhysicsWorld			= nullptr;
		m_pTransformComponent	= nullptr;
		m_pBodyComponent		= nullptr;
		m_pSpriteComponent		= nullptr;
		m_pPolgonComponent		= nullptr;
	}

	void BreakableComponent::update( float deltaTime ) const
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while( pState = componentStates.getNext() )
		{
			if( !pState->enableBreakTimer )
			{
				continue;
			}

			pState->breakAfterSeconds -= deltaTime;

			if( pState->breakAfterSeconds <= 0.0f )
			{
				//breakBody( pState );
			}
		}
	}

	void BreakableComponent::breakBody( BreakableComponentState* pState ) const
	{
		ComponentEntityIterator entityComponentIterator( m_pEntitySystem->getFirstComponentOfEntity( pState->entityId ) );

		if( pState->pFragments != nullptr )
		{
			breakToStaticFragmentEntities( entityComponentIterator, pState );
		}
		else
		{
			breakToCuttedFragmentEntities( entityComponentIterator, pState );
		}

		m_pEntitySystem->disposeEntity( pState->entityId );
	}

	bool BreakableComponent::internalInitializeState( const ComponentEntityIterator& componentIterator, BreakableComponentState* pState, const BreakableComponentInitData* pInitData )
	{
		if( pInitData->fragments.getCount() > 0u )
		{
			pState->pFragments = &pInitData->fragments;
		}
		else
		{
			pState->pFragments = nullptr;
		}

		pState->destructionForce	= pInitData->destructionForce;
		pState->breakAfterSeconds	= pInitData->breakAfterSeconds;
		pState->enableBreakTimer	= pInitData->enableBreakTimer;
		pState->fragmentMaterialId	= pInitData->fragmentMaterialId;

		return true;
	}

	void BreakableComponent::internalDisposeState( BreakableComponentState* pState )
	{
	}

	void BreakableComponent::breakToStaticFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const
	{
		const Transform2dComponentState* pTransformState	= m_pTransformComponent->getFirstStateOfEntity( parentEntityComponentIterator );
		const Physics2dBodyComponentState* pBodyState		= m_pBodyComponent->getFirstStateOfEntity( parentEntityComponentIterator );
		const SpriteComponentState* pSpriteState			= m_pSpriteComponent->getFirstStateOfEntity( parentEntityComponentIterator );

		for( uint i = 0u; i < pParentState->pFragments->getCount(); ++i )
		{
			const BreakableFragment& fragment = pParentState->pFragments->getAt( i );

			Transform2dComponentInitData transformInitData;
			createFloat2( transformInitData.position, 0.0f, 0.0f );
			createFloat2( transformInitData.scale, 1.0f, 1.0f );
			transformInitData.rotation	= 0.0f;

			PolygonComponentInitData polygonInitData;
			polygonInitData.layerId	= m_pSpriteComponent->getLayerId( pSpriteState );
			polygonInitData.points	= fragment.points;

			Physics2dBodyComponentInitData bodyInitData;
			bodyInitData.density		= m_pBodyComponent->getDensity( pBodyState );
			bodyInitData.friction		= m_pBodyComponent->getFriction( pBodyState );
			bodyInitData.materialId		= pParentState->fragmentMaterialId;
			bodyInitData.freeRotation	= true;
			memory::zero( bodyInitData.position );
			bodyInitData.shape.type		= Physics2dShapeType_Polygon;
			bodyInitData.shape.vertices	= fragment.points;

			BreakableComponentInitData breakableInitData;
			breakableInitData.destructionForce		= pParentState->destructionForce;
			breakableInitData.breakAfterSeconds		= 5.0f;
			breakableInitData.enableBreakTimer		= true;
			breakableInitData.fragmentMaterialId	= pParentState->fragmentMaterialId;

			EntityComponent components[] =
			{
				{ Components2dType_Transform, &transformInitData },
				{ Components2dType_Polygon, &polygonInitData },
				{ Physics2dComponentType_Body, &bodyInitData },
				{ MechanicaComponentType_Breakable, &breakableInitData },
			};

			EntityTemplateData templateData;
			templateData.components = ResArray<EntityComponent>( components, TIKI_COUNT( components ) );

			m_pEntitySystem->createEntityFromTemplate( m_pEntitySystem->getPoolIndexForId( pParentState->entityId ), templateData );
		}
	}

	void BreakableComponent::breakToCuttedFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const
	{

	}
}