#include "tileboard.h"

static const int STKSIZE = 2048;
static int BoundedStack[STKSIZE][16];

TileBoard::TileBoard(QRect &rect, QWidget *parent)
    : QWidget(parent), rect(rect), playing(false), isGameOver(false),
      front(0), rear(0)
{
    setFocusPolicy(Qt::StrongFocus);
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++)
            values[i][j] = 0;
    }

}

QSize TileBoard::sizeHint() const
{
    return rect.size() + QSize(4, 4);
}

void TileBoard::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::darkCyan);
    pen.setWidth(2);

    painter.setPen(pen);
    painter.drawRect(rect);

    pen.setWidth(1);
    painter.setPen(pen);
    int w = rect.width();
    int h = rect.height();
    // paint game over state
    if (isGameOver) {
        painter.fillRect(rect, QColor::fromRgba(qRgba(120, 120, 120, 128)));
        QFont font("Sans", 12);
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignCenter, "Game Over, Click Play Button to Play Again");
        return;
    }
    // paint paused state
    if ((! playing) && (! isInitStatus())) {
        painter.fillRect(rect, QColor::fromRgba(qRgba(120, 120, 120, 128)));
        QFont font("Sans", 14);
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignCenter, "Pause, Click Play Button to Resume");
        return;
    }

    // paint playing or initial state
    int i, j;
    for (i = 1; i < 4; i++) {
        painter.drawLine(rect.x(), rect.y() + h/4 * i, rect.x() + w, rect.y() + h/4 *i);
    }
    for (i = 1; i < 4; i++) {
        painter.drawLine(rect.x() + w/4 * i, rect.y(), rect.x() + w/4 * i, rect.y() + h);
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            QRect rectArea(rect.x() + w/4 * j, rect.y() + h/4 * i, w / 4, h / 4);
            QRect rect1(rectArea.topLeft() + QPoint(2, 2), rectArea.bottomRight() - QPoint(2, 2));

            paintTile(painter, rect1, values[i][j]);
        }
    }
}

void TileBoard::paintTile(QPainter &painter, QRect &rect, int value)
{
    static QRgb colorTab[] = { 0xffff4500, 0xfff59563, 0xfff2b179, 0xffedc850,
                               0xff20b2aa, 0xffb0e0e6, 0xff4169e1, 0xff8a2be2};
    int ci(0);
    QPen pen;
    QFont font("Sans");
    switch (value) {
    case 0:
        painter.fillRect(rect, QColor::fromRgba(qRgba(238, 228, 218, 90)));
        return;
    case 2:
    case 4:
        ci = 0;
        font.setPointSize(42);
        break;
    case 8:
        ci = 1;
        font.setPointSize(42);
        break;
    case 16:
    case 32:
    case 64:
        ci = 3;
        font.setPointSize(36);
        break;
    case 128:
    case 256:
    case 512:
        ci = 4;
        font.setPointSize(32);
        break;
    case 1024:
    case 2048:
    case 4096:
    case 8192:
        ci = 6;
        font.setPointSize(26);
        break;
    default:
        ci = 7;
        font.setPointSize(20);
        break;
    }
    painter.fillRect(rect, QColor::fromRgba(colorTab[ci]));

    pen.setColor(QColor::fromRgb(qRgb(0xf9, 0xf6, 0xf2)));
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(value));
}

bool TileBoard::isInitStatus()
{
    int const N = 4;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (values[i][j] != 0)
                return false;
    return true;
}

void TileBoard::togglePlaying()
{
    if (isGameOver)
        isGameOver = false;
    playing = (! playing);
    if (isInitStatus()) {
        generateRandomNumber();
        generateRandomNumber();
    }
    update();
}

void TileBoard::generateRandomNumber()
{
    QTime zero(0, 0, 0);
    qsrand(zero.secsTo(QTime::currentTime()));

    int i, j, val;
    int rn;
    do {
        rn = qrand() & (15);
        i = rn >> 2;
        j = rn & 3;
        if (values[i][j] == 0) {
            rn = qrand();
            val = (rn % 5 == 0) ? 4 : 2;
            break;
        }
    } while (1);
    values[i][j] = val;
}

bool TileBoard::checkTiles()
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (values[i][j] == 0)
                return true;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++)
            if (values[i][j] == values[i][j + 1])
                return true;
    }

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 3; i++)
            if (values[i][j] == values[i+1][j])
                return true;
    }

    return false;
}

