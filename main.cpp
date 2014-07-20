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

#include "mainwindow.h"
#include "firststart.h"
#include "databasemanager.h"

#include <QApplication>
#include <QSettings>

//create an global instance of the Databasemanager
DatabaseManager *myDBM = new DatabaseManager;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //set application name to save the settings to an OS - dependend location
    app.setOrganizationName("openAtlas 1.0 alpha");

    // check if this is the first start of openAtlas
    QSettings settings;
    settings.beginGroup("MainWindow");
        QString fStart = settings.value("FirstStart").toString();
    settings.endGroup();
    if(fStart == "")
    {
        firstStart *startwin = new firstStart;
        startwin->exec();
    }

    //start the application
    MainWindow w;
    w.show();

    return app.exec();
}
