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


#ifndef CREATECONNECTION_H
#define CREATECONNECTION_H

#include <QDialog>

namespace Ui {
class CreateConnection;
}

class CreateConnection : public QDialog
{
    Q_OBJECT

public:
    explicit CreateConnection(QWidget *parent = 0);
    ~CreateConnection();

private slots:
    void on_comboBoxDatabaseLocation_currentIndexChanged(const QString &location);
    void on_toolButtonOpenFile_clicked();
    void on_pushButtonTestConnection_clicked();
    void on_pushButtonOk_clicked();
    void on_lineEditDatabaseFile_textChanged();
    void on_lineEditHostname_textChanged();
    void on_lineEditPort_textChanged();
    void on_lineEditUserName_textChanged();
    void on_lineEditUserToken_textChanged();
    void on_lineEditPassword_textChanged();

private:
    Ui::CreateConnection *ui;
    void setUiToLocal();
    void setUiToNetwork();
    bool checkContentOfLineSQLite();
    bool checkContentOfLinePostgreSQL();
    void inputErrorMsgBox(QString);
    void saveSettings();
};

#endif // CREATECONNECTION_H
