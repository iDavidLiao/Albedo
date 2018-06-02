//
//  s2.cpp
//  VisionHW4
//
//  Created by David Liao on 11/15/17.
//  Copyright (c) 2017 David Liao. All rights reserved.
//


#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <ostream>



using namespace std;


namespace ComputerVisionProjects {
    
    // Class for representing a gray-scale image.
    // Sample usage:
    //   Image one_image;
    //   one_image.AllocateSpaceAndSetSize(100, 200);
    //   one_image.SetNumberGrayLevels(255);
    //   // Creates and image such that each pixel is 150.
    //   for (int i = 0; i < 100; ++i)
    //     for (int j = 0; j < 200; ++j)
    //       one_image.SetPixel(i, j, 150);
    //   WriteImage("output_file.pgm", an_image);
    //   // See image_demo.cc for read/write image.
    class Image {
    public:
        Image(): num_rows_{0}, num_columns_{0},
        num_gray_levels_{0}, pixels_{nullptr} { }
        
        Image(const Image &an_image);
        Image& operator=(const Image &an_image) = delete;
        
        ~Image();
        
        // Sets the size of the image to the given
        // height (num_rows) and columns (num_columns).
        void AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns);
        
        size_t num_rows() const { return num_rows_; }
        size_t num_columns() const { return num_columns_; }
        size_t num_gray_levels() const { return num_gray_levels_; }
        void SetNumberGrayLevels(size_t gray_levels) {
            num_gray_levels_ = gray_levels;
        }
        
        // Sets the pixel in the image at row i and column j
        // to a particular gray_level.
        void SetPixel(size_t i, size_t j, int gray_level) {
            if (i >= num_rows_ || j >= num_columns_) abort();
            pixels_[i][j] = gray_level;
        }
        
        int GetPixel(size_t i, size_t j) const {
            if (i >= num_rows_ || j >= num_columns_) abort();
            return pixels_[i][j];
        }
        
        
        //function to set a pixel to black or white with a threshold of 128
        void SetPixelBlackOrWhite(size_t i, size_t j, int gray_level, string threshold)
        {
            int threshold_int;
            
            stringstream(threshold)>>threshold_int;
            
            if (i >= num_rows_ || j >= num_columns_) abort();
            
            //if the pixel is greater than 128 set it to 255
            if(gray_level < threshold_int)
            {
                pixels_[i][j] = 0;
            }
            //if not, set it to 255
            else
            {
                pixels_[i][j] = 255;
            }
        }
        
        void LocateCenterAndRadius(string output_file)
        {
            int centerx = 0;
            int centery = 0;
            float radius = 0.0;
            
            unordered_multimap<int, int> xcoordycoord;
            for(int i = 0; i < num_rows(); i++)
            {
                for(int j = 0; j < num_columns(); j++)
                {
                    if(pixels_[i][j] == 255)
                    {
                        xcoordycoord.insert(pair<int, int>(i,j));
                    }
                }
            }
            
            int totalxcoord = 0;
            int totalycoord = 0;
            
            for(auto it = xcoordycoord.begin(); it != xcoordycoord.end(); it++)
            {
                totalxcoord = totalxcoord + it->first;
                totalycoord = totalycoord + it->second;
            }
            
            centerx = totalxcoord/xcoordycoord.size();
            centery = totalycoord/xcoordycoord.size();
            
            int smallestx = 2000000000;
            int smallesty = 2000000000;
            
            for(auto it = xcoordycoord.begin(); it != xcoordycoord.end(); it++)
            {
                if(it->first < smallestx)
                {
                    smallestx = it->first;
                }
                if(it->second < smallesty)
                {
                    smallesty = it->second;
                }
            }
            
            int largestx = 0;
            int largesty = 0;
            
            for(auto it = xcoordycoord.begin(); it != xcoordycoord.end(); it++)
            {
                if(it->first > largestx)
                {
                    smallestx = it->first;
                }
                if(it->second > largesty)
                {
                    smallesty = it->second;
                }
            }
            
            radius = (((largestx - smallestx)/2) + ((largesty - smallesty)/2))/2;
            
            ofstream file;
            file.open(output_file);
            
            file << centerx << " " << centery << " " << radius;
            
            file.close();
        }
        
