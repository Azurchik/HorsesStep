#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

    bool wereChange();

private slots:
    void on_pushButton_clicked();

private:
    QString _CurrentDifficulty;
    bool _Change;

    QSettings *settings;
    Ui::Options *ui;
};

#endif // OPTIONS_H
