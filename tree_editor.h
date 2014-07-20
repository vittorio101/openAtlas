#ifndef TREE_EDITOR_H
#define TREE_EDITOR_H

#include <QDialog>

#include <QStandardItemModel>
#include <QtSql>

namespace Ui {
class Tree_Editor;
}

class Tree_Editor : public QDialog
{
    Q_OBJECT
    
public:
    explicit Tree_Editor(QWidget *parent = 0);
    ~Tree_Editor();
    
private slots:

    void on_comboBox_SelectCategory_currentIndexChanged(QString index);

private:
    Ui::Tree_Editor *ui;

};

#endif // TREE_EDITOR_H
