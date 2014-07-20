#include "tree_editor.h"
#include "ui_tree_editor.h"
#include "databasemanager.h"

#include <QStandardItemModel>
#include <QtSql>
#include <QDebug>

Tree_Editor::Tree_Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tree_Editor)
{
    ui->setupUi(this);

    // fill combobox with types
    QSqlQueryModel *categoryModel = new QSqlQueryModel();
    categoryModel->setQuery("SELECT child_name, child_id FROM openatlas.types_parent_child WHERE parent_id = 15", myDBM->workingDB);
    ui->comboBox_SelectCategory->setModel(categoryModel);

    // Modell erstellen
    QStandardItemModel *commModel = new QStandardItemModel; // Modell erstellen
    QStandardItem *parentItem = commModel->invisibleRootItem(); // Root Item erstellen


//    QString rootNodesQueryString = "SELECT * FROM openatlas.types_parent_child WHERE parent_id = 15";
//    QSqlQuery rootNodesQuery(rootNodesQueryString,myDBM->workingDB);

//    while(rootNodesQuery.next())
//    {
//        QString rootNode = rootNodesQuery.value(3).toString();
//        QStandardItem *item = new QStandardItem(QString(rootNode)); // Child vom Root
//        parentItem->appendRow(item);
//        qDebug() << item;

//    }


    //hier die Items (aber Daten sinds immer noch nicht!!!)
   QStandardItem *item1 = new QStandardItem(QString("item1")); // Child vom Root
   QStandardItem *item1_1Col1 = new QStandardItem(QString("item1_1Col1")); // Spalte 1 vom ChildChild
   QStandardItem *item1_1Col2 = new QStandardItem(QString("item1_1Col2")); // Spalte 2 vom ChildChild
   QStandardItem *item1_1Col3 = new QStandardItem(QString("item1_1Col3")); // Spalte 3 vom ChildChild

   parentItem->appendRow(item1); // dem Root das Child anhängen

   // Spalten vorbereiten
   QList<QStandardItem*> itemList;
   itemList.append(item1_1Col1);
   itemList.append(item1_1Col2);
   itemList.append(item1_1Col3);

   item1->appendRow(itemList); // Child an Child hängen

   commModel->setColumnCount(3); // 3 Spalten anzeigen

   ui->treeView->setModel(commModel);
}

Tree_Editor::~Tree_Editor()
{
    delete ui;
}

void Tree_Editor::on_comboBox_SelectCategory_currentIndexChanged(QString index)
{

}