void TileBoard::keyPressEvent(QKeyEvent *event)
{
    if (! playing) {
        QWidget::keyPressEvent(event);
        return;
    }

    pushState();    // save the state before it is modified
    switch (event->key()) {
    case Qt::Key_Down:
        moveDown();
        break;
    case Qt::Key_Left:
        moveLeft();
        break;
    case Qt::Key_Right:
        moveRight();
        break;
    case Qt::Key_Up:
        moveUp();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    if (isTileMoved())
        generateRandomNumber();

    if (! checkTiles()) {
        isGameOver = true;
        playing = false;
        emit gameOver();
    }

    update();
}

void TileBoard::moveDown()
{
    const int N = 4;
    for (int column = 0; column < N; column++) {
        int count = N;
        int i;
        for (i = N - 1; i >= 0; i--)
            if (values[i][column] != 0)
                values[--count][column] = values[i][column];

        for (i = count - 1; i >= 0; i--)
            values[i][column] = 0;

        for (i = N - 1; i > count; ) {
            if (values[i][column] == values[i - 1][column]) {
                values[i][column] *= 2;
                values[i - 1][column] = 0;
                i -= 2;
            } else
                i -= 1;
        }

        count = N;
        for (i = N - 1; i >= 0; i--)
            if (values[i][column] != 0)
                values[--count][column] = values[i][column];

        for (i = count - 1; i >= 0; i--)
            values[i][column] = 0;
    }
}

void TileBoard::moveLeft()
{
    for (int row = 0; row < 4; row++) {
        int count = 0;
        int i;
        for (i = 0; i < 4; i++)
            if (values[row][i] != 0)
                values[row][count++] = values[row][i];

        for (i = count; i < 4; i++)
            values[row][i] = 0;

        for (i = 0; i < count - 1; ) {
            if (values[row][i] == values[row][i + 1]) {
                values[row][i] *= 2;
                values[row][i + 1] = 0;
                i += 2;
            } else
                i += 1;
        }

        count = 0;
        for (i = 0; i < 4; i++)
            if (values[row][i] != 0)
                values[row][count++] = values[row][i];

        for (i = count; i < 4; i++)
            values[row][i] = 0;
    }
}

void TileBoard::moveRight()
{
    const int N = 4;
    for (int row = 0; row < N; row++) {
        int count = N;
        int i;
        for (i = N - 1; i >= 0; i--)
            if (values[row][i] != 0)
                values[row][--count] = values[row][i];

        for (i = count - 1; i >= 0; i--)
            values[row][i] = 0;

        for (i = N - 1; i > count; ) {
            if (values[row][i] == values[row][i - 1]) {
                values[row][i] *= 2;
                values[row][i - 1] = 0;
                i -= 2;
            } else
                i -= 1;
        }

        count = N;
        for (i = N - 1; i >= 0; i--)
            if (values[row][i] != 0)
                values[row][--count] = values[row][i];

        for (i = count - 1; i >= 0; i--)
            values[row][i] = 0;
    }
}

void TileBoard::moveUp()
{
    for (int column = 0; column < 4; column++) {
        int count = 0;
        int i;
        for (i = 0; i < 4; i++)
            if (values[i][column] != 0)
                values[count++][column] = values[i][column];

        for (i = count; i < 4; i++)
            values[i][column] = 0;

        for (i = 0; i < count - 1; ) {
            if (values[i][column] == values[i + 1][column]) {
                values[i][column] *= 2;
                values[i + 1][column] = 0;
                i += 2;
            } else
                i += 1;
        }

        count = 0;
        for (i = 0; i < 4; i++)
            if (values[i][column] != 0)
                values[count++][column] = values[i][column];

        for (i = count; i < 4; i++)
            values[i][column] = 0;
    }
}

bool TileBoard::isTileMoved()
{
    int i, j;
    int prev = (front - 1) & (STKSIZE - 1);
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (BoundedStack[prev][4*i + j] != values[i][j])
                return true;
    return false;
}

void TileBoard::pushState()
{
    if (isTileMoved()) {
        int i, j;
        int next = (front + 1) & (STKSIZE - 1);
        if (next == rear)                       // full stack
            rear = (rear + 1) & (STKSIZE - 1);

        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                BoundedStack[front][4*i + j] = values[i][j];
        front = next;
        emit emptyStack(false);
    }
}

void TileBoard::goBackward()
{
    const int N = 4;
    int i, j;
    front = (front -1) & (STKSIZE - 1);

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            values[i][j] = BoundedStack[front][N*i + j];

    if (front == rear)
        emit emptyStack(true);

    update();
}
