#ifndef TICTACTOEWIDGET_H
#define TICTACTOEWIDGET_H

#include <QWidget>

struct TicTacToeWidgetData;
class TicTacToeWidget : public QWidget
{
    Q_OBJECT

public:
    TicTacToeWidget(QWidget *parent = 0);
    ~TicTacToeWidget();

public slots:
    // This slot marks the (row, col) spot on the board
    // with a X icon.
    void mark(int row, int col);

    // This slot clears all marks on the board.
    void clear();

    // This slot shows a message box, signalling that
    // the player using this instance of the widget
    // has lost.
    void iLost();

signals:
    // This signal is emitted when the user using this
    // instance of the widget has clicked on an empty
    // (row,col) spot.
    void marked(int row, int col);

    // This signal is emitted when this instance of the
    // widget has been cleared.
    void cleared();

    // This signal is meitted when 3 consequtive spots
    // on this instance of the widget have been maked
    // by the user who is playing on this instance.
    void iWon();

    // This signal is emitted when this widget is closed.
    void closed();

protected:
    bool event(QEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *);
    bool checkIfIWon();
    bool checkIfDraw();

private:
    TicTacToeWidgetData* d;
};

#endif // TICTACTOEWIDGET_H
