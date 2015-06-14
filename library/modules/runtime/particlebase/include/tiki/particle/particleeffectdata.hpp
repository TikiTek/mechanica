#pragma once
#ifndef TIKI_PARTICLEEFFECTDATA_HPP_INCLUDED__
#define TIKI_PARTICLEEFFECTDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"

#include "tiki/base/staticarray.hpp"

namespace tiki
{
	//////////////////////////////////////////////////////////////////////////
	// Emitter
	
	enum ParticleEmitterSpawnType : uint8
	{
		ParticleEmitterSpawnType_Invalid = -1,

		ParticleEmitterSpawnType_Once,
		ParticleEmitterSpawnType_TimeBased,
		ParticleEmitterSpawnType_External,

		ParticleEmitterSpawnType_Count
	};

	TIKI_REFLECTION_STRUCT(
		ParticleEmitterSpawnData,
		TIKI_REFLECTION_FIELD( timems, timeInTimeLine )
		TIKI_REFLECTION_FIELD( timems, timeInterval )
		TIKI_REFLECTION_FIELD( uint16, spawnToken )
	);

	TIKI_REFLECTION_STRUCT(
		ParticleEmitterData,
		TIKI_REFLECTION_FIELD( ParticleEmitterSpawnType, type )
		TIKI_REFLECTION_FIELD( uint8, particleTypeIndex )
		TIKI_REFLECTION_FIELD( uint16, maxParticleCount )
		TIKI_REFLECTION_FIELD( ParticleEmitterSpawnData, spawnData )
	);

	//////////////////////////////////////////////////////////////////////////
	// Particle

	enum ParticleType : uint8
	{
		ParticleType_Invalid = -1,

		ParticleType_Billboard,
		ParticleType_Model,
		ParticleType_Light,

		ParticleType_Count
	};

	enum ParticleFieldType : uint8
	{
		ParticleFieldType_Invalid = -1,

		ParticleType_Float,
		ParticleType_Double,
		ParticleType_SingedInt,
		ParticleType_UnsingedInt,

		ParticleFieldType_Count
	};

	TIKI_REFLECTION_STRUCT(
		ParticleParticleFieldData,
		TIKI_REFLECTION_FIELD( uint8, dimensions )
		TIKI_REFLECTION_FIELD( ParticleFieldType, type )
	);

	TIKI_REFLECTION_STRUCT(
		ParticleParticleData,
		TIKI_REFLECTION_FIELD( ParticleType, type )
		TIKI_REFLECTION_FIELD( uint8, particleTypeIndex )
	);

	//////////////////////////////////////////////////////////////////////////
	// Eval

	enum ParticleEvalType : uint8
	{
		ParticleEvalType_Invalid = -1,

		ParticleEvalType_Lerp,
		ParticleEvalType_Time,
		ParticleEvalType_Spline,
		ParticleEvalType_Value,
		ParticleEvalType_Add,
		ParticleEvalType_Sub,
		ParticleEvalType_Multiply,
		ParticleEvalType_Divide,

		ParticleEvalType_Count
	};

	TIKI_REFLECTION_STRUCT(
		ParticleEvalCommandData,
		TIKI_REFLECTION_FIELD( ParticleEvalType, type )
		TIKI_REFLECTION_FIELD( uint8, particleTypeIndex )
	);

	TIKI_REFLECTION_STRUCT(
		ParticleEvalData,
		TIKI_REFLECTION_ARRAY( uint8, byteCode )
	);

	TIKI_REFLECTION_STRUCT(
		ParticleEffectData,
		TIKI_REFLECTION_ARRAY( ParticleEmitterData, emitters )
		TIKI_REFLECTION_ARRAY( ParticleParticleData, particles )
		TIKI_REFLECTION_ARRAY( ParticleEvalData, evals )
		TIKI_REFLECTION_FIELD( bool, simulateWhenInvisible )
	);
}

#endif // TIKI_PARTICLEEFFECTDATA_HPP_INCLUDED__
