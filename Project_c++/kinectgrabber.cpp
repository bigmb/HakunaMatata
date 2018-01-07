#include "kinectgrabber.h"

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

// Constructor destructor
KinectGrabber::KinectGrabber()
{
    HRESULT hr = GetDefaultKinectSensor(&kin_sensor_);
    if (FAILED(hr))
    {
        throw std::runtime_error("Kinect sensor could not be found!");
    }

    if (kin_sensor_)
    {
        InitDepthSource();
        InitColorSource();
        InitMapper();
    }

    if (!kin_sensor_ || FAILED(hr))
    {
        throw std::runtime_error("Kinect init failed!");
    }
}

KinectGrabber::~KinectGrabber()
{
    SafeRelease(color_frame_reader_);
    SafeRelease(depth_frame_reader_);

    // Close the Kinect Sensor
    if (kin_sensor_)
    {
        kin_sensor_->Close();
    }

    SafeRelease(kin_sensor_);
}

//Private

void KinectGrabber::InitDepthSource()
{
    IDepthFrameSource* depth_frame_source = nullptr;

    HRESULT hr = kin_sensor_->Open();

    if (SUCCEEDED(hr))
    {
        hr = kin_sensor_->get_DepthFrameSource(&depth_frame_source);
    }

    if (SUCCEEDED(hr))
    {
        hr = depth_frame_source->OpenReader(&depth_frame_reader_);
    }

    SafeRelease(depth_frame_source);
}

void KinectGrabber::InitMapper(){

     int hr = kin_sensor_->get_CoordinateMapper(&mapper);
     //depth2rgb = new ColorSpacePoint[depth_w_*depth_h_];     // Maps depth pixels to rgb pixels
     //depth2xyz = new CameraSpacePoint[depth_w_*depth_h_];    //Maps pixels to 3d coordinates

}

void KinectGrabber::InitColorSource()
{
    IColorFrameSource* color_frame_source = nullptr;

    HRESULT hr = kin_sensor_->Open();

    if (SUCCEEDED(hr))
    {
        hr = kin_sensor_->get_ColorFrameSource(&color_frame_source);
    }

    if (SUCCEEDED(hr))
    {
        hr = color_frame_source->OpenReader(&color_frame_reader_);
    }

    SafeRelease(color_frame_source);
}

