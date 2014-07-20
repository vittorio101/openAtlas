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
#include "ui_mainwindow.h"
#include "version.h"
#include "databasemanager.h"
#include "login.h"
#include "createconnection.h"
#include "typewizard.h"
#include "preferences.h"
#include "tree_editor.h"

#include <marble/MarbleWidget.h>
#include <marble/GeoPainter.h>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleModel.h>

#include <QScrollArea>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
//#include <QList>
//#include <QPair>
#include <QTableWidgetItem>
//#include <QVector>

#include <QHttp>
#include <QListWidgetItem>

#include <iostream>

using namespace Marble;

// database connection data
QString conn_to_openatlas_connections_file = "Connection_to_ConnectionsDataFile";
QString conn_to_openaltas_database = "Connection_to_openAtlas_Database";
QString currentConnection;
QString connString;
QString connection_name;
QString database_name;
QString database_driver;
QString database_server;
QString database_host;
QString database_port;
QString database_username;
QString database_username_token;
QString database_password;
QString trimmedText;
QString listViewCategoriesQuery;
QString x_lon_easting;
QString y_lat_northing;
QString ProjCS;
int categoryIndex;
int itemIndex;
int locationIndex;
int itemKey;
int nameUid;
bool DatabaseStatus;
QSqlDatabase connectionDB;

//class-wide variables for different purposes
QString strIndex;
QString strItem;
QString currentSite;
QString currentFeature;
QString currentStratigraphicalUnit;
QString currentFind;
QString groupBoxTitle;

// combobox strings
QString currentComboboxText;
QString newComboboxText;

// statusbar and statusbar strings
QStatusBar *statusbar;
QLabel *StatusbarIcon;
QLabel *StatusbarText;

// UI elements
QString filter;
QString strItem_part1;
QString strItem_part2;
QString strdataLevel;
QString imagePath;

//History for browser
int dataLevel;
QString last_clickedSite;
QString last_clickedSiteUid;
QModelIndex siteIndex;
QString last_clickedFeature;
QString last_clickedFeatureUid;
QModelIndex featureIndex;
QString last_clickedSU;
QString last_clickedSUUid;
QModelIndex suIndex;
QString last_clickedFind;
QString last_clickedFindUid;
QModelIndex findIndex;

QString clickedItem;
QStringList stringUids;
QString clickedUid;
QString clickedLocationUid;

QVariant varClickedItem;
QVariant varClickedLocation;


QString strNamePath;

bool ImageView;
QString mainImage;
QString imagePrefix;
QString imagePostfix;
QString imageName;
bool tabSitesDimensions;

/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                   Constructor and Deconstructor                            *
 *                   for mainWindow                                           *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
 *****************************************************************************/

/******************************************************************************
 * MainWindow(QWidget*): Constructor, creates the MainWindow class
 * Creates the main window, passing the parent to QMainWindow, and initializing
 * the ui.
 *****************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString firstStart = "NO";
    dataLevel = 0;
    if(!myDBM->workingDB.isOpen())
    {
        myDBM->startConnectionDatabase();
    }



    //read application settings from standard settings location (OS dependend)
    QSettings settings;
    settings.beginGroup("MainWindow");
        settings.setValue("FirstStart", firstStart);
        restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
        restoreState(settings.value("mainWindowState").toByteArray());
        int TabIndex = settings.value("TabIndex").toInt();
        int TabDetailsIndex = settings.value("TabDetailsIndex").toInt();
        int TabWidgetSpatialIndex = settings.value("TabSpatialIndex").toInt();
    settings.endGroup();

    settings.beginGroup("DatabaseSettings");
        currentConnection = settings.value("currentDatabaseConnection").toString();
    settings.endGroup();

    settings.beginGroup("Preferences");
        ImageView = settings.value("ImageViewActivated").toBool();
        imagePrefix = settings.value("ImageFolderPath").toString();
        imagePostfix = settings.value("ImageFileExtension").toString();

        tabSitesDimensions = settings.value("TabSitesDimension").toBool();

    settings.endGroup();

    //create MainWindow
    ui->setupUi(this);

    // hide experimental tabs
    ui->tabWidget_ItemDetails->removeTab(6);
    ui->tabWidget_ItemDetails->removeTab(4);

    this->setWindowTitle(openAtlasVersion);
    ui->tabWidget->setCurrentIndex(TabIndex);
    ui->tabWidget_ItemDetails->setCurrentIndex(TabDetailsIndex);
    ui->tabWidget_Spatial->setCurrentIndex(TabWidgetSpatialIndex);

    //Add QCombobox for Database Connections to toolbar 'Database'
    ComboBoxConnections->setFixedHeight(24);
    ComboBoxConnections->setFixedWidth(200);
    ui->toolBarDatabase->addWidget(ComboBoxConnections);
    setupConnectionsModel();
    ComboBoxConnections->setCurrentIndex(ComboBoxConnections->findData(currentConnection, Qt::DisplayRole));

    //create statusbar
    StatusbarIcon = new QLabel(this);
    StatusbarText = new QLabel(this);
    StatusbarLoadingTime = new QLabel(this);
    StatusbarLoadingTime->setText("Time Measurement");
    StatusbarLoadingTime->setIndent(10);
    ui->statusBar->addWidget(StatusbarLoadingTime);
    ui->statusBar->addPermanentWidget(StatusbarText);
    ui->statusBar->addPermanentWidget(StatusbarIcon);

    // set some GUI settings
    ui->lineEdit_Filter->installEventFilter(this);
    ui->radioButton_OpenStreetMap->setChecked(true);
    ui->pushButton_TypeWizard->setDisabled(true);

    // set MarbleWidget properties
    ui->MarbleWidget->setShowOverviewMap(false);

    GeoDataPlacemark *place = new GeoDataPlacemark("Institute of Prehistoric and Historical Archaeology, University of Vienna");
    place->setCoordinate( 16.3488, 48.2335, 0.0, GeoDataCoordinates::Degree );
    GeoDataDocument *document = new GeoDataDocument;
    document->append(place);

    // Add the document to MarbleWidget's tree model
    ui->MarbleWidget->model()->treeModel()->addDocument(document);

    // set view to location University of Vienna, Department for Pre- and Early History
    ui->MarbleWidget->centerOn(16.3488,48.2335);
    //ui->MarbleWidget->zoomView(3500);
    ui->MarbleWidget->zoomView(5000);

    // set background color of thumbnails viewer to black
    ui->listView_Thumbnails->setStyleSheet("* { background-color: rgb(0, 0, 0); }");


    // load empty picture to graphicsview
    loadLocalPicture("placeholder_empty.png");

    view_Basic();
    checkConnectionBasics();

    // waiting for signal from combobox 'ComboBoxConnections'
    connect(ComboBoxConnections, SIGNAL(currentIndexChanged(const QString)),
            this, SLOT(check_answer_changeConnectionDlg(const QString)));
}

/******************************************************************************
 * ~MainWindow(): Destructor
 * Save application settings and deletes the ui.
 *****************************************************************************/
MainWindow::~MainWindow()
{
    //save application settings to standard location (OS dependend!)
    QSettings settings;
    settings.beginGroup("MainWindow");
        settings.setValue("mainWindowState", saveState());
        settings.setValue("mainWindowGeometry", saveGeometry());
        settings.setValue("TabIndex", ui->tabWidget->currentIndex());
        settings.setValue("TabDetailsIndex", ui->tabWidget_ItemDetails->currentIndex());
        settings.setValue("TabSpatialIndex", ui->tabWidget_Spatial->currentIndex());
    settings.endGroup();

    delete ui;
}



/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                   Private functions for MainWindow                         *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
 *****************************************************************************/

