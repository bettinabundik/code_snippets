#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>

class Marker
{
public:
	Marker() {}
	Marker(const Marker& m);
	Marker& operator=(const Marker& m);
	~Marker() {}
	void read(const cv::FileNode& node);

	void addPoint(const cv::Point2f p);
	size_t getsize() { return points.size(); }
	char getcolour() { return colour; }
	std::vector<cv::Point2f>& getpoints() { return points; }
	int getMarkerNumber() { return markerNumber; }
	void findMarkers(const cv::Mat& img, float percentUp, float percentDown);
	void drawPoints(cv::Mat& img, cv::Scalar colour);
	void clearPoints() { points.clear(); }
	bool opticalFlow(cv::Mat& prevGray, cv::Mat& nextGray, cv::Mat& nextImg, std::vector<cv::Mat>& pyrImgs, float percentUp, float percentDown);
	bool colourCheck(cv::Vec3b intensity);
	bool whiteMarkerCheck(const cv::Mat& img, cv::Point candidate, float percentUp, float percentDown);

private:
	std::vector<cv::Point2f> points;
	char colour = 'b';
	int markerNumber = 0;

	cv::Size kernelsize = cv::Size(0, 0);
	cv::Size gauss_size = cv::Size(0, 0);
	int thresh1 = 0;
	int thresh2 = 0;
	int houghdist = 0;
	int houghmin = 0;
	int houghmax = 0;
	int bl_up = 0;
	int bl_low = 0;
	int gr_up = 0;
	int gr_low = 0;
	int rd_up = 0;
	int rd_low = 0;
};