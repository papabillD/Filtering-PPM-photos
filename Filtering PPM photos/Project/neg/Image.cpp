#include <iostream>
#include "Image.h"
#include "..\ppm\ppm.h"

using namespace std;
using namespace imaging;



Color *Image::getRawDataPtr() {
	return this->buffer; //Return the pointer to the internal data.
}


Color Image::getPixel(unsigned int x, unsigned int y) const {

	if (x > this->getHeight() || y > this->getWidth() || !(this->buffer)) { //Check the buffer and if x or y is out of bounds.	
		return Color(); //Call the default constructor. Return a black (0,0,0) color.
	}
	//else the bounds are correct. So continue.

	int index = (x * this->getWidth()) + y; //Find the index of buffer. 	
	return this->buffer[index]; //Return the color of the image at location (x,y) like an object Color.

}


void Image::setPixel(unsigned int x, unsigned int y, Color & value) {

	if (x > this->getHeight() || y > this->getWidth() || !(this->buffer)) { //Check the buffer and if x or y is out of bounds.			
		return; //Do nothing
	}
	//else the bounds are correct. So continue.

	int index = (x * this->getWidth()) + y; //Find the index of buffer. 	
	this->buffer[index] = value; //Put in the index-th element of buffer the value which is a Color object.

}


void Image::setData(const Color * & data_ptr) {
	
	if (this->getWidth() == 0 || this->getHeight() == 0 || !(this->buffer)) { /*Check if the width or the height is 0 or the buffer isn't allocated. 
																			    (!(this.buffer) is the same check with (this.buffer==nullptr)) */
		return;
	}
	//else the (width or height)!=0 or buffer!=nullptr. So continue.

	int size = this->getWidth() * this->getHeight(); //The size of buffer is width * height=		
	for (int i = 0; i < size; i++) {  //A loop for the buffer size 
		this->buffer[i] = data_ptr[i]; //And copying data_ptr values to our buffer
	}

}


//The default constructor initialized the width and the height with zero and the buffer with nullptr.
Image::Image() {

	this->width = 0; //The width equal zero.
	this->height = 0; //The height equal zero.
	this->buffer = nullptr; //The buffer equal nullptr.

}


/*This constructor initialized the width and the height with the parameters and calculate 
  the required size and create the buffer with this size*/
Image::Image(unsigned int width, unsigned int height) {

	this->width = width; //Do the width equal with the given width.
	this->height = height; //Do the height equal with the given height.

	int size = width * height; //The size must be the result from width * height. 
	this->buffer= new Color [size]; //The buffer now has got the suitable size.
	
}


/*This constructor initialized the width and the height with the parameters(width,height), calculate 
  the required size and create the buffer with this size. Also it calls the setData function with data_ptr like argument */
Image::Image(unsigned int width, unsigned int height, const Color * data_ptr) {

	this->width = width; //Do the width equal with the given width.
	this->height = height; //Do the height equal with the given height.

	int size = width * height; //The size must be the result from width * height. 
	this->buffer = new Color[size];//The buffer now has got the suitable size.

	setData(data_ptr); //Call the setData function.

}


/*This constructor replicate this object with src which is the source image. Initialized the width and the height 
  with the image and the height of src, which is an Image object, calculate the required size and create the buffer 
  with this size. Also replicate the data of src buffer.*/
Image::Image(const Image &src) {

	this->width = src.width; //Do the width equal with the src width.
	this->height = src.height; //Do the height equal with src height.

	int size = width * height; //The size must be the result from width * height. 
	this->buffer = new Color[size];//The buffer now has got the suitable size.
	
	for (int i = 0; i < size; i++) {   //OR setData(src.getRawDataPtr); //A loop for the buffer size 
		this->buffer[i] = src.buffer[i]; //And copying the data of src:Image to our buffer.
	}

}


//This is a destructor for the buffer.
Image::~Image() {
	delete [] buffer;
}