/******************************************************************************
 * updateCurrentConnection(): Private function
 * Set ComboBox text to current connection name
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::updateCurrentConnection()
{
    QString newCurrentConnection = ComboBoxConnections->currentText();
    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        settings.setValue("currentDatabaseConnection", newCurrentConnection);
    settings.endGroup();
}

/******************************************************************************
 * view_Basic(): Private function
 * Set visibility and content of all elements
 * to the start view
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::view_Basic()
{
    ui->textEdit_DataLevel->setText("sites");
    ui->lineEdit_Filter->setText("");
    filter = ui->lineEdit_Filter->text();
    on_lineEdit_Filter_textEdited();
    ui->tabWidget->setDisabled(true);
    ui->toolButton_Delete_Item->setDisabled(true);
    ui->groupBox_Details->setTitle("Details for item");

    // groupbox 'Spatial Info'
    ui->textEdit_SpatialInfo_LocatedIn->clear();
    ui->lineEdit_SpatialInfo_Easting->clear();
    ui->lineEdit_SpatialInfo_Northing->clear();
    ui->lineEdit_SpatialInfo_EPSG->clear();
    ui->lineEdit_SpatialInfo_CS->clear();

    ui->textEdit_Name->clear();
    ui->textEdit_Description->clear();
    ui->textEdit_Type->clear();

    // MarbleWidget
    ui->textEdit_Overview->clear();
    ui->MarbleWidget->centerOn(16.3488,48.2335);
    //ui->MarbleWidget->zoomView(3500);
    ui->MarbleWidget->zoomView(5000);
}

/******************************************************************************
 * view_items_clicked(): Private function
 * Set visibility and content of all elements
 * to the state 'items clicked'
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::view_Items_clicked()
{
    // clear all content
    ui->textEdit_Name->clear();
    ui->textEdit_Description->clear();
    ui->textEdit_Type->clear();

    // generell view settings
    ui->tabWidget->setEnabled(true);

    ui->toolButton_Delete_Item->setEnabled(true);
    // set items in groupbox 'Overview' ReadOnly
    ui->textEdit_Name->setReadOnly(true);
    ui->textEdit_Type->setReadOnly(true);
    ui->textEdit_Description->setReadOnly(true);

    // set 'Item Details' buttons
    ui->pushButton_Save_Overview->setDisabled(true);
    ui->pushButton_Cancel_Overview->setDisabled(true);
    ui->pushButton_TypeWizard->setDisabled(true);

    // set 'Item Spatial Info' buttons
    ui->pushButton_Save_SpatialInfo->setDisabled(true);
    ui->pushButton_Cancel_SpatialInfo->setDisabled(true);

    // set items in groupbox 'Spatial Info' ReadOnly
    ui->textEdit_SpatialInfo_LocatedIn->setReadOnly(true);
    ui->lineEdit_SpatialInfo_Easting->setReadOnly(true);
    ui->lineEdit_SpatialInfo_Northing->setReadOnly(true);
    ui->lineEdit_SpatialInfo_EPSG->setReadOnly(true);
    ui->lineEdit_SpatialInfo_CS->setReadOnly(true);

    // set buttons in Tab 'Cultural/Temporal' disabled
    ui->pushButton_Cancel_Cultural->setDisabled(true);
    ui->pushButton_Save_Cultural->setDisabled(true);

    //QWidget *removedTab = ui->tabWidget->widget(3);
//    QWidget *removedTab = ui->tab_Dimensions;
//    qDebug()<< removedTab;
//    ui->tabWidget_ItemDetails->removeTab(3);
//    qDebug() << "Tab removed!";
//    //ui->tabWidget->insertTab(3, removedTab, "TestWidget");
//    ui->tabWidget->addTab(removedTab,"TestWidget");
}

/******************************************************************************
 * setupConnectionsModel(): Private function
 * Open the database connection file 'openatlas.cdf'
 * and show the available connection names in 'ComboboxConnections'
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::setupConnectionsModel()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT connection_name FROM tbl_connections", myDBM->connectionsDB);
    ComboBoxConnections->setModel(model);
}

/******************************************************************************
 * checkConnectionBasics(): Private function
 * check basis connection settings
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::checkConnectionBasics()
{
    QString passwordNew;

    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        currentConnection = settings.value("currentDatabaseConnection").toString();
    settings.endGroup();

    QSqlDatabase connectionDB = QSqlDatabase::database(myDBM->conn_file);
    QString connectionQueryString = "SELECT * FROM tbl_connections WHERE  connection_name = '" + currentConnection + "';";

    QSqlQuery connectionQuery(connectionQueryString, connectionDB);
    while(connectionQuery.next())
    {
        connection_name = connectionQuery.value(1).toString();
        database_name = connectionQuery.value(2).toString();
        database_driver = connectionQuery.value(3).toString();
        database_password = connectionQuery.value(9).toString();
    }

    if(database_password == "YES")
    {
        QString statusText = "Try to connect to " + database_name + " using connection '" + ComboBoxConnections->currentText() + "'...";
        updateStatusBar(statusText, 0);

        Login login;
        if (login.exec() == QDialog::Accepted)
        {
            const QString password = login.linePassword();
            passwordNew = password;
            login.close();
        }
        else
        {
            QMessageBox::critical(this, tr("Connection Error"), tr("Cannot connect to database!"));

            QString statusText = "Cannot connect to database " + database_name + "!";
            updateStatusBar(statusText, 1);
            return;
        }

        DatabaseStatus = myDBM->startDB(passwordNew);
    }
    else
    {
        DatabaseStatus = myDBM->startDB("");
    }

    //check, if the connection to the database works
    if(DatabaseStatus == true)
    {
        QString statusText = "Connected to database " + database_name;
        updateStatusBar(statusText, 2);
    }
    else
    {
        QString statusText = "Cannot connect to database " + database_name + "!";
        updateStatusBar(statusText, 1);
        myDBM->workingDB.close();

    }
    update_sites_overview();
}

/******************************************************************************
 * update_sites_overview(): Private function
 * load all sites on database and show in 'tableViewItems'
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::update_sites_overview()
{
    dataLevel = 0;
    qDebug() << "'update_sites_overview', datalevel: " << dataLevel;
    ui->textEdit_DataLevel->setText("sites");
    ui->lineEdit_Filter->clear();
    filter = "";
    on_lineEdit_Filter_textEdited();

    //update config settings
    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        currentConnection = settings.value("currentDatabaseConnection").toString();
    settings.endGroup();

    if(myDBM->workingDB.driverName() =="QPSQL")
    {
        QString query = "SELECT sites.uid, sites.entity_name_uri FROM openatlas.sites ORDER BY sites.entity_name_uri ASC;";

        QTime myTimer;
        myTimer.start();
        int intNumber;
        QString strNumber;

        tableViewItemsModel->setQuery(query, myDBM->workingDB);

        proxymodelSites->setSourceModel(tableViewItemsModel);

        ui->tableViewItems->setModel(proxymodelSites);
        ui->tableViewItems->resizeColumnToContents(1);
        ui->tableViewItems->resizeRowsToContents();
        ui->tableViewItems->hideColumn(0);
        ui->tableViewItems->model()->setHeaderData(1, Qt::Horizontal, QObject::tr(""));

        //ui->tableViewItems->activated(clickedUid);

        int Milliseconds = myTimer.elapsed();
        float Seconds = (float) Milliseconds/1000;
        QString strSeconds;
        strSeconds.append(QString("%1").arg(Seconds));
        intNumber = tableViewItemsModel->query().size();
        strNumber.append(QString("%1").arg(intNumber));
        QString time = strNumber +" items loaded in " + strSeconds + " sec.";
        TimeMeasurementStatusBar(time);
    }
    else
    {
        listViewCategoriesModel->setQuery("SELECT name  FROM sqlite_master WHERE type='table'", myDBM->workingDB);
        qDebug() << myDBM->workingDB.lastError();
    }
    clear_all_tabs();
}

/******************************************************************************
 * update_sites_overview(QString marked): Private function
 * load all sites on database and show in 'tableViewItems'
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::update_sites_overview(QString marked)
{
    dataLevel = 0;
    qDebug() << "'update_sites_overview', datalevel: " << dataLevel;
    ui->textEdit_DataLevel->setText("sites");
    ui->lineEdit_Filter->clear();
    filter = "";
    on_lineEdit_Filter_textEdited();

    //update config settings
    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        currentConnection = settings.value("currentDatabaseConnection").toString();
    settings.endGroup();

    if(myDBM->workingDB.driverName() =="QPSQL")
    {
        QString query = "SELECT sites.uid, sites.entity_name_uri FROM openatlas.sites ORDER BY sites.entity_name_uri ASC;";
        tableViewItemsModel->setQuery(query, myDBM->workingDB);

        proxymodelSites->setSourceModel(tableViewItemsModel);

        int intActivatedItem= marked.toInt();
        QModelIndex activatedItem(siteIndex);
        ui->tableViewItems->setModel(proxymodelSites);
        ui->tableViewItems->resizeColumnToContents(1);
        ui->tableViewItems->resizeRowsToContents();
        ui->tableViewItems->hideColumn(0);

        //ui->tableViewItems->setCurrentIndex(siteIndex);
    }
    else
    {
//        listViewCategoriesModel->setQuery("SELECT name  FROM sqlite_master WHERE type='table'", myDBM->workingDB);
//        qDebug() << myDBM->workingDB.lastError();
    }
}

/******************************************************************************
 * updateStatusBar(QString Text, int intIcon): Private function
 * write state of database connection to the right status ar
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::updateStatusBar(QString Text, int intIcon)
{
    switch(intIcon)
    {
    case 0:
        StatusbarIcon->setPixmap(QPixmap(QString::fromUtf8(":/small/working")));
        break;
    case 1:
        StatusbarIcon->setPixmap(QPixmap(QString::fromUtf8(":/small/disconnected")));
        break;
    case 2:
        StatusbarIcon->setPixmap(QPixmap(QString::fromUtf8(":/small/connected")));
        break;
    }
    StatusbarText->setText(Text);
}

/******************************************************************************
 * TimeMeasurementStatusBar(QString timeString): Private function
 * write loading time to the left status bar
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::TimeMeasurementStatusBar(QString timeString)
{
    StatusbarLoadingTime->setText(timeString);
}

/******************************************************************************
 * on_lineEdit_Filter_textEdited(): Private function
 * search filter for 'tableItemsView'
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::on_lineEdit_Filter_textEdited()
{
    filter = ui->lineEdit_Filter->text();
    proxymodelSites->setFilterRegExp(QRegExp(filter, Qt::CaseInsensitive, QRegExp::Wildcard));
    proxymodelSites->setFilterKeyColumn(1);
}

/******************************************************************************
 * extractStringsFromModel(QAbstractItemModel *model,
 * const QModelIndex &parent, int column): Private function
 * Extract strings from datamodel
 * Last edit: 09.05.2014
 *****************************************************************************/
