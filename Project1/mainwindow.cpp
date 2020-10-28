#include "mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QThread>
#include <QMessageBox>
#include <opencv2/imgproc.hpp>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->optionBox->setCurrentIndex(0);
    //ex a
    ui->selectWatermark->setGeometry(ui->thresholdTypeLabel->geometry());
    ui->selectWatermark->setVisible(false);
    ui->pictureName->setGeometry(ui->thresholdType->geometry());
    ui->pictureName->setVisible(false);
    ui->textLabel->setGeometry(660,45,101,61);
    ui->textLabel->setVisible(false);
    ui->watermarkText->setVisible(false);

    //b
    ui->colorSpaceLabel->setGeometry(270,64,91,21);
    ui->colorSpaceLabel->setVisible(false);
    ui->colorSpace->setGeometry(ui->thresholdType->geometry());
    ui->colorSpace->setVisible(false);
    ui->colorSpace->setCurrentIndex(0);

    //e
    ui->kernelWidthLabel->setGeometry(250,65,100,21);
    ui->kernelWidthLabel->setVisible(false);
    ui->kernelWidth->setGeometry(370,60,81,31);
    ui->kernelWidth->setVisible(false);
    ui->kernelHeightLabel->setGeometry(470,65,100,21);
    ui->kernelHeightLabel->setVisible(false);
    ui->kernelHeight->setGeometry(590,60,81,31);
    ui->kernelHeight->setVisible(false);


    //f
    ui->thresholdValueLabel->setVisible(false);
    ui->thresholdValue->setVisible(false);
    ui->thresholdTypeLabel->setVisible(false);
    ui->thresholdType->setVisible(false);

    //g
    ui->operatorLabel->setVisible(false);
    ui->operatorLabel->setGeometry(260,60,91,31);
    ui->operator_2->setVisible(false);
    ui->operator_2->setGeometry(360,60,131,31);
    ui->operator_2->setCurrentIndex(0);
    ui->elementLabel->setVisible(false);
    ui->elementLabel->setGeometry(540,65,81,21);
    ui->element->setVisible(false);
    ui->element->setCurrentIndex(0);
    ui->element->setGeometry(630,60,131,31);
    ui->kernelSizeLbel->setVisible(false);
    ui->kernelSizeLbel->setGeometry(820,65,91,21);
    ui->kernelSize->setVisible(false);
    ui->kernelSize->setGeometry(920,60,81,31);


    //h
    ui->gradientLabel->setVisible(false);
    ui->gradientLabel->setGeometry(260, 65,85,21);
    ui->gradient->setVisible(false);
    ui->gradient->setGeometry(340,60,101,31);
    ui->scaleLabel->setGeometry(450, 65,55,21);
    ui->scaleLabel->setVisible(false);
    ui->scale->setVisible(false);
    ui->scale->setGeometry(520,60,81,31);
    ui->deltalabel->setVisible(false);
    ui->deltalabel->setGeometry(630,65,61,21);
    ui->delta->setVisible(false);
    ui->delta->setGeometry(700,60,81,31);
    ui->kernelSizeLabel2->setGeometry(800,65,91,21);
    ui->kernelSizeLabel2->setVisible(false);
    ui->kernelSize_2->setGeometry(900,60,81,31);
    ui->kernelSize_2->setVisible(false);


    //i
    ui->lowThresholdLabel->setVisible(false);
    ui->lowThresholdLabel->setGeometry(260, 65,120,21);
    ui->lowThreshold->setVisible(false);
    ui->lowThreshold->setGeometry(380,60,101,31);
    ui->ratioLabel->setVisible(false);
    ui->ratioLabel->setGeometry(515, 65,55,21);
    ui->ratio->setVisible(false);
    ui->ratio->setGeometry(585,60,81,31);
    ui->cannyKsizeLabel->setVisible(false);
    ui->cannyKsizeLabel->setGeometry(700,65,101,21);
    ui->cannyKsize->setVisible(false);
    ui->cannyKsize->setGeometry(820,60,81,31);

    //ex 3
    ui->convLabel->setVisible(false);
    ui->convLabel->setGeometry(260, 65,120,21);
    ui->conv->setVisible(false);
    ui->conv->setGeometry(360,60,141,31);

    p.setLabel(ui->playzone);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openImage_clicked()
{
    QFileInfo fileInfo = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:",
                                                 tr("Image or Video (*.png *.jpg *.mp4 *.avi *.wmv *.mkv)"));

    QString fileName = fileInfo.fileName();
    ui->fileNameLabel->setText(fileName);
    if(empty(fileName.toStdString())) return;
    String absoluteFilePath = fileInfo.absoluteFilePath().toStdString();
    openFile(absoluteFilePath);
}

