#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>

#include "tileboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    QPushButton *ctrlButton;
    QPushButton *backwardButton;
    QPushButton *forwardButton;
    TileBoard *table;
    bool playing;

public slots:
    void togglePlayingStatus();
    void resetStatus();
    void enableBackward(bool isEmpty);
    void enableForward(bool isTop);
};

#endif // MAINWINDOW_H
