#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class video_writer{
public:
  ros::NodeHandle n;
  ros::NodeHandle n_image_param;
  
  VideoCapture cap;
  int cam_num;
  bool show_video_flag;
  bool save_result_video_flag;
  
  double video_rate;
  double image_height;
  double image_width;
  
  VideoWriter result_video;
  string result_video_location;
  
  double loop_rate;
  Mat frame;
  
  video_writer():
  n_image_param("~")
  {
    if(!n_image_param.getParam("show_video_flag",show_video_flag))  show_video_flag = true;
    if(!n_image_param.getParam("save_result_video_flag",save_result_video_flag))  save_result_video_flag = true;
    if(!n_image_param.getParam("loop_rate",loop_rate))  loop_rate = 30;
    if(!n_image_param.getParam("cam_num",cam_num))  cam_num = 1;
    if(!n_image_param.getParam("result_video_location",result_video_location))  result_video_location="/home/kevin/workspace/IARC/iarc2018.7.16/src/video_writer_c/result_video/result.avi";
    
    cap = VideoCapture(cam_num);
    if(!cap.isOpened())
    {
      cout << "Error opening video stream" << endl; 
      return ; 
    } 
    image_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
    image_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
    video_rate = cap.get(CV_CAP_PROP_FPS);
    
    result_video = VideoWriter(result_video_location,CV_FOURCC('M','J','P','G'),video_rate,Size(image_width,image_height));
    if(!result_video.isOpened()){
      cout << "result_video error .. " << endl; 
      return ;
    }
  }
  
  ~video_writer(){
    cap.release();
    result_video.release();
    destroyAllWindows();
  }
  
  void run(){
    ros::Rate loop_rate_class(loop_rate);
    while(ros::ok()){
      ros::spinOnce();
      cap >> frame;
      if(frame.empty()){
	ROS_ERROR("image is empty!!!**********************");
	continue;
      }
      if(show_video_flag){
	imshow("raw video",frame);
	waitKey(1);
      }
      if(save_result_video_flag){
	result_video << frame;
      }
      
      loop_rate_class.sleep();
    }
  }
  
};

int main(int argc,char **argv){
  ros::init(argc,argv,"video_wrter_c_node");
  video_writer VR;
  VR.run();
  return 0;
}
