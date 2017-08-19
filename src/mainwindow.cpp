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
    ctrlButton->setFocusPolicy(Qt::NoFocus);
    hlay->addWidget(ctrlButton);

    hlay->addStretch();
    backwardButton = new QPushButton;
    backwardButton->setText("<--");
    backwardButton->setToolTip("Backward");
    backwardButton->setFixedSize(72, 30);
    backwardButton->setEnabled(false);
    backwardButton->setFocusPolicy(Qt::NoFocus);
    hlay->addWidget(backwardButton);

    hlay->addSpacing(24);
    forwardButton = new QPushButton;
    forwardButton->setText("-->");
    forwardButton->setFixedSize(72, 30);
    forwardButton->setEnabled(false);
    forwardButton->setFocusPolicy(Qt::NoFocus);
    hlay->addWidget(forwardButton);

    QRect rect(2, 2, 400, 400);
    table = new TileBoard(rect, this);
    vlay->addLayout(hlay);
    vlay->addSpacing(8);
    vlay->addWidget(table);

    vbox->setLayout(vlay);
    vbox->setContentsMargins(85, 0, 0, 0);
    setCentralWidget(central);

    connect(ctrlButton, SIGNAL(clicked()), table, SLOT(togglePlaying()));
    connect(ctrlButton, SIGNAL(clicked()), this, SLOT(togglePlayingStatus()));
    connect(backwardButton, SIGNAL(clicked()), table, SLOT(goBackward()));
    connect(forwardButton, SIGNAL(clicked()), table, SLOT(goForward()));
    connect(table, SIGNAL(gameOver()), this, SLOT(resetStatus()));
    connect(table, SIGNAL(emptyStack(bool)), this, SLOT(enableBackward(bool)));
    connect(table, SIGNAL(currentTop(bool)), this, SLOT(enableForward(bool)));
}

void MainWindow::togglePlayingStatus()
{
    playing = (! playing);

    if (playing) {
        ctrlButton->setText("Pause");
        ctrlButton->setToolTip("Pause");
        // instead of enabling backward and forward buttons here, use slots to do it

        // switch keyboard focus to table
        table->setFocus();
    } else {
        ctrlButton->setText("Play");
        ctrlButton->setToolTip("Play");
        backwardButton->setEnabled(false);
        forwardButton->setEnabled(false);
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

void MainWindow::enableForward(bool isTop)
{
    forwardButton->setEnabled(! isTop);
    if (isTop)
        table->setFocus();
}

MainWindow::~MainWindow()
{

}