QStringList MainWindow::extractStringsFromModel(QAbstractItemModel *model, const QModelIndex &parent, int column)
{
    QStringList retval;
    int rowCount = model->rowCount(parent);
    for(int i = 0; i < rowCount; ++i)
    {
        QModelIndex idx = model->index(i, column, parent);

        if(idx.isValid())
        {
            retval << idx.data(Qt::DisplayRole).toString();
            retval << extractStringsFromModel(model, idx, column);
        }
    }
    return(retval);
}

void MainWindow::analyseMainImagePath(QString mainImage)
{
    QString mainImagepath = mainImage.left(3);
    if(mainImagepath == "htt" || mainImagepath == "ftp")
    {
        loadOnlinePicture(mainImage);
    }
    else
    {
        loadLocalPicture(mainImage);
    }
}

void MainWindow::loadOnlinePicture(QString mainImage)
{
    QNetworkAccessManager *m_netwManager = new QNetworkAccessManager(this);
    connect(m_netwManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_netwManagerFinished(QNetworkReply*)));
    QUrl url(mainImage);
    QNetworkRequest request(url);
    m_netwManager->get(request);
}

void MainWindow::loadLocalPicture(QString mainImage)
{
    QPixmap *pm = new QPixmap(mainImage);
    showMainpicture(pm);
}


void MainWindow::loadPlaceholder()
{
    QPixmap *pm_empty = new QPixmap("/home/viktor/0000_openAtlas_Project/openAtlas-dev/placeholder_empty.png");

    showMainpicture(pm_empty);
}


void MainWindow::showMainpicture(QPixmap *pm)
{
    QPixmap scaledImage;
    //scaledImage = pm->scaled(ui->graphicsView->width()-10, ui->graphicsView->height()-10, Qt::KeepAspectRatio);
    gs->addPixmap(scaledImage);
    //ui->graphicsView->setScene(gs);
}

void MainWindow::show_sites_in_database()
{
    // clear filter
    ui->lineEdit_Filter->clear();
    filter.clear();
    on_lineEdit_Filter_textEdited();

    QTime myTimer;
    myTimer.start();
    int intNumber;
    QString strNumber;

    if(myDBM->workingDB.driverName()== "QPSQL")
    {
        QString sitesQuery = "SELECT sites.uid, sites.entity_name_uri FROM openatlas.sites ORDER BY sites.entity_name_uri ASC;";
        tableViewItemsModel->setQuery(sitesQuery, myDBM->workingDB);
        if (myDBM->workingDB.lastError().isValid())
        qDebug() << myDBM->workingDB.lastError();
    }
    else
    {
        tableViewItemsModel->setQuery("SELECT name  FROM sqlite_master WHERE type='table'", myDBM->workingDB);
    }

    proxymodelSites->setSourceModel(tableViewItemsModel);
    ui->tableViewItems->setModel(proxymodelSites);

    strdataLevel = "sites";
    ui->textEdit_DataLevel->setText(strdataLevel);

    int Milliseconds = myTimer.elapsed();
    float Seconds = (float) Milliseconds/1000;
    QString strSeconds;
    strSeconds.append(QString("%1").arg(Seconds));
    intNumber = tableViewItemsModel->query().size();
    strNumber.append(QString("%1").arg(intNumber));
    QString time = strNumber +" items loaded in " + strSeconds + " sec.";
    TimeMeasurementStatusBar(time);
}

void MainWindow::show_features_in_site()
{
    // clear filter
    ui->lineEdit_Filter->clear();
    filter.clear();
    on_lineEdit_Filter_textEdited();

    QTime myTimer;
    myTimer.start();
    int intNumber;
    QString strNumber;

    if(myDBM->workingDB.driverName()== "QPSQL")
    {
        QString featuresQuery = "SELECT features.uid, features.entity_name_uri FROM openatlas.features WHERE features.parent = " + clickedUid + " ORDER BY features.entity_name_uri ASC;";
        tableViewItemsModel->setQuery(featuresQuery, myDBM->workingDB);
        if (myDBM->workingDB.lastError().isValid())
        qDebug() << myDBM->workingDB.lastError();
    }
    else
    {
        tableViewItemsModel->setQuery("SELECT name  FROM sqlite_master WHERE type='table'", myDBM->workingDB);
    }

    proxymodelSites->setSourceModel(tableViewItemsModel);
    ui->tableViewItems->setModel(proxymodelSites);

    int Milliseconds = myTimer.elapsed();
    float Seconds = (float) Milliseconds/1000;
    QString strSeconds;
    strSeconds.append(QString("%1").arg(Seconds));
    intNumber = tableViewItemsModel->query().size();
    strNumber.append(QString("%1").arg(intNumber));
    QString time = strNumber +" items loaded in " + strSeconds + " sec.";
    TimeMeasurementStatusBar(time);
}

