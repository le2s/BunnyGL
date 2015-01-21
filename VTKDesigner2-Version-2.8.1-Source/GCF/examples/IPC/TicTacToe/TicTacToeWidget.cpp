#include "TicTacToeWidget.h"

#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>

enum MarkType
{
    NoMark,
    MyMark,
    OthersMark
};

struct TicTacToeWidgetData
{
    TicTacToeWidgetData() {
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                marks[i][j] = NoMark;
        this->isClear = true;
        this->isMyTurn = true;
    }

    MarkType marks[3][3];
    bool isClear;
    bool isMyTurn;

    void paintMyMark(QPainter* paint, const QRectF& rect);
    void paintOtherMark(QPainter* paint, const QRectF& rect);
};

TicTacToeWidget::TicTacToeWidget(QWidget *parent) :
    QWidget(parent)
{
    d = new TicTacToeWidgetData;
    this->setMinimumSize(170, 170);
}

TicTacToeWidget::~TicTacToeWidget()
{
    delete d;
}

// This slot marks the (row, col) spot on the board
// with a X icon.
void TicTacToeWidget::mark(int row, int col)
{
    if(d->marks[row][col] != NoMark)
        return;

    d->marks[row][col] = OthersMark;
    d->isClear = false;
    d->isMyTurn = true;
    this->update();

    if( this->checkIfDraw() )
    {
        QMessageBox::information(this, "Match DRAW",
                                 "Arrrgg... The match was drawn. Nobody won. Click Ok to "
                                 "play again");
        this->clear();
    }
}

// This slot clears all marks on the board.
void TicTacToeWidget::clear()
{
    if(d->isClear)
        return;

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            d->marks[i][j] = NoMark;
    d->isClear = true;
    d->isMyTurn = true;

    emit cleared();
    this->update();
}

// This slot shows a message box, signalling that
// the player using this instance of the widget
// has lost.
void TicTacToeWidget::iLost()
{
    QMessageBox::information(this, "You Lost",
                             "The other player out-smarted you. You LOST!!!");
}

bool TicTacToeWidget::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::WindowTitleChange: {
        QString title = this->windowTitle();
        if(title.endsWith("]"))
            break;
#ifdef QT_NO_DEBUG
        title += " [Release]";
#else
        title += " [Debug]";
#endif
        this->setWindowTitle(title);
        return true;
        } break;
    default:
        break;
    }

    return QWidget::event(event);
}

void TicTacToeWidget::paintEvent(QPaintEvent *)
{
    QRectF rect = this->rect();
    rect.adjust(10, 10, -10, -25);

    qreal cellWidth = rect.width()/3.0;
    qreal cellHeight = rect.height()/3.0;

    QPainter paint(this);
    paint.setPen(QPen(Qt::black,3));
    paint.setRenderHint(QPainter::Antialiasing);

    for(int i=1; i<=2; i++)
    {
        paint.drawLine( rect.left(), rect.top()+cellHeight*i,
                       rect.right(), rect.top()+cellHeight*i );
        paint.drawLine( rect.left()+cellWidth*i, rect.top(),
                       rect.left()+cellWidth*i, rect.bottom() );
    }

    for(int col=0; col<3; col++)
    {
        for(int row=0; row<3; row++)
        {
            QRectF cellRect( col*cellWidth + rect.left(),
                            row*cellHeight + rect.top(),
                            cellWidth, cellHeight );
            QRectF paintableCellRect = cellRect.adjusted(10, 10, -10, -10);

            if(cellWidth > cellHeight)
                paintableCellRect.setWidth(cellHeight);
            else
                paintableCellRect.setHeight(cellWidth);
            paintableCellRect.moveCenter(cellRect.center());

            if(d->marks[row][col] == MyMark)
                d->paintMyMark(&paint, paintableCellRect);
            else if(d->marks[row][col] == OthersMark)
                d->paintOtherMark(&paint, paintableCellRect);
        }
    }

    QRectF statusTextRect = this->rect();
    statusTextRect.setTop( statusTextRect.bottom()-25 );
    paint.setPen(Qt::black);

    if(this->isEnabled())
    {
        if(d->isMyTurn)
        {
            if(d->isClear)
                paint.drawText(statusTextRect, Qt::AlignCenter,
                               "You can start the game, or wait for the other player to start");
            else
                paint.drawText(statusTextRect, Qt::AlignCenter, "Your turn");
        }
        else
            paint.drawText(statusTextRect, Qt::AlignCenter, "Other player's turn");
    }
    else
        paint.drawText(statusTextRect, Qt::AlignCenter, this->windowTitle());
}

void TicTacToeWidget::mousePressEvent(QMouseEvent *me)
{
    if(!d->isMyTurn)
        return;

    QRectF rect = this->rect();
    rect.adjust(10, 10, -10, -10);
    if(!rect.contains(me->pos()))
        return;

    qreal cellWidth = rect.width()/3;
    qreal cellHeight = rect.height()/3;

    int col = int(double(me->x()-rect.left())/cellWidth);
    int row = int(double(me->y()-rect.top())/cellHeight);
    if(d->marks[row][col] != NoMark)
        return;

    d->marks[row][col] = MyMark;
    d->isMyTurn = false;
    this->update();
    emit marked(row, col);

    if( this->checkIfIWon() )
    {
        emit iWon();
        QMessageBox::information(this, "You WON",
                                 "Congratulations you won the game! Click Ok to restart");
        this->clear();
    }
    else if( this->checkIfDraw() )
    {
        QMessageBox::information(this, "Match DRAW",
                                 "Arrrgg... The match was drawn. Nobody won. Click Ok to "
                                 "play again");
        this->clear();
    }
}

void TicTacToeWidget::closeEvent(QCloseEvent *)
{
    emit closed();
}

bool TicTacToeWidget::checkIfIWon()
{
    // Check diagonals
    bool success = true;
    for(int i=0; i<3; i++)
        success &= (d->marks[i][i] == MyMark);
    if(success)
        return true;

    success = true;
    for(int i=0; i<3; i++)
        success &= (d->marks[2-i][i] == MyMark);
    if(success)
        return true;

    // Check columns
    for(int i=0; i<3; i++)
    {
        success = true;
        for(int j=0; j<3; j++)
            success &= (d->marks[i][j] == MyMark);
        if(success)
            return true;
    }

    // Check rows
    for(int i=0; i<3; i++)
    {
        success = true;
        for(int j=0; j<3; j++)
            success &= (d->marks[j][i] == MyMark);
        if(success)
            return true;
    }

    return false;
}

bool TicTacToeWidget::checkIfDraw()
{
    if( this->checkIfIWon() )
        return false;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(d->marks[i][j] == NoMark)
                return false;
        }
    }

    return true;
}

void TicTacToeWidgetData::paintMyMark(QPainter* paint, const QRectF& rect)
{
    QRectF rect2 = rect;
    rect2.setSize(QSizeF(50, 50));
    rect2.moveCenter(rect.center());

    paint->setBrush(Qt::NoBrush);
    paint->setPen(QPen(Qt::blue,9));
    paint->drawChord(rect2, 0, 5760);
}

void TicTacToeWidgetData::paintOtherMark(QPainter* paint, const QRectF& rect)
{
    QRectF rect2 = rect.adjusted(20, 20, -20, -20);

    paint->setPen( QPen(Qt::red,9) );
    paint->drawLine( rect2.topLeft(), rect2.bottomRight() );
    paint->drawLine( rect2.bottomLeft(), rect2.topRight() );
}

