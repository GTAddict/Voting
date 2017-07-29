#include"main.h"

void CleanUpAndExit(char* ProcessedTextBuffer, std::ifstream& fHandle_input, std::ofstream& fhandle_output, int ret_code)
{
	if (ProcessedTextBuffer != NULL)
		delete ProcessedTextBuffer;
	fhandle_output.close();
	fHandle_input.close();
	system("PAUSE");
	exit(ret_code);
}

int main(int argc, char* argv[])
{

	int bRet = 0;
	
	char INPUTNAME[255] = "";
	char OUTPUTNAME[255] = "";

	char* ProcessedTextBuffer;
	ProcessedTextBuffer = NULL;

	std::ifstream fHandle_input;
	std::ofstream fHandle_output;

		/*	Get input and output file names	*/
	GetFileNames(argc, argv, INPUTNAME, OUTPUTNAME);

		/*	Create file handles	*/
	if ((bRet = CreateFileHandles(fHandle_input, fHandle_output, INPUTNAME, OUTPUTNAME)) != 0)
		CleanUpAndExit(NULL, fHandle_input, fHandle_output, bRet);
		
		/* Main part: Parse the file */
	if ((ProcessedTextBuffer = ParseFile(fHandle_input)) == NULL)
		CleanUpAndExit(NULL, fHandle_input, fHandle_output, -2);

	if ((bRet = WriteToFile(fHandle_output, ProcessedTextBuffer)) != 0)
		CleanUpAndExit(NULL, fHandle_input, fHandle_output, bRet);
	
	std::cout<<"File was successfully created.\n";
	
	CleanUpAndExit(NULL, fHandle_input, fHandle_output, 0);		// Success

}