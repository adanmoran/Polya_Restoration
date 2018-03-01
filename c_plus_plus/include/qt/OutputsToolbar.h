//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#ifndef OUTPUTS_TOOLBAR_H_
#define OUTPUTS_TOOLBAR_H_

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QToolBar>
#include <QString>
#include <QSize>
#include <QAction>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>

#include "qt/slidersgroup.h"

class OutputsToolbar : public QToolBar
{
	Q_OBJECT

public:
	/**
	* Creates a toolbar which contains the Polya options allowed by
	* the Polya Process
	*/
	OutputsToolbar(const QString& title, QWidget* parent = nullptr);

	public slots:
	// Scale the sliders to fit inside this width
	void scaleToWidth(const QSize&);
	void scaleToWidth(int);

signals:
	void widthChanged(int);

private:
	auto initializeComboLabel() -> void;
	auto initializePushButtons() -> void;
	auto initializeSaveButton() -> void;
	auto initializeLogButton() -> void;
	auto initializeMSELabel() -> void;
	auto initializePSNRLabel() -> void;
	auto initializeSSIMLabel() -> void;
	auto initializeLabels() -> void;


	QLabel* outputsLabel_;

	const QString COMBO_LABEL = "Outputs";

	QLabel* mseLabel_;
	QLabel* psnrLabel_;
	QLabel* ssimLabel_;


	QPushButton* save_;
	QPushButton* log_;


};

#endif

/* vim: set ts=4 sw=4 et : */#pragma once
#pragma once
