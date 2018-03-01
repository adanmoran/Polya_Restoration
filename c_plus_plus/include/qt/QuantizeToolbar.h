//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#ifndef QUANTIZE_TOOLBAR_H_
#define QUANTIZE_TOOLBAR_H_

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QToolBar>
#include <QString>
#include <QSize>
#include <QAction>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>

#include "qt/slidersgroup.h"

class QuantizeToolbar : public QToolBar
{
	Q_OBJECT

public:
	/**
	* Creates a toolbar which contains the quantization options allowed by
	* the Polya Process
	*/
	QuantizeToolbar(const QString& title, QWidget* parent = nullptr);

	public slots:
	// Scale the sliders to fit inside this width
	void scaleToWidth(const QSize&);
	void scaleToWidth(int);
	void useQuantization(int);

signals:
	void widthChanged(int);
	void boxChecked(bool);
	void numballChanged(int);

private:
	auto initializeComboLabel() -> void;
	auto initializeSliders() -> void;
	auto initializeNumBallSlider() -> void;
	auto initializeCheckBox() -> void;
	auto Connections() -> void;
	auto sliderConnections() -> void;
	auto initializeRadioButtons() -> void;
	auto initializeTypeButtons() -> void;
	auto initializeInverseButtons() -> void;

	SlidersGroup* numballtypes_;

	QLabel* quantizeLabel_;

	const QString COMBO_LABEL = "Quantize";

	QCheckBox* useQuantize_;

	QAction* numballtypesAction_;

	QGroupBox* quantizeTypeBox_;
	QGroupBox* inverseQuantizeBox_;

	QHBoxLayout* quantizeTypeHBox_;
	QHBoxLayout* inverseQuantizeHBox_;

	QRadioButton* typeLloyd_;
	QRadioButton* typeUniform_;
	QRadioButton* inverseLow_;
	QRadioButton* inverseMean_;
	QRadioButton* inverseHigh_;

	QAction* quantizeTypeAction_;
	QAction* inverseQuantizeAction_;

};

#endif

/* vim: set ts=4 sw=4 et : */