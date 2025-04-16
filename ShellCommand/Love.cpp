#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

char pixelToChar(int pixelValue) {
    const std::string ascii = " .:-=+*%@#";  // Character set for ASCII art
    return ascii[pixelValue * (ascii.length() - 1) / 255];
}

// Function to convert the image to ASCII and print it
void printAsciiArt(const std::string& imagePath) {
    // Load the image in grayscale
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    
    if (img.empty()) {
        std::cerr << "Error loading image!" << std::endl;
        return;
    }

    // Increase the resolution by changing the height and width
    float aspect_ratio = float(img.cols) / float(img.rows);
    int new_height = 80;  // Increased height for more resolution
    int new_width = int(aspect_ratio * new_height * 2);  // Adjust width to maintain aspect ratio

    cv::resize(img, img, cv::Size(new_width, new_height));

    // Iterate over the image pixels and map them to ASCII characters
    for (int y = 0; y < img.rows; ++y) {
        for (int x = 0; x < img.cols; ++x) {
            char asciiChar = pixelToChar(img.at<uchar>(y, x));
            std::cout << asciiChar;
        }
        std::cout << std::endl;
    }
}

int main() {
    // Specify the path to the image
    std::string imagePath = "Reshmi.jpg";  // Replace with the path to your image file
    printAsciiArt(imagePath);  // Convert the image to ASCII art and print it
    return 0;
}
