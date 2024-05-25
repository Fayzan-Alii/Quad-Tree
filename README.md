# Quad-Tree

This project consists of three parts that perform image processing using a QuadTree data structure. The project uses the OpenCV library to read and write images.

## Part 1

The first part of the project reads an image, converts it into a grayscale image, and then constructs a QuadTree from the image data. Each node in the QuadTree, representing a rectangular region of the image which is subdivided into four smaller rectangles, is then written to a separate file.

## Part 2

The second part of the project reads a QuadTree from multiple files and converts it back into an image. Each node is read from a separate file, where the file contains the node's x and y coordinates, size, color, and the filenames of the children nodes. The QuadTree is then converted back into a 2D array and written back into an image.

## Part 3

The third part of the project reads an image, converts it into a QuadTree, and then converts it back into a 2D array. It then calculates the sum of the squared differences between the original image and the reconstructed image. This sum is used to calculate an accuracy score, which is a measure of how closely the reconstructed image matches the original image. The closer the score is to 100, the better the match.

## Usage

To use this project, you will need to have the OpenCV library installed. You can then compile and run the code using a C++ compiler. The main function in each part of the project ties everything together and serves as a good starting point for understanding the flow of the program.