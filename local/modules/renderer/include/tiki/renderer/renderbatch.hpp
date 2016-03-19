#pragma once
#ifndef __TIKI_RENDERBATCH_HPP_INCLUDED__
#define __TIKI_RENDERBATCH_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/zoneallocator.hpp"
#include "tiki/renderer/rendercommand.hpp"

#include "renderer.hpp"

namespace tiki
{
	class Material;
	class Model;
	class ModelGeometry;
	struct Matrix43;
	struct SkinningData;

	class RenderSequenceEnumerator
	{
	public:

		RenderSequenceEnumerator( const RenderSequence* pBegin, const RenderSequence* pEnd, RenderPass pass );

		const RenderSequence*	enumerate();

	private:

		RenderPass				m_renderPass;

		const RenderSequence*	m_pBeginSequence;
		const RenderSequence*	m_pEndSequence;

		const RenderSequence*	m_pIterator;

	};

	class RenderBatch
	{
		TIKI_NONCOPYABLE_CLASS( RenderBatch );

	public:

									RenderBatch();
									~RenderBatch();

		bool						create( uint sequenceCount, uint commandCount );
		void						dispose();

		void						reset();

		RenderSequenceEnumerator	createEnumerator( RenderPass pass ) const;

		void						queueModel( const Model* pModel, const Matrix43* pWorldTransform = nullptr, const SkinningData** ppSkinningData = nullptr );

		void						beginSequence( RenderPassMask passMask, RenderEffectId renderEffectId, uint8 renderFlags );
		void						queueGeometry( const ModelGeometry& geometry, const Material* pMaterial, const Matrix43* pWorldTransform = nullptr, const SkinningData* pSkinningData = nullptr );
		bool						endSequence();

	private:
		
		RenderSequence*			m_pFirstSequence;
		RenderSequence*			m_pEndSequence;

		bool					m_currentSequenceFailed;
		RenderSequence*			m_pCurrentSequence;

		ZoneAllocator			m_sequences;
		ZoneAllocator			m_commands;

	};
}

#endif // __TIKI_RENDERBATCH_HPP_INCLUDED__
