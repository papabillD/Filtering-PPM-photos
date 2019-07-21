#include <iostream>
#include <fstream>
#include <string>
#include "ppm.h"


using namespace std;

const string FORMAT = "P6"; //A const for the supported format. Only P6.
const int MAX_STORE_VALUE = 255; //A const for the max store value. Only 255.
const string NEW_LINE = "\n"; //A const for the new line.

float * imaging::ReadPPM(const char * filename, int * w, int * h) {

	ifstream imageFile;
	imageFile.open(filename, ios::in | ios::binary); //Open the file with the given filename (for in).

	if (!imageFile.is_open()) { //Check if the file doesn't exist or it doesn't open.
		
		cerr << "File could not be opened!" << endl; //An error message.
		return nullptr; //Stop immediately.

	}
	//else the file open, so continue.
	
	string format; //Must be "P6"
	int maxStoreValue; //Must be 255.
	
	imageFile >> format >> *w >> *h >> maxStoreValue ; //Read the header.
	
	if (format.compare(FORMAT)!=0) { //Check if the format (first part of the header) is the supported format.

		cerr << "The format is not P6!" << endl; //An error message.
		return nullptr; //Stop immediately.

	}//else the format is P6, so continue.

	else if (*w < 0 || *h < 0 || maxStoreValue <0) { //Check if the width or the height or the maxStoreValue are less than zero.

		cerr << "Missing parts from the header (Width or Height or Max Store Value)!" << endl; //An error message.
		return nullptr; //Stop immediately.

	}//else the width and the height are correct, so continue.

	else if (*w == 0) { //Check if the width is equal to zero.

		cerr << "The width is equal to 0!" << endl; //An error message. 
		return nullptr; //Stop immediately.

	}//else the width!=0, so continue.

	else if (*h == 0) { //Check if the height is equal to zero.

		cerr << "The height is equal to 0!" << endl; //An error message.
		return nullptr; //Stop immediately.

	}//else the height!=0, so continue.

	else if (maxStoreValue > MAX_STORE_VALUE || maxStoreValue == 0) { //Check if the maxStoreValue is more than 255 or is equal to zero.

		cerr << "The max store value must be in space -> (0,255]!" << endl; //An error message.
		return nullptr; //Stop immediately.

	}//else 0 < maxStoreValue <= 255, so continue.

	/*The header is good,so continue*/

	int dataBufferSize = ((*w) * (*h)) * 3; //Calculate the size of the buffer which contains 3 values per pixels. So the size must be (width*height*3).
	float *tmpBuffer = new float[dataBufferSize]; //Create the buffer.

	imageFile.get(); //This get() return the space or the '\n' between the header and the data.

	unsigned char readerForTheNextComponentT; //An unsigned char variable, "readerForTheNextComponentT" holds in each loop the value of the component_t.

	for (int i_Triplet = 0; i_Triplet < dataBufferSize; i_Triplet = i_Triplet + 3) { //Scan the buffer with step 3. Each cell of the array holds a value for color channel(r,g,b).

		for (int j_Component = i_Triplet; j_Component < i_Triplet + 3; j_Component++) { //An internal loop which scan the i+3 cells.
			readerForTheNextComponentT = (unsigned char) imageFile.get(); //Read the next component from the image and cast it (byte to unsigned char).
			tmpBuffer[j_Component] = (float) readerForTheNextComponentT / maxStoreValue; //Cast unsigned char (readerForTheNextComponentT) to float and divide it by the maxStoreValue(=255).(interval -> [0.0,1.0])	
		}
		
	}

	imageFile.close(); //Close the file.

	return tmpBuffer; //Return the buffer which hold the data.
	
}


bool imaging::WritePPM(const float * data, int w, int h, const char * filename) {

	if (data == nullptr) { //Check if the data parameter is nullptr.
		return false; //Stop immediately.
	}

	if (w <= 0 || h <= 0) { //Check if the w or h parameter is 0.
		return false; //Stop immediately.
	}

	ofstream negativeImageFile;
	negativeImageFile.open(filename, ios::out | ios::binary); //Open the file with the given filename (for out).

	if (!negativeImageFile.is_open()) { //Check if the file doesn't open.

		cerr << "File could not be opened!" << endl; //An error message.
		return false; //Stop immediately.

	}
	//else the file open, so continue.
	
	negativeImageFile << FORMAT << NEW_LINE << w << NEW_LINE << h << NEW_LINE << MAX_STORE_VALUE << NEW_LINE; //Write in the file the header. After the parts write a new line (='\n').

	int dataSize = w * h * 3; //Calculate the buffer size.

	unsigned char * dataChar = new unsigned char[dataSize]; //Create a char array, which contains the data like chars.

	for (int i = 0; i < dataSize; i++) { //A loop for the data.
		dataChar[i] = data[i] * (float) MAX_STORE_VALUE; //Fill the dataChar array with the values of the data array multiplied with the (cast float) max store value (=255.0f).
	}

	negativeImageFile.write((char *)dataChar,dataSize); //Write the data in the file.
	negativeImageFile.close(); //Close the file.

	delete[] dataChar; //Delete the dataChar array, because the program does not need it anymore.

	return true; //Return true because everything went well.

}