Image & Image::operator = (const Image & right) {
	
	this->width = right.width; //Do the width equal with the width which has got the right side of the operator = .
	this->height = right.height; //Do the height equal with the height which has got the right side of the operator = .

	int size = width * height; //The size must be the result from width * height. 
	delete[] buffer; //Delete the old buffer.
	this->buffer = new Color[size];//The buffer now has got the suitable size.

	for (int i = 0; i < size; i++) {   //OR setData(src.getRawDataPtr); //A loop for the buffer size 
		this->buffer[i] = right.buffer[i]; //And copying the data of src:Image to our buffer.
	}


	return *this; //Return the current object.

}


bool Image::load(const string & filename, const string & format) {
	
	string extension; //A string which store the extension of filename.

	const char *formatCStr = format.c_str(); //Create a pointer which shows to an array of characters for the string format.
	
	if (_strcmpi(formatCStr, "ppm") != 0) { //Check if the format is ppm (case-insensitive), because this program cares about ppm format, so if it isn't the program stops immediately.
		cerr << "ERROR! The format isn't ppm (case-insensitive). This program manages only images with ppm format." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else format=ppm or PPM, so continue.
	
	int found = filename.find_last_of("."); /* Find the last ".", with the help of "find_last_of()" function which it finds the last "." 
											  and return the size from the rest string(filename), until the start. */

	if (found < 0) { //If found < 0 stop immediately.
		cerr << "ERROR! The name of file hasn't got extension." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else found>0, so continue.

	if (filename.substr(0, found).size() < 1) { //Check if the name of file,before the extension, is null(size<1).
		cerr << "ERROR! Too short name." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else the name of file without extension is normal, so continue.

	extension = filename.substr(found + 1);	//Store the substring, from the "." to the end of string (i.e. extension).

	const char *extensionCStr = extension.c_str(); //Create a pointer which shows to an array of characters for the string extension.	

	if (_strcmpi(extensionCStr, formatCStr) != 0) { //Check if extensionCStr and formatCStr are not equal, and with help of _strcmpi this comparison is case-insensitive.
		cerr << "ERROR! The extension of the file isn't ppm (case-insensitive)." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else extension and format are equal, so continue.

	const char *fileNameCStr = filename.c_str(); //Create a pointer which shows to an array of characters for the string filename.

	if (this->buffer) {  //Check if the buffer has got data.
		delete[] buffer; //Delete the buffer.
	}
	//else the buffer is empty, so continue.

	int newWidth = 0; //Create a new variable newWidth which initialized with zero.
	int newHeight = 0; //Create a new variable newHeight which initialized with zero.
	
	float * newImageData = ReadPPM(fileNameCStr, &newWidth, &newHeight); //Call the function ReadPPM from library ppm.
	
	if (newImageData == nullptr) { //Check if the read of the image,went wrong.
		cerr << "Failed to read data from the image!" << endl; //An error message.
		return false; //Stop immediately.
	}
	//else newImageData!=nullptr. Successfully read. So continue.

	this->width = (unsigned int)newWidth; //Cast int (newWidth) to unsigned int and equalize it with the object width.
	this->height = (unsigned int)newHeight; //Cast int (newHeight) to unsigned int and equalize it with the object height.

	int bufferSize = this->getWidth() * this->getHeight(); //Calculate the buffer size with the help of width and height.
	buffer = new Color[bufferSize]; //Create a new buffer, with the suitable size.

	Color temporary; //Call the default constructor from class Color.

	for (int i_Buffer = 0; i_Buffer < bufferSize; i_Buffer++) { //A loop for the buffer.

		/* Use the temporary Color and the operator []. Also with the newImageData values create the r,g,b values for temporary.
		Use the setPixel to fill the buffer with the specific color value which hold the temporary. For each loop change this value. */
		for (int j_Triplet = 0; j_Triplet < 3; j_Triplet++) { //j_Triplet variable shows, the one of the three components(r,g,b). 

			temporary[j_Triplet] = newImageData[(i_Buffer * 3) + j_Triplet];

		}
		setPixel(i_Buffer / this->getWidth(), i_Buffer % this->getWidth(), temporary);

	}

	delete[] newImageData; //Delete the newImageData array, because the program does not need it anymore.

	return true; //Return true because everything went well.

}


bool Image::save(const string & filename, const string & format) {

	string extension; //A string which store the extension of filename.

	const char *formatCStr = format.c_str(); //Create a pointer which shows to an array of characters for the string format.

	if (_strcmpi(formatCStr, "ppm") != 0) { //Check if the format is ppm (case-insensitive), because this program cares about ppm format, so if it isn't the program stops immediately.
		cerr << "ERROR! The format isn't ppm (case-insensitive). This program manages only images with ppm format." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else format=ppm or PPM, so continue.

	int found = filename.find_last_of("."); /* Find the last ".", with the help of "find_last_of()" function which it finds the last "."
											and return the size from the rest string(filename), until the start. */

	if (found < 0) { //If found < 0 stop immediately.
		cerr << "ERROR! The name of file hasn't got extension." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else found>0, so continue.

	if (filename.substr(0, found).size() < 1) { //Check if the name of file,before the extension, is null(size<1).
		cerr << "ERROR! Too short name." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else the name of file without extension is normal, so continue.

	extension = filename.substr(found + 1);	//Store the substring, from the "." to the end of string (i.e. extension).

	const char *extensionCStr = extension.c_str(); //Create a pointer which shows to an array of characters for the string extension.	

	if (_strcmpi(extensionCStr, formatCStr) != 0) { //Check if extensionCStr and formatCStr are not equal, and with help of _strcmpi this comparison is case-insensitive.
		cerr << "ERROR! The extension of the file isn't ppm (case-insensitive)." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else extension and format are equal, so continue.
	
	if (this->getWidth() == 0 || this->getHeight() == 0 || !(this->buffer)) {
		cerr << "ERROR! The Image object is not initialized." << endl; //An error message.
		return false; //Stop immediately.
	}
	//else width and height >0 and buffer!=nullptr, so continue.
		
	int imageWidth = this->getWidth(); //Store the width of image in a variable, using the funcion getWidth(). 
	int imageHeight = this->getHeight(); //Store the height of image in a variable, using the funcion getHeight(). 

	int bufferSize = imageWidth * imageHeight; //Calculate the buffer size (width*height).
	int imageDataSize = bufferSize * 3; //Calculate the size of the image data (width*height*3).

	float *imageData = new float[imageDataSize]; //Create a float array, with zise imageDataSize.
	
	for (int i_Buffer = 0; i_Buffer < bufferSize; i_Buffer++) { //A loop for the buffer.
		
		/* Call the getPixel to get the Color object which is located to a specific point (x,y).
		Pass like x:(i_Buffer / width) and like y:(i_Buffer % width). Store the specific component (r or g or b)
		in imageData array. 3 cells of imagaData correspond to 1 Color object (to 1 cell of buffer). */
		for (int j_Triplet = 0; j_Triplet<3; j_Triplet++) { //j_Triplet variable shows, the one of the three components(r,g,b).

			imageData[(i_Buffer * 3)+ j_Triplet] = getPixel(i_Buffer / imageWidth, i_Buffer % imageWidth)[j_Triplet]; //Use the operator [].
			
		}
	
	}
	
	const char *filenameCStr = filename.c_str(); //Create a pointer which shows to an array of characters for the string filename.
	bool writeProgress = WritePPM(imageData, imageWidth, imageHeight, filenameCStr); //Call the function WritePPM from library ppm.

	delete[] imageData; //Delete the imageData array, because the program does not need it anymore.

	if (!writeProgress) { //Check if the write of the image,went wrong.
		cerr << "Failed to write data to the image!" << endl; //An error message.
		return false; //Stop immediately.
	}
	//else writeProgress==true. Successfully write. So continue.

	return true;

}

