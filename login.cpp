//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  This file is part of openATLAS.                                         //
//                                                                          //
//    openATLAS is free software: you can redistribute it and/or modify     //
//    it under the terms of the GNU General Public License as published by  //
//    the Free Software Foundation, either version 3 of the License, or     //
//    any later version.                                                    //
//                                                                          //
//    openATLAS is distributed in the hope that it will be useful,          //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of        //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
//    GNU General Public License for more details.                          //
//                                                                          //
//    You should have received a copy of the GNU General Public License     //
//    along with openATLAS. If not, see <http://www.gnu.org/licenses/>.     //
//                                                                          //
//    Database Design by Stefan Eichert 2013 - 2014                         //
//    stefan.eichert@univie.ac.at                                           //
//    Frontend developed by Viktor Jansa 2013 - 2014                        //
//    viktor.jansa@gmx.net                                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "databasemanager.h"


#include <QSettings>

//extern DatabaseManager *myDBM;

Login::Login(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        QString ConnectionName = settings.value("currentDatabaseConnection").toString();
    settings.endGroup();


    QSqlDatabase connectionDB = QSqlDatabase::database(myDBM->conn_file);
    QString connectionQueryString = "SELECT * FROM tbl_connections WHERE  connection_name = '" + ConnectionName + "';";
    QSqlQuery connectionQuery(connectionQueryString, connectionDB);
    while(connectionQuery.next())
    {
        QString database_name = connectionQuery.value(2).toString();
        QString database_host = connectionQuery.value(5).toString();
        QString database_username = connectionQuery.value(7).toString();

        ui->label_ConnectionName->setText(ConnectionName);
        ui->label_DatabaseName->setText(database_name);
        ui->label_DatabaseHost->setText(database_host);
        ui->label_Username->setText(database_username);
    }
}

Login::~Login()
{
    delete ui;
}
