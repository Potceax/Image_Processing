#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat KernelBlur(const std::string& input, int strg = 5)
{
    // Load an image
    cv::Mat image = cv::imread(input);

    if (image.empty())
    {
        std::cerr << "Could not open or find the image" << std::endl;
        return cv::Mat();
    }

    // Define the kernel for a simple averaging blur
    cv::Mat kernel = cv::Mat::ones(strg, strg, CV_32F) / static_cast<double>(strg * strg);  // (strg x strg) kernel for averaging

    // Create a Mat to store the blurred image
    cv::Mat blurredImage;

    // Perform convolution to apply the blur
    cv::filter2D(image, blurredImage, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    // Save the blurred image


    return blurredImage;
}


/**
  * Function that mesures time for Kernel type blurs
  * @param: input -> takes image path
  * @param: output -> takes image path
*/
double TestTime(const std::string& input, int reps = 5)
{
    std::vector<float> time{};
    double sum{};
    for (int i{ 0 }; i < 5; ++i)
    {
        int64_t start = cv::getTickCount();
        KernelBlur(input);
        int64_t end = cv::getTickCount();
        time.push_back((end - start) / cv::getTickFrequency());

        sum += time.back();
        std::cout << "Kernel Time: " << time.back() << "s\n";
    }

    return (sum / reps);
}

int main()
{
    std::string Input{ };
    std::string Output{ };
    int strength{};

    std::ifstream file("file.txt"); // Replace with your file name
    if (file.is_open())
    {
        // Read the first string from the file
        file >> Input >> Output >> strength;
        file.close(); // Close the file after reading
    }

    std::cout << "Kernel blur algorithm time: " << TestTime(Input) << '\n';
    cv::imwrite(Output, KernelBlur(Input, 10));

    return 0;
}