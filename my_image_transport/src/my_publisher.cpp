#include <ros/ros.h>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>

#include <image_transport/image_transport.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
class my_video_transport{
public:
  std::string advertise_topic_name;
  std::string video_location;
  bool show_video;
  int video_rate = 0;
  
  
  ros::NodeHandle nh_param;
  cv::Mat frame;
  sensor_msgs::ImagePtr msg;
  
  cv::VideoCapture cap;
  
  image_transport::Publisher pub;
  
  my_video_transport():
  nh_param("~"){
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    if(!nh_param.getParam("advertise_topic_name",advertise_topic_name))  advertise_topic_name = "/fisheye/image";
    if(!nh_param.getParam("video_location",video_location))  video_location = "/home/kevin/workspace/KCF/KCF_wang-master/KCFsourcecode/sourceVideo/b.avi";
    if(!nh_param.getParam("show_video",show_video)) show_video = true;
    
    pub = it.advertise(advertise_topic_name, 1);
    
    cap = cv::VideoCapture(video_location);
    video_rate = cap.get(CV_CAP_PROP_FPS);
    if(video_rate == 0 ){
      if(!nh_param.getParam("loop_rate_num",video_rate))  video_rate=30;
    }
      
    if(!cap.isOpened())
    {
      std::cout<<"video not open."<<std::endl;
      return ;
    }
  }
  
  ~my_video_transport(){
    cap.release();
    cv::destroyAllWindows();
  }
  
  void run(){
    ros::Rate loop_rate_class(video_rate);
    while (ros::ok()) {
      cap >>frame;
      if(frame.empty()){
	std::cout<<"empty"<<std::endl;
	continue;
      }
      if(show_video){
	cv::imshow("sourceVideo",frame);
	cv::waitKey(1);
      }
      msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
      pub.publish(msg);
      cv::waitKey(1);
      ros::spinOnce();
      loop_rate_class.sleep();
    }
    return ;
  }
  
};
int main(int argc, char** argv)
{
  ros::init(argc, argv, "my_image_transport_node");
  my_video_transport Tran;
  Tran.run();
  return 0;
  
}
