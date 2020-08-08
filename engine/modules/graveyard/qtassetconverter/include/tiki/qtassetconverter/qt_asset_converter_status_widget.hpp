#pragma once
#ifndef TIKI_QT_CONVERTER_WIDGET_HPP_INCLUDED
#define TIKI_QT_CONVERTER_WIDGET_HPP_INCLUDED

#include <QWidget>

namespace tiki
{
	class AssetConverterStatus;

	class QtAssetConverterStatusWidget : public QWidget
	{
		Q_OBJECT

	public:

								QtAssetConverterStatusWidget();
		virtual					~QtAssetConverterStatusWidget();

		void					setAssetConverterStatus( const AssetConverterStatus* pStatus );

	private:

		AssetConverterStatus*	m_pStatus;

		void					updateStatus( int test );
	};
}

#endif // TIKI_QT_ASSET_CONVERTER_WIDGET_HPP_INCLUDED
