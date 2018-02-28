/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <iostream>
#include <QMainWindow>
#include <QImage>
#include <QObject>
//SG
#include <QComboBox>
#include <qwidget.h>
/*#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif*/
//SG
#include <QAction>
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
//SG Drop Down Menu
class QComboBox;
//SG
//SG Sliders
class QCheckBox;
class QGroupBox;
class QLabel;
class QSpinBox;
class QStackedWidget;
class SlidersGroup;
//SG

class ImageViewer : public QMainWindow
{
	Q_OBJECT

signals:
	void resized(const QSize&);

public slots:
	void chooseNoise(const QString& noiseType)
	{
		if (noiseType == QString("None"))
		{
			bsaction->setVisible(false);
			gsaction->setVisible(false);
			// disable gsaction
		}
		else if (noiseType == QString("Binary Burst"))
		{
			bsaction->setVisible(true);
			gsaction->setVisible(false);
			// disable gsaction
		}
		else
		{
			//enable gsaction
			gsaction->setVisible(true);
			bsaction->setVisible(false);
		}
	}
public:
	ImageViewer();
	bool loadFile(const QString &);

	private slots:
	void open();
	void saveAs();
	void print();
	void copy();
	void paste();
	void zoomIn();
	void zoomOut();
	void normalSize();
	void fitToWindow();
	void about();
	void resizeEvent(QResizeEvent* event)
	{
		emit resized(size());
	}

private:
	void createActions();
	void createMenus() {}
	void updateActions();
	bool saveFile(const QString &fileName);
	void setImage(const QImage &newImage);
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);
//SG Sliders
	void createControls(const QString &title);
//SG
	QImage image;
	QLabel *imageLabel;
	QScrollArea *scrollArea;
	double scaleFactor;

/*SG#ifndef QT_NO_PRINTER
	QPrinter printer;
#endifSG*/

	QAction *saveAsAct;
	QAction *printAct;
	QAction *copyAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	QAction *fitToWindowAct;
//SG Drop Down Menu
	QComboBox *typeComboBox;
//SG
//SG Sliders
	SlidersGroup *burstsigma;
	QAction* bsaction = nullptr;
	QAction* gsaction = nullptr;
	SlidersGroup *horizontalSliders;
	SlidersGroup *verticalSliders;
	QStackedWidget *stackedWidget;

	QGroupBox *controlsGroup;
	QLabel *minimumLabel;
	QLabel *maximumLabel;
	QLabel *valueLabel;
	QCheckBox *invertedAppearance;
	QCheckBox *invertedKeyBindings;
	QSpinBox *minimumSpinBox;
	QSpinBox *maximumSpinBox;
	QSpinBox *valueSpinBox;
	QComboBox *orientationCombo;
//SG
};

#endif
