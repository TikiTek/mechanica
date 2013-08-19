#include "tiki/navmeshconverter/navmeshconverter.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/resourcewriter.hpp"

#include "tiki/modelexport/toolmodel.hpp"
#include "Recast.h"

namespace tiki
{
	NavMeshConverter::NavMeshConverter()
	{

	}

	NavMeshConverter::~NavMeshConverter()
	{

	}

	tiki::crc32 NavMeshConverter::getInputType() const
	{
		return crcString( "navmesh" );
	}

	tiki::crc32 NavMeshConverter::getOutputType() const
	{
		return crcString( "navmesh" );
	}

	void NavMeshConverter::getDependingType( List< crc32 >& types ) const
	{
		types.add( crcString( "model" ) );
	}

	bool NavMeshConverter::initializeConverter()
	{
		return true;
	}

	void NavMeshConverter::disposeConverter()
	{

	}

	bool NavMeshConverter::startConversionJob( const ConversionParameters& params ) const
	{
		for (size_t i = 0u; i < params.inputFiles.getCount(); ++i)
		{
			const ConversionInputFile& file = params.inputFiles[ i ];
			
			float scale = params.arguments.getOptionalFloat( "scale", 1.0f );

			ToolModel model;
			model.create( file.fileName, scale );
			model.parseGeometies( false );

			rcContext  mContext( false );
			rcContext* pContext = &mContext;

			rcConfig config;

			config.cs = 0.65f;
			config.ch = 0.1f;
			config.walkableSlopeAngle = 45.0f;
			config.walkableHeight = 4;
			config.walkableClimb = 8;
			config.walkableRadius = 3;
			config.maxEdgeLen = 200;
			config.maxSimplificationError = 1.5f;
			config.minRegionArea = 35;
			config.mergeRegionArea = 100*100;
			config.maxVertsPerPoly  = 6;
			config.detailSampleDist = 1.8f;
			config.detailSampleMaxError = 0.2f;

			rcHeightfield* m_solid			= rcAllocHeightfield();
			rcCompactHeightfield* m_chf		= rcAllocCompactHeightfield();
			rcContourSet* m_cset			= rcAllocContourSet();

			createHeightfield( config, pContext, m_solid, model );
			createCompactHeightfield( config, pContext, m_solid, m_chf );
			if( !rcBuildContours( pContext, *m_chf, config.maxSimplificationError, config.maxEdgeLen, *m_cset ) )
				return false;

			rcPolyMesh* pMesh = rcAllocPolyMesh();
			if( !pMesh || !rcBuildPolyMesh( pContext, *m_cset, config.maxVertsPerPoly, *pMesh ) )
				return false;

			rcPolyMeshDetail* pDMesh = rcAllocPolyMeshDetail();
			if( !pDMesh || !rcBuildPolyMeshDetail( pContext, *pMesh, *m_chf, config.detailSampleDist, config.detailSampleMaxError, *pDMesh) )
				return false;

			rcFreeHeightField( m_solid);
			rcFreeCompactHeightfield( m_chf );
			rcFreeContourSet( m_cset );

			m_solid = nullptr;
			m_chf	= nullptr;
			m_cset	= nullptr;

			model.dispose();	

			ResourceWriter writer;
			openResourceWriter( &writer, TIKI_FOURCC( 'N', 'A', 'V', 'I' ), params.outputName, "navmesh" );

			writer.writeData( pMesh, sizeof( rcPolyMesh ) );
			writer.writeData( pMesh->verts, sizeof( unsigned short ) * 3 * pMesh->nverts );
			writer.writeData( pMesh->polys, sizeof( unsigned short ) * 2 * pMesh->maxpolys * pMesh->nvp );
			writer.writeData( pMesh->regs,  sizeof( unsigned short ) * pMesh->maxpolys );
			writer.writeData( pMesh->flags, sizeof( unsigned short ) * pMesh->maxpolys );
			writer.writeData( pMesh->areas, sizeof( unsigned char )  * pMesh->maxpolys );

			writer.writeData( pDMesh, sizeof( rcPolyMeshDetail ) );
			writer.writeData( pDMesh->meshes, sizeof( unsigned int ) * 4 * pDMesh->nmeshes );
			writer.writeData( pDMesh->verts, sizeof( float ) * 3 * pDMesh->nverts );
			writer.writeData( pDMesh->tris, sizeof( unsigned char ) * 4 * pDMesh->ntris );

			writer.writeData( &config, sizeof( rcConfig ) );

			closeResourceWriter( &writer );

			rcFreePolyMesh( pMesh );
			rcFreePolyMeshDetail( pDMesh );
		}
		return true;
	}

