#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <algorithm>

static void Error_internal(const char* function, int line, const char* message)
{
	std::cerr << "ERROR (line #" << line << ") " << function << "(): " << message << std::endl;
	//abort();
	exit(EXIT_FAILURE);
}

#define Error(message) Error_internal(__func__, __LINE__, message)

static void ProcessImage(const std::string& imageName, const std::string& outputName)
//static void ProcessImage(const std::string& imageName, )
{
	cv::Mat originalImage = cv::imread(imageName, cv::IMREAD_COLOR);
	if (originalImage.empty())
		Error("");
	cv::Mat image = originalImage;
	//cv::cvtColor(originalImage, image, cv::COLOR_BGR2HSV);
	//cv::imshow("", originalImage);
	//cv::waitKey(0);
	//return 0;
	cv::Mat channels[3];
	cv::split(image, channels);
	channels[2] -= (channels[1] + channels[0]);
	//cv::Mat& huechannel = channels[0];
	//cv::Mat part1, part2;
	//cv::threshold(huechannel, part1, 224, 255, cv::THRESH_BINARY);
	//cv::threshold(huechannel, part2, 0, 255, cv::THRESH_BINARY_INV);
	int dilationSize = 1;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1), cv::Point(dilationSize, dilationSize));
	cv::Mat onlyred = channels[2];
	cv::threshold(channels[2], onlyred, 0, 255, cv::THRESH_BINARY);
	cv::erode(onlyred, onlyred, element, cv::Point(-1, -1), 1);
	cv::dilate(onlyred, onlyred, element);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(onlyred, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::RotatedRect originRect(cv::Point2f(320, 320), cv::Point2f(80, 80), 0);
	//std::map<double, size_t> areas;
	double maxArea = -1;
	size_t maxAreaIndex = 0;
	size_t currentIndex = 0;
	for (auto contourGroupIterator = contours.begin(); contourGroupIterator not_eq contours.end(); contourGroupIterator++)
	{
		double area = cv::contourArea(*contourGroupIterator);
		//areas[area] = 
		if (maxArea < area and area < 3000 and area > 500)
		{
			maxAreaIndex = currentIndex;
			maxArea = area;
		}
		currentIndex++;
		//if (cv::contourArea(*contourGroupIterator) < 1000)
		//	continue;
		//cv::RotatedRect rect = cv::minAreaRect(*contourGroupIterator);
		//cv::Point2f points[4];
		//rect.points(points);
	}
	//std::cout << "Max Area is " << maxArea << std::endl;
	if (maxArea <= 100)
	{
		std::cout << "No good" << std::endl;
		return;
	}
	cv::Point2f originPoints[4];
	originRect.points(originPoints);
	cv::Point2f points[4];
	cv::RotatedRect rect = cv::minAreaRect(contours[maxAreaIndex]);
	rect.size = originRect.size;
	rect.angle = originRect.angle;
	rect.points(points);
	cv::Mat transform = cv::getAffineTransform(points, originPoints);
	cv::Mat output;
	cv::warpAffine(originalImage, output, transform, cv::Size(640, 640));
	//cv::imshow("", output);
	//cv::waitKey(0);
	cv::imwrite(outputName, output);
	std::cout << "Saved " << outputName << std::endl;
}

