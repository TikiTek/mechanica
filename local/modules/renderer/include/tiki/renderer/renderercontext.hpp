#pragma once
#ifndef __TIKI_RENDERERCONTEXT_HPP_INCLUDED__
#define __TIKI_RENDERERCONTEXT_HPP_INCLUDED__

#include "tiki/base/fixedsizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/vector.hpp"

#include "renderer.hpp"

namespace tiki
{
	class GraphicsSystem;
	class TextureData;
	
	enum 
	{
		GameRendererLimits_MaxDirectionalLights = 4u,
		GameRendererLimits_MaxPointLights		= 16u,
		GameRendererLimits_MaxSpotLights		= 4u,
	};

	struct RendererContext
	{
		GraphicsSystem*	pGraphicsSystem;

		uint			rendererWidth;
		uint			rendererHeight;

		const TextureData*	pGBuffer0;
		const TextureData*	pGBuffer1;
		const TextureData*	pGBuffer2;
		const TextureData*	pAccumulationBuffer;
		const TextureData*	pDepthBuffer;
	};

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

	struct FrameData
	{
		typedef FixedSizedArray< DirectionalLightData, GameRendererLimits_MaxDirectionalLights >	DirectionalLightDataArray;
		typedef FixedSizedArray< PointLightData, GameRendererLimits_MaxPointLights >				PointLightDataArray;
		typedef FixedSizedArray< SpotLightData, GameRendererLimits_MaxSpotLights >					SpotLightDataArray;

		float						nearPlane;
		float						farPlane;
		float						aspectRatio;
		Camera						mainCamera;

		DirectionalLightDataArray	directionalLights;
		PointLightDataArray			pointLights;
		SpotLightDataArray			spotLights;

		void reset()
		{
			directionalLights.clear();
			pointLights.clear();
			spotLights.clear();
		}
	};
}

#endif // __TIKI_RENDERERCONTEXT_HPP_INCLUDED__
