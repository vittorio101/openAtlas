//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//    This file is part of openATLAS.                                       //
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


#include "firststart.h"
#include "ui_firststart.h"
#include "createconnection.h"

#include <QFileDialog>
#include <QtSql>
#include <QMessageBox>
#include <QSettings>
#include <QString>

QString connectionsDir;
QString connectionFileName;

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                  Constructor and Deconstructor                           //
//                  for firstStart                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

// Constructor
//////////////////////////////////////////////////////////////////////////////
firstStart::firstStart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstStart)
{
    ui->setupUi(this);
    firstScreen();
}

// Deconstructor
//////////////////////////////////////////////////////////////////////////////
firstStart::~firstStart()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                  Private slots for firstStart                            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

void firstStart::on_toolButtonOpenFileDialog_clicked()
{
    connectionsDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                               "",
                                                               QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);
    if(!connectionsDir.isEmpty())
    {
        QDir DirDatabaseFile = connectionsDir;
        connectionFileName = DirDatabaseFile.absoluteFilePath("openatlas.cdf");
        ui->lineEditPathConnections->setText((connectionFileName));
    }
}

void firstStart::on_pushButton_01_Next_clicked()
{
    secondScreen();
}

void firstStart::on_pushButton_02_Next_clicked()
{
    thirdScreen();
    QString create_DB_02;
    QString create_DB_01 = "Create database '" + connectionFileName + "'...";
    ui->plainTextEdit->setPlainText(create_DB_01);

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(connectionFileName);
    database.open();
    if(!database.open())
    {
        QMessageBox::critical(0, tr("Cannot open database!"),
                              tr("Cannot create the connection data file!\n\nTip: Check, if you have write permission in this directory and if there is enough disk space!"),
                              QMessageBox::Cancel);
        firstScreen();
    }
    else
    {
        create_DB_02 = create_DB_01 + "\nDone!\nCreate table 'tbl_connections'...";
        ui->plainTextEdit->setPlainText(create_DB_02);


    QSqlQuery query;
    QString QueryString = "CREATE TABLE tbl_connections (oid INTEGER PRIMARY KEY, connection_name TEXT, database_name TEXT, database_driver TEXT, database_server TEXT, database_host TEXT, database_port INTEGER, database_username TEXT, database_username_token TEXT, database_password TEXT, UNIQUE (connection_name));";
    query.exec(QueryString);
    database.close();
    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        settings.setValue("DatafileConnections", connectionFileName);
    QString create_DB_03 = create_DB_02 + "\nDone!\n\nClick the 'Next'-button to create a new database connection...";
    ui->plainTextEdit->setPlainText(create_DB_03);
    }
}

void firstStart::on_pushButton_02_Back_clicked()
{
    firstScreen();
}

void firstStart::on_pushButton_03_Next_clicked()
{
    CreateConnection *conndlg = new CreateConnection;
    conndlg->exec();
    ui->pushButton_01_Next->setVisible(false);
    ui->pushButton_02_Next->setVisible(false);
    ui->pushButton_02_Back->setVisible(false);
    ui->pushButton_03_Next->setVisible(false);
    QIcon icon1;
    icon1.addFile(QString(":/buttons/Button_Ok"), QSize(), QIcon::Normal, QIcon::Off);
    icon1.addFile((":/buttons/Button_Ok"), QSize(), QIcon::Normal, QIcon::Off);
    ui->pushButton_01_Cancel->setIcon(icon1);
    ui->pushButton_01_Cancel->setText("Finish");
    ui->plainTextEdit->setPlainText(tr("Congratulations!\n\nOpenATLAS is now ready for use. Close this window by clicking the 'Finish'-button to start openAtlas.\n\nHave fun!!"));
}


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                  Private functions for CreateConnection                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


//This functions are paiting the screen in different modes
//////////////////////////////////////////////////////////////////////////////
void firstStart::firstScreen()
{
    //function NOT ready!!!
    ui->groupBox->setTitle(tr("Step 1"));
    ui->labelPathQuestion->setVisible(false);
    ui->lineEditPathConnections->setVisible(true);
    ui->toolButtonOpenFileDialog->setVisible(true);
    ui->lineEditPathConnections->setText(connectionFileName);
    ui->plainTextEdit->setVisible(true);
    ui->plainTextEdit->setGeometry(QRect(10, 70, 461, 111));
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setPlainText(tr("This seems to be the first start of openAtlas! Please set a folder, where the database connections will be saved..."));
    ui->pushButton_01_Next->setVisible(true);
    ui->pushButton_02_Next->setVisible(false);
    ui->pushButton_02_Back->setVisible(false);
    ui->pushButton_03_Next->setVisible(false);

}

void firstStart::secondScreen()
{
    ui->groupBox->setTitle(tr("Step 2"));
    ui->plainTextEdit->setVisible(false);
    ui->labelPathQuestion->setVisible(true);
    QString pathQuestion = "Do you want to create the connection data file 'openatlas.cdf' in\n\n" + connectionsDir + "?\n\nIf you want to change the folder, please click the 'Back'-Button, otherwise the 'Next'-Button.";
    ui->labelPathQuestion->setText(pathQuestion);
    ui->lineEditPathConnections->setVisible(false);
    ui->toolButtonOpenFileDialog->setVisible(false);
    ui->pushButton_01_Next->setVisible(false);
    ui->pushButton_02_Next->setVisible(true);
    ui->pushButton_02_Back->setVisible(true);
    ui->pushButton_03_Next->setVisible(false);
}

void firstStart::thirdScreen()
{
    ui->groupBox->setTitle(tr("Step 3"));
    ui->plainTextEdit->setVisible(true);
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setGeometry(QRect(10, 30, 461, 151));
    ui->labelPathQuestion->setVisible(false);
    ui->pushButton_01_Next->setVisible(false);
    ui->pushButton_02_Next->setVisible(false);
    ui->pushButton_02_Back->setVisible(false);
    ui->pushButton_03_Next->setVisible(true);
}













