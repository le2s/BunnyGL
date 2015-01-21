/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef C_FULLSCREEN_RENDER_WINDOW_H
#define C_FULLSCREEN_RENDER_WINDOW_H

#include <QWidget>

class vtkRenderWindow;
class QVTKWidget;
class CFullscreenRenderWindowToolbar;

struct CFullscreenRenderWindowData;
class CFullscreenRenderWindow : public QWidget
{
	Q_OBJECT

public:
	CFullscreenRenderWindow();
	~CFullscreenRenderWindow();

	QVTKWidget* vtkWidget() const;
	CFullscreenRenderWindowToolbar* toolbar() const;

	void setRenderWindow(vtkRenderWindow* rWin);
	vtkRenderWindow* renderWindow() const;

protected:
	bool eventFilter(QObject* obj, QEvent* event);

protected slots:
	void slotActiveRenderWindowChanged();

private:
	CFullscreenRenderWindowData* d;
};

struct CFullscreenRenderWindowToolbarData;
class CFullscreenRenderWindowToolbar : public QWidget
{
	Q_OBJECT

public:
	CFullscreenRenderWindowToolbar(QWidget* parent=0);
	~CFullscreenRenderWindowToolbar();

	void addRenderWindow(vtkRenderWindow* ren, const QString& title);
	void removeRenderWindow(vtkRenderWindow* ren);
	int renderWindowCount() const;
	vtkRenderWindow* renderWindow(int index) const;

	void setActiveRenderWindow(vtkRenderWindow* rWin);
	vtkRenderWindow* activeRenderWindow() const;

    bool isMenuVisible() const;

signals:
	void activeRenderWindowChanged();
	void closeButtonClicked();

private slots:
	void slotActionTriggered(QAction* action);

private:
	CFullscreenRenderWindowToolbarData* d;
};

#endif
