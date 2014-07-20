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


#include "databasemanager.h"
#include <QSettings>
#include <QMessageBox>
#include <QFile>

QString conn_file = "connection_to_openAtlas_connections_file";
QString conn_Postgres_DB = "connection_to_Postgres_Database";
QString conn_SQLite_DB = "connection_to_SQLite_Database";



DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{

}

void DatabaseManager::startConnectionDatabase()
{
    // read name of current connection from settings file
    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        QString connString = settings.value("DatafileConnections").toString();
    settings.endGroup();

    //check whether file connString exists
    QFile file(connString);
    if(!file.exists())
        {
            QMessageBox::critical(0, QObject::tr("Input Error"),
                                 tr("The connection database file does not exist, please change the path or create a new one in the 'Settings'-menu!"));
        }
        else
        {
        connectionsDB = QSqlDatabase::addDatabase("QSQLITE", conn_file);
        connectionsDB.setDatabaseName(connString);

        connectionsDB = QSqlDatabase::database(conn_file);

        QString Name = connectionsDB.databaseName();
        QString Test = connectionsDB.connectionName();
    }
}

bool DatabaseManager::startDB(QString password)
{
    // read name of current connection and place of data file from settings file
    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        QString currentConnection = settings.value("currentDatabaseConnection").toString();
    settings.endGroup();

    QString connectionQueryString = "SELECT * FROM tbl_connections WHERE  connection_name = '" + currentConnection + "';";
    QSqlQuery connectionQuery(connectionQueryString, connectionsDB);

    while(connectionQuery.next())
    {
        connection_name = connectionQuery.value(1).toString();
        database_name = connectionQuery.value(2).toString();
        database_driver = connectionQuery.value(3).toString();
        database_server = connectionQuery.value(4).toString();
        database_host = connectionQuery.value(5).toString();
        database_port = connectionQuery.value(6).toInt();
        database_username = connectionQuery.value(7).toString();
        database_username_token = connectionQuery.value(8).toString();
    }

    if(database_driver == "QPSQL")
    {
        workingDB.close();
        DB_Status = false;
        workingDB = QSqlDatabase::addDatabase("QPSQL", conn_Postgres_DB);
        workingDB.setDatabaseName(database_name);
        workingDB.setHostName(database_host);
        workingDB.setPort(database_port);
        workingDB.setUserName(database_username);
        workingDB.setPassword(password);

        if (!workingDB.open())
        {
            QMessageBox::critical(0, QObject::tr("Database Error"),
                              workingDB.lastError().text());
        }
        else
        {
            DB_Status = true;
            return DB_Status;
        }
    }
    else
    {
    DB_Status = false;
    QFile file(database_name);
    if(!file.exists())
        {
            QMessageBox::critical(0, QObject::tr("Input Error"),
                                 tr("The database file does not exist, please change the path or create a new one in the 'Settings'-menu!"));
         DB_Status = false;
        }
        else
        {
            workingDB = QSqlDatabase::addDatabase("QSQLITE", conn_SQLite_DB);
            workingDB.setDatabaseName(database_name);
            DB_Status = true;
        }
    return DB_Status;
    }
}
