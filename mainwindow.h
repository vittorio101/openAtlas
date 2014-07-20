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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <QtSql>
#include <ui_mainwindow.h>
#include <QSortFilterProxyModel>
#include <QNetworkReply>
#include <QStandardItemModel>

#include <marble/GeoPainter.h>
#include <marble/GeoDataDocument.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStandardItemModel* iStandardModel;

private:
    Ui::MainWindow *ui;
    void setupConnectionsModel();
    void updateCurrentConnection();
    void checkConnectionBasics();
    void updateStatusBar(QString Text, int intIcon); //int Icon: 0 = try to connect, 1 = no connection, 2 = connected
    void TimeMeasurementStatusBar(QString timeString);
    void view_Basic();
    void view_Items_clicked();
    QStringList extractUidFromModel(QAbstractItemModel *model, const QModelIndex &parent);
    QStringList extractStringsFromModel(QAbstractItemModel *model, const QModelIndex &parent, int column);
    void loadLocalPicture(QString);
    void loadOnlinePicture(QString);
    void showMainpicture(QPixmap *pm);
    void analyseMainImagePath(QString);
    void HERE__START__THE__PRIVATE__SLOTS();

    void update_sites_overview();
    void update_sites_overview(QString marked);
    void show_sites_in_database();
    void show_features_in_site();
    void show_su_in_feature();
    void show_finds_in_su();
    void clear_all_tabs();

    void fill_listviewThumbnails(QString clickedUid);
    void loadPlaceholder();

private slots:
//    void on_listView_Content_clicked(const QModelIndex &index);
    void update_tableViewItems(QString strIndex);
    void on_lineEdit_Filter_textEdited();
    void on_tableViewItems_clicked(const QModelIndex &index);
    void on_tableViewItems_doubleClicked(const QModelIndex &index);
    void on_createConnection_clicked();
    void check_answer_changeConnectionDlg(const QString &newComboboxText);
    void slot_netwManagerFinished(QNetworkReply *reply);
    void on_pushButton_Edit_Overview_clicked();
    void on_pushButton_TypeWizard_clicked();
    void on_toolButton_Datalevel_home_clicked();
    void on_toolButton_Datalevel_up_clicked();
    void on_tableViewLocation_clicked();
    void on_tableViewCultural_clicked();
    void on_tableViewChronological_clicked();
    void on_tableViewBibliography_clicked();
    void on_tableViewEvidence_clicked();
    void on_actionAbout_Qt_clicked();
    void on_actionAbout_openAtlas_clicked();
    void on_actionApplication_Preferences_clicked();
    void on_actionTreeEditor_clicked();

protected:
    QComboBox *ComboBoxConnections = new QComboBox;
    QLabel *StatusbarLoadingTime = new QLabel;
    QSqlQueryModel *listViewCategoriesModel = new QSqlQueryModel;
    QSqlQueryModel *tableViewItemsModel = new QSqlQueryModel;
    QSqlQueryModel *tableViewLocationModel = new QSqlQueryModel;
    QSqlQueryModel *tableViewCulturalModel = new QSqlQueryModel;
    QSqlQueryModel *tableViewChronologicalModel = new QSqlQueryModel;
    QSqlQueryModel *tableViewBibliographyModel = new QSqlQueryModel;
    QSqlQueryModel *tableViewEvidenceModel = new QSqlQueryModel;

    QSortFilterProxyModel *proxymodelSites = new QSortFilterProxyModel;
    QSortFilterProxyModel *proxymodelLocation = new QSortFilterProxyModel;
    QSortFilterProxyModel *proxymodelCultural = new QSortFilterProxyModel;
    QSortFilterProxyModel *proxymodelChronological = new QSortFilterProxyModel;
    QSortFilterProxyModel *proxymodelBibliography = new QSortFilterProxyModel;
    QSortFilterProxyModel *proxymodelEvidence = new QSortFilterProxyModel;

    QSqlQueryModel *emptyViewItemsModel = new QSqlQueryModel;
    Marble::GeoDataDocument *document = new Marble::GeoDataDocument;
    virtual void wheelEvent(QWheelEvent* wheel);
    void resizeEvent(QResizeEvent *resize);
    QGraphicsScene *gs = new QGraphicsScene;
    QPixmap *pm = new QPixmap;

    QStandardItemModel *itemModel = new QStandardItemModel;

};

#endif // MAINWINDOW_H
