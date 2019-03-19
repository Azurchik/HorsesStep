#include "statistics.h"
#include "ui_statistics.h"

#include <QSettings>
#include <QMessageBox>
#include <QPushButton>

Statistics::Statistics(QWidget *parent) :
    QDialog(parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
            | Qt::WindowType::WindowCloseButtonHint),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);
    on_boxDifficulty_activated(ui->boxDifficulty->currentText()); // запуск події
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::on_boxDifficulty_activated(const QString &arg1)
{ // подія статистики вибраного рівня складності
    QSettings settings("Azurchik", "Horse");
    // назви складності
    if (arg1 == "Початківець") {
        difficulty = "Beginner";
    } else if (arg1 == "Середній") {
        difficulty = "Intermediate";
    } else {
        difficulty = "Advanced";
    }

    settings.beginGroup(difficulty);
    QString buf;

    buf.setNum(settings.value("bestTime", 0).toInt());
    ui->insertDate->setText(settings.value("date").toString());
    buf.append("с. ");
    ui->insertBestTime->setText(buf);

    float played = settings.value("numberGames", 0).toFloat();
    buf.setNum(played);
    ui->insertPlayed->setText(buf);

    int gamesWon = settings.value("gamesWon", 0).toInt();
    buf.setNum(gamesWon);
    ui->insertWon->setText(buf);

    if (gamesWon != 0) {
        buf.setNum(gamesWon / (played / 100 ));
    } else {
        buf.setNum(0);
    }
    buf.append("%");
    ui->insertPercentage->setText(buf);


    buf.setNum(settings.value("winStreak", 0).toInt());
    ui->insertWinStreak->setText(buf);

    buf.setNum(settings.value("loseStreak", 0).toInt());
    ui->insertLoseStreak->setText(buf);

    buf.setNum(settings.value("streak", 0).toInt());
    ui->insertCurStreak->setText(buf);
    settings.endGroup();
}

void Statistics::on_buttonReset_clicked()
{
    QMessageBox msgBox;
    QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);
    QPushButton *resetButton = msgBox.addButton(tr("Скинути"), QMessageBox::AcceptRole);
    abortButton->setText("Скасувати");
    msgBox.setDefaultButton(abortButton);

    QString text = ui->boxDifficulty->currentText();
    text.prepend("Ви точно хочете видалити статистику рівня: ");
    msgBox.setWindowTitle("Скидання");
    msgBox.setText(text);
    msgBox.exec();

    if (msgBox.clickedButton() == resetButton) {
        QSettings settings("Azurchik", "Horse");
        settings.beginGroup(difficulty);
        settings.remove("");
        settings.endGroup();

        on_boxDifficulty_activated(ui->boxDifficulty->currentText());
    }
}

void Statistics::on_buttonClose_clicked()
{
    close();
}
