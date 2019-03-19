#include "advert.h"
#include "ui_advert.h"

#include <QSettings>
#include <QDate>

Advert::Advert(bool gameWon, int time, QWidget *parent)
    : QDialog(parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
              | Qt::WindowType::WindowCloseButtonHint)
    , restart(false)
    , ui(new Ui::Advert)
{
    ui->setupUi(this);

    QSettings settings("Azurchik", "Horse");

    QString difficulty = settings.value("options/difficulty", "Beginner").toString();

    ui->insertDate->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    ui->insertTime->setNum(time);

    settings.beginGroup(difficulty);
    QString buf;

    buf.setNum(settings.value("bestTime", 0).toInt());
    buf.append("с.");
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

    settings.endGroup();


    if (gameWon) {
        setWindowTitle("Гра виграна");
    } else {
        setWindowTitle("Гра програна");
        ui->labelText->setText("Ви програли. Спробуйте ще раз!");
    }

}

Advert::~Advert()
{
    delete ui;
}

