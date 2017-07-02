#include "mainwindow.h"
#include "tileboard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), playing(false)
{

    QWidget *central = new QWidget(this);
    central->setFixedSize(600, 480);

    QWidget *vbox = new QWidget(central);
    QVBoxLayout *vlay = new QVBoxLayout;

    QHBoxLayout *hlay = new QHBoxLayout;
    ctrlButton = new QPushButton;
    ctrlButton->setFixedSize(72, 30);
    ctrlButton->setText("Play");
    ctrlButton->setToolTip("Play");
    hlay->addWidget(ctrlButton);

    hlay->addStretch();
    backwardButton = new QPushButton;
    backwardButton->setText("<--");
    backwardButton->setToolTip("Backward");
    backwardButton->setFixedSize(72, 30);
    backwardButton->setEnabled(false);
    hlay->addWidget(backwardButton);

    hlay->addSpacing(24);
    QPushButton *forwardBtn = new QPushButton;
    forwardBtn->setText("-->");
    forwardBtn->setFixedSize(72, 30);
    hlay->addWidget(forwardBtn);

    QRect rect(2, 2, 400, 400);
    table = new TileBoard(rect, this);
    vlay->addLayout(hlay);
    vlay->addSpacing(8);
    vlay->addWidget(table);

    vbox->setLayout(vlay);
    setCentralWidget(central);

    connect(ctrlButton, SIGNAL(clicked()), table, SLOT(togglePlaying()));
    connect(ctrlButton, SIGNAL(clicked()), this, SLOT(togglePlayingStatus()));
    connect(backwardButton, SIGNAL(clicked()), table, SLOT(goBackward()));
    connect(table, SIGNAL(gameOver()), this, SLOT(resetStatus()));
    connect(table, SIGNAL(emptyStack(bool)), this, SLOT(enableBackward(bool)));
}

void MainWindow::togglePlayingStatus()
{
    playing = (! playing);

    if (playing) {
        ctrlButton->setText("Pause");
        ctrlButton->setToolTip("Pause");
        // switch keyboard focus to table
        table->setFocus();
    } else {
        ctrlButton->setText("Play");
        ctrlButton->setToolTip("Play");
    }
}

void MainWindow::resetStatus()
{
    playing = false;
    ctrlButton->setText("Play");
    ctrlButton->setToolTip("Play");
}

void MainWindow::enableBackward(bool isEmpty)
{
    backwardButton->setEnabled(! isEmpty);
    if (isEmpty)
        table->setFocus();
}

MainWindow::~MainWindow()
{

}