/*static void ProcessImage2(const std::string& imageName, const std::string& outputName)
{
	cv::Mat originalImage = cv::imread(imageName, cv::IMREAD_COLOR);
	if (originalImage.empty())
		Error("");
	cv::OrbFeatureDetector detector(400);
	cv::OrbDescriptorExtractor extractor;

	//static 
	std::vector<cv::KeyPoint> referenceKeypoints;
	//static 
	cv::Mat referenceDescriptors;
	//static 
	//bool referenceIsInitialized = false;
	//if (not referenceIsInitialized)
	//{
	//	referenceIsInitialized = true;
		cv::Mat referenceImage = cv::imread("Signs/eTMpyXATn.jpeg", cv::IMREAD_COLOR);
		detector.detect(referenceImage, referenceKeypoints);
		extractor.compute(referenceImage, referenceKeypoints, referenceDescriptors);
	//}

	std::vector<cv::KeyPoint> keypoints;
	detector.detect(originalImage, keypoints);
	cv::Mat descriptors;
	extractor.compute(originalImage, keypoints, descriptors);
	//cv::BFMatcher<cv::L2<float>> matcher;
	cv::BFMatcher matcher;
	std::vector<cv::DMatch> matches;
	matcher.match(referenceDescriptors, descriptors, matches);
	std::sort(matches.begin(), matches.end(), [&](const cv::DMatch& a, const cv::DMatch& b){return a.distance < b.distance;});
	if (matches.size() < 10)
		return;
	cv::Mat matchImage;
	cv::drawMatches(referenceImage, referenceKeypoints, originalImage, keypoints, matches, matchImage);
	cv::imshow("", matchImage);
	cv::waitKey(0);
}

static void ProcessImage3(const std::string& imageName, const std::string& outputName)
//static void ProcessImage(const std::string& imageName, )
{
	cv::Mat originalImage = cv::imread(imageName, cv::IMREAD_COLOR);
	if (originalImage.empty())
		Error("");
	cv::Mat image = originalImage;
	//cv::cvtColor(originalImage, image, cv::COLOR_BGR2HSV);
	//cv::imshow("", originalImage);
	//cv::waitKey(0);
	//return 0;
	cv::Mat channels[3];
	cv::split(image, channels);
	channels[2] -= (channels[1] + channels[0]);
	//cv::Mat& huechannel = channels[0];
	//cv::Mat part1, part2;
	//cv::threshold(huechannel, part1, 224, 255, cv::THRESH_BINARY);
	//cv::threshold(huechannel, part2, 0, 255, cv::THRESH_BINARY_INV);
	int dilationSize = 1;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1), cv::Point(dilationSize, dilationSize));
	cv::Mat onlyred = channels[2];
	cv::threshold(channels[2], onlyred, 0, 255, cv::THRESH_BINARY);
	cv::erode(onlyred, onlyred, element, cv::Point(-1, -1), 1);
	cv::dilate(onlyred, onlyred, element);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(onlyred, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::RotatedRect originRect(cv::Point2f(320, 320), cv::Point2f(80, 80), 0);
	//std::map<double, size_t> areas;
	cv::OrbFeatureDetector detector(400);
	cv::OrbDescriptorExtractor extractor;

	//static 
	std::vector<cv::KeyPoint> referenceKeypoints;
	//static 
	cv::Mat referenceDescriptors;
	//static 
	//bool referenceIsInitialized = false;
	//if (not referenceIsInitialized)
	//{
	//	referenceIsInitialized = true;
		cv::Mat referenceImage = cv::imread("Signs/eTMpyXATn.jpeg", cv::IMREAD_COLOR);
		detector.detect(referenceImage, referenceKeypoints);
		extractor.compute(referenceImage, referenceKeypoints, referenceDescriptors);
	//}

	double maxArea = -1;
	size_t maxAreaIndex = 0;
	size_t currentIndex = 0;
	for (auto contourGroupIterator = contours.begin(); contourGroupIterator not_eq contours.end(); contourGroupIterator++)
	{
		double area = cv::contourArea(*contourGroupIterator);
		//areas[area] = 
		if (maxArea < area and area < 3000 and area > 500)
		//if (false)
		{
			maxAreaIndex = currentIndex;
			maxArea = area;
			
			cv::Mat maskImage(originalImage.rows, originalImage.cols, originalImage.type());
			cv::Mat maskedOriginalImage;
			//originalImage.copyTo(maskedOriginalImage);
			cv::RotatedRect rect = cv::minAreaRect(*contourGroupIterator);
			cv::Point2f point2fs[4];
			rect.points(point2fs);
			cv::Point points[4];
			for (int i = 0; i < 4; i++) points[i] = cv::Point(point2fs[i].x, point2fs[i].y);
			cv::fillConvexPoly(maskImage, points, 4, cv::Scalar(255, 255, 255));
			cv::bitwise_and(originalImage, maskImage, maskedOriginalImage);
			
			std::vector<cv::KeyPoint> keypoints;
			detector.detect(maskedOriginalImage, keypoints);
			cv::Mat descriptors;
			extractor.compute(maskedOriginalImage, keypoints, descriptors);
			//cv::BFMatcher<cv::L2<float>> matcher;
			cv::BFMatcher matcher;
			std::vector<cv::DMatch> matches;
			matcher.match(referenceDescriptors, descriptors, matches);
			std::sort(matches.begin(), matches.end(), [&](const cv::DMatch& a, const cv::DMatch& b){return a.distance < b.distance;});
			std::vector<cv::KeyPoint> rkps, kps;
			if (matches.size() < 4)
			{
				currentIndex++;
				continue;
			}
			constexpr int maxMatchCount = 4;
			for (int i = 0; i < maxMatchCount and i < matches.size(); i++)
			{
				rkps.push_back(referenceKeypoints[matches[i].trainIdx]);
				kps.push_back(keypoints[matches[i].queryIdx]);
			}
			cv::Mat matchImage;
			cv::drawMatches(referenceImage, referenceKeypoints, originalImage, keypoints, matches, matchImage);
			cv::imshow("", matchImage);
			cv::waitKey(0);
			cv::Mat simpleMatchImage;
			cv::drawMatches(referenceImage, referenceKeypoints, originalImage, keypoints, std::vector<cv::DMatch>(matches.begin(), matches.begin() + maxMatchCount), simpleMatchImage);
			cv::imshow("", simpleMatchImage);	
			cv::waitKey(0);
			std::vector<cv::Point2f> rps, ps;
			cv::KeyPoint::convert(rkps, rps);
			cv::KeyPoint::convert(kps, ps);
			cv::Mat H = cv::findHomography(rps, ps);
			cv::Mat outputImage;
			cv::warpPerspective(originalImage, outputImage, H, cv::Size(640, 640));
			cv::imshow("", outputImage);
			cv::waitKey(0);
		}
		currentIndex++;
		//if (cv::contourArea(*contourGroupIterator) < 1000)
		//	continue;
		//cv::RotatedRect rect = cv::minAreaRect(*contourGroupIterator);
		//cv::Point2f points[4];
		//rect.points(points);
	}
	//std::cout << "Max Area is " << maxArea << std::endl;
	if (maxArea <= 100)
	{
		std::cout << "No good" << std::endl;
		return;
	}
	cv::Point2f originPoints[4];
	originRect.points(originPoints);
	cv::Point2f points[4];
	cv::RotatedRect rect = cv::minAreaRect(contours[maxAreaIndex]);
	rect.size = originRect.size;
	rect.angle = originRect.angle;
	rect.points(points);
	cv::Mat transform = cv::getAffineTransform(points, originPoints);
	cv::Mat output;
	cv::warpAffine(originalImage, output, transform, cv::Size(640, 640));
	//cv::imshow("", output);
	//cv::waitKey(0);
	cv::imwrite(outputName, output);
	std::cout << "Saved " << outputName << std::endl;
}*/

int main(int argc, const char** argv)
{
	if (argc not_eq 3)
		return EXIT_FAILURE;
	std::string directory("");
	std::string imageName = directory + std::string(argv[1]);
	std::string outputName = directory + std::string(argv[2]);
	ProcessImage(imageName, outputName);
	return 0;
}
