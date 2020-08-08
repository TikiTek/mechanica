//#ifndef TIKI_NAVMESHCONVERTER_HPP__
//#define TIKI_NAVMESHCONVERTER_HPP__
//
//#include "tiki/converterbase/converterbase.hpp"
//
//class rcContext;
//
//struct rcConfig;
//struct rcHeightfield;
//struct rcCompactHeightfield;
//
//namespace tiki
//{
//	class ToolModel;
//
//	class NavMeshConverter : public ConverterBase
//	{
//	public:
//						NavMeshConverter();
//		virtual			~NavMeshConverter();
//
//	protected:
//
//		virtual uintreg			getConverterRevision() const { return 1u; }
//
//		virtual crc32	getInputType() const;
//
//		virtual crc32	getOutputType() const;
//
//		virtual void	getDependingType( List< crc32 >& types ) const;
//
//		virtual bool	initializeConverter();
//
//		virtual void	disposeConverter();
//
//		virtual bool	startConversionJob( const ConversionParameters& params ) const;
//
//	private:
//
//		void			createHeightfield( rcConfig& config, rcContext* pContext, rcHeightfield* m_solid, ToolModel& model ) const;
//		void			createCompactHeightfield( rcConfig& config, rcContext* pContext, rcHeightfield* m_solid, rcCompactHeightfield* m_chf ) const;
//	};
//}
//
//#endif // TIKI_NAVMESHCONVERTER_HPP__
