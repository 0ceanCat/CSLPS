#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QMainWindow>
#include "Player.h"
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_optionBox_currentIndexChanged(int index);

    void openFile(String filePath);

    void on_stopButton_clicked();

    void on_playButton_clicked();

    void on_openImage_clicked();

    void on_selectWatermark_clicked();

    void on_watermarkText_textChanged();

    void on_thresholdType_currentIndexChanged(int index);

    void on_thresholdValue_valueChanged(int arg1);

    void on_slider_valueChanged(int value);

    void on_colorSpace_currentIndexChanged(int index);

    void on_kernelWidth_valueChanged(int arg1);

    void on_kernelHeight_valueChanged(int arg1);

    void on_operator_2_currentIndexChanged(int index);

    void on_element_currentIndexChanged(int index);

    void on_kernelSize_valueChanged(int arg1);

    void on_scale_valueChanged(int arg1);

    void on_delta_valueChanged(int arg1);

    void on_kernelSize_2_valueChanged(int arg1);

    void on_gradient_currentIndexChanged(int index);

    void on_lowThreshold_valueChanged(int arg1);

    void on_ratio_valueChanged(int arg1);

    void on_cannyKsize_valueChanged(int arg1);

    void on_conv_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    Player p;

};
#endif // MAINWINDOW_H