void MainWindow::openFile(String filePath){
    p.changeResource();
    p.stop = true;
    String end = filePath.substr(filePath.size()-4,filePath.size());

    if(end == ".png" || end == ".jpg"){
        Mat img = imread(filePath);
        p.setImage(img);
        p.displayImg();
    }else if(end == ".mp4"){
        VideoCapture capt;
        capt.open(filePath);
        if (capt.isOpened()){
            p.setVideo(capt);
            int totalFrames = capt.get(CAP_PROP_FRAME_COUNT);
            ui->slider->setMaximum(totalFrames);
            ui->slider->setValue(1);
            p.displayVideo();
        }else{
            QMessageBox::information(NULL,"Alert","Could not open video",QMessageBox::Yes);
        }
    }

}


void MainWindow::on_selectWatermark_clicked()
{
    QFileInfo fileInfo = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:",
                                                 tr("Image (*.png *.jpg)"));

    QString fileName = fileInfo.fileName();
    ui->pictureName->setText(fileName);
    if(empty(fileName.toStdString())) return;
    String absoluteFilePath = fileInfo.absoluteFilePath().toStdString();
    p.setWaterMarkPicPath(absoluteFilePath);
}

void MainWindow::on_stopButton_clicked()
{
    p.stop = true;
}


void MainWindow::on_slider_valueChanged(int value)
{
    p.frameIndex = value;
}

void MainWindow::on_playButton_clicked()
{
    ui->slider->setValue(1);
    p.displayVideo();
}


void MainWindow::on_thresholdValue_valueChanged(int value)
{
    p.setThresholdValue(value);
}

void MainWindow::on_thresholdType_currentIndexChanged(int index)
{
    p.setThresholdType(index);
}

