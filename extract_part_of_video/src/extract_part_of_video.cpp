#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

class extractPartOfVideo{
public:
  ros::NodeHandle nh;
  ros::NodeHandle nh_param;
  double fps,width,height;
  double beginTime,endTime;
  string sourceVideoLocation,resultVideoLocation;
  
  bool ifshowSourceVideo,ifSaveVideo;
  VideoCapture cap;
  VideoWriter writer;
  
  Mat frame;
  int count;
  
  extractPartOfVideo(ros::NodeHandle n);
  ~extractPartOfVideo();
  void run();
};
extractPartOfVideo::extractPartOfVideo(ros::NodeHandle n):
nh(n),
nh_param("~")
{
  if(!nh_param.getParam("fps",fps))  fps = 30;
  if(!nh_param.getParam("width",width))  width = 640;
  if(!nh_param.getParam("height",height)) height = 480;
  if(!nh_param.getParam("beginTime",beginTime)) beginTime = 0;
  cout<<"beginTime: "<<to_string(beginTime)<<endl;
  if(!nh_param.getParam("endTime",endTime))  endTime = 5;
  cout<<"endTime"<<to_string(endTime)<<endl;
  if(!nh_param.getParam("sourceVideoLocation",sourceVideoLocation)) sourceVideoLocation = "/home/kevin/workspace/IARC/video/bottom_view_0826.avi";
  if(!nh_param.getParam("resultVideoLocation",resultVideoLocation)) resultVideoLocation = "/home/kevin/workspace/IARC/video/bottom_view_0826_short.avi";
  if(!nh_param.getParam("ifshowSourceVideo" , ifshowSourceVideo))  ifshowSourceVideo = true;
  if(!nh_param.getParam("ifSaveVideo",ifSaveVideo)) ifSaveVideo = true;
  
  cap = VideoCapture(sourceVideoLocation);
  if(!cap.isOpened()){
    cout<<"video open fail!!!!!"<<endl;
    return ;
  }
  fps = cap.get(CV_CAP_PROP_FPS);
  width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  writer = VideoWriter(resultVideoLocation ,CV_FOURCC('M', 'J', 'P', 'G'), fps, Size(width, height) );
  if(!writer.isOpened()){
    cout<<"write video fail"<<endl;
    return ;
  }
}
extractPartOfVideo::~extractPartOfVideo()
{
  destroyAllWindows();
}
void extractPartOfVideo::run()
{
  count = 0;
  ros::Rate loop_rate_class(fps);
  while(ros::ok()){
    cap >> frame;
    if(frame.empty()){
      cout<<"frame is empty"<<endl;
      continue;
    }
    if(ifshowSourceVideo){
      namedWindow("sourceVideo");
      imshow("sourceVideo",frame);
      waitKey(1);
    }
    if(count>(beginTime * fps)){
      if(ifSaveVideo){
        //cout << "yes" << endl;
	       writer << frame;
      }
      if(count > (endTime * fps)) return ;
    }
    count++;
    ros::spinOnce();
    loop_rate_class.sleep();
  }
}



int main(int argc,char **argv){
  ros::init(argc,argv,"extract_part_of_video_node");
  ros::NodeHandle n;
  extractPartOfVideo Extract(n);
  Extract.run();
  return 0;
}