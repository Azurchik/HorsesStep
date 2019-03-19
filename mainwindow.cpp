#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow (parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
                   | Qt::WindowType::WindowCloseButtonHint)
    , _Background (149, 137, 118)
    , _UpperIndent  (20)
    , _BottomIndent (20)
    , _LeftIndent   (11)
    , _RightIndent  (11)
    , ui (new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette_ = palette();
    palette_.setBrush(QPalette::ColorRole::Background, QBrush(_Background));
    setPalette(palette_);

    table    = new Table(this);
    settings = new QSettings("Azurchik", "Horse");
    lcdTimer = new QTimer(this);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    startGame();

    ui->lcdTime->setVisible(!settings->value("options/hideTime", false).toBool());
    ui->lcdTime->setPalette(QPalette(table->black()));

    connect(table, &Table::endGame,
            this,  &MainWindow::advertWindow);

    connect(table, &Table::firstStep,
            this, &MainWindow::startLcd);

    connect(lcdTimer, &QTimer::timeout,
            this, &MainWindow::incrementLcd);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGame()
{
    int row    = settings->value("field/row",    3).toInt();
    int column = settings->value("field/column", 4).toInt();

    int cellSize = 120;
    switch (row) {
    case 8:
        cellSize /= 2;
        break;

    case 16:
        cellSize /= 3;
    }


    table->createField(row, column, cellSize);

    ui->graphicsView->setScene(table);

    QSize size = table->size();
    int frame  = ui->graphicsView->frameWidth() * 2;
    size.setWidth (frame + size.width());
    size.setHeight(frame + size.height());

    ui->graphicsView->setFixedSize(size);
    ui->graphicsView->resize(size);

    size = QSize(size.width() + _RightIndent + _LeftIndent,
                 size.height() + ui->lcdTime->geometry().height()
                 + _UpperIndent + _BottomIndent);

    setFixedSize(size);
    resize(size);

    ui->lcdTime->display(0);
}

void MainWindow::startLcd()
{
    lcdTimer->start(1000);
}

void MainWindow::advertWindow(bool gameWon)
{
    lcdTimer->stop();
    Advert advert(gameWon, ui->lcdTime->intValue(), this);
    advert.setModal(true);
    int result = advert.exec();

    if (result == QDialog::Accepted) {
        recordStatistics(table->isGameWon());
        startGame();
    } else {
        close();
    }
}

void MainWindow::incrementLcd()
{
    int num = ui->lcdTime->intValue();
    ui->lcdTime->display(++num);

    if (num == 999)
        lcdTimer->stop();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!(table->isFirstStep() || table->isGameWon())) {
        recordStatistics(false);
        event->accept();
    }
}

void MainWindow::recordStatistics(bool gameWon)
{
    QString difficulty = settings->value("options/difficulty", "Beginner").toString();

    settings->beginGroup(difficulty);

    int numberGames = settings->value("numberGames", 0).toInt();
    settings->setValue("numberGames", ++numberGames);

    int streak = settings->value("streak", 0).toInt();

    if (gameWon) {
        int gamesWon = settings->value("gamesWon", 0).toInt();
        settings->setValue("gamesWon", ++gamesWon);

        if (streak <= 0) {
           settings->setValue("streak", 1);
        } else {
            settings->setValue("streak", ++streak);

            int winStreak = settings->value("winStreak", 0).toInt();
            if (winStreak < streak)
                settings->setValue("winStreak", streak);
        }

        int bestTime = settings->value("bestTime", 999).toInt();
        int currTime = ui->lcdTime->intValue();
        if (currTime < bestTime) {
            settings->setValue("bestTime", currTime);
            settings->setValue("date", QDate::currentDate().toString("dd.MM.yyyy"));
        }

    } else {
        if (streak >= 0) {
           settings->setValue("streak", -1);
        } else {
            settings->setValue("streak", --streak);

            int loseStreak = settings->value("loseStreak", 0).toInt();
            if (loseStreak > streak)
                settings->setValue("loseStreak", streak);
        }

    }

    settings->endGroup();
}

void MainWindow::on_newGame_action_triggered()
{
    startGame();
}

void MainWindow::on_options_action_triggered()
{
    Options opt(this);
    opt.setModal(true);

    if (opt.exec() == QDialog::Accepted) {
        if (opt.wereChange())
            startGame();
    }
    ui->lcdTime->setVisible(!settings->value("options/hideTime", false).toBool());
}

void MainWindow::on_statistics_action_triggered()
{
    Statistics statistics(this);
    statistics.setModal(true);
    statistics.exec();
}

void MainWindow::on_exit_action_triggered()
{
    close();
}

void MainWindow::on_about_action_triggered()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Про програму");
    msg.setText("Програма написанна мовою: С++\nНа платформі: Qt\nВерсії: 5.11.2\nДля операційних систем:\nWindows 7 - 10\nРозробив: kihcruzA");
    msg.setIconPixmap(QPixmap(":/image/qt.png").scaled(250, 250));

    msg.setStandardButtons(QMessageBox::Yes);
    msg.setButtonText(QMessageBox::Yes, "Добре");
    msg.exec();
}

void MainWindow::on_help_action_triggered()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Правила");
    msg.setText("Правило:\n\nЗнайти маршрут шахового коня, що проходить через всі поля дошки по одному разу.");
    msg.setIconPixmap(QPixmap(":/image/horse3.png"));

    msg.setStandardButtons(QMessageBox::Yes);
    msg.setButtonText(QMessageBox::Yes, "Добре");
    msg.exec();
}
