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


int main()
{

    Mat img = imread("C:/Users/hp/OneDrive/Desktop/Student Test Images/t1.bmp");


    int rows = img.rows;
    int cols = img.cols;

    QuadTreeNode* tree = convertFromFile("root", ".txt");
    
    int** array = new int* [rows];
    for (int i = 0; i < rows; i++) 
    {
        array[i] = new int[cols];
    }

    convertTo2D(tree, array);
    cout << "\nFirst time" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (array[i][j] != 0 && array[i][j] != 255)
            {
                cout << "error" << endl;
                cout << array[i][j] << endl;
                cout << "row: " << i << "  col: " << j << endl;
                array[i][j] = 255;
            }
        }
    }
    cout << "\nSecond time" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (array[i][j] != 0 && array[i][j] != 255)
            {
                cout << "error" << endl;
                cout << array[i][j] << endl;
            }
        }
    }

    Mat image(rows, cols, CV_8UC3);
    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++) 
        {
            image.at<Vec3b>(i, j) = Vec3b(array[i][j], array[i][j], array[i][j]);
        }
    }

    imwrite("output.bmp", image);
   

    return 0;
}