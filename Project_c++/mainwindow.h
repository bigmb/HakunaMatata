#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "pop_up_window.h"

#include "functions.h"
#include "kinectgrabber.h"
#include "pclviewer.h"
#include "pop_up_window.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <OpenNI.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <pcl/visualization/pcl_visualizer.h>
#include <vtkRenderWindow.h>
#include <pcl/io/vtk_lib_io.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
//#include <cvimagewidget.h>
#include <boost/make_shared.hpp>

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QListWidgetItem>



#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
   // void on_Register_button_clicked();

  //  void on_Mesh_button_clicked();

   // void on_Start_Kinect_clicked();

private:
    Ui::MainWindow *ui;
    Pop_up_window *pop_register;
    Pop_up_window *pop_mesh;
};

#endif // MAINWINDOW_H
