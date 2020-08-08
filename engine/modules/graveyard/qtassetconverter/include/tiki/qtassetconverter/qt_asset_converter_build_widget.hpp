#pragma once
#ifndef TIKI_QT_CONVERT_BUILD_WIDGET_HPP_INCLUDED
#define TIKI_QT_CONVERT_BUILD_WIDGET_HPP_INCLUDED

#include <QDockWidget>

class QHBoxLayout;
class QLineEdit;
class QListView;
class QPushButton;
class QVBoxLayout;

namespace tiki
{
	class AssetConverterInterface;

	class QtAssetConverterBuildWidget : public QDockWidget
	{
		Q_OBJECT

	public:

									QtAssetConverterBuildWidget( AssetConverterInterface* pConverter );
		virtual						~QtAssetConverterBuildWidget();

	private:

		AssetConverterInterface*		m_pConverter;

		QWidget*						m_pMainWidget;
		QVBoxLayout*					m_pMainLayout;
		QHBoxLayout*					m_pSearchLayout;
		QLineEdit*						m_pSearchText;
		QPushButton*					m_pRescanButton;
		QListView*						m_pAssetList;
		QPushButton*					m_pBuildButton;
	};
}

#endif // TIKI_QT_CONVERT_RESULT_WIDGET_HPP_INCLUDED
