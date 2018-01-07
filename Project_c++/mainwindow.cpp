#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pop_up_window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vtkObject::GlobalWarningDisplayOff();

    KinectGrabber = new KinectGrabber();
    //QObject::connect(KinectGrabber, SIGNAL(frameReady(QImage)), this, SLOT(renderFrame(QImage)));
   // QObject::connect(KinectGrabber, SIGNAL(depthAndColorsReady(std::vector<cv::Mat>, std::vector<cv::Mat>)),
     //                this, SLOT(saveDepthAndColorMat(std::vector<cv::Mat>, std::vector<cv::Mat>)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Register_button_clicked()
{
    pop_register = new Pop_up_window(this);
    pop_register->show();
}

void MainWindow::on_Mesh_button_clicked()
{
    pop_mesh = new Pop_up_window(this);
    pop_mesh->show();
}

void MainWindow::on_Start_Kinect_clicked()
{
    KinectGrabber->GetColorFrame(ui->window->size());

}
