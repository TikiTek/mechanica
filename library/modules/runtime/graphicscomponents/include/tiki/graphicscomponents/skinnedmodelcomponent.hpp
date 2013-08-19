#pragma once
#ifndef TIKI_SKINNEDMODELCOMPONENT_HPP
#define TIKI_SKINNEDMODELCOMPONENT_HPP

#include "tiki/component/component.hpp"

#include "tiki/base/array.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/graphics/buffer.hpp"
#include "tiki/graphicsbase/color.hpp"
#include "tiki/math/vector3.hpp"

namespace tiki
{
	class Model;
	class Transform;
	class Material;

	class SkinnedModelComponent : public IComponent
	{
		TIKI_DEFINE_COMPONENT( SkinnedModelComponent );

	public:

		virtual								~SkinnedModelComponent();

		void								initialize( const Transform* pTransform, const Model* pModel, const Material* pMaterial = nullptr );
		void								dispose();

		const Model*						getModel() const { return m_pModel; }
		Array< Matrix >&					getPoseMatrices() { return m_poseMatrices; }
		const Matrix&						getJointMatrix( uint index ) const;
		void								getJointPoseMatrix( uint index, Matrix& mat ) const;

		static void							initializeSystem();
		static void							disposeSystem();

		static void							render( GpuContext* pContext );

		const Transform*					getTransform() const { return m_pTransform; }

		void								setMaterial( const Material* pMaterial );
		void								setColor( const Color& color );
		Color								getColor( void );

		void								setRenderOffset( const Vector3& renderOffset ) { m_renderOffset = renderOffset; }
		const Vector3&						getRenderOffset() const { return m_renderOffset; }

	private:

		enum
		{
			DefaultPoolSize = 50u
		};

											SkinnedModelComponent();

		const Transform*					m_pTransform;
		const Model*						m_pModel;

		const Material*						m_pMaterial;
		bool								m_ownMaterial;
		Color								m_color;

		Vector3								m_renderOffset;

		Array< Matrix >						m_poseMatrices;

		struct Pose
		{
			Matrix pose[ 256u ];
		};

		static Buffer< Pose, 4u >			s_skinningBuffer;

	};
}

#endif // TIKI_SKINNEDMODELCOMPONENT_HPP
