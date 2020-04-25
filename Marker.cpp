#include "Marker.h"

Marker::Marker(const Marker& m)
{
	this->points = m.points;
	this->colour = m.colour;
	this->markerNumber = m.markerNumber;

	this->kernelsize = m.kernelsize;
	this->gauss_size = m.gauss_size;
	this->thresh1 = m.thresh1;
	this->thresh2 = m.thresh2;
	this->houghdist = m.houghdist;
	this->houghmin = m.houghmin;
	this->houghmax = m.houghmax;
	this->bl_up = m.bl_up;
	this->bl_low = m.bl_low;
	this->gr_up = m.gr_up;
	this->gr_low = m.gr_low;
	this->rd_up = m.rd_up;
	this->rd_low = m.rd_low;
}

Marker& Marker::operator=(const Marker& m)
{
	this->points = m.points;
	this->colour = m.colour;
	this->markerNumber = m.markerNumber;

	this->kernelsize = m.kernelsize;
	this->gauss_size = m.gauss_size;
	this->thresh1 = m.thresh1;
	this->thresh2 = m.thresh2;
	this->houghdist = m.houghdist;
	this->houghmin = m.houghmin;
	this->houghmax = m.houghmax;
	this->bl_up = m.bl_up;
	this->bl_low = m.bl_low;
	this->gr_up = m.gr_up;
	this->gr_low = m.gr_low;
	this->rd_up = m.rd_up;
	this->rd_low = m.rd_low;

	return *this;
}

void Marker::read(const cv::FileNode& node)
{
	colour = ((std::string)node["colour"])[0];
	markerNumber = (int)node["markerNumber"];

	int x = (int)node["kernelsize"];
	kernelsize = cv::Size(x, x);
	x = (int)node["gauss_size"];
	gauss_size = cv::Size(x, x);

	thresh1 = (int)node["thresh1"];
	thresh2 = (int)node["thresh2"];
	houghdist = (int)node["houghdist"];
	houghmin = (int)node["houghmin"];
	houghmax = (int)node["houghmax"];
	bl_up = (int)node["bl_up"];
	bl_low = (int)node["bl_low"];
	gr_up = (int)node["gr_up"];
	gr_low = (int)node["gr_low"];
	rd_up = (int)node["rd_up"];
	rd_low = (int)node["rd_low"];
}

void Marker::addPoint(const cv::Point2f p)
{
	this->points.push_back(p);
}

void Marker::drawPoints(cv::Mat& img, cv::Scalar colour)
{
	for (int i = 0; i < points.size(); i++)
	{
		circle(img, points[i], 2, colour, 2);
	}
}

bool Marker::colourCheck(cv::Vec3b intensity)
{
	return ((int)intensity.val[0] >= bl_low && (int)intensity.val[0] <= bl_up &&
		(int)intensity.val[1] >= (gr_low - 10) && (int)intensity.val[1] <= gr_up &&
		(int)intensity.val[2] >= rd_low && (int)intensity.val[2] <= rd_up);
}

bool Marker::whiteMarkerCheck(const cv::Mat& img, cv::Point candidate, float percentUp, float percentDown)
{
	if (candidate.x < 0 || candidate.x > img.rows || candidate.y < 0 || candidate.y > img.cols)
	{
		std::cout << "Error: Candidate marker out of range!\n";
		return false;
	}

	cv::Point p1(candidate.x - 10, candidate.y - 10);
	cv::Point p2(candidate.x + 10, candidate.y + 10);
	cv::Rect r(p1, p2);

	//error handling

	cv::Mat roi(img(r));
	int countBlackPx = 0;

	for (int x = 0; x < roi.rows; x++)
	{
		for (int y = 0; y < roi.cols; y++)
		{
			cv::Vec3b intensity = roi.at<cv::Vec3b>(cv::Point(x, y));
			if ((int)intensity.val[0] <= 20 &&
				(int)intensity.val[1] <= 20 &&
				(int)intensity.val[2] <= 20) // the pixel is black for sure
			{
				countBlackPx++;
			}
		}
	}

	float blackPxPercent = ((float)countBlackPx / (float)(roi.cols * roi.rows)) * 100;

	if (blackPxPercent <= percentUp && blackPxPercent >= percentDown)
		return true;
	else
		return false;
}