void MainWindow::show_su_in_feature()
{
    // clear filter
    ui->lineEdit_Filter->clear();
    filter.clear();
    on_lineEdit_Filter_textEdited();

    QTime myTimer;
    myTimer.start();
    int intNumber;
    QString strNumber;

    if(myDBM->workingDB.driverName()== "QPSQL")
    {
        QString suQuery = "SELECT stratigraphical_units.uid, stratigraphical_units.entity_name_uri FROM openatlas.stratigraphical_units WHERE stratigraphical_units.parent = " + clickedUid + " ORDER BY stratigraphical_units.entity_name_uri ASC;";
        qDebug() << "Querystring: " << suQuery;
        tableViewItemsModel->setQuery(suQuery, myDBM->workingDB);
        if (myDBM->workingDB.lastError().isValid())
        qDebug() << myDBM->workingDB.lastError();
    }
    else
    {
        tableViewItemsModel->setQuery("SELECT name  FROM sqlite_master WHERE type='table'", myDBM->workingDB);
    }

    proxymodelSites->setSourceModel(tableViewItemsModel);
    ui->tableViewItems->setModel(proxymodelSites);

    int Milliseconds = myTimer.elapsed();
    float Seconds = (float) Milliseconds/1000;
    QString strSeconds;
    strSeconds.append(QString("%1").arg(Seconds));
    intNumber = tableViewItemsModel->query().size();
    strNumber.append(QString("%1").arg(intNumber));
    QString time = strNumber +" items loaded in " + strSeconds + " sec.";
    TimeMeasurementStatusBar(time);
}

void MainWindow::show_finds_in_su()
{
    // clear filter
    ui->lineEdit_Filter->clear();
    filter.clear();
    on_lineEdit_Filter_textEdited();

    QTime myTimer;
    myTimer.start();
    int intNumber;
    QString strNumber;

    if(myDBM->workingDB.driverName()== "QPSQL")
    {
        QString suQuery = "SELECT finds.uid, finds.entity_name_uri FROM openatlas.finds WHERE finds.parent = " + clickedUid + "ORDER BY finds.entity_name_uri ASC;";
        tableViewItemsModel->setQuery(suQuery, myDBM->workingDB);
        if (myDBM->workingDB.lastError().isValid())
        qDebug() << myDBM->workingDB.lastError();
    }
    else
    {
        tableViewItemsModel->setQuery("SELECT name  FROM sqlite_master WHERE type='table'", myDBM->workingDB);
    }

    proxymodelSites->setSourceModel(tableViewItemsModel);
    ui->tableViewItems->setModel(proxymodelSites);

    int Milliseconds = myTimer.elapsed();
    float Seconds = (float) Milliseconds/1000;
    QString strSeconds;
    strSeconds.append(QString("%1").arg(Seconds));
    intNumber = tableViewItemsModel->query().size();
    strNumber.append(QString("%1").arg(intNumber));
    QString time = strNumber +" items loaded in " + strSeconds + " sec.";
    TimeMeasurementStatusBar(time);
}


/******************************************************************************
 * clear_all_tabs()
 * Clear all fields in the right upper and lower Tab
 * Last edit: 30.06.2014
 *****************************************************************************/
void MainWindow::clear_all_tabs()
{
    QSqlQueryModel *emptyModel = new QSqlQueryModel;
    emptyModel->setQuery("");

    //clear filter textfield
    ui->lineEdit_Filter->clear();
    filter = "";

    //clear all fields in 'Details of Item'
    ui->textEdit_Name->clear();
    ui->textEdit_Description->clear();
    ui->textEdit_Type->clear();
    ui->groupBox_Details->setTitle("Details");

    //clear all fields in tab 'Overview'
    ui->textEdit_Overview->clear();
    ui->label_Imageview->clear();

    //clear all fields in 'Spatial Info'
    ui->textEdit_SpatialInfo_LocatedIn->clear();
    ui->lineEdit_SpatialInfo_Easting->clear();
    ui->lineEdit_SpatialInfo_Northing->clear();
    ui->lineEdit_SpatialInfo_EPSG->clear();
    ui->lineEdit_SpatialInfo_CS->clear();

    // clear all fields in tab 'Classification'
    ui->tableViewChronological->setModel(emptyModel);
    ui->textEdit_Chronological->clear();
    ui->tableViewCultural->setModel(emptyModel);
    ui->textEdit_Cultural->clear();

    // clear all fields in tab 'Bibliography/Evidence'
    ui->tableViewBibliography->setModel(emptyModel);
    ui->textEdit_Bibliography->clear();
    ui->tableViewEvidence->setModel(emptyModel);
    ui->textEdit_Evidence->clear();

    //set MARBLE view to location University of Vienna, Department for Pre- and Early History
    ui->MarbleWidget->centerOn(16.3488,48.2335);
    ui->MarbleWidget->zoomView(3500);

    //set Tabs disabled
    ui->tabWidget->setDisabled(true);
}


void MainWindow::fill_listviewThumbnails(QString clickedUid)
{
    QString strimageQuery = "SELECT links_images.entity_uri FROM openatlas.links_images WHERE links_images.links_entity_uid_from = " + clickedUid + ";";
    QSqlQuery imageQuery(strimageQuery, myDBM->workingDB);

    int i = 0;
    while(imageQuery.next())
    {
        i++;
        qDebug() << "imageQuery number " << i << " called, clicked item: " << clickedItem;
        QString partPath = imageQuery.value(0).toString();
        imagePath = imagePrefix + partPath + imagePostfix;

        loadOnlinePicture(imagePath);
    }
}

/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                   Private slots for MainWindow                             *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
 *****************************************************************************/

void MainWindow::HERE__START__THE__PRIVATE__SLOTS()
{
    // senseless empty function, just to see the start of the section 'private
    // slots' in qtcreator overview
}

/******************************************************************************
 * resizeEvent(): Private Slot
 * updates the content of graphicsView every time the MainWindow is resized
 * Last edit: 04.05.2014
 *****************************************************************************/
void MainWindow::resizeEvent(QResizeEvent *resize)
{
    //ui->graphicsView->fitInView(gs->itemsBoundingRect() ,Qt::KeepAspectRatio);

    // expermiental picture view in tab 'Testtab'
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int w = ui->label_Imageview->width();
    int h = ui->label_Imageview->height();
    ui->label_Imageview->setPixmap(pm->scaled(w, h, Qt::KeepAspectRatio));
    //ui->label_Imageview->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //ui->label_Imageview->setScaledContents(true);
}

/******************************************************************************
 * wheelEvent(): Private Slot
 * zoom mainPicture in and out by scrolling the mouse wheel
 * Last edit: 04.05.2014
 *****************************************************************************/
void MainWindow::wheelEvent(QWheelEvent* wheel)
{
    //ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
//    int Height = ui->graphicsView->height();
//    int Width = ui->graphicsView->width();
//    ui->graphicsView->setFixedHeight(Height);
//    ui->graphicsView->setFixedWidth(Width);

//    // Scale the view / do the zoom
//    double scaleFactor = 1.15;
//    if(wheel->delta() > 0) {
//        // Zoom in
//        ui->graphicsView->scale(scaleFactor, scaleFactor);
//    } else
//    {
//        // Zooming out
//        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
//    }
}

/******************************************************************************
 * on_pushButton_Edit_Overview_clicked(): Private Slot
 * Set line edits and text edits in  group box'item Details' editable and
 * the variable bool_overviewEditable to 'true'.
 * Last edit: 04.05.2014
 *****************************************************************************/
void MainWindow::on_pushButton_Edit_Overview_clicked()
{
    ui->textEdit_Name->setReadOnly(false);
    ui->textEdit_Description->setReadOnly(false);
    ui->pushButton_Cancel_Overview->setEnabled(true);
    ui->pushButton_Save_Overview->setEnabled(true);
    ui->pushButton_TypeWizard->setEnabled(true);
}

/******************************************************************************
 * on_pushButton_Edit_TypeWizard_clicked(): Private Slot
 * call dialog 'Type Wizard'
 * Last edit: 04.05.2014
 *****************************************************************************/
void MainWindow::on_pushButton_TypeWizard_clicked()
{
    TypeWizard *dialog = new TypeWizard(dataLevel);
    qDebug() << "Calling TypeWizard, datalevel: " << dataLevel;

    switch(dataLevel)
    {
    case 0:
        dialog->setWindowTitle("Type Wizard - Sites");
        break;
    case 1:
        dialog->setWindowTitle("Type Wizard - Features");
        break;
    case 2:
        dialog->setWindowTitle("Type Wizard - Stratigraphical Units");
        break;
    case 3:
        dialog->setWindowTitle("Type Wizard - Finds");
        break;
    }

    dialog->exec();
}

