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
    ui->thread_value_label->setVisible(false);
    ui->threshold_value->setVisible(false);
    ui->threshold_type_label->setVisible(false);
    ui->threshold_type->setVisible(false);
    p.setLabel(ui->playzone);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_open_image_clicked()
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

void MainWindow::on_optionBox_currentIndexChanged(const QString &arg1)
{
    char option = arg1.toStdString()[0];
    if (option == 'f'){
        ui->thread_value_label->setVisible(true);
        ui->threshold_value->setValue(0);
        ui->threshold_value->setVisible(true);

        ui->threshold_type->setCurrentIndex(0);
        ui->threshold_type_label->setVisible(true);
        ui->threshold_type->setVisible(true);
    }else{
        ui->thread_value_label->setVisible(false);
        ui->threshold_value->setVisible(false);

        ui->threshold_type_label->setVisible(false);
        ui->threshold_type->setVisible(false);
    }
    p.setOption(option);
}

void MainWindow::on_stopButton_clicked()
{
    p.stop = true;
}


void MainWindow::on_slider_valueChanged(int value)
{
    p.setFrame(value);
}

void MainWindow::on_playButton_clicked()
{
    ui->slider->setValue(1);
    p.displayVideo();
}


void MainWindow::on_threshold_value_valueChanged(int value)
{
    p.setThresholdValue(value);
}

void MainWindow::on_threshold_type_currentIndexChanged(int index)
{
    p.setThresholdType(index);
}
