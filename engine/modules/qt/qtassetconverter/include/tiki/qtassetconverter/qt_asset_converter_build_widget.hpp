#pragma once
#ifndef TIKI_QT_CONVERT_BUILD_WIDGET_HPP_INCLUDED
#define TIKI_QT_CONVERT_BUILD_WIDGET_HPP_INCLUDED

#include <QWidget>

namespace tiki
{
	class IAssetConverter;
	class IAssetConverterBuild;

	class QtAssetConverterBuildWidget : public QWidget
	{
		Q_OBJECT

	public:

								QtAssetConverterBuildWidget();
		virtual					~QtAssetConverterBuildWidget();

	private:

		IAssetConverter*		m_pConverter;
		IAssetConverterBuild*	m_pBuild;
	};
}

#endif // TIKI_QT_CONVERT_RESULT_WIDGET_HPP_INCLUDED
