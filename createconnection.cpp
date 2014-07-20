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

#include <QFileDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QtSql>

#include "createconnection.h"
#include "ui_createconnection.h"
#include "mainwindow.h"

bool ConnectionTestStatus = false;
bool checkOfContentsStatus = false;
QString noNeed = "not required for SQLite";
QString SQLiteTestConnection = "testconnection_to_SQLite";
QString PostgresTestConnection = "testconnection_to_PostgreSQL";

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                  Constructor and Deconstructor                           //
//                  for CreateConnection                                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

// Constructor
//////////////////////////////////////////////////////////////////////////////
CreateConnection::CreateConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateConnection)
{
    ui->setupUi(this);

    //allow just integers in field 'Port'
    QIntValidator *validator = new QIntValidator(this);
    ui->lineEditPort->setValidator(validator);
}

// Deconstructor
//////////////////////////////////////////////////////////////////////////////
CreateConnection::~CreateConnection()
{
    delete ui;
}


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                  Private slots for CreateConnection                      //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

void CreateConnection::on_comboBoxDatabaseLocation_currentIndexChanged(const QString &location)
{
    ConnectionTestStatus = false;
    if(location == "Local")
    {
        setUiToLocal();
    }
    else
    {
        setUiToNetwork();
    }
}

void CreateConnection::on_toolButtonOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open openAtlas Datafile"),
                                            tr("/home/viktor"),
                                            tr("openAtlas Datafiles (*.oad)"));
    if (!fileName.isEmpty())
    {
        ui->lineEditDatabaseFile->setText(fileName);
    }

    //check, if the choosen file is a valid SQLite-file
    QFile filetest(fileName);
    filetest.open(QIODevice::ReadOnly);
    QDataStream in(&filetest);
    qint32 magicNumber;
    in >> magicNumber;
    if(magicNumber != 1397836905)
    {
        inputErrorMsgBox(tr("The choosen file is NOT a valid SQLite 3.x-file!"));
        ui->lineEditDatabaseFile->setText("");
        ui->lineEditDatabaseFile->setFocus();
    }
}

void CreateConnection::on_pushButtonTestConnection_clicked()
{
    //check content of lineEdits
    if (ui->lineEditDatabaseDriver->text()=="QSQLITE")
    {
        checkOfContentsStatus = checkContentOfLineSQLite();

        //if all necessary lines are filled, test the connection
        if(checkOfContentsStatus == true)
        {
            ConnectionTestStatus = false;  //value will be true after successfull connection test
            QSqlDatabase SQLite = QSqlDatabase::addDatabase("QSQLITE", SQLiteTestConnection);
            SQLite.setDatabaseName(ui->lineEditDatabaseFile->text());
            SQLite.open();
            if (!SQLite.isOpen())
            {
                QMessageBox::critical(0, QObject::tr("Database Error"),
                                      SQLite.lastError().text());
            }
            else
            {
                QString info = "Successfully connected!\nDatabase file:\t" + SQLite.databaseName() + "" + "\nConnection name:\t" +  SQLite.connectionName();
                QMessageBox::information(0, QObject::tr("Connection Info"),
                                         info);
                ConnectionTestStatus = true;
            }
            SQLite.close();
        }
    }
    else
    {
        //check content of lineEdits
        checkOfContentsStatus = checkContentOfLinePostgreSQL();
        if(checkOfContentsStatus == true)
        {
            ConnectionTestStatus = false;
            QSqlDatabase PostgreSQL = QSqlDatabase::addDatabase("QPSQL", PostgresTestConnection);
            PostgreSQL.setDatabaseName(ui->lineEditDatabaseFile->text());
            PostgreSQL.setHostName(ui->lineEditHostname->text());

            QString strPort;
            int intPort;
            strPort = ui->lineEditPort->text();
            intPort = strPort.toInt();

            PostgreSQL.setPort(intPort);
            PostgreSQL.setUserName(ui->lineEditUserName->text());
            PostgreSQL.setPassword(ui->lineEditPassword->text());

            PostgreSQL.open();
            if(!PostgreSQL.open())
            {
                QMessageBox::critical(0, QObject::tr("Connection Error"),
                                                  PostgreSQL.lastError().text());
            }
            else
            {
                QString info = "Successfully connected!\nDatabase name:\t" + PostgreSQL.databaseName() + "" + "\nConnection name:\t" +  PostgreSQL.connectionName();
                QMessageBox::information(0, QObject::tr("Connection Info"),
                                         info);
                ConnectionTestStatus = true;
            }
        PostgreSQL.close();
        }
    }
}