	void NavMeshConverter::createHeightfield( rcConfig& config, rcContext* pContext, rcHeightfield* m_solid, ToolModel& model ) const
	{
		TIKI_ASSERT( model.getGeometyCount() == 1 );
		const ToolModelGeometrie& meshGeometry = model.getGeometryByIndex( 0 );

		Vector3 rmin, rmax;

		const uint nverts = meshGeometry.getVertexCount(); 

		float* pverts = TIKI_NEW float[ nverts * 3 ];

		for( uint i = 0; i < nverts; i++ )
		{
			int di = i * 3;
			const ToolModelVertex& t = meshGeometry.getVertexByIndex( i );
			pverts[ di   ] = t.position.x;
			pverts[ di+1 ] = t.position.y;
			pverts[ di+2 ] = t.position.z;

			Vector3 tmp( t.position.x, t.position.y, t.position.z );
			rmin.minVec(tmp);
			rmax.maxVec(tmp);
		}

		Array<int> indices;
		indices.create( meshGeometry.getIndexCount() );
		memory::copy( indices.getData(), meshGeometry.getIndexData(), sizeof(int) * meshGeometry.getIndexCount() );

		for( uint i = 0; i < indices.getCount(); i += 3 )
		{
			int tmp = indices[ i + 1 ];
			indices[ i + 1 ] = indices[ i + 2 ];
			indices[ i + 2 ] = tmp;
		}

		int* tris = (int*)indices.getData();
		int  ntris = meshGeometry.getIndexCount() / 3;

		rcVcopy(config.bmin, &rmin.arr[0]);
		rcVcopy(config.bmax, &rmax.arr[0]);
		rcCalcGridSize(config.bmin, config.bmax, config.cs, &config.width, &config.height);

		if( !m_solid || !rcCreateHeightfield(pContext, *m_solid, config.width, config.height, config.bmin, config.bmax, config.cs, config.ch))
			return;

		byte* m_triareas = TIKI_NEW byte[ntris];
		if(!m_triareas)
			return;

		memset(m_triareas, 0, ntris*sizeof(unsigned char));
		rcMarkWalkableTriangles(pContext, config.walkableSlopeAngle, pverts, nverts, tris, ntris, m_triareas);
		rcRasterizeTriangles(pContext, pverts, nverts, tris, m_triareas, ntris, *m_solid, config.walkableClimb);

		delete [] pverts;
		delete [] m_triareas;
		indices.dispose();

		rcFilterLowHangingWalkableObstacles(pContext, config.walkableClimb, *m_solid);
		rcFilterLedgeSpans(pContext, config.walkableHeight, config.walkableClimb, *m_solid);
		rcFilterWalkableLowHeightSpans(pContext, config.walkableHeight, *m_solid);
	}

	void NavMeshConverter::createCompactHeightfield( rcConfig& config, rcContext* pContext, rcHeightfield* m_solid, rcCompactHeightfield* m_chf ) const
	{
		if( !m_chf || !rcBuildCompactHeightfield(pContext, config.walkableHeight, config.walkableClimb, *m_solid, *m_chf))
			return;

		if(!rcErodeWalkableArea(pContext, config.walkableRadius, *m_chf))
			return;

		if(!rcBuildDistanceField(pContext, *m_chf))
			return;

		if(!rcBuildRegions(pContext, *m_chf, 0, config.minRegionArea, config.mergeRegionArea))
			return;
	}
}


