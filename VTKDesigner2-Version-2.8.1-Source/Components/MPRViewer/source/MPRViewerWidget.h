/****************************************************************************
**
** Copyright (C) VCreateLogic
**
** Use of this file is limited according to the terms specified by
** VCreateLogic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef MPRVIEWERWIDGET_H
#define MPRVIEWERWIDGET_H

#include <QWidget>

class vtkImageData;

struct MPRViewerWidgetData;
class MPRViewerWidget : public QWidget
{
	Q_OBJECT

public:
	MPRViewerWidget(QWidget* parent=0);
	~MPRViewerWidget();

    void setDicomImage(vtkImageData* image);
    vtkImageData* dicomImage() const;

private slots:
    void axialSliderValueChanged(int val);
    void sagittalSliderValueChanged(int val);
    void coronalSliderValueChanged(int val);

protected:
    void showEvent(QShowEvent* se);

private:
	MPRViewerWidgetData* d;
};

#endif