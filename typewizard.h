#ifndef TYPEWIZARD_H
#define TYPEWIZARD_H

#include <QDialog>
#include <QtSql>
#include <QSortFilterProxyModel>

namespace Ui {
class TypeWizard;
}

class TypeWizard : public QDialog
{
    Q_OBJECT
    
public:
    explicit TypeWizard(const int &Level, QWidget *parent = 0);
    ~TypeWizard();
    
private:
    int DataLevel;
    Ui::TypeWizard *ui;

private slots:
    void on_lineEdit_Filter_textEdited();
};

#endif // TYPEWIZARD_H
