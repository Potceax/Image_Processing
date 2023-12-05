void Merge(
    const std::string& photoPath1, const std::string& photoPath2,
    const std::string& outputPath, cv::Mat& output, float alpha = 0.5)
{
    // Load the images
    cv::Mat image1 = cv::imread(photoPath1);
    cv::Mat image2 = cv::imread(photoPath2);

    if (image1.empty() || image2.empty())
    {
        std::cout << "Could not open or find the images!" << std::endl;
        return;
    }

    // Resize image2 to match image1
    if (image1.size() != image2.size())
    {
        resize(image2, image2, image1.size());
    }

    // Transparency for image1
    float beta = (1.0 - alpha);  // Transparency for image2

    // Blend the images
    addWeighted(image1, alpha, image2, beta, 0.0, output);

    // Save the blended image
    imwrite(outputPath, output);
}

int main()
{
    int main()
    {
        cv::Mat out;
        Merge("Maklowicz.jpg", "mountain.png", "out.jpg", out);

        return 0;
    }
}