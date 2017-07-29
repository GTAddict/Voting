#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>

#define M_INVALID_NAME "\\%\'"
#define F_TOKEN_FROM "From: "
#define F_TOKEN_DEVICE "Device: "
#define F_TOKEN_DATE "Date: "
#define M_TOKEN_COLUMNS "FROM;DEVICE;DATE;DATA;\n"


void RenameFileWithExtension(const char* inputFilename, char* outputFilename)
{
		const char ext[5] = ".ssv";
		
		int len = strlen(inputFilename);
		int i = len;
		
		/* Why are we going through all this trouble instead of
			just copying until a '.' is encountered? Because file
			names can consist of periods, such as a.b.cpp. File
			name would be incorrect if we copied only up to the 
			first period. So we go from the end to find the last
			period and copy till there.						*/

		for (i = len-1; inputFilename[i]!='.' && i>=0; i--);
		if (i == -1)						// Extensionless file (no period was found)
		{
			strcpy(outputFilename, inputFilename);
			strcat(outputFilename, ext);
		}
		else
		{
			int j = 0;
			for (j = 0; j<i; j++)
				outputFilename[j] = inputFilename[j];
			outputFilename[j] = '\0';
			strcat(outputFilename, ext);
		}
}

void GetFileNames(const int argc, char* argv[], char* INPUTNAME, char* OUTPUTNAME)
{
	strcpy(INPUTNAME, M_INVALID_NAME);
	strcpy(OUTPUTNAME, M_INVALID_NAME);
	if (argc == 3)						// Both input and output file names are specified
	{
		strcpy(INPUTNAME, argv[1]);
		strcpy(OUTPUTNAME, argv[2]);
	}

	else if (argc == 2)					// Only input filename is specified
	{
		strcpy(INPUTNAME, argv[1]);
		RenameFileWithExtension(INPUTNAME, OUTPUTNAME);
	}

	else								// Nothing is specified
	{
		std::cout<<"Enter input file: ";
		std::cout.flush();
		std::cin>>INPUTNAME;		// Mandatory
		std::cin.ignore(INT_MAX, '\n');
		std::cout<<"Enter output file (Press enter to keep the same name): ";
		std::cout.flush();
		std::cin.getline(OUTPUTNAME, 255, '\n');
		if (strcpy(OUTPUTNAME, M_INVALID_NAME) != 0)
		{
			RenameFileWithExtension(INPUTNAME, OUTPUTNAME);
		}
	}
}

int CreateFileHandles(std::ifstream& inputHandle, std::ofstream& outputHandle, const char* INPUTNAME, const char* OUTPUTNAME)
{
		/*	Create handles	*/
	inputHandle.open(INPUTNAME, std::ios::in);
	if (!inputHandle)
	{
		std::cout<<"Error: Unable to open specified input file: "<<GetLastError()<<"\n";
		std::cout.flush();
		return -1;
	}
	outputHandle.open(OUTPUTNAME, std::ios::out|std::ios::trunc);
	if (!outputHandle)
	{
		std::cout<<"Error: Unable to open specified output file: "<<GetLastError()<<"\n";
		std::cout.flush();
		return -1;
	}

	inputHandle.seekg(0, std::ios::beg);

	return 0;
	
}

