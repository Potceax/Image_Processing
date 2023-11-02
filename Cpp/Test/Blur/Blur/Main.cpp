#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

void KernelBlur(const std::string& input, const std::string& output = "Default.jpg")
{
    // Load an image
    cv::Mat image = cv::imread(input);

    if (image.empty())
    {
        std::cerr << "Could not open or find the image" << std::endl;
        return;
    }

    // Define the kernel for a simple averaging blur
    cv::Mat kernel = cv::Mat::ones(5, 5, CV_32F) / 25.0;  // 5x5 kernel for averaging

    // Create a Mat to store the blurred image
    cv::Mat blurredImage;

    // Perform convolution to apply the blur
    cv::filter2D(image, blurredImage, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    // Save the blurred image
    cv::imwrite(output, blurredImage);

    std::cout << "Image blurred using a custom kernel and saved as blurred.jpg" << std::endl;
}

/**
  * Function that mesures time for Kernel type blurs
  * @param: input -> takes image path
  * @param: output -> takes image path
*/
void TestTime(const std::string& input, const std::string& output = "Default.jpg")
{
    int64_t start = cv::getTickCount();
    KernelBlur(input, output);
    int64_t end = cv::getTickCount();
    double elapsedTime = (end - start) / cv::getTickFrequency();
    std::cout << "Kernel Time: " << elapsedTime << "s" << std::endl;
}

int main()
{
    const std::string Input{ "Original.jpg" };
    const std::string Output{ "blurred.jpg" };

    //KernelBlur(Input, Output);

    TestTime(Input, Output);

    return 0;
}