#include <iostream>
#include <string>
#include "Image.h"

using namespace imaging;

using namespace std;

int main(int argc, char * argv[]) {

	string imageName; //This variable will save the files name.

	if (argc > 1) { // If the argc>1, then there are arguments during the running of the programm.
		imageName=argv[1] ; // The argument (image name) is the second element of the table. First element is the programm name.
	}
	else { //Else (arguments=1), then there aren't arguments.
		cout << "File name of the image to load : "; //Print a prompt for user.
		getline(cin, imageName); //User give an image file.
	}
	

	Image image; //Create an Image with defualt constructor.
	bool readImage = image.load(imageName, "ppm"); //Call the load function.
	
	if (readImage) { //If the readImage==TRUE that means, everything went well.
		cout << "\n"; //Print an empty line.

		cout <<"Image dimensions are: " << image.getWidth() << " X " << image.getHeight() << endl; /* Print the dimensions. The load function has set
																									  the width,height and the buffer of the image. */
		
		/* The folowing are about the negative of the image*/

		int found = imageName.find_last_of(".");  /* Find the last ".", with the help of "find_last_of()" function which it finds the last "."
												     and return the size from the rest string(filename), until the start. */

		string extension = imageName.substr(found + 1); //Store the substring, from the "." to the end of string(i.e.extension). This is the format of image.

		string filenameNeg = imageName.substr(0, found); //Find the filename without extension, and save it in a string variable.	
		filenameNeg.append("_neg." + extension); //Add the ending "_neg" to the end of the name and then add the extension(.ppm). Now the filename has the "_neg" at the end.
		
									 
		Image normalImage = image; /* This line doesn't change anything, just using operator "=".
									  Create a new object Image which has got exactly the same width,height and buffer (values of buffer) with the first image. */
		 
		int negativeImageWidth = normalImage.getWidth(); //Use the getWidth() to take the height of the image and create a new variable for the negative image.
		int negativeImageHeight = normalImage.getHeight(); //Use the getHeight() to take the height of the image and create a new variable for the negative image.

		int negativeBufferSize = negativeImageWidth * negativeImageHeight; //Calculate the suitable size for the new array (pNegative).
		Color *pNegative = new Color[negativeBufferSize]; //Create an array which hold the negative values of the image.

		Color WHITE = Color(1, 1, 1); //Create the white color.

		for (int i_Buffer = 0; i_Buffer < negativeBufferSize; i_Buffer++) { //A loop for the buffer.
			
			//Fill the pNegative array by subtraction of the color, of each pixel, of the white color. Use the getPixel to take this specific color.Use the operator "-".
			pNegative[i_Buffer] = WHITE - normalImage.getPixel(i_Buffer / negativeImageWidth, i_Buffer % negativeImageWidth);
			
		}

		Image negativeImage = Image(negativeImageWidth, negativeImageHeight, pNegative); /* Call one of the constructors and pass like arguments the width and the heigth of the image 
																						    and the Color array which has got the negative colors */
																									
		bool writeImage = negativeImage.save(filenameNeg, "ppm"); //Call the save function.
		
		delete[] pNegative; //Delete the pNegative array, because the program does not need it anymore.

		if (writeImage) { //If the writeImage==TRUE that means, everything went well.
			cout << "Successfully created negative image to the same path!" << endl; //Print the successfully message.
		}

		cout <<"\n"; //Print an empty line.
	}

	system("PAUSE");
	return 0;
}