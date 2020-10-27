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
    void on_open_image_clicked();

    void on_optionBox_currentIndexChanged(const QString &arg1);

    void openFile(String filePath);

    void on_stopButton_clicked();

    void on_playButton_clicked();

    void on_slider_valueChanged(int value);

    void on_threshold_value_valueChanged(int value);

    void on_threshold_type_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    Player p;

};
#endif // MAINWINDOW_H
