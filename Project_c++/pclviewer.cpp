#include "pclviewer.h"
#include "ui_pclviewer.h"

PCLViewer::PCLViewer (QWidget *parent) :
  QMainWindow (parent),
  ui (new Ui::PCLViewer)
{
  ui->setupUi (this);

  this->setWindowTitle ("PCL_viewer");

  // Setup the cloud pointer

  cloud.reset (new PointCloudT);

  // Set up the QVTK window
  viewer.reset (new pcl::visualization::PCLVisualizer ("viewer", false));
  ui->qvtkWidget->SetRenderWindow (viewer->getRenderWindow ());
  viewer->setupInteractor (ui->qvtkWidget->GetInteractor (), ui->qvtkWidget->GetRenderWindow ());
  ui->qvtkWidget->update ();


  viewer->addPointCloud (cloud, "cloud");
  //pSliderValueChanged (0);
  viewer->resetCamera ();
  ui->qvtkWidget->update ();
}


void PCLViewer::setPointCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr n_cloud)
{

    cloud = n_cloud;

    viewer->updatePointCloud (cloud, "cloud");
    ui->qvtkWidget->update ();
}

PCLViewer::~PCLViewer ()
{
  delete ui;
}

