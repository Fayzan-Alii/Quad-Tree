#include <iostream>
#include<fstream>
#include<cmath>
using namespace std;

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;

enum Color { WHITE, BLACK, GREY };
struct QuadTreeNode
{
    int x, y, size;
    Color color;
    QuadTreeNode* children[4];
    QuadTreeNode(int _x, int _y, int _size, Color _color)
    {
        x = _x;
        y = _y;
        size = _size;
        color = _color;
        for (int i = 0; i < 4; i++)
            children[i] = NULL;
    }
};


QuadTreeNode* convertFromFile(string filename, string extension)
{

    ifstream file(filename + extension);
    if (!file.is_open()) 
    {
        cout << "File " << filename << " not open for reading" << endl;
        return NULL;
    }

    int x, y, size;
    string colorStr;
    file >> x >> y >> size >> colorStr;
    Color color;
    if (colorStr == "WHITE")
    {
        color = WHITE;
    }
    else if (colorStr == "BLACK")
    {
        color = BLACK;
    }
    else if (colorStr == "GREY") 
    {
        color = GREY;
    }
 
    QuadTreeNode* node = new QuadTreeNode(x, y, size, color);


    for (int i = 0; i < 4; i++)
    {
        string childFilename;
        file >> childFilename;
        if (childFilename == "NULL") 
        {
            node->children[i] = NULL;
        }
        else 
        {
            QuadTreeNode* childNode = convertFromFile(childFilename, extension);
            node->children[i] = childNode;
        }
    }


    file.close();
    return node;
}


void convertTo2D(QuadTreeNode* node, int** array) 
{
    if (node == NULL)
    {
        return;
    }


    if (node->color == WHITE) 
    {
        for (int i = node->x; i < node->x + node->size; i++) 
        {
            for (int j = node->y; j < node->y + node->size; j++) 
            {
                array[i][j] = 255;
            }
        }
    }

    else if (node->color == BLACK)
    {
        for (int i = node->x; i < node->x + node->size; i++) 
        {
            for (int j = node->y; j < node->y + node->size; j++)
            {
                array[i][j] = 0;
            }
        }
    }

    else if (node->color == GREY) 
    {
        for (int i = 0; i < 4; i++) 
        {
            convertTo2D(node->children[i], array);
        }
    }
}

int main() {

    Mat img = imread("C:/Users/hp/OneDrive/Desktop/Student Test Images/t1.bmp");


    int rows = img.rows;
    int cols = img.cols;

    cout << "Rows: " << rows << endl << "Columns: " << cols << endl;

    int** pixels = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        pixels[i] = new int[cols];
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            pixels[i][j] = static_cast<int>(img.at<uchar>(i, j));
        }
    }


    QuadTreeNode* tree = convertFromFile("root", ".txt");
    

    int** array = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        array[i] = new int[cols];
    }

    convertTo2D(tree, array);

    int n = rows * cols;

    double sum = 0.0;
    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++)
        {
            sum = sum + pow((pixels[i][j] - array[i][j]), 2);
        }
    }
    sum /= n;

    cout << "Sum: " << sum << endl;

    double accuracy = 0;

    accuracy = 100 * (1 - (sum / (255 * 255)));

    cout << "Accuracy: " << accuracy << endl;

    return 0;
}