#include "tiki/mechanica_components/breakable_component.hpp"

#include "tiki/components/component_state.hpp"
#include "tiki/components_2d/polygon_component.hpp"
#include "tiki/components_2d/sprite_component.hpp"
#include "tiki/components_2d/transform_2d_component.hpp"
#include "tiki/container/fixed_array.hpp"
#include "tiki/entity_system/entity_system.hpp"
#include "tiki/math/circle.hpp"
#include "tiki/math/line2.hpp"
#include "tiki/math/polygon2.hpp"
#include "tiki/physics_2d/physics_2d_body.hpp"
#include "tiki/physics_2d/physics_2d_joint.hpp"
#include "tiki/physics_2d/physics_2d_none_shape.hpp"
#include "tiki/physics_2d_components/physics_2d_body_component.hpp"

#include "entities.hpp"
#include "components_2d.hpp"
#include "physics_2d_components.hpp"
#include "mechanica_components.hpp"

#include "tiki/debug_renderer/debug_renderer.hpp"

namespace tiki
{
	struct BreakableComponentState : public ComponentState
	{
		const ResArray< BreakableFragment >*	pFragments;

		float									breakAfterSeconds;

		float									destructionForce;
		uint32									fragmentMaxDepth;
		float									fragmentMinBreakAfterSeconds;
		float									fragmentMaxBreakAfterSeconds;
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

		m_random.createFromTime();

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

	void BreakableComponent::update( double deltaTime )
	{
		Iterator componentStates = getIterator();

		State* pState = nullptr;
		while( pState = componentStates.getNext() )
		{
			if( pState->pFragments != nullptr )
			{
				continue;
			}

			pState->breakAfterSeconds -= float( deltaTime );
			if( pState->breakAfterSeconds <= 0.0f )
			{
				breakBody( pState );
			}
		}
	}

	void BreakableComponent::breakBody( BreakableComponentState* pState )
	{
		if( pState->fragmentMaxDepth )
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
			pState->pFragments			= nullptr;
			pState->breakAfterSeconds	= m_random.nextUniformFloat32( pInitData->fragmentMinBreakAfterSeconds, pInitData->fragmentMaxBreakAfterSeconds );
		}

		pState->destructionForce				= pInitData->destructionForce;
		pState->fragmentMaxDepth				= pInitData->fragmentMaxDepth;
		pState->fragmentMinBreakAfterSeconds	= pInitData->fragmentMinBreakAfterSeconds;
		pState->fragmentMaxBreakAfterSeconds	= pInitData->fragmentMaxBreakAfterSeconds;
		pState->fragmentMaterialId				= pInitData->fragmentMaterialId;

		return true;
	}

	void BreakableComponent::internalDisposeState( BreakableComponentState* pState )
	{
	}

	void BreakableComponent::breakToStaticFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const
	{
		const SpriteComponentState* pSpriteState			= m_pSpriteComponent->getFirstStateOfEntity( parentEntityComponentIterator );

		for( uint i = 0u; i < pParentState->pFragments->getCount(); ++i )
		{
			const BreakableFragment& fragment = pParentState->pFragments->getAt( i );
			createChildBreakable( m_pSpriteComponent->getLayerId( pSpriteState ), fragment.points.getView(), parentEntityComponentIterator, pParentState );
		}
	}

