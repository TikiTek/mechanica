#pragma once
#ifndef TIKI_DEBUGRENDERER_TYPES_HPP_INCLUDED
#define TIKI_DEBUGRENDERER_TYPES_HPP_INCLUDED

#include "tiki/container/fixedarray.hpp"
#include "tiki/math/axisalignedbox.hpp"
#include "tiki/math/box.hpp"
#include "tiki/math/frustum.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/ray.hpp"

namespace tiki
{
	enum DebugRenderCommandType
	{
		DebugRenderCommandType_DrawLines,
		DebugRenderCommandType_DrawLineRay,
		DebugRenderCommandType_DrawLineBox,
		DebugRenderCommandType_DrawLineAxisAlignedBox,
		DebugRenderCommandType_DrawLineAxes,
		DebugRenderCommandType_DrawLineGrid,
		DebugRenderCommandType_DrawLineCircle,
		DebugRenderCommandType_DrawLineSphere,
		DebugRenderCommandType_DrawLineFrustum,

		DebugRenderCommandType_DrawSolidBox,
		DebugRenderCommandType_DrawSolidAxes,
		DebugRenderCommandType_DrawSolidTriangle,
		DebugRenderCommandType_DrawSolidCircle,
		DebugRenderCommandType_DrawSolidRectangle,

		DebugRenderCommandType_DrawText,
		DebugRenderCommandType_DrawText3D,

		DebugRenderCommandType_SetOption
	};
	
	struct DebugRenderCommand
	{
		DebugRenderCommand*		pNext;
		DebugRenderCommandType	type;
	};

	struct DebugRenderLinesCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLines;

		Color					color;
		uint					pointCount;
		bool					drawAsStrip;
		Vector3					aPoints[ 1u ];
	};

	struct DebugRenderLineRayCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineRay;

		Ray						ray;
		float					length;
		Color					color;
	};

	struct DebugRenderLineBoxCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineBox;

		Box						box;
		Color					color;
	};

	struct DebugRenderLineAxisAlignedBoxCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineAxisAlignedBox;

		AxisAlignedBox			box;
		Color					color;
	};

	struct DebugRenderLineAxesCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineAxes;

		float					lineLength;
		float					lineOffset;
		Matrix43				worldMatrix;
	};

	struct DebugRenderLineGridCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineGrid;

		float					gridSpace;
		float					gridSize;
		Color					color;
	};

	struct DebugRenderLineCircleCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineCircle;

		Vector3					center;
		float					radius;
		Vector3					normal;
		Vector3					tangent;
		Color					color;
	};

	struct DebugRenderLineSphereCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineSphere;

		Vector3					center;
		float					radius;
		Color					color;
	};

	struct DebugRenderLineFrustumCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawLineFrustum;

		Frustum					frustum;
		Color					color;
	};

	struct DebugRenderSolidBoxCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawSolidBox;

		Box						box;
		Color					color;
	};

	struct DebugRenderSolidAxesCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawSolidAxes;

		float					lineLength;
		float					lineOffset;
		Matrix43				worldMatrix;
	};

	struct DebugRenderSolidTriangleCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawSolidTriangle;

		FixedArray<Vector3, 3u>	points;
		Color					color;
	};

	struct DebugRenderSolidCircleCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawSolidCircle;

		Vector3					center;
		float					radius;
		Vector3					normal;
		Vector3					tangent;
		Color					color;
	};

	struct DebugRenderSolidRectangleCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawSolidRectangle;

		Vector3					center;
		Vector2					extends;
		Vector3					normal;
		Vector3					tangent;
		Color					color;
	};

	struct DebugRenderTextCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawText;

		Vector2					position;
		Color					color;
		char					text[ 1u ];
	};

	struct DebugRenderText3DCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_DrawText3D;

		Vector3					position;
		Color					color;
		char					text[ 1u ];
	};

	struct DebugRenderSetOptionCommand : public DebugRenderCommand
	{
		static const DebugRenderCommandType CommandType = DebugRenderCommandType_SetOption;


	};
}

#endif // TIKI_DEBUGRENDERER_TYPES_HPP_INCLUDED
