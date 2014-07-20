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


#ifndef FIRSTSTART_H
#define FIRSTSTART_H

#include <QDialog>

namespace Ui {
class firstStart;
}

class firstStart : public QDialog
{
    Q_OBJECT

public:
    explicit firstStart(QWidget *parent = 0);
    ~firstStart();

private:
    Ui::firstStart *ui;

private slots:
    void firstScreen();
    void secondScreen();
    void thirdScreen();
    void on_toolButtonOpenFileDialog_clicked();
    void on_pushButton_01_Next_clicked();
    void on_pushButton_02_Back_clicked();
    void on_pushButton_02_Next_clicked();
    void on_pushButton_03_Next_clicked();
};

#endif // FIRSTSTART_H