void MainWindow::on_optionBox_currentIndexChanged(int index)
{
    if(index == 1){
        ui->selectWatermark->setVisible(true);
        ui->pictureName->setText("");
        ui->pictureName->setVisible(true);
        ui->watermarkText->setVisible(true);
        ui->watermarkText->setText("");
        ui->textLabel->setVisible(true);
    }else{
        ui->selectWatermark->setVisible(false);
        ui->pictureName->setVisible(false);
        ui->watermarkText->setVisible(false);
        ui->textLabel->setVisible(false);
    }



    if(index == 2){
        ui->colorSpaceLabel->setVisible(true);
        ui->colorSpace->setVisible(true);
        ui->colorSpace->setCurrentIndex(0);
    }else{
        ui->colorSpaceLabel->setVisible(false);
        ui->colorSpace->setVisible(false);
    }



    if(index == 5){
        ui->kernelWidthLabel->setVisible(true);
        ui->kernelWidth->setVisible(true);
        ui->kernelHeightLabel->setVisible(true);
        ui->kernelHeight->setVisible(true);
    }else{
        ui->kernelWidthLabel->setVisible(false);
        ui->kernelWidth->setVisible(false);
        ui->kernelHeightLabel->setVisible(false);
        ui->kernelHeight->setVisible(false);
    }

    if (index == 6){
        ui->thresholdValueLabel->setVisible(true);
        ui->thresholdValue->setValue(0);
        ui->thresholdValue->setVisible(true);

        ui->thresholdTypeLabel->setVisible(true);
        ui->thresholdType->setCurrentIndex(0);
        ui->thresholdType->setVisible(true);
    }else{
        ui->thresholdValueLabel->setVisible(false);
        ui->thresholdValue->setVisible(false);
        ui->thresholdTypeLabel->setVisible(false);
        ui->thresholdType->setVisible(false);
    }



    if(index == 7){
        ui->operatorLabel->setVisible(true);
        ui->operator_2->setVisible(true);
        ui->operator_2->setCurrentIndex(0);
        ui->elementLabel->setVisible(true);
        ui->element->setVisible(true);
        ui->element->setCurrentIndex(0);
        ui->kernelSizeLbel->setVisible(true);
        ui->kernelSize->setValue(0);
        ui->kernelSize->setVisible(true);
    }else{
        ui->operatorLabel->setVisible(false);
        ui->operator_2->setVisible(false);
        ui->elementLabel->setVisible(false);
        ui->element->setVisible(false);
        ui->kernelSizeLbel->setVisible(false);
        ui->kernelSize->setVisible(false);
    }

    if(index == 8){
        ui->scaleLabel->setVisible(true);
        ui->scale->setVisible(true);
        ui->deltalabel->setVisible(true);
        ui->delta->setVisible(true);
        ui->kernelSizeLabel2->setVisible(true);
        ui->kernelSize_2->setVisible(true);
        ui->gradientLabel->setVisible(true);
        ui->gradient->setVisible(true);
    }else{
        ui->scaleLabel->setVisible(false);
        ui->scale->setVisible(false);
        ui->deltalabel->setVisible(false);
        ui->delta->setVisible(false);
        ui->gradientLabel->setVisible(false);
        ui->gradient->setVisible(false);
        ui->kernelSizeLabel2->setVisible(false);
        ui->kernelSize_2->setVisible(false);
    }

    if(index == 9){
        ui->lowThresholdLabel->setVisible(true);
        ui->lowThreshold->setVisible(true);
        ui->ratioLabel->setVisible(true);
        ui->ratio->setVisible(true);
        ui->cannyKsizeLabel->setVisible(true);
        ui->cannyKsize->setVisible(true);
    }else{
        ui->lowThresholdLabel->setVisible(false);
        ui->lowThreshold->setVisible(false);
        ui->ratioLabel->setVisible(false);
        ui->ratio->setVisible(false);
        ui->cannyKsizeLabel->setVisible(false);
        ui->cannyKsize->setVisible(false);
    }

    if(index == 10){
        ui->convLabel->setVisible(true);
        ui->conv->setVisible(true);
    }else{
        ui->convLabel->setVisible(false);
        ui->conv->setVisible(false);
    }

    p.setOption(index);
}


void MainWindow::on_watermarkText_textChanged()
{
    if(ui->optionBox->currentIndex() == 1)
        p.setWaterMarkText(ui->watermarkText->toPlainText().toStdString());
}

void MainWindow::on_colorSpace_currentIndexChanged(int index)
{
    p.setColorSpace(index);
}


void MainWindow::on_kernelWidth_valueChanged(int arg1)
{
    p.setKernelWidth(arg1);
}

void MainWindow::on_kernelHeight_valueChanged(int arg1)
{
    p.setKernelHeight(arg1);
}

void MainWindow::on_operator_2_currentIndexChanged(int index)
{
    p.setMorphOperator(index);
}

void MainWindow::on_element_currentIndexChanged(int index)
{
    p.setMorhElem(index);
}

void MainWindow::on_kernelSize_valueChanged(int arg1)
{
    if(ui->optionBox->currentIndex() == 7)
        p.setMorphSize(arg1);

}

void MainWindow::on_scale_valueChanged(int arg1)
{
    p.setScale(arg1);
}

void MainWindow::on_delta_valueChanged(int arg1)
{
    p.setDelta(arg1);
}

void MainWindow::on_kernelSize_2_valueChanged(int arg1)
{
    if(ui->optionBox->currentIndex() == 8)
            p.setKsize(arg1);
}

void MainWindow::on_gradient_currentIndexChanged(int index)
{
    p.setGradient(index);
    if(index == 1){
        ui->kernelSizeLabel2->setVisible(false);
        ui->kernelSize_2->setVisible(false);
    }else{
        ui->kernelSizeLabel2->setVisible(true);
        ui->kernelSize_2->setVisible(true);
    }
}

void MainWindow::on_lowThreshold_valueChanged(int arg1)
{
    p.setLowThreshold(arg1);
}

void MainWindow::on_ratio_valueChanged(int arg1)
{
    p.setRatio(arg1);
}

void MainWindow::on_cannyKsize_valueChanged(int arg1)
{
    p.setCannyKsize(arg1);
}

void MainWindow::on_conv_currentIndexChanged(int index)
{
    p.setConversion(index);
}
