//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#ifndef POLYA_TOOLBAR_H_
#define POLYA_TOOLBAR_H_

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
#include <QSpinBox>
#include <QPushButton>

#include "qt/slidersgroup.h"

class PolyaToolbar : public QToolBar
{
	Q_OBJECT

public:
	/**
	* Creates a toolbar which contains the Polya options allowed by
	* the Polya Process
	*/
	PolyaToolbar(const QString& title, QWidget* parent = nullptr);

	public slots:
	// Scale the sliders to fit inside this width
	void scaleToWidth(const QSize&);
	void scaleToWidth(int);
	void edgeMapSelected(bool);

signals:
	void widthChanged(int);
	void startBallChanged(int);

private:
	auto initializeComboLabel() -> void;
	auto initializeStartingBallInput() -> void;
	auto initializeRadiusSlider() -> void;
	auto intializeRadius() -> void;
	auto sliderConnections() -> void;
	auto initializeRadioButtons() -> void;
	auto initializeRadiusButtons() -> void;
	auto initializeNormButtons() -> void;
	auto initializeIterationsInput() -> void;
	auto initializeAddBallsInput() -> void;
	auto initializePushButtons() -> void;
	auto initializePlayButton() -> void;
	auto initializeResetButton() -> void;

	SlidersGroup* radiusSlider_;

	QLabel* polyaLabel_;

	const QString COMBO_LABEL = "Polya";

	QGroupBox* radiusBox_;
	QGroupBox* normBox_;
	QGroupBox* iterationsBox_;
	QGroupBox* addBallsBox_;
	QGroupBox* startingBallsBox_;

	QHBoxLayout* radiusHBox_;
	QHBoxLayout* normHBox_;
	QHBoxLayout* iterationsHBox_;
	QHBoxLayout* addBallsHBox_;
	QHBoxLayout* startingBallsHBox_;

	QRadioButton* radius1_;
	QRadioButton* radius2_;
	QRadioButton* norm1_;
	QRadioButton* norm2_;
	QRadioButton* normInf_;

	QAction* radiusButtonAction_;
	QAction* radiusSliderAction_;

	QSpinBox* iterations_;
	QSpinBox* addBalls_;
	QSpinBox* startingBalls_;

	QPushButton* play_;
	QPushButton* reset_;


};

#endif

/* vim: set ts=4 sw=4 et : */#pragma once