/******************************************************************************
 * on_toolButton_Datalevel_home_clicked(): Private Slot
 * set Item browser to level 'sites' and clear history
 * Last edit: 04.05.2014
 *****************************************************************************/
void MainWindow::on_toolButton_Datalevel_home_clicked()
{
    update_sites_overview();
    last_clickedSite = "";
    last_clickedSiteUid = "";
    last_clickedFeature = "";
    last_clickedFeatureUid = "";
    last_clickedSU = "";
    last_clickedSUUid = "";
    last_clickedFind = "";
    last_clickedFindUid = "";
    ui->toolButton_Datalevel_home->setEnabled(false);
    ui->toolButton_Datalevel_up->setEnabled(false);
    clear_all_tabs();
}

/******************************************************************************
 * on_toolButton_Datalevel_down_clicked(): Private Slot
 *
 * Last edit: 04.05.2014
 *****************************************************************************/
void MainWindow::on_toolButton_Datalevel_up_clicked()
{
    // do something
    switch(dataLevel)
    {
    case 1:
        clickedUid = last_clickedSiteUid;
        strdataLevel = "Site '" + last_clickedSite + " -> Features";
//        groupBoxTitle = "Details for site '" + currentSite +"' -> Feature '" + currentFeature + "'";
//        ui->groupBox_Details->setTitle(groupBoxTitle);
        ui->textEdit_DataLevel->setText(strdataLevel);
        //update_sites_overview();
        show_sites_in_database();
        ui->toolButton_Datalevel_up->setEnabled(false);
        ui->toolButton_Datalevel_home->setEnabled(false);
        break;
    case 2:
        clickedUid = last_clickedSiteUid;
        strdataLevel = "Site '" + last_clickedSite + " -> Features";
//        groupBoxTitle = "Details for site '" + currentSite +"' -> Feature '" + currentFeature + "'";
//        ui->groupBox_Details->setTitle(groupBoxTitle);
        ui->textEdit_DataLevel->setText(strdataLevel);
        show_features_in_site();
        break;

    case 3:
        clickedUid = last_clickedFeatureUid;
        strdataLevel = "Site '" + last_clickedSite + " -> Feature '" + last_clickedFeature + "' -> Stratigraphical Units";
//        groupBoxTitle = "Details for site '" + currentSite +"' -> Feature '" + currentFeature + "'";
//        ui->groupBox_Details->setTitle(groupBoxTitle);
        ui->textEdit_DataLevel->setText(strdataLevel);
        show_su_in_feature();
        break;
    }
    dataLevel--;
    clear_all_tabs();
}

/******************************************************************************
 * check_answer_changeConnectionDlg(): Private Slot
 * Check, if the content of 'ComboBoxConnection has really changed
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::check_answer_changeConnectionDlg(const QString &newComboboxText)
{
    QSettings settings;
    settings.beginGroup("DatabaseSettings");
        QString currentComboboxText = settings.value("currentDatabaseConnection").toString();
    settings.endGroup();
    if(newComboboxText == currentComboboxText)
    {
        return;
    }
    else
    {
        //reset time measurement and clear lineEditFilter
        TimeMeasurementStatusBar("Time Measurement");
        ui->lineEdit_Filter->clear();
        filter = "";

        currentConnection = ComboBoxConnections->currentText();
        QSettings settings;
        settings.beginGroup("DatabaseSettings");
            settings.setValue("currentDatabaseConnection", currentConnection);
        settings.endGroup();
        checkConnectionBasics();
    }
}

/******************************************************************************
 * update_tableViewItems(QString categoryQueryResult): Private Slot
 * update item list after listView_Content changed
 * Last edit: 29.06.2014
 *****************************************************************************/
void MainWindow::update_tableViewItems(QString categoryQueryResult)
{
    // set timer to meassure loading time
    QTime myTimer;
    myTimer.start();
    int intNumber;
    QString strNumber;

    if(myDBM->workingDB.driverName()== "QPSQL")
    {
        QString QueryItems = "SELECT " + categoryQueryResult + "." + "entity_name_uri, " + categoryQueryResult + ".uid FROM openatlas." + categoryQueryResult + " ORDER BY " + categoryQueryResult + ".entity_name_uri ASC";
        tableViewItemsModel->setQuery(QueryItems, myDBM->workingDB);
    }
    else
    {
        tableViewItemsModel->setQuery("SELECT name  FROM sqlite_master WHERE type='table'", myDBM->workingDB);
    }

    proxymodelSites->setSourceModel(tableViewItemsModel);
    ui->tableViewItems->setModel(proxymodelSites);
    ui->tableViewItems->resizeColumnToContents(0);
    ui->tableViewItems->hideColumn(1);
    ui->tableViewItems->resizeRowsToContents();

    // measure the loading time
    int Milliseconds = myTimer.elapsed();
    float Seconds = (float) Milliseconds/1000;
    QString strSeconds;
    strSeconds.append(QString("%1").arg(Seconds));
    intNumber = tableViewItemsModel->query().size();
    strNumber.append(QString("%1").arg(intNumber));

    // write measurement time to status bar
    QString time = strNumber +" items loaded in " + strSeconds + " sec.";
    TimeMeasurementStatusBar(time);
}