	void BreakableComponent::breakToCuttedFragmentEntities( const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState )
	{
		const Transform2dComponentState* pTransformState	= m_pTransformComponent->getFirstStateOfEntity( parentEntityComponentIterator );
		const Physics2dBodyComponentState* pBodyState		= m_pBodyComponent->getFirstStateOfEntity( parentEntityComponentIterator );
		const SpriteComponentState* pSpriteState			= m_pSpriteComponent->getFirstStateOfEntity( parentEntityComponentIterator );

		FixedSizedArray< Vector2, 8u > vertices;
		m_pBodyComponent->getPhysicsObject( pBodyState ).getVertices( vertices.toSlice() );

		Circle boundingCircle;
		boundingCircle.center	= Vector2::zero;
		boundingCircle.radius	= 0.0f;
		for( uint i = 0u; i < vertices.getCount(); ++i )
		{
			vector::add( boundingCircle.center, vertices[ i ] );
		}
		vector::scale( boundingCircle.center, 1.0f / vertices.getCount() );

		for( uint i = 0u; i < vertices.getCount(); ++i )
		{
			vector::sub( vertices[ i ], boundingCircle.center );
			boundingCircle.radius = TIKI_MAX( boundingCircle.radius, vector::lengthSquared( vertices[ i ] ) );
		}
		boundingCircle.radius = f32::sqrt( boundingCircle.radius );

		const float angle1 = m_random.nextUniformFloat32( 0.0f, f32::twoPi );
		const float angle2 = angle1 + f32::pi;

		Line2 cutLine;
		cutLine.start	= boundingCircle.getPointWithoutCenter( angle1 );
		cutLine.end		= boundingCircle.getPointWithoutCenter( angle2 );

		debugrenderer::drawLineCircle( boundingCircle, TIKI_COLOR_GREEN );
		debugrenderer::drawLine( cutLine, TIKI_COLOR_GRAY );

		Polygon2 sourcePolygon;
		sourcePolygon.createPolygon( vertices.getBegin(), vertices.getCount(), vertices.getCount() );

		FixedSizePolygon2< 8u > polygon1;
		FixedSizePolygon2< 8u > polygon2;
		if( !polygon::splitPolygon( polygon1, polygon2, sourcePolygon, cutLine ) )
		{
			return;
		}

		for( uint i = 0u; i < polygon1.getPointCount(); ++i )
		{
			Vector2 point = polygon1.getPoint( i );
			vector::add( point, boundingCircle.center );

			polygon1.setPoint( i, point );
		}

		for( uint i = 0u; i < polygon2.getPointCount(); ++i )
		{
			Vector2 point = polygon2.getPoint( i );
			vector::add( point, boundingCircle.center );

			polygon2.setPoint( i, point );
		}

		if( polygon1.getPointCount() >= 3u )
		{
			createChildBreakable( 5u, ArrayView< float2 >( (const float2*)polygon1.getPoints(), polygon1.getPointCount() ), parentEntityComponentIterator, pParentState );
		}

		if( polygon2.getPointCount() >= 3u )
		{
			createChildBreakable( 5u, ArrayView< float2 >( (const float2*)polygon2.getPoints(), polygon2.getPointCount() ), parentEntityComponentIterator, pParentState );
		}
	}

	void BreakableComponent::createChildBreakable( uint32 layerId, const ArrayView< float2 >& polygonPoints, const ComponentEntityIterator& parentEntityComponentIterator, const BreakableComponentState* pParentState ) const
	{
		const Transform2dComponentState* pTransformState	= m_pTransformComponent->getFirstStateOfEntity( parentEntityComponentIterator );
		const Physics2dBodyComponentState* pBodyState		= m_pBodyComponent->getFirstStateOfEntity( parentEntityComponentIterator );

		Transform2dComponentInitData transformInitData;
		createFloat2( transformInitData.position, 0.0f, 0.0f );
		createFloat2( transformInitData.scale, 1.0f, 1.0f );
		transformInitData.rotation	= 0.0f;

		PolygonComponentInitData polygonInitData;
		polygonInitData.layerId	= layerId;
		polygonInitData.points	= ResArray< float2 >( (const float2*)polygonPoints.getBegin(), polygonPoints.getCount() );

		Physics2dBodyComponentInitData bodyInitData;
		bodyInitData.density		= m_pBodyComponent->getPhysicsObject( pBodyState ).getDensity();
		bodyInitData.friction		= m_pBodyComponent->getPhysicsObject( pBodyState ).getFriction();
		bodyInitData.materialId		= pParentState->fragmentMaterialId;
		bodyInitData.freeRotation	= true;
		memory::zero( bodyInitData.position );
		bodyInitData.shape.type		= Physics2dShapeType_Polygon;
		bodyInitData.shape.vertices	= ResArray< float2 >( polygonPoints.getBegin(), polygonPoints.getCount() );

		BreakableComponentInitData breakableInitData;
		breakableInitData.destructionForce				= pParentState->destructionForce;
		breakableInitData.fragmentMaxDepth				= pParentState->fragmentMaxDepth - 1u;
		breakableInitData.fragmentMinBreakAfterSeconds	= pParentState->fragmentMinBreakAfterSeconds;
		breakableInitData.fragmentMaxBreakAfterSeconds	= pParentState->fragmentMaxBreakAfterSeconds;
		breakableInitData.fragmentMaterialId			= pParentState->fragmentMaterialId;

		EntityComponent components[] =
		{
			{ (crc32)Components2dType_Transform, &transformInitData },
			{ (crc32)Components2dType_Polygon, &polygonInitData },
			{ (crc32)Physics2dComponentType_Body, &bodyInitData },
			{ (crc32)MechanicaComponentType_Breakable, &breakableInitData }
		};

		EntityTemplateData templateData;
		templateData.components = ResArray<EntityComponent>( components, TIKI_COUNT( components ) );

		m_pEntitySystem->createEntityFromTemplate( m_pEntitySystem->getPoolIndexForId( pParentState->entityId ), templateData );
	}
}