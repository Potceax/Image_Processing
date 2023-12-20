#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace fs = std::filesystem;

static std::string CreateNewFolder(const std::string& folderName)
{
    // Get the current working directory
    fs::path currentDir = fs::current_path();

    // Name of the new folder
    std::string newFolderName = folderName;

    // Construct the path to the new folder
    fs::path newFolderPath = currentDir / newFolderName;

    // Create directory
    if (!fs::exists(newFolderPath))
    {
        fs::create_directory(newFolderPath);
    }

    std::string FolderPath{ newFolderPath.string() + "\\\\" };

    return FolderPath;
}

/**
  * Shapes for Cut method
  * each class define a shape that will be cut in the chosen image (var: mask)
*/
class Shape
{
public:
    Shape(const cv::Point& _position, const cv::Scalar _color, int _size = 1) :
        position{ _position }, color{ _color }, size{ _size }
    {
    }
public:
    virtual void CreateShape(const cv::Mat& mask) const = 0;

protected:
    cv::Point position;
    cv::Scalar color;
    int size{};
};

class Circle : public Shape
{
public:
    Circle(const cv::Point& _position, const cv::Scalar _color, int _size = 1) :
        Shape(_position, _color, _size), radius{ size / 2 }
    {
    }

public:
    virtual void CreateShape(const cv::Mat& mask) const override
    {
        cv::circle(mask, position, radius, color, size);
    }
protected:
    int radius{};
};

class Square : public Shape
{
public:
    Square(const cv::Point& _position, const cv::Scalar _color, int _size = 1) :
        Shape(_position, _color, _size),
        pt1{ cv::Point(_position.x - _size / 2, _position.y - _size / 2) },
        pt2{ cv::Point(_position.x + _size / 2, _position.y + _size / 2) }
    {
    }

public:
    virtual void CreateShape(const cv::Mat& mask) const override
    {
        // TODO: add variables for last parameter (different values give different looking shapes)
        cv::rectangle(mask, pt1, pt2, color, size);
    }
protected:
    cv::Point pt1;
    cv::Point pt2;
};

class Triangle : public Shape
{
public:
    Triangle(const cv::Point& _position, const cv::Scalar _color, int _size = 1) :
        Shape(_position, _color, _size)
    {
        vertices[0] = _position;
        vertices[1] = cv::Point(_position.x + _size, _position.y);
        vertices[2] = cv::Point(_position.x, _position.y + _size);
    }
public:
    virtual void CreateShape(const cv::Mat& mask) const override
    {
        const cv::Point* ppt[] = { vertices };
        const int ncontours{ 1 };
        int npt[] = { 3 };
        cv::fillPoly(mask, ppt, npt, ncontours, color);
    }
protected:
    cv::Point vertices[3];
};

static cv::Mat CutShape(const std::string& imagePath, const Shape* shape)
{
    cv::Mat image{ cv::imread(imagePath) };
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);

    shape->CreateShape(mask);

    // Invert mask to get the shape in white
    cv::Mat mask_inv;
    cv::bitwise_not(mask, mask_inv);

    // Cut the shape from the image and add a white shape in its place
    image.setTo(cv::Scalar(255, 255, 255), mask);
    return image;
}


int main()
{
    // Image path values
	std::string ImagePath{ "Maklowicz.jpg" };
	std::string OutputPath{ "Mk1.jpg" };
	OutputPath = CreateNewFolder("Cut") + OutputPath;

	// shape variables
	cv::Point position{ 600,600 };
	cv::Scalar white{ 255 };
	int size{ 200 };

	// Premade shapes
	Circle* circle{ new Circle(position, white, size) };
	Square* square{ new Square(position, white, size) };
	Triangle* triangle{ new Triangle(position, white, size) };

	// choosen shape
	Shape* currentShape{ (Shape*)square };

	// Cutting a white circle shape out of the image at a given position
	cv::imwrite(OutputPath, CutShape(ImagePath, currentShape));

	return 0;
}