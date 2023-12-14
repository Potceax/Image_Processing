#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace fs = std::filesystem;

/**
  * Function to merge two photos together
  * @param: string photoPathX -> path to original photos
  * @param: float alpha -> alpha value of the top photo after merge
  *
  * @return: cv::Mat -> merged photo data
*/
static cv::Mat Merge(
    const std::string& photoPath1, const std::string& photoPath2, float alpha = 0.5)
{
    // Load the images
    cv::Mat image1 = cv::imread(photoPath1);
    cv::Mat image2 = cv::imread(photoPath2);

    if (image1.empty() || image2.empty())
    {
        std::cout << "Could not open or find the images!" << std::endl;
        return cv::Mat();
    }

    // Resize image2 to match image1
    if (image1.size() != image2.size())
    {
        resize(image2, image2, image1.size());
    }

    // Transparency for image1
    float beta = (1.0 - alpha);  // Transparency for image2

    // final result of merge
    cv::Mat output;

    // Blend the images
    addWeighted(image1, alpha, image2, beta, 0.0, output);

    return output;
}

/**
  * Function that mesures time for merging
  * @param: input -> takes image path
  * @param: output -> takes image path
*/
static double TestTime(const std::string& input1, const std::string& input2, int reps = 5)
{
    std::vector<double> time{};
    double sum{};
    for (int i{ 0 }; i < reps; ++i)
    {
        int64_t start = cv::getTickCount();
        Merge(input1, input2);
        int64_t end = cv::getTickCount();
        time.push_back((end - start) / cv::getTickFrequency());

        sum += time.back();
        std::cout << "Time: " << time.back() << "s\n";
    }

    return (sum / reps);
}

/**
  * Create new folder in the current one
  * @param: string folderName -> name of the new folder
  * @return: string           -> path to the new folder
*/
static std::string CreateNewFolder(const std::string& folderName)
{
    // Get the current working directory
    fs::path currentDir = fs::current_path();

    // Construct the path to the new folder
    fs::path newFolderPath = currentDir / folderName;

    // Create directory
    if (!fs::exists(newFolderPath))
    {
        fs::create_directory(newFolderPath);
    }

    std::string FolderPath{ newFolderPath.string() + "\\\\" };

    return FolderPath;
}


int main()
{
    std::string PhotoPath1{};
    std::string PhotoPath2{};
    std::string OutputPath{};

    // fill paths and create folder dedicated for output
    std::ifstream file("Mergefile.txt"); // Replace with your file name
    if (file.is_open())
    {
        // Read the first string from the file
        file >> PhotoPath1 >> PhotoPath2 >> OutputPath;
        file.close(); // Close the file after reading
    }
    OutputPath = CreateNewFolder("Merge") + OutputPath;

    // Time test for merge (5 reps)
    std::cout << TestTime(PhotoPath1, PhotoPath2) << "s\n";

    // Create output image
    cv::imwrite(OutputPath, Merge(PhotoPath1, PhotoPath2));

    return 0;
}