#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QSize>
#include <QSettings>
#include <vector>

#include "advert.h"
#include "options.h"
#include "statistics.h"
#include "table.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startGame();

private slots:
    void startLcd();
    void advertWindow(bool gameWon);
    void incrementLcd();

private slots:
    void on_newGame_action_triggered();
    void on_options_action_triggered();
    void on_statistics_action_triggered();
    void on_exit_action_triggered();

    void on_about_action_triggered();

    void on_help_action_triggered();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void recordStatistics(bool gameWon);

    const QColor _Background;

    QTimer *lcdTimer;

    Table *table;
    QSettings *settings;

    const int _UpperIndent;
    const int _BottomIndent;
    const int _LeftIndent;
    const int _RightIndent;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
