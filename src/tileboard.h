#ifndef TILEBOARD_H
#define TILEBOARD_H

#include <QtCore>
#include <QtWidgets>

class TileBoard : public QWidget {
    Q_OBJECT

public:
    TileBoard(QRect &rect, QWidget *parent = nullptr);
    QSize sizeHint() const;
    bool isInitStatus();

public slots:
    void togglePlaying();
    void goBackward();
    void goForward();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void gameOver();
    void emptyStack(bool);
    void currentTop(bool);

private:
    QRect rect;
    int values[4][4];
    bool playing;
    bool isGameOver;
    bool backwarding;
    int front;
    int rear;
    int currentIndex;

    void moveDown();
    void moveLeft();
    void moveRight();
    void moveUp();

    void pushState();
    void paintTile(QPainter &painter, QRect &rect, int value);
    void generateRandomNumber();

    int detectDirection();

    enum {
        down_ = 1,
        left_ = 2,
        right_ = 4,
        up_ = 8,
    };

};

#endif // TILEBOARD_H
