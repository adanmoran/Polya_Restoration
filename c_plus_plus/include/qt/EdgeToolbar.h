//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#ifndef EDGE_TOOLBAR_H_
#define EDGE_TOOLBAR_H_

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QToolBar>
#include <QString>
#include <QSize>
#include <QAction>
#include <QCheckbox>

#include "qt/slidersgroup.h"

class EdgeToolbar : public QToolBar
{
	Q_OBJECT

public:
	/**
	* Creates a toolbar which contains the edge map options allowed by
	* the Canny Filter
	*/
	EdgeToolbar(const QString& title, QWidget* parent = nullptr);

	public slots:
	// Scale the sliders to fit inside this width
	void scaleToWidth(const QSize&);
	void scaleToWidth(int);
	void useEdgeMap(int);

signals:
	void widthChanged(int);
	void boxChecked(bool);
	void sigmaedgeChanged(int);
	void thresholdChanged(int);

private:
	auto initializeComboBox() -> void;
	auto initializeComboLabel() -> void;
	auto initializeSliders() -> void;
	auto initializeSigmaSlider() -> void;
	auto initializeThresholdSlider() -> void;
	auto initializeCheckBox() -> void;
	auto Connections() -> void;
	auto sliderConnections() -> void;
	/**
	* Dropdown box which allows one to choose the noise type.
	*/

	SlidersGroup* sigmaedge_;
	SlidersGroup* threshold_;


	QLabel* edgeLabel_;

	const QString COMBO_LABEL = "Edge Map";

	QCheckBox* useedge_;

	QAction* sigmaedgeAction_;
	QAction* thresholdAction_;


};

#endif

/* vim: set ts=4 sw=4 et : */
#pragma once
