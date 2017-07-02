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

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void gameOver();
    void emptyStack(bool);

private:
    QRect rect;
    int values[4][4];
    bool playing;
    bool isGameOver;
    int front;
    int rear;

    bool checkTiles();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveUp();

    void pushState();
    void paintTile(QPainter &painter, QRect &rect, int value);
    void generateRandomNumber();
    bool isTileMoved();
};

#endif // TILEBOARD_H
