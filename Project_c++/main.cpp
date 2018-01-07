#include "kinectgrabber.h"
#include "pclviewer.h"
#include "functions.h"

#define sizemat 10
#include <QApplication>
#include <QMainWindow>
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/video/video.hpp>
#include <opencv/highgui.h>
#include <OpenNI.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include "opencv2/imgproc/imgproc.hpp"

void capture() {
    int i = 0;
    std::string folder = "malav1";
    KinectGrabber grab1;
    cv::Mat depth_img,color_img,color_mapped_img;
    int frame_delay_ = 1000;

    while(i<sizemat){
        depth_img = grab1.GetDepthFrame();
        color_img = grab1.GetColorFrame();
     //  color_mapped_img = grab1.map_color_to_depth(depth_img,color_img);
   //  cv::cvtColor(color_mapped_img,color_mapped_img, cv::COLOR_RGBA2RGB);


        imwrite("./"+folder+"/depth_"+std::to_string(i)+".png", depth_img);
        imwrite("./"+folder+"/color_"+std::to_string(i)+".png", color_img);
     //  cv::imwrite("/"+folder+"/color_mapped_"+std::to_string(i)+".png", color_mapped_img);
        cv::imshow("img",color_img);

        i++;
        cv::waitKey(frame_delay_);

    }

}



int main(int argc, char *argv[])
{

    QApplication a (argc, argv);

    std::string folder = "Malav";
    KinectGrabber grab ;
    functions func;
    PCLViewer pclout;
    cv::Mat depth_img, color_img, color_map_img, depth_map_img,color_map_img8,color_map_img7, depth_map_img2 ,depth_thresh, color_map_img2,color_map_img3,color_map_img4 , rgb[sizemat] , depthGray;
    std::vector<cv::KeyPoint> keyPts;keyPts.clear();
    int frame_delay_ = 1000;

    while(1){
        depth_img = grab.GetDepthFrame();
        color_img = grab.GetColorFrame();
        cv::imshow("ColorImg", color_img);
        cv::imshow("DepthImg", depth_img);
    //   cv::imshow("Depth_Mod", grab.ModDepthForDisplay(depth_img));

        color_map_img = grab.map_depth_to_color(depth_img,color_img);

         color_map_img.convertTo(color_map_img2,CV_8UC3);
        cv::imshow("Color_mapped", color_map_img2);

        depth_map_img = grab.map_color_to_depth(depth_img,color_img);
        cv::imshow("depth mapped", depth_map_img);

        depth_thresh = grab.threshold_depth(depth_img);
        cv::imshow("depth_thresh_img", depth_thresh);

        color_map_img3 = grab.map_depth_to_color(depth_thresh,color_img);

         color_map_img3.convertTo(color_map_img4,CV_8UC3);
        cv::imshow("Color_mapped1", color_map_img4);

        cv::Mat depthMap = cv::imread("./"+folder+"/depth_"+std::to_string(2)+".png", CV_LOAD_IMAGE_UNCHANGED);
        cv::Mat colorImg = cv::imread("./"+folder+"/color_"+std::to_string(2)+".png", CV_LOAD_IMAGE_UNCHANGED);
        cv::Mat colorImg_9 = cv::imread("./"+folder+"/color_map_"+std::to_string(3)+".png", CV_LOAD_IMAGE_UNCHANGED);
     //   cv::imshow("color1",colorImg);
         cv::Mat depthMap_2 = cv::imread("./"+folder+"/depth_"+std::to_string(3)+".png", CV_LOAD_IMAGE_UNCHANGED);
         cv::Mat colorImg_2 = cv::imread("./"+folder+"/color_"+std::to_string(3)+".png", CV_LOAD_IMAGE_UNCHANGED);
         cv::Mat colorImg_10 = cv::imread("./"+folder+"/color_map_"+std::to_string(3)+".png", CV_LOAD_IMAGE_UNCHANGED);
      //   cv::imshow("color2",colorImg_2);
       cv::Mat  depth_thresh_inp2 =grab.threshold_depth(depthMap_2);
       cv::Mat  depth_thresh_inp1 =grab.threshold_depth(depthMap);

       cv::Mat color_map_img5 = grab.map_depth_to_color(depth_thresh_inp1,colorImg);
       cv::Mat color_map_img6 = grab.map_depth_to_color(depth_thresh_inp2,colorImg_2);
       color_map_img5.convertTo(color_map_img7,CV_8UC3);
       color_map_img6.convertTo(color_map_img8,CV_8UC3);
        cv::imshow("Color_mapped5", color_map_img7);
        cv::imshow("color_mapped6",color_map_img8);

         cv::cvtColor(color_map_img7,color_map_img7, cv::COLOR_RGBA2RGB);
          cv::cvtColor(color_map_img8,color_map_img8, cv::COLOR_RGBA2RGB);

        func.featureExtraction(color_map_img7, keyPts);
        func.showKeyPoints(color_map_img7, keyPts);

        func.featureExtraction(color_map_img8, keyPts);
        func.showKeyPoints(color_map_img8, keyPts);

           std::vector<cv::KeyPoint> keyPts_1, keyPts_2; keyPts_1.clear(); keyPts_2.clear();
        std::vector< cv::DMatch > *matches = new std::vector< cv::DMatch >;
        func.featureMatching(color_map_img7, keyPts_1, color_map_img8, keyPts_2, matches);



//        cv::KeyPoint::convert(keypts_1,selPoints1,pointIndexes1);
//        cv::KeyPoint::convert(keypts_2,selPoints2,pointIndexes2);

//        // check by drawing the points
//        std::vector<cv::Point2f>::const_iterator it= selPoints1.begin();
//        while (it!=selPoints1.end()) {

//            // draw a circle at each corner location
//            cv::circle(image,*it,3,cv::Scalar(255,255,255),2);
//            ++it;
//        }

//        it= selPoints2.begin();
//        while (it!=selPoints2.end()) {

//            // draw a circle at each corner location
//            cv::circle(image2,*it,3,cv::Scalar(255,255,255),2);
//            ++it;
//        }


        if(matches->size()>0)
          {
            std::cout<<"matches number "<<matches->size()<<std::endl;
            // Show feature matches
            func.showFeatureMatches(color_map_img7, keyPts_1, color_map_img8, keyPts_2, matches);
          }

        bool robustMatch = true;

       func.featureMatching(color_map_img7, keyPts_1, color_map_img8, keyPts_2, matches, robustMatch);

        if(matches->size()>0)
          {
            std::cout<<"matches number "<<matches->size()<<std::endl;
            // Show feature matches
            func.showFeatureMatches(color_map_img7, keyPts_1, color_map_img8, keyPts_2, matches);

          }

    //   cv::Mat fundamental= cv::findFundamentalMat(cv::Mat(keypts_1), cv::Mat(keypts_2), CV_FM_8POINT);


         cv::waitKey(0);
         return 0;

         cv::waitKey(frame_delay_);

         pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud = grab.rgbd2pcl(color_map_img,depth_thresh);

         pclout.setPointCloud(cloud);
         pclout.show();

       //  capture();

        }
         return 0;
}



