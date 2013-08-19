#ifndef TIKI_ANIMATIONSYSTEM_HPP
#define TIKI_ANIMATIONSYSTEM_HPP

namespace tiki
{
	namespace animation
	{
		struct AnimationJoint
		{
			Quaternion	orientation;
			Vector3		translation;
			Vector3		scale;
		};

		class AnimationCommandBuffer
		{
		public:

			void		create( size_t maxCount, size_t stackSize );
			void		dispose();

			void		begin( const ModelHierarchy* pHierarchy, const bool writeDefaultPose );
			void		end();

			void		sampleAnimation( const Animation* pAnimation, const float time );

			void		lerpJoints( const float weigth );
			void		lerpJoints( const float* pWeigths );
			void		addJoints( const float weight );
			void		addJoints( const float* pWeights );

			void		exportJoints( AnimationJoint* pJoints, const size_t capacity );
			void		exportPoseMatrices( Matrix * pMatrices, const size_t capacity );
			void		exportSkinningMatrices( Matrix* pMatrices, const size_t capacity );

			void		extractJoint( AnimationJoint& joint, const size_t index );
			void		extractMatrix( Matrix& joint, const size_t index );

			void		waitForSync();

		};

		class Animation;

		struct AnimationSample 
		{
			const Animation*	pAnim;
			float				time;
			float				weight;
		};

		int main()
		{
			const Model* pModel = nullptr;
			const Animation* pAnimation1 = nullptr;
			const Animation* pAnimation2 = nullptr;

			Matrix matrices[ 128u ];

			AnimationCommandBuffer commandBuffer;
			commandBuffer.create( 256u, 24000u );

			commandBuffer.begin( pModel->getHierarchy(), false );

			AnimationSample sampels[] =
			{
				{ pAnimation1, 0.45f, 0.33f };
				{ pAnimation2, 0.45f, 0.33f };
				{ pAnimation3, 0.45f, 0.33f };
			};

			commandBuffer.sampleAnimation( sampels );

			commandBuffer.sampleAnimation( pAnimation1, 0.45f, 0.7 );
			commandBuffer.sampleAnimation( pAnimation2, 0.45f, 0.5 );
			commandBuffer.sampleAnimation( pAnimation2, 0.45f );

			float bla[] = { 0.2f, 0.4f, 0.5 };

			commandBuffer.lerpJoints( bla );
			commandBuffer.lerpJoints( 0.66f );

			commandBuffer.exportSkinningMatrices( matrices, TIKI_COUNT( matrices ) );
			commandBuffer.end();

			commandBuffer.waitForSync();

			// use matrices
		}
	}
}

#endif // TIKI_ANIMATIONSYSTEM_HPP