char* ParseFile(std::ifstream& inputHandle)
{
	char* TemporaryProcessString;
	char* FileTextBuffer;
	int TempCount = 0;
	char* PtrToCurrentToken;
	char* NextEmptySpaceInBuffer;
	
	// Find size of file and allocate it
	inputHandle.seekg(0, std::ios::end);
	long FileLength = inputHandle.tellg();

	FileTextBuffer = new char[FileLength];
	TemporaryProcessString = new char[FileLength];
	strcpy(FileTextBuffer, M_TOKEN_COLUMNS);

	NextEmptySpaceInBuffer = &FileTextBuffer[strlen(FileTextBuffer)];

	// Seek back to the beginning
	inputHandle.seekg(0, std::ios::beg);

	int i = 0;

	while (inputHandle)

	{

		i = 0;
	
		do						// Loop for FROM
		{
			do
			{
				inputHandle.get(TemporaryProcessString[i]);
			} while (TemporaryProcessString[i++] != '\n' && inputHandle);
			if (TemporaryProcessString[i-1] == '\n')
				--i;
			TemporaryProcessString[i++] = '\0';
			PtrToCurrentToken = strstr(TemporaryProcessString, F_TOKEN_FROM);
		} while ((PtrToCurrentToken == NULL) && inputHandle);

		if (PtrToCurrentToken != NULL)
		{
			PtrToCurrentToken += strlen(F_TOKEN_FROM);
			for (int i = 0; *PtrToCurrentToken != '\0'; i++)
				*NextEmptySpaceInBuffer++ = *PtrToCurrentToken++;
								// FROM is done
		}
	
		*NextEmptySpaceInBuffer++ = ';';	// Delimiter

		if (!inputHandle)
			break;

		i = 0;

		do						// Loop for DEVICE
		{
			do
			{
				inputHandle.get(TemporaryProcessString[i]);
			} while (TemporaryProcessString[i++] != '\n' && inputHandle);
			if (TemporaryProcessString[i-1] == '\n')
				--i;
			TemporaryProcessString[i++] = '\0';
			PtrToCurrentToken = strstr(TemporaryProcessString, F_TOKEN_DEVICE);
		} while ((PtrToCurrentToken == NULL) && inputHandle);

		if (PtrToCurrentToken != NULL)
		{
			PtrToCurrentToken += strlen(F_TOKEN_DEVICE);
			for (int i = 0; *PtrToCurrentToken != '\0'; i++)
				*NextEmptySpaceInBuffer++ = *PtrToCurrentToken++;
								// DEVICE is done
		}

		*NextEmptySpaceInBuffer++ = ';';	// Delimiter

		if (!inputHandle)
			break;

		i = 0;

		do						// Loop for DATE
		{
			do
			{
				inputHandle.get(TemporaryProcessString[i]);
			} while (TemporaryProcessString[i++] != '\n' && inputHandle);
			if (TemporaryProcessString[i-1] == '\n')
				--i;
			TemporaryProcessString[i++] = '\0';
			PtrToCurrentToken = strstr(TemporaryProcessString, F_TOKEN_DATE);
		} while ((PtrToCurrentToken == NULL) && inputHandle);

		if (PtrToCurrentToken != NULL)
		{
			PtrToCurrentToken += strlen(F_TOKEN_DATE);
			for (int i = 0; *PtrToCurrentToken != '\0'; i++)
				*NextEmptySpaceInBuffer++ = *PtrToCurrentToken++;
								// DATE is done
		}

		*NextEmptySpaceInBuffer++ = ';';	// Delimiter

		char* CopyFromTemporaryString;
		long position;

		while (inputHandle)				// Loop for DATA
		{

			i = 0;

			position = inputHandle.tellg();

			do
			{
				inputHandle.get(TemporaryProcessString[i]);
			} while (TemporaryProcessString[i++] != '\n' && inputHandle);
			if (TemporaryProcessString[i-1] == '\n')
				--i;
			TemporaryProcessString[i++] = '\0';
			PtrToCurrentToken = strstr(TemporaryProcessString, F_TOKEN_FROM);
			if (PtrToCurrentToken != NULL)
			{
				inputHandle.seekg(position, std::ios::beg);
				break;
			}
			for (CopyFromTemporaryString = TemporaryProcessString; *CopyFromTemporaryString != '\0'; )
				*NextEmptySpaceInBuffer++ = *CopyFromTemporaryString++;
		}						// DATA is done

		*NextEmptySpaceInBuffer++ = ';';			// Delimiter
		*NextEmptySpaceInBuffer++ = '\n';			// Newline after completion of one set

	}

	*NextEmptySpaceInBuffer = '\0';				// EOF

	delete TemporaryProcessString;

	return FileTextBuffer;										// Success

}

int WriteToFile(std::ofstream& outputHandle, const char* ProcessedTextToWrite)
{
	int BytesToWrite = strlen(ProcessedTextToWrite);
	int BytesWritten = 0;
	while (*ProcessedTextToWrite != '\0')
	{
		outputHandle.put(*ProcessedTextToWrite++);
		BytesWritten++;
	}
	if (BytesToWrite != BytesWritten)
	{
		std::cout<<"Error: File was not written successfully.\nError code:"<<GetLastError()<<std::endl;
		std::cout<<"Please contact your developer for support.\n";
		std::cout.flush();
		return -1;
	}
	else
		return 0;
}