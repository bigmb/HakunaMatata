#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include <opencv/cv.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/PolygonMesh.h>


class functions
{
public:
    functions();

    ~functions();

    void rgb2gray(cv::Mat &rgbImg, cv::Mat &grayImg);

    void flipImage(cv::Mat &rgbImg);

    void featureExtraction(cv::Mat &rgbImg, std::vector<cv::KeyPoint> &keyPts);


    void showKeyPoints(cv::Mat &rgbImg, std::vector<cv::KeyPoint> &keyPts);


    void featureMatching(cv::Mat &rgbImg01, std::vector<cv::KeyPoint> &keyPts01,
                         cv::Mat &rgbImg02, std::vector<cv::KeyPoint> &keyPts02,
                         std::vector< cv::DMatch > *matches, bool robustMatch = false);

    void showFeatureMatches(cv::Mat &rgb_1, std::vector<cv::KeyPoint> &keyPts_1,
                            cv::Mat &rgb_2, std::vector<cv::KeyPoint> &keyPts_2,
                            std::vector< cv::DMatch > *matches);


};




#endif // FUNCTIONS_H