/******************************************************************************
 * on_createConnection_clicked(): Private Slot
 * Call dialog 'createConnection'
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::on_createConnection_clicked()
{
    CreateConnection *dlg = new CreateConnection;
    dlg->exec();
    setupConnectionsModel();
    updateCurrentConnection();
}

void MainWindow::slot_netwManagerFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        return;
    }
    QByteArray jpegData = reply->readAll();
    //QPixmap *pm = new QPixmap;

    pm->loadFromData(jpegData);
    showMainpicture(pm);

    // expermiental picture view
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int w = ui->label_Imageview->width();
    int h = ui->label_Imageview->height();
    ui->label_Imageview->setPixmap(pm->scaled(w, h, Qt::KeepAspectRatio));
    //ui->label_Imageview->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    // same code in resize-event...

}

/******************************************************************************
 * on_tableViewItems_clicked(): Private slot
 * read data from database
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::on_tableViewItems_clicked(const QModelIndex &index)
{
    clear_all_tabs();
    QString queryString;
    qDebug() << "'on_tableViewItems_clicked' called...";

    view_Items_clicked();
    varClickedItem = index.data();
    clickedItem = varClickedItem.toString();

    itemIndex = ui->tableViewItems->selectionModel()->currentIndex().row();
    QString itemIndexString;
    itemIndexString.append(QString("%1").arg(itemIndex));

    QStringList stringUids = extractStringsFromModel(ui->tableViewItems->model(), QModelIndex(), 0);
    QString clickedUid = stringUids.at(itemIndex);

    switch(dataLevel)
    {
    case 0:
        // write values for history
        last_clickedSite = clickedItem;
        last_clickedSiteUid = clickedUid;

        // call query for site details
        queryString = "SELECT * FROM openatlas.sites WHERE openatlas.sites.uid = '" + clickedUid + "';";
        strItem_part1 = "Details for site '" + last_clickedSite + "'";
        ui->groupBox_Details->setTitle(strItem_part1);
        break;

    case 1:
        // write values for history
        last_clickedFeature = clickedItem;
        last_clickedFeatureUid = clickedUid;

        // call query for features details
        strItem_part1 = "Details for feature '" + last_clickedFeature + "'";
        ui->groupBox_Details->setTitle(strItem_part1);
        queryString = "SELECT * FROM openatlas.features WHERE openatlas.features.uid = '" + clickedUid + "';";

        break;
    case 2:
        // write values for history
        last_clickedSU = clickedItem;
        last_clickedSUUid = clickedUid;

        // call query for stratigraphical_unit details
        strItem_part1 = "Details for stratigraphical unit '" + last_clickedSU + "'";
        //groupBoxTitle = strItem_part1;
        ui->groupBox_Details->setTitle(strItem_part1);
        queryString = "SELECT * FROM openatlas.stratigraphical_units WHERE openatlas.stratigraphical_units.uid = '" + clickedUid + "';";

    case 3:
        // write values for history
        last_clickedFind = clickedItem;
//        last_clickedFindUid = clickedUid;

        // call query for find details
        strItem_part1 = "Details for find '" + last_clickedFind + "'";
        ui->groupBox_Details->setTitle(strItem_part1);
        queryString = "SELECT * FROM openatlas.finds WHERE openatlas.finds.uid = '" + clickedUid + "';";
        break;
    }

    QString epsgCode;
    QString absPeriod;
    QString chronPeriod;
    QString geogPosition;
    QString location;
    QString culturalClassification;
    QString culturalClassificationString;
    QString chronologicalClassification;
    QString chronologicalClassificationString;
    QString overviewSeperator = "==================<br />";

    QSqlQuery workingQuery(queryString, myDBM->workingDB);

    // get indices of columns
    QSqlRecord rec = workingQuery.record();
    nameUid = rec.indexOf("uid");
    int nameUri = rec.indexOf("entity_name_uri");
    int nameDescription = rec.indexOf("entity_description");
    int namePath = rec.indexOf("name_path");
    int nameStartTimeAbs = rec.indexOf("start_time_abs");
    int nameEndTimeAbs = rec.indexOf("end_time_abs");
    int nameStartTimeText = rec.indexOf("start_time_text");
    int nameEndTimeText = rec.indexOf("end_time_text");
    int nameEPSG = rec.indexOf("srid_epsg");
    int nameX = rec.indexOf("x_lon_easting");
    int nameY = rec.indexOf("y_lat_northing");
    int nameEasting = rec.indexOf("x_wgs84");
    int nameNorthing = rec.indexOf("y_wgs84");
    int nameDimWidth = rec.indexOf("dim_width");
    int nameDimLength = rec.indexOf("dim_length");
    int nameDimHeight = rec.indexOf("dim_height");
    int nameDimThickness = rec.indexOf("dim_thickness");
    int nameDimWeight = rec.indexOf("dim_weight");
    int nameDimWeightUnits = rec.indexOf("dim_units_weight");
    int nameDimDiameter = rec.indexOf("dim_diameter");
    int nameDimUnits = rec.indexOf("dim_units");
    int nameDimDegrees = rec.indexOf("dim_degrees");

    while(workingQuery.next())
    {
        // fill out tab view 'Dimensions'
        // read values from database
        QString length = workingQuery.value(nameDimLength).toString();
        QString width = workingQuery.value(nameDimWidth).toString();
        QString height = workingQuery.value(nameDimHeight).toString();
        QString thickness = workingQuery.value(nameDimThickness).toString();
        QString weight = workingQuery.value((nameDimWeight)).toString();
        QString WeightUnits = workingQuery.value(nameDimWeightUnits).toString();
        QString diameter = workingQuery.value(nameDimDiameter).toString();
        QString degrees = workingQuery.value(nameDimDegrees).toString();
        QString units = workingQuery.value(nameDimUnits).toString();

        // write values to view
        ui->lineEdit_Length->setText(length);
        ui->lineEdit_Width->setText(width);
        ui->lineEdit_Height->setText(height);
        ui->lineEdit_Thickness->setText(thickness);
        ui->lineEdit_Weight->setText(weight);
        ui->lineEdit_Diameter->setText(diameter);
        ui->lineEdit_Degrees->setText(degrees);

        // write units to view and labels
        ui->comboBox_Length_Unit->setItemText(0, units);
        ui->comboBox_Weight_Unit->setItemText(0, WeightUnits);
        ui->label_Unit_Diameter->setText(units);
        ui->label_Unit_Height->setText(units);
        ui->label_Unit_Thickness->setText(units);
        ui->label_Unit_Width->setText(units);

        // read WGS84 coordinates an call map view
        qreal dbl_x_lon_easting = workingQuery.value(nameEasting).toDouble();
        qreal dbl_y_lat_northing = workingQuery.value(nameNorthing).toDouble();
        ui->MarbleWidget->centerOn(dbl_x_lon_easting, dbl_y_lat_northing);
        ui->MarbleWidget->setShowOverviewMap(false);

        // fill out details for item
        ui->textEdit_Name->setText(workingQuery.value(nameUri).toString());
        ui->textEdit_Description->setText(workingQuery.value(nameDescription).toString());
        QString strTypeTmp = workingQuery.value(namePath).toString();
        QString strTypeLong = strTypeTmp.section('>',2);
        int strTypeSize = strTypeLong.size();
        int strTypeSizeRight = strTypeSize - 1; // to delete the space at the beginning of 'namePathTmp2'
        QString strType = strTypeLong.right(strTypeSizeRight);

        ui->textEdit_Type->setText(strType);

        // TODO: read path of main Window to QString image (@Stefan: implement in database view!)

        // TODO: create chronological periods string

        // create absolut period string for item
        QString startTime = workingQuery.value(nameStartTimeAbs).toString();
        QString endTime = workingQuery.value(nameEndTimeAbs).toString();
        QString startTimeText = workingQuery.value(nameStartTimeText).toString();
        QString endTimeText = workingQuery.value(nameEndTimeText).toString();

        // fill out groupbox 'Temporal/Cultural'
        QString strStartTimeText;
        QString strEndTimeText;

        if(startTimeText == "")
        {
            strStartTimeText = "";
        }
        else
        {
            strStartTimeText = startTimeText + " ";
        }
        if(endTimeText == "")
        {
            strEndTimeText = "";
        }
        else
        {
            strEndTimeText = endTimeText + " ";
        }

        QString strStartTime = strStartTimeText + startTime + " ";
        QString strEndTime = endTimeText + " " + endTime;
        ui->lineEdit_StartTime_Prefix->setText(startTimeText);
        ui->lineEdit_EndTime_Prefix->setText(endTimeText);
        ui->lineEdit_Cultural_StartTimeAbsolut->setText(startTime);
        ui->lineEdit_Cultural_EndTimeAbsolut->setText(endTime);

        if((startTime == "" && endTime == "") || (startTime == "0" && endTime == "0"))
        {
            absPeriod = "";
        }
        else
        {
            absPeriod = "<b>Absolut Period:</b><br />" + strStartTime + "to " + strEndTime + "<br />" + overviewSeperator;
        }

        // get epsg -code and coordinates from database
        epsgCode = "EPSG:" + workingQuery.value(nameEPSG).toString();

        QString strEPSGCode = workingQuery.value(nameEPSG).toString();
        QString EPSGQueryString = "SELECT spatial_ref_sys.srText FROM public.spatial_ref_sys WHERE spatial_ref_sys.srid = " + strEPSGCode + ";";
        QSqlQuery EPSGQuery(EPSGQueryString, myDBM->workingDB);
        while(EPSGQuery.next())
        {
            QString ProjCSTmp = EPSGQuery.value(0).toString();
            QString delimiterPattern("\"");
            QStringList ProjCSList = ProjCSTmp.split(delimiterPattern);
            ProjCS = ProjCSList[1];
        }

        x_lon_easting = workingQuery.value(nameX).toString();
        y_lat_northing = workingQuery.value(nameY).toString();

        if(workingQuery.value(25).toString() == "" || workingQuery.value(25).toString() == "0")
        {
            geogPosition = "";
        }
        else
        {
            geogPosition = "<b>Geographic Position:</b><br />Easting " + x_lon_easting + ", Northing " + y_lat_northing + " (" + epsgCode + " - " + ProjCS + ")"  + "<br />" +  overviewSeperator;

            // fill out tab Spatial Info
            ui->lineEdit_SpatialInfo_Easting->setText(x_lon_easting);
            ui->lineEdit_SpatialInfo_Northing->setText(y_lat_northing);
            ui->lineEdit_SpatialInfo_EPSG->setText(epsgCode);
            ui->lineEdit_SpatialInfo_CS->setText(ProjCS);
        }
    }

    // create location query string
    QString strlocationQuery = "SELECT name_path, name, type_name, links_annotation FROM openatlas.links_places WHERE openatlas.links_places.links_entity_uid_from = " + clickedUid + ";";

    QSqlQuery locationQuery(strlocationQuery, myDBM->workingDB);
    QSqlRecord reclocationQuery = locationQuery.record();

    // get indices of columns
    int locationName = reclocationQuery.indexOf("name");
    int locationType = reclocationQuery.indexOf("type_name");

    while(locationQuery.next())
    {
        // build string for overview window
        QString nameFieldname = locationQuery.value(locationName).toString();
        QString strLocationType = locationQuery.value(locationType).toString();
        location = "<b>Location:</b><br />" + nameFieldname + " (" + strLocationType + ")<br />" + overviewSeperator;
    }

    tableViewLocationModel->setQuery(strlocationQuery, myDBM->workingDB);
    proxymodelLocation->setSourceModel(tableViewLocationModel);

    ui->tableViewLocation->setModel(proxymodelLocation);
    ui->tableViewLocation->resizeRowsToContents();
    ui->tableViewLocation->resizeColumnsToContents();
    ui->tableViewLocation->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableViewLocation->hideColumn(0);
    ui->tableViewLocation->setSelectionBehavior(QAbstractItemView::SelectRows);

    // set column titles
    ui->tableViewLocation->model()->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    ui->tableViewLocation->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    ui->tableViewLocation->model()->setHeaderData(3, Qt::Horizontal, QObject::tr("Parcel Number"));

    // create cultural query string
    QString strCulturalQuery = "SELECT links_cultural.name_path, links_cultural.name, links_cultural.links_uid FROM openatlas.links_cultural WHERE links_entity_uid_from = " + clickedUid + "ORDER BY links_cultural.name ASC;";
    QList<QString> culturalClassificationList;

    QSqlQuery culturalQuery(strCulturalQuery, myDBM->workingDB);
    QSqlRecord recculturalQuery = culturalQuery.record();

    int culturalName = recculturalQuery.indexOf("name");

    while(culturalQuery.next())
    {
        QString name = culturalQuery.value(culturalName).toString();
        culturalClassificationList.append(name);
    }

    QString culturalClassificationtmp = "";

    foreach(culturalClassification, culturalClassificationList)
    {
        culturalClassificationtmp = culturalClassificationtmp + culturalClassification + "<br />";
    }

    if(culturalClassificationList.isEmpty())
    {
        culturalClassificationString = "";
    }
    else
    {
        culturalClassificationString = "<b>Cultural Classification:</b><br />" + culturalClassificationtmp + overviewSeperator;
    }

    // fill out tabView 'Classification' -> group box 'cultural classification'
    tableViewCulturalModel->setQuery(strCulturalQuery, myDBM->workingDB);
    proxymodelCultural->setSourceModel(tableViewCulturalModel);

    ui->tableViewCultural->setModel(proxymodelCultural);
    ui->tableViewCultural->resizeColumnsToContents();
    ui->tableViewCultural->resizeRowsToContents();
    ui->tableViewCultural->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableViewCultural->hideColumn(0);
    ui->tableViewCultural->hideColumn(2);

    // create chronological query string
    QString strChronologicalQuery = "SELECT links_chronological.name_path, links_chronological.name, links_chronological.links_uid FROM openatlas.links_chronological WHERE links_entity_uid_from = " + clickedUid + ";";
    QList<QString> chronologicalClassificationList;

    QSqlQuery chronologicalQuery(strChronologicalQuery, myDBM->workingDB);
    QSqlRecord recchronologicalQuery = chronologicalQuery.record();

    int chronologicalName = recchronologicalQuery.indexOf("name");

    while(chronologicalQuery.next())
    {
        QString name = chronologicalQuery.value(chronologicalName).toString();
        chronologicalClassificationList.append(name);
    }

    QString chronologicalClassificationtmp = "";

    foreach(chronologicalClassification, chronologicalClassificationList)
    {
        chronologicalClassificationtmp = chronologicalClassificationtmp + chronologicalClassification + "<br />";
    }

    if(chronologicalClassificationList.isEmpty())
    {
        chronologicalClassificationString = "";
    }
    else
    {
        chronologicalClassificationString = "<b>Chronological Classification:</b><br />" + chronologicalClassificationtmp + overviewSeperator;
    }

    // fill out tabView 'Classification' -> group box 'chronological classification'
    tableViewChronologicalModel->setQuery(strChronologicalQuery, myDBM->workingDB);
    proxymodelChronological->setSourceModel(tableViewChronologicalModel);

    ui->tableViewChronological->setModel(proxymodelChronological);
    ui->tableViewChronological->resizeColumnsToContents();
    ui->tableViewChronological->resizeRowsToContents();
    ui->tableViewChronological->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableViewChronological->hideColumn(0);
    ui->tableViewChronological->hideColumn(2);

    // fill out tabView 'Bibliography'
    QString strBibliographyQuery = "SELECT links_bibliography.entity_name_uri, links_bibliography.entity_description FROM openatlas.links_bibliography WHERE links_entity_uid_from = " + clickedUid + ";";
    tableViewBibliographyModel->setQuery(strBibliographyQuery, myDBM->workingDB);
    proxymodelBibliography->setSourceModel(tableViewBibliographyModel);

    ui->tableViewBibliography->setModel(proxymodelBibliography);
    ui->tableViewBibliography->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableViewBibliography->hideColumn(1);

    // fill out evidences
    QString strEvidenceQuery = "SELECT links_evidence.name_path, links_evidence.name FROM openatlas.links_evidence WHERE links_entity_uid_from = " + clickedUid + ";";

    tableViewEvidenceModel->setQuery(strEvidenceQuery, myDBM->workingDB);
    proxymodelEvidence->setSourceModel(tableViewEvidenceModel);

    ui->tableViewEvidence->setModel(proxymodelEvidence);
    ui->tableViewEvidence->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableViewEvidence->hideColumn(0);

    // merge strings and write to 'textEdit_Overview'
    QString overviewText = chronologicalClassificationString + culturalClassificationString + chronPeriod + absPeriod + location + geogPosition;
    ui->textEdit_Overview->setText(overviewText);

    // read path of mainImage from database
    QString strimageQuery = "SELECT links_images.entity_id FROM openatlas.links_images WHERE links_images.links_entity_uid_from = " + clickedUid + ";";
    QSqlQuery imageQuery(strimageQuery, myDBM->workingDB);

    QString partPath = imageQuery.value(0).toString();

    QString imagePath = "";
    while(imageQuery.next())
    {
        partPath = imageQuery.value(0).toString();
        //imagePath = "http://nasorium.synology.me/Funde_bmp/" + partPath + ".png";
        imagePath = imagePrefix + partPath + imagePostfix;

        imageName = partPath + imagePostfix;
        //ui->groupBox_Image_Overview->setTitle(imageName);

        ui->listView_Thumbnails->setModel(itemModel);
        ui->listView_Thumbnails->setSelectionMode(QAbstractItemView::SingleSelection);
        QStandardItem *item = new QStandardItem;

        if(ImageView==true)
        {
            analyseMainImagePath(imagePath);
        }
    }
}

/******************************************************************************
 * on_tableViewItems_doubleClicked(const QModelIndex &index): Private Slot
 * change tableViewItems to next lower level
 * Last edit: 09.05.2014
 *****************************************************************************/