        //find the brightest/highest pixel value and return the x coordinate of it
        double FindBrightestX()
        {
            double brightestX_value = 0;
            double brightestX = 0;
            
            for(int i = 0; i < num_rows(); i++)
            {
                for(int j = 0; j < num_columns(); j++)
                {
                    if(pixels_[i][j] > brightestX_value)
                    {
                        brightestX_value = pixels_[i][j];
                        brightestX = j;
                    }
                }
            }
            
            return brightestX;
        }
        
        //find the brightest/highest pixel value and return the y coordinate of it
        double FindBrightestY()
        {
            double brightestY_value = 0;
            double brightestY = 0;
            
            for(int i = 0; i < num_rows(); i++)
            {
                for(int j = 0; j < num_columns(); j++)
                {
                    if(pixels_[i][j] > brightestY_value)
                    {
                        brightestY_value = pixels_[i][j];
                        brightestY = i;
                    }
                }
            }
            
            return brightestY;
        }
        
        //find the brightest/highest pixel value and return value
        double FindBrightestValue()
        {
            double brightest_value = 0;
            
            for(int i = 0; i < num_rows(); i++)
            {
                for(int j = 0; j < num_columns(); j++)
                {
                    if(pixels_[i][j] > brightest_value)
                    {
                        brightest_value = pixels_[i][j];
                    }
                }
            }
            
            return brightest_value;
        }
        
        //function to find the direction vector
        void NormalVector(string parameter, string output_file, double x, double y, int brightestvalue)
        {
            double magnitude = 0;
            double centerx = 0;
            double centery = 0;
            double radius = 0;
            ifstream file;
            file.open(parameter);
            
            file >> centerx >> centery >> radius;
            
            file.close();
            
            //calculate the p,q and z for the normal vector
            double q = (y-centerx)/sqrt((radius*radius) - pow((y - centerx),2) - pow((x - centery),2));
            double p = (x-centery)/sqrt((radius*radius) - ((y-centerx)*(y-centerx)) - ((x-centery)*(x-centery)));
            double z = 1;
            
            //calculate the magnitude 
            magnitude = sqrt((p*p) + (q*q) + 1);
            
            //scaling the vector
            p = p/magnitude;
            q = q/magnitude;
            z = z/magnitude;
            
            p = p*brightestvalue;
            q = q*brightestvalue;
            z = z*brightestvalue;
            
            //write the direction vector to the file
            ofstream output(output_file, std::ios::app);
            
            output << p << " "<< q << " " << z << endl;
            
            output.close();
        }
        
        
    private:
        void DeallocateSpace();
        
        size_t num_rows_;
        size_t num_columns_;
        size_t num_gray_levels_;
        int **pixels_;
    };
    
    // Reads a pgm image from file input_filename.
    // an_image is the resulting image.
    // Returns true if  everyhing is OK, false otherwise.
    bool ReadImage(const std::string &input_filename, Image *an_image);
    
    // Writes image an_iamge into the pgm file output_filename.
    // Returns true if  everyhing is OK, false otherwise.
    bool WriteImage(const std::string &output_filename, const Image &an_image);
    
    //  Draws a line of given gray-level color from (x0,y0) to (x1,y1);
    //  an_image is the output_image.
    // IMPORTANT: (x0,y0) and (x1,y1) can lie outside the image
    //   boundaries, so SetPixel() should check the coordinates passed to it.
    void DrawLine(int x0, int y0, int x1, int y1, int color,
                  Image *an_image);
    
}  // namespace ComputerVisionProjects


namespace ComputerVisionProjects {
    
    Image::Image(const Image &an_image){
        AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
        SetNumberGrayLevels(an_image.num_gray_levels());
        
        for (size_t i = 0; i < num_rows(); ++i)
            for (size_t j = 0; j < num_columns(); ++j){
                SetPixel(i,j, an_image.GetPixel(i,j));
            }
    }
    