void Marker::findMarkers(const cv::Mat& img, float percentUp, float percentDown)
{
	clearPoints();

	std::vector<cv::Mat> channels;
	cv::split(img, channels);

	cv::Mat bl, gr, rd, thrBl, thrGr, thrRd, thresh, hatkernel, kernel;

	bl = channels[0].clone();
	gr = channels[1].clone();
	rd = channels[2].clone();

	cv::inRange(bl, cv::Scalar(bl_low), cv::Scalar(bl_up), thrBl);
	cv::inRange(gr, cv::Scalar(gr_low), cv::Scalar(gr_up), thrGr);
	cv::inRange(rd, cv::Scalar(rd_low), cv::Scalar(rd_up), thrRd);

	thresh = thrBl&thrGr&thrRd;

	int distance = thresh.rows / houghdist;

	if (getcolour() == 'b') //blue
	{
		kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, kernelsize, cv::Point(0, 0));
		cv::morphologyEx(thresh, thresh, cv::MORPH_DILATE, kernel);
	}
	else if (getcolour() == 'g') {} //light green
	else if (getcolour() == 'd') //dark green
	{
		kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, kernelsize, cv::Point(0, 0));
		cv::morphologyEx(thresh, thresh, cv::MORPH_CLOSE, kernel);
	}
	else if (getcolour() == 'r') //red
	{
		kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, kernelsize, cv::Point(0, 0));
		cv::morphologyEx(thresh, thresh, cv::MORPH_DILATE, kernel);
	}
	else if (getcolour() == 'w') // white
	{
		kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, kernelsize, cv::Point(0, 0));
		cv::morphologyEx(thresh, thresh, cv::MORPH_DILATE, kernel);
	}
	else { std::cout << "Error"; return; }


	cv::GaussianBlur(thresh, thresh, gauss_size, 1.5, 1.5);

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(thresh, circles, CV_HOUGH_GRADIENT, 1, distance, thresh1, thresh2, houghmin, houghmax);

	cv::Vec3b intensity;

	for (size_t j = 0; j < circles.size(); j++)
	{
		cv::Point center(cvRound(circles[j][0]), cvRound(circles[j][1]));

		if (colourCheck(img.at<cv::Vec3b>(center)))
		{
			if (getcolour() == 'w')
			{
				if (whiteMarkerCheck(img, center, percentUp, percentDown))
					addPoint(center);
			}
			else
				addPoint(center);
		}
	}
}

bool Marker::opticalFlow(cv::Mat& prevGray, cv::Mat& nextGray, cv::Mat& nextImg, std::vector<cv::Mat>& pyrImgs, float percentUp, float percentDown)
{
	std::vector<cv::Point2f> prevpoints = points;

	std::vector<cv::Point2f> nextpoints;
	std::vector<uchar> status;
	std::vector<float> err;

	cv::calcOpticalFlowPyrLK(pyrImgs, nextGray, points, nextpoints, status, err);

	for (size_t i = 0; i < nextpoints.size(); i++)
	{
		if (nextpoints[i].x > nextImg.cols || nextpoints[i].x < 0 ||
			nextpoints[i].y > nextImg.rows || nextpoints[i].y < 0)
		{
			std::cout << "All markers must stay within the frame!\n";
			return false;
		}

		if (!colourCheck(nextImg.at<cv::Vec3b>(nextpoints[i])))
		{
			cv::Rect r((int)nextpoints[i].x - 5, (int)nextpoints[i].y - 5, 10, 10);
			cv::Mat small = nextImg(r);

			bool foundRightPixel = false;
			int j = 0;
			int k;
			while (!foundRightPixel && j < small.rows)
			{
				k = 0;
				while (!foundRightPixel && k < small.cols)
				{
					cv::Vec3b intensity = small.at<cv::Vec3b>(j, k);
					if ((int)intensity.val[0] >= bl_low && (int)intensity.val[0] <= bl_up &&
						(int)intensity.val[1] >= (gr_low - 10) && (int)intensity.val[1] <= gr_up &&
						(int)intensity.val[2] >= rd_low && (int)intensity.val[2] <= rd_up)
					{
						foundRightPixel = true;
						nextpoints[i] = cv::Point2f((float)(nextpoints[i].x - 2 + k), (float)(nextpoints[i].y - 2 + j));
					}
					++k;
				}
				++j;
			}

			if (!foundRightPixel)
			{
				findMarkers(nextImg, percentUp, percentDown);

				int j = 0;
				while (j < points.size() &&
					(prevpoints[i].x > points[j].x + 25 || prevpoints[i].x < points[j].x - 25 ||
						prevpoints[i].y > points[j].y + 25 || prevpoints[i].y < points[j].y - 25))
				{
					++j;
				}
				if (j < points.size()) nextpoints[i] = points[j];
				else nextpoints[i] = prevpoints[i];

			}
		}
	}

	clearPoints();

	for (int i = 0; i < nextpoints.size(); ++i)
	{
		addPoint(nextpoints[i]);
	}

	return true;
}