void MainWindow::
on_tableViewItems_doubleClicked(const QModelIndex &index)
{
    ui->toolButton_Datalevel_up->setEnabled(true);
    ui->toolButton_Datalevel_home->setEnabled(true);

    //set datalevel
    if(dataLevel < 3)
    {
         dataLevel++;

        qDebug() << "Datalevel: " << dataLevel;
        qDebug() << "Last clicked Site: " << last_clickedSite << " Uid: " << last_clickedSiteUid;
        qDebug() << "Last clicked Feature: " << last_clickedFeature << " Uid: " << last_clickedFeatureUid;
        qDebug() << "Last clicked Stratigraphical Unit: " << last_clickedSU << " Uid: " << last_clickedSUUid;

        // clear filter
        ui->lineEdit_Filter->clear();
        filter.clear();
        on_lineEdit_Filter_textEdited();

        QVariant varClickedItem = index.data();
        clickedItem = varClickedItem.toString();
        itemIndex = ui->tableViewItems->selectionModel()->currentIndex().row();
        QString itemIndexString;
        itemIndexString.append(QString("%1").arg(itemIndex));

        //get uid
        stringUids = extractStringsFromModel(ui->tableViewItems->model(), QModelIndex(), 0);
        clickedUid = stringUids.at(itemIndex);

        switch(dataLevel)
        {
        case 1:
            // show features in site and set string for datalevel text
            last_clickedSite = clickedItem;
            strdataLevel = "Site '" + last_clickedSite + "' -> Features";
            //groupBoxTitle = "Details for site '" + last_clickedSite +"'";
            ui->textEdit_DataLevel->setText(strdataLevel);
    //        ui->groupBox_Details->setTitle(groupBoxTitle);
            show_features_in_site();
            break;

        case 2:
            // show stratigraphical units in feature and set string for datalevel text
            last_clickedFeature = clickedItem;
            strdataLevel = "Site '" + last_clickedSite + " -> Feature '" + last_clickedFeature + "' -> Stratigraphical Units";
            //groupBoxTitle = "Details for site '" + last_clickedSite +"' -> Feature '" + last_clickedFeature + "'";
    //        ui->groupBox_Details->setTitle(groupBoxTitle);
            ui->textEdit_DataLevel->setText(strdataLevel);
            show_su_in_feature();
            break;

        case 3:
            // show finds in stratigraphical unit and set string for datalevel text
            last_clickedSU = clickedItem;
            strdataLevel = "Site '" + last_clickedSite + " -> Feature '" + last_clickedFeature + "' -> Stratigraphical Unit '" + last_clickedSU + "' -> Finds";
            //groupBoxTitle = "Details for site '" + last_clickedSite +"' -> Feature '" + last_clickedFeature + "'";
            ui->textEdit_DataLevel->setText(strdataLevel);
            show_finds_in_su();
        }
    }
}