    Image::~Image(){
        DeallocateSpace();
    }
    
    void
    Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
        if (pixels_ != nullptr) DeallocateSpace();
        pixels_ = new int*[num_rows];
        for (size_t i = 0; i < num_rows; ++i)
            pixels_[i] = new int[num_columns];
        
        num_rows_ = num_rows;
        num_columns_ = num_columns;
    }
    
    void
    Image::DeallocateSpace() {
        for (size_t i = 0; i < num_rows_; i++)
            delete pixels_[i];
        delete pixels_;
        pixels_ = nullptr;
        num_rows_ = 0;
        num_columns_ = 0;
    }
    
    bool ReadImage(const string &filename, Image *an_image) {
        if (an_image == nullptr) abort();
        FILE *input = fopen(filename.c_str(),"rb");
        if (input == 0) {
            cout << "ReadImage: Cannot open file" << endl;
            return false;
        }
        
        // Check for the right "magic number".
        char line[1024];
        if (fread(line, 1, 3, input) != 3 || strncmp(line,"P5\n",3)) {
            fclose(input);
            cout << "ReadImage: Expected .pgm file" << endl;
            return false;
        }
        
        // Skip comments.
        do
            fgets(line, sizeof line, input);
        while(*line == '#');
        
        // Read the width and height.
        int num_columns,num_rows;
        sscanf(line,"%d %d\n", &num_columns, &num_rows);
        an_image->AllocateSpaceAndSetSize(num_rows, num_columns);
        
        
        // Read # of gray levels.
        fgets(line, sizeof line, input);
        int levels;
        sscanf(line,"%d\n", &levels);
        an_image->SetNumberGrayLevels(levels);
        
        // read pixel row by row.
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0;j < num_columns; ++j) {
                const int byte=fgetc(input);
                if (byte == EOF) {
                    fclose(input);
                    cout << "ReadImage: short file" << endl;
                    return false;
                }
                an_image->SetPixel(i, j, byte);
                //an_image->SetPixelBlackOrWhite(i, j, byte, threshold);
                
            }
        }
        
        fclose(input);
        return true;
    }
    
    bool WriteImage(const string &filename, const Image &an_image) {
        FILE *output = fopen(filename.c_str(), "w");
        if (output == 0) {
            cout << "WriteImage: cannot open file" << endl;
            return false;
        }
        const int num_rows = an_image.num_rows();
        const int num_columns = an_image.num_columns();
        const int colors = an_image.num_gray_levels();
        
        // Write the header.
        fprintf(output, "P5\n"); // Magic number.
        fprintf(output, "#\n");  // Empty comment.
        fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);
        
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_columns; ++j) {
                const int byte = an_image.GetPixel(i , j);
                if (fputc(byte,output) == EOF) {
                    fclose(output);
                    cout << "WriteImage: could not write" << endl;
                    return false;
                }
            }
        }
        
        fclose(output);
        return true;
    }
    
    // Implements the Bresenham's incremental midpoint algorithm;
    // (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
    // "Computer Graphics. Principles and practice",
    // 2nd ed., 1990, section 3.2.2);
    void
    DrawLine(int x0, int y0, int x1, int y1, int color,
             Image *an_image) {
        if (an_image == nullptr) abort();
        
#ifdef SWAP
#undef SWAP
#endif
#define SWAP(a,b) {a^=b; b^=a; a^=b;}
        
        const int DIR_X = 0;
        const int DIR_Y = 1;
        
        // Increments: East, North-East, South, South-East, North.
        int incrE,
        incrNE,
        incrS,
        incrSE,
        incrN;
        int d;         /* the D */
        int x,y;       /* running coordinates */
        int mpCase;    /* midpoint algorithm's case */
        int done;      /* set to 1 when done */
        
        int xmin = x0;
        int xmax = x1;
        int ymin = y0;
        int ymax = y1;
        
        int dx = xmax - xmin;
        int dy = ymax - ymin;
        int dir;
        
        if (dx * dx > dy * dy) {  // Horizontal scan.
            dir=DIR_X;
            if (xmax < xmin) {
                SWAP(xmin, xmax);
                SWAP(ymin , ymax);
            }
            dx = xmax - xmin;
            dy = ymax - ymin;
            
            if (dy >= 0) {
                mpCase = 1;
                d = 2 * dy - dx;
            } else {
                mpCase = 2;
                d = 2 * dy + dx;
            }
            
            incrNE = 2 * (dy - dx);
            incrE = 2 * dy;
            incrSE = 2 * (dy + dx);
        } else {// vertical scan.
            dir = DIR_Y;
            if (ymax < ymin) {
                SWAP(xmin, xmax);
                SWAP(ymin, ymax);
            }
            dx = xmax - xmin;
            dy = ymax-ymin;
            
            if (dx >=0 ) {
                mpCase = 1;
                d = 2 * dx - dy;
            } else {
                mpCase = 2;
                d = 2 * dx + dy;
            }
            
            incrNE = 2 * (dx - dy);
            incrE = 2 * dx;
            incrSE = 2 * (dx + dy);
        }
        
        /// Start the scan.
        x = xmin;
        y = ymin;
        done = 0;
        
        while (!done) {
            an_image->SetPixel(x,y,color);
            
            // Move to the next point.
            switch(dir) {
                case DIR_X:
                    if (x < xmax) {
                        switch(mpCase) {
                            case 1:
                                if (d <= 0) {
                                    d += incrE;
                                    x++;
                                } else {
                                    d += incrNE;
                                    x++;
                                    y++;
                                }
                                break;
                                
                            case 2:
                                if (d <= 0) {
                                    d += incrSE;
                                    x++;
                                    y--;
                                } else {
                                    d += incrE;
                                    x++;
                                }
                                break;
                        }
                    } else {
                        done=1;
                    }
                    break;
                    
                case DIR_Y:
                    if (y < ymax) {
                        switch(mpCase) {
                            case 1:
                                if (d <= 0) {
                                    d += incrE;
                                    y++;
                                } else {
                                    d += incrNE;
                                    y++;
                                    x++;
                                }
                                break;
                                
                            case 2:
                                if (d <= 0) {
                                    d += incrSE;
                                    y++;
                                    x--;
                                } else {
                                    d += incrE;
                                    y++;
                                }
                                break;
                        } // mpCase
                    } // y < ymin
                    else {
                        done=1;
                    }
                    break;
            }
        }
    }
    
}  // namespace ComputerVisionProjects



