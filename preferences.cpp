#include "preferences.h"
#include "ui_preferences.h"

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

bool ImageViewActivated;
QString fileExtension;
QString ImagePath;
int TabIndex;
int TabDataLevelIndex;
bool TabSitesOverview;
bool TabSitesSpatial;
bool TabSitesClassification;
bool TabSitesDimensions;
bool TabSitesImages;
bool TabSitesBibliography;
bool TabFeaturesOverview;
bool TabFeaturesSpatial;
bool TabFeaturesClassification;
bool TabFeaturesDimensions;
bool TabFeaturesImages;
bool TabFeaturesBibliography;
bool TabSUsOverview;
bool TabSUsSpatial;
bool TabSUsClassification;
bool TabSUsDimensions;
bool TabSUsImages;
bool TabSUsBibliography;
bool TabFindsOverview;
bool TabFindsSpatial;
bool TabFindsClassification;
bool TabFindsDimensions;
bool TabFindsImages;
bool TabFindsBibliography;

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    QSettings settings;

    // read settings
    settings.beginGroup("Preferences");

        TabIndex = settings.value("TabIndex").toInt();
        TabDataLevelIndex = settings.value("TabDataLevelIndex").toInt();

        ImageViewActivated = settings.value("ImageViewActivated").toBool();
        ImagePath = settings.value("ImageFolderPath").toString();
        fileExtension = settings.value("ImageFileExtension").toString();

        TabSitesOverview = settings.value("TabSitesOverview").toBool();
        TabSitesSpatial = settings.value("TabSitesSpatial").toBool();
        TabSitesClassification = settings.value("TabSitesClassification").toBool();
        TabSitesDimensions = settings.value("TabSitesDimensions").toBool();
        TabSitesImages = settings.value("TabSitesImages").toBool();
        TabSitesBibliography = settings.value("TabSitesBibliography").toBool();

        TabFeaturesOverview = settings.value("TabFeaturesOverview").toBool();
        TabFeaturesSpatial = settings.value("TabFeaturesSpatial").toBool();
        TabFeaturesClassification = settings.value("TabFeaturesClassification").toBool();
        TabFeaturesDimensions = settings.value("TabFeaturesDimensions").toBool();
        TabFeaturesImages = settings.value("TabFeaturesImages").toBool();
        TabFeaturesBibliography = settings.value("TabFeaturesBibliography").toBool();

        TabSUsOverview = settings.value("TabSUsOverview").toBool();
        TabSUsSpatial = settings.value("TabSUsSpatial").toBool();
        TabSUsClassification = settings.value("TabSUsClassification").toBool();
        TabSUsDimensions = settings.value("TabSUsDimensions").toBool();
        TabSUsImages = settings.value("TabSUsImages").toBool();
        TabSUsBibliography = settings.value("TabSUsBibliography").toBool();

        TabFindsOverview = settings.value("TabFindsOverview").toBool();
        TabFindsSpatial = settings.value("TabFindsSpatial").toBool();
        TabFindsClassification = settings.value("TabFindsClassification").toBool();
        TabFindsDimensions = settings.value("TabFindsDimensions").toBool();
        TabFindsImages = settings.value("TabFindsImages").toBool();
        TabFindsBibliography = settings.value("TabFindsBibliography").toBool();

    settings.endGroup();

    ui->tabWidget->setCurrentIndex(TabIndex);
    ui->tabWidget_DataLevel->setCurrentIndex(TabDataLevelIndex);

    if(ImageViewActivated == true)
    {
        ui->checkBox_ImageView->setChecked(true);
        ui->groupBox_imagePath->setEnabled(true);
        ui->groupBox_imageExtension ->setEnabled(true);
    }
    else
    {
        ui->checkBox_ImageView->setChecked(false);
        ui->groupBox_imagePath->setEnabled(false);
        ui->groupBox_imageExtension ->setEnabled(false);
    }

    ui->label_currentImagefolderPath->setText(ImagePath);
    ui->lineEdit_ImagefolderPath->setText(ImagePath);

    // remove tab 'DataLevel until the function is implemented
    ui->tabWidget->removeTab(1);

    ui->checkBox_Sites_Overview->setChecked(TabSitesOverview);
    ui->checkBox_Sites_Spatial->setChecked(TabSitesSpatial);
    ui->checkBox_Sites_Classification->setChecked(TabSitesClassification);
    ui->checkBox_Sites_Dimensions->setChecked(TabSitesDimensions);
    ui->checkBox_Sites_Images->setChecked(TabSitesImages);
    ui->checkBox_Sites_Bibliography_Evidence->setChecked(TabSitesBibliography);

    ui->checkBox_Features_Overview->setChecked(TabFeaturesOverview);
    ui->checkBox_Features_Spatial->setChecked(TabFeaturesSpatial);
    ui->checkBox_Features_Classification->setChecked(TabFeaturesClassification);
    ui->checkBox_Features_Dimensions->setChecked(TabFeaturesDimensions);
    ui->checkBox_Features_Images->setChecked(TabFeaturesImages);
    ui->checkBox_Features_Bibliography_Evidence->setChecked(TabFeaturesBibliography);

    ui->checkBox_SUs_Overview->setChecked(TabSUsOverview);
    ui->checkBox_SUs_Spatial->setChecked(TabSUsSpatial);
    ui->checkBox_SUs_Classification->setChecked(TabSUsClassification);
    ui->checkBox_SUs_Dimensions->setChecked(TabSUsDimensions);
    ui->checkBox_SUs_Images->setChecked(TabSUsImages);
    ui->checkBox_SUs_Bibliography_Evidence->setChecked(TabSUsBibliography);

    ui->checkBox_Finds_Overview->setChecked(TabFindsOverview);
    ui->checkBox_Finds_Spatial->setChecked(TabFindsSpatial);
    ui->checkBox_Finds_Classification->setChecked(TabFindsClassification);
    ui->checkBox_Finds_Dimensions->setChecked(TabFindsDimensions);
    ui->checkBox_Finds_Images->setChecked(TabFindsImages);
    ui->checkBox_Finds_Bibliography_Evidence->setChecked(TabFindsBibliography);

    if(fileExtension == ".png") ui->radioButton_png->setChecked(true);
    if(fileExtension == ".jpg") ui->radioButton_jpg->setChecked(true);
    if(fileExtension == ".bmp") ui->radioButton_bmp->setChecked(true);
    if(fileExtension == ".tiff") ui->radioButton_tiff->setChecked(true);
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::on_pushButton_Save_clicked()
{
    //save openAtlas Preferences to standard location (OS dependend!)
    QSettings settings;
    settings.beginGroup("Preferences");
        settings.setValue("TabIndex", ui->tabWidget->currentIndex());
        settings.setValue("TabDataLevelIndex", ui->tabWidget_DataLevel->currentIndex());

        // write settings for Images
        settings.setValue("ImageViewActivated", ui->checkBox_ImageView->isChecked());
        settings.setValue("ImageFolderPath", ui->lineEdit_ImagefolderPath->text());
        settings.setValue("ImageFileExtension", fileExtension);

        // write settings for enabled tabs
        settings.setValue("TabSitesOverview", ui->checkBox_Sites_Overview->isChecked());
        settings.setValue("TabSitesSpatial", ui->checkBox_Sites_Spatial->isChecked());
        settings.setValue("TabSitesClassification", ui->checkBox_Sites_Classification->isChecked());
        settings.setValue("TabSitesDimension", ui->checkBox_Sites_Dimensions->isChecked());
        settings.setValue("TabSitesImages", ui->checkBox_Sites_Images->isChecked());
        settings.setValue("TabSitesBibliography", ui->checkBox_Sites_Bibliography_Evidence->isChecked());

        settings.setValue("TabFeaturesOverview", ui->checkBox_Features_Overview->isChecked());
        settings.setValue("TabFeaturesSpatial", ui->checkBox_Features_Spatial->isChecked());
        settings.setValue("TabFeaturesClassification", ui->checkBox_Features_Classification->isChecked());
        settings.setValue("TabFeaturesDimension", ui->checkBox_Features_Dimensions->isChecked());
        settings.setValue("TabFeaturesImages", ui->checkBox_Features_Images->isChecked());
        settings.setValue("TabFeaturesBibliography", ui->checkBox_Features_Bibliography_Evidence->isChecked());

        settings.setValue("TabSUsOverview", ui->checkBox_SUs_Overview->isChecked());
        settings.setValue("TabSUsSpatial", ui->checkBox_SUs_Spatial->isChecked());
        settings.setValue("TabSUsClassification", ui->checkBox_SUs_Classification->isChecked());
        settings.setValue("TabSUsDimension", ui->checkBox_SUs_Dimensions->isChecked());
        settings.setValue("TabSUsImages", ui->checkBox_SUs_Images->isChecked());
        settings.setValue("TabSUsBibliography", ui->checkBox_SUs_Bibliography_Evidence->isChecked());

        settings.setValue("TabFindsOverview", ui->checkBox_Finds_Overview->isChecked());
        settings.setValue("TabFindsSpatial", ui->checkBox_Finds_Spatial->isChecked());
        settings.setValue("TabFindsClassification", ui->checkBox_Finds_Classification->isChecked());
        settings.setValue("TabFindsDimension", ui->checkBox_Finds_Dimensions->isChecked());
        settings.setValue("TabFindsImages", ui->checkBox_Finds_Images->isChecked());
        settings.setValue("TabFindsBibliography", ui->checkBox_Finds_Bibliography_Evidence->isChecked());

    settings.endGroup();

    QMessageBox::information(0, QObject::tr("Preferences Info"),
                             tr("Please restart openAtlas to use the new settings!"));
    close();

}

void Preferences::on_pushButton_ImagefolderPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                            tr("Imagefolder Path"));
    if(path.isNull() == false)
    {
        ui->lineEdit_ImagefolderPath->setText(path);
    }
}


void Preferences::on_radioButton_png_clicked()
{
    if(ui->radioButton_png->isChecked())
    {
        fileExtension = ".png";
    }
}

void Preferences::on_radioButton_jpg_clicked()
{
    if(ui->radioButton_jpg->isChecked())
    {
        fileExtension = ".jpg";
    }
}

void Preferences::on_radioButton_bmp_clicked()
{
    if(ui->radioButton_bmp->isChecked())
    {
        fileExtension = ".bmp";
    }
}

void Preferences::on_radioButton_tiff_clicked()
{
    if(ui->radioButton_tiff->isChecked())
    {
        fileExtension = ".tiff";
    }
}

void Preferences::on_checkBox_ImageView_clicked(bool checked)
{
    if(checked == false)
    {
        ui->groupBox_imagePath->setEnabled(false);
        ui->groupBox_imageExtension->setEnabled(false);
    }
    else
    {
        ui->groupBox_imagePath->setEnabled(true);
        ui->groupBox_imageExtension->setEnabled(true);
    }
}