IDepthFrame* KinectGrabber::WaitForDepthFrame()
{
    while (true)
    {
        IDepthFrame* depth_frame = nullptr;
        HRESULT hr = depth_frame_reader_->AcquireLatestFrame(&depth_frame);

        if (SUCCEEDED(hr))
        {
            return depth_frame;
        }

        SafeRelease(depth_frame);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

IColorFrame* KinectGrabber::WaitForColorFrame()
{
    while (true)
    {
        IColorFrame* color_frame = nullptr;
        HRESULT hr = color_frame_reader_->AcquireLatestFrame(&color_frame);

        if (SUCCEEDED(hr))
        {
            return color_frame;
        }

        SafeRelease(color_frame);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


//Public

cv::Mat KinectGrabber::GetDepthFrame(){
    IDepthFrame* depth_frame = WaitForDepthFrame();

    IFrameDescription* frame_desc = nullptr;
    UINT nBufferSize = 0;
    pBuffer = NULL;

    HRESULT hr = depth_frame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);

    cv::Mat depth_mat(depth_h_, depth_w_, CV_16UC1);

    memcpy(depth_mat.data, pBuffer, nBufferSize * sizeof(uint16_t));

    cv::Mat disp_mat = ModDepthForDisplay(depth_mat);


    SafeRelease(depth_frame);

    return depth_mat;
}

cv::Mat KinectGrabber::ModDepthForDisplay(const cv::Mat& mat)
{
    const float depth_near = 500;
    const float depth_far = 1500;

    const float alpha = 255.0f / (depth_far - depth_near);
    const float beta = -depth_near * alpha;

    cv::Mat fmat;
    mat.convertTo(fmat, CV_32F);

    for (int r = 0; r < mat.rows; ++r)
    {
        for (int c = 0; c < mat.cols; ++c)
        {
            float v = fmat.at<float>(r, c) * alpha + beta;

            if (v > 255) v = 255;
            if (v < 0)   v = 0;

            fmat.at<float>(r, c) = v;
        }
    }

    cv::Mat bmat;
    fmat.convertTo(bmat, CV_8U);

    cv::Mat cmat;
   // cv::cvtColor(bmat, cmat, CV_GRAY2BGR);
    cv::applyColorMap(cmat, cmat, cv::COLORMAP_OCEAN);

    return cmat;
}

cv::Mat KinectGrabber::GetColorFrame(){

    IColorFrame* color_frame = WaitForColorFrame();
    ColorImageFormat imageFormat = ColorImageFormat_None;
    HRESULT hr = color_frame->get_RawColorImageFormat(&imageFormat);

    cv::Mat color_mat(color_h_, color_w_, CV_8UC4);
    const int buf_size = color_h_ * color_w_ * sizeof(uint8_t) * 4;
    hr = color_frame->CopyConvertedFrameDataToArray(buf_size, color_mat.data, ColorImageFormat_Bgra);

    SafeRelease(color_frame);

    return color_mat;
}

void KinectGrabber::ShowDepthFrame()
{
    IDepthFrame* depth_frame = WaitForDepthFrame();

    IFrameDescription* frame_desc = nullptr;
    UINT nBufferSize = 0;
    UINT16 *pBuffer = NULL;

    HRESULT hr = depth_frame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);

    cv::Mat depth_mat(depth_h_, depth_w_, CV_16UC1);

    memcpy(depth_mat.data, pBuffer, nBufferSize * sizeof(uint16_t));

    cv::Mat disp_mat = ModDepthForDisplay(depth_mat);
    cv::imshow("Depth", disp_mat);

    SafeRelease(depth_frame);
}

void KinectGrabber::ShowColorFrame()
{
    IColorFrame* color_frame = WaitForColorFrame();
    ColorImageFormat imageFormat = ColorImageFormat_None;
    HRESULT hr = color_frame->get_RawColorImageFormat(&imageFormat);

    cv::Mat color_mat(color_h_, color_w_, CV_8UC4);
    const int buf_size = color_h_ * color_w_ * sizeof(uint8_t) * 4;
    hr = color_frame->CopyConvertedFrameDataToArray(buf_size, color_mat.data, ColorImageFormat_Bgra);

    cv::imshow("Color", color_mat);

    SafeRelease(color_frame);
}

cv::Mat KinectGrabber::threshold_depth(cv::Mat depth_img)
{
    float scale_factor = 1000;
    int min_dis = 0,max_dis = 1.0;
    for(int j=0; j<depth_img.cols; j++) // y
      {
        for(int i=0; i<depth_img.rows; i++) // x
          {

            unsigned short depth = depth_img.at<unsigned short>(i,j)/scale_factor;
            if(min_dis>depth || depth>max_dis){
                 depth_img.at<unsigned short>(i,j)=0;
            }

          };
    }
    return depth_img;
}
cv::Mat KinectGrabber::map_depth_to_color(cv::Mat& depth_im, cv::Mat& rgb_im){


    int colorBytesPerPixel = 4;

    // Retrieve Mapped Coordinates
    std::vector<ColorSpacePoint> colorSpacePoints(depth_w_ * depth_h_ );
    mapper->MapDepthFrameToColorSpace(depth_w_ * depth_h_, (UINT16*)depth_im.data, colorSpacePoints.size(), &colorSpacePoints[0] );

    // Mapped Color Buffer
    std::vector<BYTE> buffer( depth_w_ * depth_h_ * colorBytesPerPixel );

    // Mapping Color Data to Depth Resolution
    for( int depthY = 0; depthY < depth_h_; depthY++ ){
        for( int depthX = 0; depthX < depth_w_; depthX++ ){
            const unsigned int depthIndex = depthY * depth_w_ + depthX;
            const int colorX = static_cast<int>( colorSpacePoints[depthIndex].X + 0.5f );
            const int colorY = static_cast<int>( colorSpacePoints[depthIndex].Y + 0.5f );
            if( ( 0 <= colorX ) && ( colorX < color_w_ ) && ( 0 <= colorY ) && ( colorY < color_h_ ) ){
                const unsigned int colorIndex = colorY * color_w_ + colorX;
                buffer[depthIndex * colorBytesPerPixel + 0] = rgb_im.data[colorIndex * colorBytesPerPixel + 0];
                buffer[depthIndex * colorBytesPerPixel + 1] = rgb_im.data[colorIndex * colorBytesPerPixel + 1];
                buffer[depthIndex * colorBytesPerPixel + 2] = rgb_im.data[colorIndex * colorBytesPerPixel + 2];
                buffer[depthIndex * colorBytesPerPixel + 3] = rgb_im.data[colorIndex * colorBytesPerPixel + 3];
            }
        }
    }


    // e.g. Mapped Color Buffer to cv::Mat
    cv::Mat colorMat = cv::Mat( depth_h_, depth_w_, CV_8UC4, &buffer[0] ).clone();
    return colorMat;

}


cv::Mat KinectGrabber::map_color_to_depth(cv::Mat& depth_im, cv::Mat& rgb_im){

    // Retrieve Mapped Coordinates
    std::vector<DepthSpacePoint> depthSpacePoints( color_w_ * color_h_ );
    mapper->MapColorFrameToDepthSpace(depth_w_ * depth_h_, (UINT16*)depth_im.data, depthSpacePoints.size(), &depthSpacePoints[0] );

    // Mapped Depth Buffer
    std::vector<UINT16> buffer( color_w_ * color_h_ );

    // Mapping Depth Data to Color Resolution
    for ( int colorY = 0; colorY < color_h_; colorY++ ){
        for ( int colorX = 0; colorX < color_w_; colorX++ ){
            const unsigned int colorIndex = colorY * color_w_ + colorX;
            const int depthX = static_cast<int>( depthSpacePoints[colorIndex].X + 0.5f );
            const int depthY = static_cast<int>( depthSpacePoints[colorIndex].Y + 0.5f );
            if ( ( 0 <= depthX ) && ( depthX < depth_w_ ) && ( 0 <= depthY ) && ( depthY < depth_h_ ) ){
                const unsigned int depthIndex = depthY * depth_w_ + depthX;
                buffer[colorIndex] = depth_im.data[depthIndex];

            }

        }
    }

    // e.g. Mapped Depth Buffer to cv::Mat
    cv::Mat depthMat = cv::Mat( color_h_, color_w_, CV_16UC1, &buffer[0] ).clone();
    return depthMat;
}



void KinectGrabber::myDepth2meter(const float feat_x, const float feat_y, const float rawDisparity,
                             float &X, float &Y, float &Z)
{
  // reject invalid points
  if(rawDisparity <= 0)
    {
      X = 0; Y = 0; Z = 0; return;
    }

  float fx = 525.0; // focal length x
  float fy = 525.0; // focal length y
  float cx = 319.5; // optical center x
  float cy = 239.5; // optical center y
  float sclFactor = 1000.0;

  // Recall the camera projective projection model
  Z = rawDisparity / sclFactor;
  X = (feat_x - cx) * Z / fx;
  Y = (feat_y - cy) * Z / fy;
}

void KinectGrabber::depth2meter(const float feat_x, const float feat_y, const float rawDisparity,
                             float &X, float &Y, float &Z)
{
  float minDistance = -10;
  float scaleFactor = 0.0021;
  Z = 0.1236 * std::tan(rawDisparity / 2842.5 + 1.1863);
  X = (feat_x - 480 / 2) * (Z + minDistance) * scaleFactor;
  Y = (feat_y - 640 / 2) * (Z + minDistance) * scaleFactor;
}


pcl::PointCloud<pcl::PointXYZRGB>::Ptr KinectGrabber::rgbd2pcl(const cv::Mat &rgbImg, const cv::Mat &depthImg){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud;
    cloud.reset (new pcl::PointCloud<pcl::PointXYZRGB>);
    cloud->points.resize (rgbImg.rows*rgbImg.cols);
    int p_count = 0;
    for(int j=0; j<rgbImg.cols; j++) // y
      {
        for(int i=0; i<rgbImg.rows; i++) // x
          {
            float X, Y, Z;
            unsigned short depth = depthImg.at<unsigned short>(i,j);
            // Render the 3D values
            myDepth2meter(i,j,depth, X, Y, Z);

            // Remove features which are out of Kinect senser range
            if(X>5 || Y > 5 || Z == 0.0){continue; }
            // Write out the colored 3D point
            cloud->points[p_count].x = X;
            cloud->points[p_count].y = Y;
            cloud->points[p_count].z = Z;


            cloud->points[p_count].r = (float)rgbImg.at<cv::Vec3b>(i,j)[0];
            cloud->points[p_count].g = (float)rgbImg.at<cv::Vec3b>(i,j)[1];
            cloud->points[p_count].b = (float)rgbImg.at<cv::Vec3b>(i,j)[2];
            p_count++;

          }
      }
      return cloud;
}






