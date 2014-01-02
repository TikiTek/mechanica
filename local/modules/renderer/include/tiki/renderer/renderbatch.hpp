#pragma once
#ifndef __TIKI_RENDERBATCH_HPP_INCLUDED__
#define __TIKI_RENDERBATCH_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/zoneallocator.hpp"
#include "tiki/renderer/rendercommand.hpp"

namespace tiki
{
	class RenderBatch
	{
		TIKI_NONCOPYABLE_CLASS( RenderBatch );

	public:

								RenderBatch();
								~RenderBatch();

		bool					create( uint sequenceCount, uint commandCount );
		void					dispose();

		void					reset();

		void					beginSequence( uint8 renderEffectId );
		void					queueGeometry( const ModelGeometry* pGeometry, const Matrix43* pWorldTransform = nullptr );
		bool					endSequence();

		const RenderSequence*	getRenderSequences() const		{ return m_pFirstSequence; }
		uint					getRenderSequenceCount() const	{ return m_sequenceCount; }

	private:
		
		RenderSequence*	m_pFirstSequence;
		uint			m_sequenceCount;

		bool			m_currentSequenceFailed;
		RenderSequence*	m_pCurrentSequence;
		RenderCommand*	m_pFirstSequenceCommand;

		ZoneAllocator	m_sequences;
		ZoneAllocator	m_commands;

	};
}

#endif // __TIKI_RENDERBATCH_HPP_INCLUDED__
