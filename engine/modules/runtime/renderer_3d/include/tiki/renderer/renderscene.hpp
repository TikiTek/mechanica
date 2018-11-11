#pragma once
#ifndef TIKI_RENDERSCENE_HPP_INCLUDED
#define TIKI_RENDERSCENE_HPP_INCLUDED

#include "tiki/graphics/color.hpp"
#include "tiki/renderer/renderbatch.hpp"

namespace tiki
{
	struct RenderViewParameters;
	class RenderView;

	struct DirectionalLightData
	{
		Vector3		direction;
		Color		color;
	};

	struct PointLightData
	{
		Vector3		position;
		Color		color;
		float		range;
	};

	struct SpotLightData
	{
		Vector3		position;
		Vector3		direction;
		Color		color;
		float		range;
		float		theta;
		float		phi;
	};

	struct RenderSceneParameters
	{
		RenderSceneParameters()
		{
			maxViewCount				= 1u;
			maxDirectionalLightCount	= 1u;
			maxPointLightCount			= 1u;
			maxSpotLightCount			= 1u;

			maxRenderSequenceCount		= 1024u;
			maxRenderCommandCount		= 4096u;
		}

		uint		maxViewCount;
		uint		maxDirectionalLightCount;
		uint		maxPointLightCount;
		uint		maxSpotLightCount;

		uint		maxRenderSequenceCount;
		uint		maxRenderCommandCount;
	};

	class RenderScene
	{
		TIKI_NONCOPYABLE_CLASS( RenderScene );

	public:

		typedef SizedArray< DirectionalLightData >	DirectionalLightDataArray;
		typedef SizedArray< PointLightData >		PointLightDataArray;
		typedef SizedArray< SpotLightData >			SpotLightDataArray;
		
											RenderScene();
											~RenderScene();

		bool								create( const RenderSceneParameters& parameters );
		void								dispose();

		const RenderBatch&					getRenderBatch() const { return m_batch; }

		const DirectionalLightDataArray&	getDirectionalLights() const { return m_directionalLights; }
		const PointLightDataArray&			getPointLights() const { return m_pointLights; }
		const SpotLightDataArray&			getSpotLights() const { return m_spotLights; }

		RenderView*							addView( const RenderViewParameters& parameters );
		void								removeView( RenderView*& pView );

		DirectionalLightData&				addDirectionalLight();
		PointLightData&						addPointLight();
		SpotLightData&						addSpotLight();

		void								queueModel( const Model* pModel, const Matrix43* pWorldTransform = nullptr, const SkinningData** ppSkinningData = nullptr );

		void								clearState();

	private:

		Array< RenderView >					m_views;

		RenderBatch							m_batch;

		DirectionalLightDataArray			m_directionalLights;
		PointLightDataArray					m_pointLights;
		SpotLightDataArray					m_spotLights;

	};
}

#endif // TIKI_RENDERSCENE_HPP_INCLUDED