using namespace ComputerVisionProjects;

int
main(int argc, char **argv){
    
    if (argc!=6) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 0;
    }
    
    const string input_parameter_file(argv[1]);
    const string input_file1(argv[2]);
    const string input_file2(argv[3]);
    const string input_file3(argv[4]);
    const string output_file(argv[5]);
    
    
    Image an_image;
    if (!ReadImage(input_file1, &an_image)) {
        cout <<"Can't open file " << input_file1 << endl;
        return 0;
    }
    
    Image an_image2;
    if (!ReadImage(input_file2, &an_image2)) {
        cout <<"Can't open file " << input_file2 << endl;
        return 0;
    }
    
    Image an_image3;
    if (!ReadImage(input_file3, &an_image3)) {
        cout <<"Can't open file " << input_file3 << endl;
        return 0;
    }
    
    an_image.NormalVector(input_parameter_file, output_file, an_image.FindBrightestX(), an_image.FindBrightestY(), an_image.FindBrightestValue());
    
    an_image2.NormalVector(input_parameter_file, output_file, an_image2.FindBrightestX(), an_image2.FindBrightestY(), an_image2.FindBrightestValue());
    
    an_image3.NormalVector(input_parameter_file, output_file, an_image3.FindBrightestX(), an_image3.FindBrightestY(), an_image3.FindBrightestValue());
    
    
    
    /*if (!WriteImage(output_file, an_image)){
     cout << "Can't write to file " << output_file << endl;
     return 0;
     }*/
}
