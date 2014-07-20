#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT
    
public:
    explicit Preferences(QWidget *parent = 0);
    ~Preferences();

private slots:
    void on_pushButton_Save_clicked();

    void on_pushButton_ImagefolderPath_clicked();

    void on_radioButton_png_clicked();

    void on_radioButton_jpg_clicked();

    void on_radioButton_bmp_clicked();

    void on_radioButton_tiff_clicked();

    void on_checkBox_ImageView_clicked(bool checked);

private:
    Ui::Preferences *ui;
};

#endif // PREFERENCES_H