void CreateConnection::on_lineEditDatabaseFile_textChanged()
{
    ConnectionTestStatus = false;
}

void CreateConnection::on_lineEditHostname_textChanged()
{
    ConnectionTestStatus = false;
}

void CreateConnection::on_lineEditPort_textChanged()
{
    ConnectionTestStatus = false;
}

void CreateConnection::on_lineEditUserName_textChanged()
{
    ConnectionTestStatus = false;
}

void CreateConnection::on_lineEditUserToken_textChanged()
{
    ConnectionTestStatus = false;
}

void CreateConnection::on_lineEditPassword_textChanged()
{
    ConnectionTestStatus = false;
}

void CreateConnection::on_pushButtonOk_clicked()
{
    if(ui->lineEditConnectionName->text()=="")
    {
        inputErrorMsgBox(tr("Please insert a Connection Name!"));
        ui->lineEditConnectionName->setFocus();
    }
    else
    {
        if(ConnectionTestStatus != true)
        {
            inputErrorMsgBox(tr("There was no successfull connection test carried out!"));
        }

        else
        {
            QSettings settings;
            settings.beginGroup("DatabaseSettings");
                QString connString = settings.value("DatafileConnections").toString();
            settings.endGroup();
            QSqlDatabase connectionDB = QSqlDatabase::addDatabase("QSQLITE");
            connectionDB.setDatabaseName(connString);
            connectionDB.open();
            if(!connectionDB.open())
            {
                QMessageBox::critical(0, QObject::tr("Connection Error"),
                                                  connectionDB.lastError().text());
            }
            else
            {
                QString ConnectionName = ui->lineEditConnectionName->text();

                QString nameQueryString = "SELECT connection_name FROM tbl_connections WHERE connection_name='" + ConnectionName + "';";
                QSqlQuery nameQuery(nameQueryString);
                QVariant queryResult = nameQuery.first();
                if(queryResult == ConnectionName)
                {
                    inputErrorMsgBox(tr("Connection already exists, please choose another connection name!"));
                    ui->lineEditConnectionName->setFocus();
                }
                else
                {
                    saveSettings();
                }
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                  Private functions for CreateConnection                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

// Changes the User Interface of Dialog 'Create new Connection'
// for the value of the combobox 'Database Location' = 'Network'
//////////////////////////////////////////////////////////////////////////////
void CreateConnection::setUiToNetwork()
{
    //set all necessary Elements 'Enabled' or 'Disabled'
    //and fill the fields with standard values
    ui->lineEditDatabaseDriver->setText("QPSQL");
    ui->lineEditDatabaseServer->setText("PostgreSQL/PostGIS");

    ui->toolButtonOpenFile->hide();
    ui->labelDatabaseFile->setText("Database Name:");
    ui->lineEditDatabaseFile->setFixedWidth(200);
    ui->lineEditDatabaseFile->setText("openatla_main_db");

    ui->labelHostname->setEnabled(true);
    ui->lineEditHostname->setEnabled(true);
    ui->lineEditHostname->setText("www.openatlas.eu");

    ui->labelPort->setEnabled(true);
    ui->lineEditPort->setEnabled(true);
    ui->lineEditPort->setText("5432");

    ui->labelUserName->setEnabled(true);
    ui->lineEditUserName->setEnabled(true);
    ui->lineEditUserName->setText("openatla_jansaviktor");

    ui->labelPassword->setEnabled(true);
    ui->lineEditPassword->setEnabled(true);
    ui->lineEditPassword->clear();
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
}

// Changes the User Interface of Dialog 'Create new Connection'
// for the value of the combobox 'Database Location' = 'Network'
//////////////////////////////////////////////////////////////////////////////
void CreateConnection::setUiToLocal()
{
    //set all Elements 'Disabled' or 'Enabled'
    //and fill the fields with standard values

    ui->lineEditDatabaseDriver->setText("QSQLITE");
    ui->lineEditDatabaseServer->setText("SQLite/SpatialLite");

    ui->toolButtonOpenFile->show();
    ui->labelDatabaseFile->setText("Database File:");
    ui->lineEditDatabaseFile->setFixedWidth(160);
    ui->lineEditDatabaseFile->clear();

    ui->labelHostname->setDisabled(true);
    ui->lineEditHostname->setDisabled(true);
    ui->lineEditHostname->setText(noNeed);

    ui->labelPort->setDisabled(true);
    ui->lineEditPort->setDisabled(true);
    ui->lineEditPort->setText(noNeed);

    ui->labelUserName->setDisabled(true);
    ui->lineEditUserName->setDisabled(true);
    ui->lineEditUserName->setText(noNeed);

    ui->labelPassword->setDisabled(true);
    ui->lineEditPassword->setDisabled(true);
    ui->lineEditPassword->setText(noNeed);
    ui->lineEditPassword->setEchoMode(QLineEdit::Normal);
}

// Check the Content of all necessary lines for a SQLite-database connection
//////////////////////////////////////////////////////////////////////////////
bool CreateConnection::checkContentOfLineSQLite()
{
    if (ui->lineEditDatabaseFile->text()=="")
    {
        inputErrorMsgBox(tr("There is no datafile specified"));
        return false;
    }
    else if (ui->lineEditUserToken->text()=="")
    {
        inputErrorMsgBox(tr("There is no user token specified"));
        return false;
    }
    return true;
}

// Check the Content of all necessary lines for a PostgreSQL-database connection
//////////////////////////////////////////////////////////////////////////////
bool CreateConnection::checkContentOfLinePostgreSQL()
{
    if (ui->lineEditDatabaseFile->text()=="")
    {
        inputErrorMsgBox(tr("There is no database specified"));
        return false;
    }
    else if (ui->lineEditHostname->text()=="")
    {
        inputErrorMsgBox(tr("There is no hostname or IP-adress specified"));
        return false;
    }
    else if (ui->lineEditPort->text()=="")
    {
        inputErrorMsgBox(tr("There is no port specified"));
        return false;
    }
    else if (ui->lineEditUserName->text()=="")
    {
        inputErrorMsgBox(tr("There is no username specified"));
        return false;
    }
    else if (ui->lineEditUserToken->text()=="")
    {
        inputErrorMsgBox(tr("There is no user token specified"));
        return false;
    }
    return true;

}

// Show a messagebox with error messages depending on the input error
//////////////////////////////////////////////////////////////////////////////
void CreateConnection::inputErrorMsgBox(QString Message)
{
    QMessageBox::critical(0, QObject::tr("Input Error"),
                         Message);
}

// Save the settings to the database
//////////////////////////////////////////////////////////////////////////////
void CreateConnection::saveSettings()
{
    QString ConnectionName = ui->lineEditConnectionName->text();
    QString DatabaseDriver = ui->lineEditDatabaseDriver->text();
    QString DatabaseServer = ui->lineEditDatabaseServer->text();
    QString DatabaseName = ui->lineEditDatabaseFile->text();
    QString DatabaseHost = ui->lineEditHostname->text();
    QString DatabasePort = ui->lineEditPort->text();
    QString DatabaseUsername = ui->lineEditUserName->text();
    QString DatabaseUsernameToken = ui->lineEditUserToken->text();
    QString DatabasePassword;
    if (ui->lineEditPassword->text() != "" && ui->lineEditPassword->text() != noNeed)
    {
        DatabasePassword = "YES";
    }
    else
    {
        DatabasePassword = "NO";
    }
    QSqlQuery myquery;
    QString queryString = "INSERT INTO tbl_connections ('connection_name', 'database_driver', 'database_server', 'database_name', 'database_host', 'database_port', 'database_username', 'database_username_token', 'database_password') VALUES ('" + ConnectionName + "', '" + DatabaseDriver + "', '" + DatabaseServer + "', '" + DatabaseName + "', '" + DatabaseHost + "', '" + DatabasePort + "', '" + DatabaseUsername + "', '" + DatabaseUsernameToken + "', '" + DatabasePassword + "');";
    myquery.exec(queryString);

    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        settings.setValue("currentDatabaseConnection", ConnectionName);
    settings.endGroup();

    QMessageBox::information(0, QObject::tr("Connection successfully saved"),
                        tr("Connection successfully saved!\nIf you want to use the new connection, choose it in the toolbar!"));
    CreateConnection::close();
}
