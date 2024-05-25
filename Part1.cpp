#include <iostream>
#include<fstream>
#include<cmath>
using namespace std;

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;



template<class t>
class node
{
public:
    t data;
    node* right;
    node* down;

    node()
    {
        data = 0;
        right = NULL;
        down = NULL;
    }

};

template<class T>
class LL2d
{
public:
    node<T>* head_2dLL;
   
    LL2d()
    {
        head_2dLL = NULL;
    }



void convertTo2dLL(int** array, int row, int col)
{
    int i = 0;
    head_2dLL = new node<T>;
    head_2dLL->data = i;

    node<T>* temp = head_2dLL;
    for (i = 0; i < row; i++)
    {
        if (i > 0)
        {
            temp->down = new node<T>;
            temp = temp->down;
        }
        temp->data = i + 1;
        if (temp->right == NULL)
        {
            temp->right = new node<T>;
        }

        temp = temp->right;

        for (int j = 0; j < col; j++)
        {
            if (temp->right == NULL)
            {
                temp->right = new node<T>;
            }

            if (array[i][j] == 255)
            {
                temp->data = j;
                temp = temp->right;

                if (temp->right == NULL)
                {
                    temp->right = new node<T>;
                }

                while (j < col && array[i][j] == 255)
                {
                    temp->data = j;
                    j++;
                }

                temp = temp->right;
            }
        }

        temp->data = -2;

        if (temp->right == NULL)
        {
            temp->right = new node<T>;
        }

        temp = temp->right;

        for (int j = 0; j < col; j++)
        {
            if (temp->right == NULL)
            {
                temp->right = new node<T>;
            }

            if (array[i][j] == 0)
            {
                temp->data = j;
                temp = temp->right;

                if (temp->right == NULL)
                {
                    temp->right = new node<T>;
                }

                while (j < col && array[i][j] == 0)
                {
                    temp->data = j;
                    j++;
                }

                temp = temp->right;
            }
        }

        temp->data = -1;

        if (temp->right == NULL)
        {
            temp->right = new node<T>;
        }

        temp = temp->right;
    }
}


};



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


QuadTreeNode* buildQuadTree(int** image, int x, int y, int size)
{
    if (size == 1) 
    {
       
        return new QuadTreeNode(x, y, size, (image[x][y] == 0) ? BLACK : WHITE);
    }
    
    bool isSame = true;
    int firstPixel = image[x][y];
    for (int i = x; i < x + size; i++)
    {
        for (int j = y; j < y + size; j++) 
        {
            if (image[i][j] != firstPixel)
            {
                isSame = false;
                break;
            }
        }
        if (!isSame)
            break;
    }
    
    if (isSame)
    {
        return new QuadTreeNode(x, y, size, (image[x][y] == 0) ? BLACK : WHITE);
    }
    
    QuadTreeNode* node = new QuadTreeNode(x, y, size, GREY);
   
    int subSize = size / 2;
    node->children[0] = buildQuadTree(image, x, y, subSize);
    node->children[1] = buildQuadTree(image, x + subSize, y, subSize);
    node->children[2] = buildQuadTree(image, x, y + subSize, subSize);
    node->children[3] = buildQuadTree(image, x + subSize, y + subSize, subSize);
    return node;
}

void printQuadTree(QuadTreeNode* node)
{
    if (node == NULL)
        return;
    cout << node->x << " " << node->y << " " << node->size << " ";
    switch (node->color) {
    case WHITE:
        cout << "WHITE";
        break;
    case BLACK:
        cout << "BLACK";
        break;
    case GREY:
        cout << "GREY";
        break;
    }
    cout << endl;
    for (int i = 0; i < 4; i++)
        printQuadTree(node->children[i]);
}



void convertToFile(QuadTreeNode* node, string filename,string extension) 
{
    
    ofstream file(filename+extension);
    if (!file.is_open())
    {
        cout << "File " << filename << " not open for writing" << endl;
        return;
    }

   
    file << node->x << " " << node->y << " " << node->size << " ";
    switch (node->color) 
    {
    case WHITE:
        file << "WHITE";
        break;
    case BLACK:
        file << "BLACK";
        break;
    case GREY:
        file << "GREY";
        break;
    }
    file << endl;
    int l = 0;
   
    for (int i = 0; i < 4; i++) 
    {
        if (node->children[i] != NULL) 
        {
            l++;
            
            string childFilename = filename + to_string(l);
            
            convertToFile(node->children[i], childFilename, extension);
            
            file << childFilename << endl;
        }
        else 
        {
           
            file << "NULL" << endl;
        }
    }

    
    file.close();
}




int main()
{

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

    LL2d<int> ll;
    ll.convertTo2dLL(pixels, rows, cols);

    node<int>* temp = ll.head_2dLL;

    cout << "\n\nPrinting 2D Linked List" << endl;
    while (temp != NULL)
    {
        while (temp->right != NULL)
        {
            cout << temp->data << " ";
            temp = temp->right;
        }
        cout << endl;
        temp = temp->down;
    }
    
    QuadTreeNode* root = buildQuadTree(pixels, 0, 0, rows);
    convertToFile(root, "root",".txt");
   


    return 0;
}