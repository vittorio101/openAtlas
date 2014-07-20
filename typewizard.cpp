#include "typewizard.h"
#include "ui_typewizard.h"
#include "databasemanager.h"

#include <QDebug>

// UI elements
QString filterTypeWizard;

// data elements
int DataLevel;


QSqlQueryModel *tableViewTypesModel = new QSqlQueryModel;
QSortFilterProxyModel *proxymodel = new QSortFilterProxyModel;

TypeWizard::TypeWizard(const int &Level, QWidget *parent) :
    QDialog(parent),
    DataLevel(Level),
    ui(new Ui::TypeWizard)
{
    ui->setupUi(this);
    ui->toolButton_Delete_Type->setDisabled(true);
;

    QString QueryTypes;
    switch(DataLevel)
    {
    case 0:
        // define type querystring for sites
        QueryTypes = "SELECT child_id, child_name FROM openatlas.types_parent_child WHERE parent_id = 3 ORDER BY child_name ASC;";
        break;

    case 1:
        // define type querystring for features
        QueryTypes = "SELECT child_id, child_name FROM openatlas.types_parent_child WHERE parent_id = 1 ORDER BY child_name ASC;";

        break;
    case 2:
        // define type querystring for stratigraphical_units
        QueryTypes = "SELECT child_id, child_name FROM openatlas.types_parent_child WHERE parent_id = 4 ORDER BY child_name ASC;";
        break;

    case 3:
        // define type querystring for finds
        QueryTypes = "SELECT child_id, child_name FROM openatlas.types_parent_child WHERE parent_id = 2 ORDER BY child_name ASC;";
        break;
    }
    //QString
    tableViewTypesModel->setQuery(QueryTypes, myDBM->workingDB);
    proxymodel->setSourceModel(tableViewTypesModel);
    ui->tableViewTypes->setModel(proxymodel);
    ui->tableViewTypes->resizeColumnToContents(0);
    ui->tableViewTypes->hideColumn(0);
    ui->tableViewTypes->resizeRowsToContents();
    ui->tableViewTypes->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    qDebug() << "class TypeWizard called, Datalevel: " << DataLevel;
}

TypeWizard::~TypeWizard()
{
    delete ui;
}

void TypeWizard::on_lineEdit_Filter_textEdited()
{
    filterTypeWizard = ui->lineEdit_Filter->text();
    proxymodel->setFilterRegExp(QRegExp(filterTypeWizard, Qt::CaseInsensitive, QRegExp::Wildcard));
    proxymodel->setFilterKeyColumn(0);
}