/******************************************************************************
 * on_tableViewLocation_clicked(): Private Slot
 * show location path after clicking in table location name
 * Last edit: 30.06.2014
 *****************************************************************************/
void MainWindow::on_tableViewLocation_clicked()
{
    int index = ui->tableViewLocation->selectionModel()->currentIndex().row();
    QStringList stringList = extractStringsFromModel(ui->tableViewLocation->model(), QModelIndex(), 0);
    QString string = stringList.at(index);

    QString StringLong = string.section('>', 1, -2);
    int intStringLong = StringLong.size();
    int intStringSizeRight = intStringLong - 1; // to delete the space at the beginning
    QString strLocation = StringLong.right(intStringSizeRight);

    ui->textEdit_SpatialInfo_LocatedIn->setText(strLocation);
}

/******************************************************************************
 * on_tableViewCultural_clicked(): Private Slot
 * show cultural path after clicking in table Cultural Classification
 * Last edit: 30.06.2014
 *****************************************************************************/
void MainWindow::on_tableViewCultural_clicked()
{
    int index = ui->tableViewCultural->selectionModel()->currentIndex().row();
    QStringList stringList = extractStringsFromModel(ui->tableViewCultural->model(), QModelIndex(), 0);
    QString string = stringList.at(index);
    ui->textEdit_Cultural->setText(string);
}

/******************************************************************************
 * on_tableViewChronological_clicked(): Private Slot
 * show chronological path after clicking in table Chronological Classification
 * Last edit: 30.06.2014
 *****************************************************************************/
void MainWindow::on_tableViewChronological_clicked()
{
    int index = ui->tableViewChronological->selectionModel()->currentIndex().row();
    QStringList stringList = extractStringsFromModel(ui->tableViewChronological->model(), QModelIndex(), 0);
    QString string = stringList.at(index);
    ui->textEdit_Chronological->setText(string);
}

/******************************************************************************
 * on_tableViewBibliography_clicked(): Private Slot
 * show long citation after clicking in table Bibliography
 * Last edit: 30.06.2014
 *****************************************************************************/
void MainWindow::on_tableViewBibliography_clicked()
{
    int index = ui->tableViewBibliography->selectionModel()->currentIndex().row();
    QStringList stringList = extractStringsFromModel(ui->tableViewBibliography->model(), QModelIndex(), 1);
    QString string = stringList.at(index);
    ui->textEdit_Bibliography->setText(string);
}

/******************************************************************************
 * on_tableViewEvidence_clicked(): Private Slot
 * show evidence path after clicking in table Evidence
 * Last edit: 30.06.2014
 *****************************************************************************/
void MainWindow::on_tableViewEvidence_clicked()
{
    int index = ui->tableViewEvidence->selectionModel()->currentIndex().row();
    QStringList stringList = extractStringsFromModel(ui->tableViewEvidence->model(), QModelIndex(), 0);
    QString string = stringList.at(index);
    ui->textEdit_Evidence->setText(string);
}

/******************************************************************************
 * on_actionAbout_Qt_clicked(): Private Slot
 * show message box 'About Qt'
 * Last edit: 01.07.2014
 *****************************************************************************/
void MainWindow::on_actionAbout_Qt_clicked()
{
    QMessageBox::aboutQt(this, "About Qt");
}

/******************************************************************************
 * on_actionAbout_openAtlas_clicked(): Private Slot
 * show message box 'About openAtlas'
 * Last edit: 01.07.2014
 *****************************************************************************/
void MainWindow::on_actionAbout_openAtlas_clicked()
{
    QString aboutopenAtlasString = "openATLAS is a database application for the work with "
    "archaeological, historical and spatial data.<br><br>"

    "The developement is currently (2014) at an early stage and carried "
    "out by a small team from the University of Vienna,"
    "Institute of Prehistoric and Historical Archaeology.<br><br>"

    "openATLAS is free software: you can redistribute "
    "it and/or modify it under the terms of the GNU General Public Licenses "
    "as published by the Free Software Foundation, either version 3 "
    "of the License, or any later version.<br><br>"

    "openATLAS is distributed in the hope that it will "
    "be useful, but WITHOUT ANY WARRANTY; without even "
    "the implied warranty of MERCHANTABILITY or FITNESS "
    "FOR A PARTICULAR PURPOSE. See the "
    "<a href=\"http://www.gnu.org/licenses/gpl.html\">GNU General Public License</a>"
    " for more details.<br><br>"

    "Database Design by Stefan Eichert 2013 - 2014<br>"
    "<a href=\"mailto:stefan.eichert@univie.ac.at\">stefan.eichert@univie.ac.at</a><br><br>"
    "Frontend Development by Viktor Jansa 2013 - 2014\n"
    "<a href=\"mailto:viktor.jansa@archpro.lbg.ac.at\">viktor.jansa@archpro.lbg.ac.at</a><br><br>"

    "For more information and technical support see "
    "<a href=\"http://www.openatlas.eu\">http://www.openatlas.eu</a>.";
    QString title = "<h4>About " + openAtlasVersion + "</h4>";
    QMessageBox::about( this, "About openAtlas", title + aboutopenAtlasString);
}

/******************************************************************************
 * on_actionApplication_Preferences_clicked(): Private Slot
 * call Application Preferences <dialog
 * Last edit: 05.07.2014
 *****************************************************************************/
void MainWindow::on_actionApplication_Preferences_clicked()
{
    Preferences *dlg = new Preferences;
    dlg->exec();
}

/******************************************************************************
 * on_actionTreeEditor_clicked(): Private Slot
 * show message box 'About openAtlas'
 * Last edit: 05.07.2014
 *****************************************************************************/
void MainWindow::on_actionTreeEditor_clicked()
{
    Tree_Editor *dlg = new Tree_Editor;
    dlg->exec();
}
