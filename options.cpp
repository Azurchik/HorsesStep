#include "options.h"
#include "ui_options.h"

#include <QSettings>
#include <QRegExp>

#include <QDebug>

Options::Options(QWidget *parent) :
    QDialog(parent, Qt::WindowType::MSWindowsFixedSizeDialogHint
            | Qt::WindowType::WindowCloseButtonHint)
    , _Change(false)
    , ui(new Ui::Options)

{
    ui->setupUi(this);

    settings = new QSettings("Azurchik", "Horse");
    _CurrentDifficulty = settings->value("options/difficulty", "Beginner").toString();

    if (_CurrentDifficulty == "Beginner") {
        ui->radioBgn->setChecked(true);

    } else if (_CurrentDifficulty == "Intermediate") {
        ui->radioIntm->setChecked(true);

    } else {
        ui->radioAdvc->setChecked(true);
    }

    ui->checkTime->setChecked(settings->value("options/hideTime", false).toBool());
}

Options::~Options()
{
    delete ui;
}

bool Options::wereChange()
{
    return  _Change;
}


void Options::on_pushButton_clicked()
{
    QString newDifficulty;
    settings->setValue("options/hideTime", ui->checkTime->isChecked());

    if (ui->radioBgn->isChecked()) {
        settings->setValue("field/row",    3);
        settings->setValue("field/column", 4);
        settings->setValue("options/difficulty", "Beginner");
        newDifficulty = "Beginner";

    } else if (ui->radioIntm->isChecked()) {
        settings->setValue("field/row",    8);
        settings->setValue("field/column", 8);
        settings->setValue("options/difficulty", "Intermediate");
        newDifficulty = "Intermediate";

    } else {
        settings->setValue("field/row",    16);
        settings->setValue("field/column", 16);
        settings->setValue("options/difficulty", "Advance");
        newDifficulty = "Advance";
    }

    _Change = !(_CurrentDifficulty == newDifficulty);
    accept();
}
