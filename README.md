This contains 4 programs, s1, s2, s3, and s4.

s1 - This program converts a grayscale image into a binary image to locate the object in the image, and computes for the object's center coordinates (x,y) and radius.

s2 - This program uses the output from s1, to solve for the direction vectors of each object in each image.

s3 - This program uses the output from s2 and computes for the surface normal, which is then used to create a needle map of the image.

s4 - This program uses the output from s2 and computes for the surface normals, which is then used to compute for the albedo. The albedo is then translated to pixel value and inputed into the output image.


----------------------
To compile/clean in Linux:
----------
 
   make all

   make clean

To run:
---------

s1 - ./s1 {input original image} {input threshold value} {output parameters file}

s2 - ./s2 {input parameters} {image 1} {image 2} {image 3} {output directions}

s3 - ./s3 {input directions} {image 1} {image 2} {image 3} {threshold} {output}

s4 - ./s4 {input directions} {image 1} {image 2} {image 3} {step} {threshold} {output}


*****
Threshold used during testing: 128
Step used during test: 10

Formulas used:
Center coordinates: TotalX/# Of X   TotalY/# Of Y
Radius: [ (largestX - smallestX)/2 + (largestY - smallestY)/2 ] / 2
Normal q: (y-center)/sqrt(r^2 - (y-center)^2 - (x-center)^2
Normal p: (x-center)/sqrt(r^2 - (y-center)^2 - (x-center)^2
Surface Normal: pn = S^-1 [I1 I2 I3] = N
Albedo: p = |N|

Please include your own .pgm files and/or .txt file when running any of the programs above or use the provided ones if you wish.

After running s2 to create the directions.txt file. please delete the last empty line in the .txt file.
*****
-----------
