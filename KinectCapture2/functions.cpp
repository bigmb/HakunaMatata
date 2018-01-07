#include "functions.h"
#include <opencv/highgui.h>

// Feature detection function headers
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

functions::functions()
{

}
functions::~functions(){}

void functions::rgb2gray(cv::Mat &rgbImg, cv::Mat &grayImg)
{
    cv::cvtColor(rgbImg, grayImg, cv::COLOR_RGB2GRAY);

}

// Flip image left to right

void functions::flipImage(cv::Mat &rgbImg)
{
  cv::flip(rgbImg, rgbImg, 1);
}

void functions::featureExtraction
(cv::Mat &rgbImg, std::vector<cv::KeyPoint> &keyPts)
{
  std::cout<<"Start feature detection...\n";
  // Extract SIFT features
  cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
   cv::Mat grayImg; rgb2gray(rgbImg, grayImg);
  //  cv::imshow("grey",grayImg);
  f2d->detect(grayImg,keyPts);

  std::cout<<"Number of detected key points: "<<keyPts.size()<<std::endl;

}


void functions::showKeyPoints
(cv::Mat &rgbImg, std::vector<cv::KeyPoint> &keyPts){
  //-- Draw keypoints
  cv::Mat img_keyPts;
  drawKeypoints( rgbImg, keyPts, img_keyPts, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

  //-- Show detected (drawn) keypoints
//  cv::imshow("Keypoints_"+std::to_string(1)+"img", img_keyPts );

  //cv::waitKey(0);
  std::cout<<"End feature detection...\n";
}


void functions::featureMatching(cv::Mat &rgb_1, std::vector<cv::KeyPoint> &keyPts_1,
                                 cv::Mat &rgb_2, std::vector<cv::KeyPoint> &keyPts_2,
                                 std::vector< cv::DMatch > *matches, bool robustMatch)
{
  std::cout<<"Start feature matching...\n";
  //-- Extract SIFT features

  cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
 // cv::Ptr<cv::FeatureDetector> fd1 = cv::ORB::create();
  cv::Mat gray_1; rgb2gray(rgb_1, gray_1);
  cv::Mat gray_2; rgb2gray(rgb_2, gray_2);
  f2d->detect(gray_1,keyPts_1);
  f2d->detect(gray_2,keyPts_2);

  //-- Draw keypoints
  cv::Mat img_keyPts_1;
  cv::Mat img_keyPts_2;
  drawKeypoints( rgb_1, keyPts_1, img_keyPts_1, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
  drawKeypoints( rgb_2, keyPts_2, img_keyPts_2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );

  //-- Compute descriptor
  cv::Ptr<cv::xfeatures2d::SIFT> extractor = cv::xfeatures2d::SiftDescriptorExtractor::create();
  cv::Mat descriptors_1, descriptors_2;
  extractor->compute( gray_1, keyPts_1, descriptors_1 );
  extractor->compute( gray_2, keyPts_2, descriptors_2 );

  extractor->std::cout<<"descriptors type: "<<descriptors_1.type()<<std::endl;


  cv::FlannBasedMatcher matcher;

//  if ( descriptors_1.empty() || descriptors_2.empty() )
//   {
//       matches->clear(); //set to 0
//       return;
//   }

  matcher.match( descriptors_1, descriptors_2, *matches );

  std::cout<<"Matched descriptors: "<<matches->size()<<std::endl;
    sort(matches->begin(),matches->end(),[](const cv::DMatch& a,const cv::DMatch& b){
        return a.distance<b.distance;
    });

  if(robustMatch)
    {
      std::cout<<"\n Start outlier removal...\n";
      double max_dist = 0; double min_dist = 1000;
      //-- Quick calculation of max and min distances between keypoints
       for( int i = 0; i < descriptors_1.rows; i++ )
       { double dist = (*matches)[i].distance;
         if( dist < min_dist ) min_dist = dist;
         if( dist > max_dist ) max_dist = dist;
       }
       std::cout<<"max descriptor distance = "<<max_dist<<", min descriptor disance = " <<min_dist<<std::endl;
       // Only consider matches with small distances
       std::vector< cv::DMatch > good_matches;
       for( int i = 0; i < descriptors_1.rows; i++ )
       { if( (*matches)[i].distance <= cv::max(9*min_dist, 0.02) )
         { good_matches.push_back( (*matches)[i]); }
       }

      std::nth_element(matches->begin(), matches->begin()+8, matches->end()); // end position
       matches->erase(matches->begin()+9, matches->end());

 //      matches->clear();


//       for(int i = 0; i<10; i++ )
//         {
//     //      good_matches.push_back((*matches[i]));
//          matches->push_back( good_matches[i]);
//         }
       std::cout<<"Good matches number: "<<matches->size()<<std::endl;

  std::cout<<"End feature matching...\n";
 }
}



void functions::showFeatureMatches
(cv::Mat &rgb_1, std::vector<cv::KeyPoint> &keyPts_1,
 cv::Mat &rgb_2, std::vector<cv::KeyPoint> &keyPts_2,
 std::vector< cv::DMatch > *matches)
{
  //-- Show detected (drawn) matches
  cv::Mat img_matches;



  cv::drawMatches( rgb_1, keyPts_1, rgb_2, keyPts_2,
                   *matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
                   std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Show detected matches
  imshow( "Feature Matches", img_matches );
  //cv::waitKey(0);
}



