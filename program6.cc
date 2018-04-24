//Program6: MAIN FILE
//PROGRAMMED BY: NICHOLAS STEELE (NSS150130)
//CS3377.501

//LIBRARIES
#include <iostream>
#include "cdk.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdint.h>

//DEFINE MATRIX PROPERTIES
#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 30
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

const int maxRecordStringLength = 25;

//class for binary header information
class BinaryFileHeader
{
  public:
     uint32_t magicNumber;
     uint32_t versionNumber;
     uint64_t numRecords;
};

//class for binary file records
class BinaryFileRecord
{
  public:
  	uint8_t strLength;
	char stringBuffer[maxRecordStringLength];
};

int main()
{

  //MATRIX CODE
  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;          
  const char 		*rowTitles[] = {"a", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"a", "a", "b", "c", "d", "e"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};
  window = initscr();
  cdkscreen = initCDKScreen(window);
  initCDKColor();
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);
  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  drawCDKMatrix(myMatrix, true);

  //create pointers to class objects 
  BinaryFileRecord *myRecord = new BinaryFileRecord();
  BinaryFileHeader *binHeader = new BinaryFileHeader();

  //open binary file
  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);
  if(binInfile)
  {
  	//read binary file contents
  	binInfile.read((char*)binHeader, sizeof(BinaryFileHeader));
	stringstream ss1, ss2, ss3;
	
	//create string streams and fill them with the binary file header data
	//find the hex value of the magicNumber and print it to the correct matrix cell
	ss1  << "Magic: 0x" << hex << binHeader->magicNumber << endl;
  	string s = ss1.str();
	setCDKMatrixCell(myMatrix, 1, 1, s.c_str());;

	//get the version and print it to the correct matrix cell
	ss2 << "Version: "<< binHeader->versionNumber << endl;
  	s = ss2.str();
	setCDKMatrixCell(myMatrix, 1, 2, s.c_str());

	//get the number of records and print it to the correct matrix cell
	ss3 << "numRecords= " << binHeader->numRecords << endl;
	s = ss3.str();
	setCDKMatrixCell(myMatrix, 1, 3, s.c_str());
	
	//correct variables for the rows and columns in the matrix
	int row = 2, col = 1;

	//while the file is not at the end
	while(!binInfile.eof())
	{
	   //read the binary file records 
	   binInfile.read((char*)myRecord, sizeof(BinaryFileRecord));	
 	   stringstream ss4, ss5;

	   //save the string values in the file to the string buffers in the record class
	   //print the string to the correct matrix cell
	   ss4 << "string: " << myRecord->stringBuffer << endl;
	   string s = ss4.str();
	   setCDKMatrixCell(myMatrix, row, col+1, s.c_str());
	    
	   //find the string length of the string buffers in the record class
	   //print the strlen to the correct matrix cell
	   ss5 << "strlen: " << strlen(myRecord->stringBuffer)<< endl;
	   s = ss5.str();
	   setCDKMatrixCell(myMatrix, row, col, s.c_str());

	   //increment the row
	   row++;
	}
	//close the file
 	binInfile.close();
 }
 else 
 	cout<< "ERROR: cannot find binary file" << endl;

  //create the matrix and exit when the user hits a key
  drawCDKMatrix(myMatrix, true);    
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
  return 0;
}
