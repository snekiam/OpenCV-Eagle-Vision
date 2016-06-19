#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

double normalizeAngle(double angle);

int main()
{
	int i = 0;
	cv::VideoCapture cap("http://10.0.1.7:8080/?action=stream");
	cv::Mat image;
	cv::Mat goalImage = cv::imread("goal.jpg", CV_LOAD_IMAGE_COLOR);

	cap.read(image);
	Mat thresh, binary, hsv, contour_mat;
	vector<vector<Point> > contours;
	vector<vector<Point> > goal;
	vector<Vec4i> hierarchy;
	vector<Vec4i> hierarchyGoal;
    double contourOfInterest = 0;
    double distance;
    double ratio;
    double focalLength;
    double area;
    double angle;
    Mat element = getStructuringElement( cv::MORPH_RECT,
    									 cv::Size(2 * 2 + 1, 2 * 2 + 1),
										 cv::Point(2, 2) );
	cv::cvtColor( goalImage, goalImage, COLOR_BGR2HSV );//convert to HSV
	cv::inRange( goalImage, Scalar( 70, 154, 73 ), Scalar( 112, 249, 156 ), goalImage );//threshold the HSV colortype
	cv::dilate( goalImage , goalImage, element );
	cv::erode( goalImage , goalImage, element ); //dilate then erode to close any irregularities
    cv::namedWindow("test",CV_WINDOW_AUTOSIZE);
    cv::imshow("test",goalImage);
    Scalar color = Scalar( 0,255,0 );

    cv::findContours(goalImage,goal,hierarchy, cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
    while(i<=10){
		if(image.empty())
		{
		 	cout<<"Bad input image!";
		}
		cv::namedWindow( "Cam", CV_WINDOW_AUTOSIZE );// create a window for the og image.
		cv::imshow( "Cam", image );// show our image inside it.
		cv::namedWindow( "Thresholded", CV_WINDOW_AUTOSIZE );//create a window for the thresholded image
		cv::cvtColor( image, hsv, COLOR_BGR2HSV );//convert to HSV
		cv::waitKey( 16.67 );//wait 16.67 ms (30fps)
		cv::inRange( hsv, Scalar( 70, 154, 73 ), Scalar( 112, 249, 156 ), thresh );//threshold the HSV colortype
		cv::dilate( thresh , thresh, element );
		cv::erode( thresh , thresh, element ); //dilate then erode to close any irregularities
		cv::imshow( "Thresholded", thresh );
		cv::findContours(thresh,contours,hierarchy, cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
		Mat goalDrawing = Mat::zeros( thresh.size(), CV_8UC3 );
		Mat drawing = Mat::zeros( thresh.size(), CV_8UC3 );
	    for( unsigned int i = 0; i < contours.size(); i++ )
	    {
	        double similarity = matchShapes(contours[i],goal[0],CV_CONTOURS_MATCH_I2,0);
	        cout <<"similarity\n"<< similarity << " ";

	        if(similarity <= 2){
	        	cout<<"found the goal!\n";
	        	printf("found the goal");
	        	contourOfInterest = i;
	           	drawContours(goalDrawing,contours,contourOfInterest,color,2,8,hierarchy,0,Point());
				Rect rect = boundingRect(contours[contourOfInterest]);
				rectangle(goalDrawing, rect.tl(), rect.br(), cv::Scalar(100, 100, 200), 2, CV_AA);
	            cv::namedWindow("selected contour",CV_WINDOW_AUTOSIZE);
	            imshow("selected contour",goalDrawing);
	        }
	        else{
	        	contourOfInterest = -1;
	        }

	        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	    }
	    Point centerPoint;
	    cv::namedWindow("contours",CV_WINDOW_AUTOSIZE);
		cv::imshow( "contours", drawing);
		if(contourOfInterest != -1){
			Rect rect = boundingRect(contours[contourOfInterest]);
			distance = (4.4 * 14 * 240)/(rect.height * 3);//focal length (mm) * height (inches) * photo hieght(pixels)/(obj height(pixels)*vertical sensor size(mm))
			cout <<"dist"<<distance<<"\n";
			centerPoint = Point((rect.x + rect.width/2), (rect.y + rect.height/2));
			float imageCntr = 432/2-.5;
			cout << "center point " <<centerPoint.x<<","<<centerPoint.y;
			focalLength = (0.5 * 432 / tan(74/2));
			angle = atan( ( centerPoint.x - 215 ) / focalLength);
			cout<<"azimuth " <<angle * 180 / 3.14;
		}
		cap.read( image );
	}
    return 0;
}

double normalizeAngle(double angle){
	while(angle >= 360.0){
		angle -=360;
	}
	while(angle <0.0){
		angle +=360.0;
	}
	return angle;
}
