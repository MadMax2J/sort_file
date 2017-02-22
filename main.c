//Access to Standard Library functions
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

//Preprocessor constants
#define INPUT_BUF 255   //User input buffer used for filename.

//Function Prototypes
void getUserInput(const char* question, char *destinationArray, size_t arraySize);
void assessRecordDetails(char inputFilename[32], size_t recordDetailsAry[2]);
char** initDataStorageArray(char **dataStorageAry, size_t numOfStrings, size_t maxLengthOfString);
void populateDataArray(char inputFilename[32], char **dataStorageAry, size_t arraySize);
void bubbleSort(char **unsortedArray, size_t arraySize);
void outputArrayDataToFile(char outputFilename[32], char **dataStorageAry, size_t arraySize);


int main() {

    char inputFilename[INPUT_BUF] = ""; //A character array to hold the Input filename
    char outputFilename[INPUT_BUF] = "";//A character array to hold the Output filename
    char **dataStorageAry = NULL;       //String Array declaration, initialised to NULL (i.e. Pointing nowhere!)
    size_t recordDetailsAry[2] = {0, 0};  //[0] holds the number of records / [1] holds the MAX length


    //1. Get the Input filename from the user
    getUserInput("Input filename :", inputFilename, INPUT_BUF);

    //2. Get the Output filename from the user
    getUserInput("Output filename :", outputFilename, INPUT_BUF);

    //3. Assess the input file and determine the Number of records and the Max record length
    assessRecordDetails(inputFilename, recordDetailsAry);

    //4. Initialise the dataStorageArray for the determined Number of records and the Max record length
    dataStorageAry = initDataStorageArray(dataStorageAry, recordDetailsAry[0], recordDetailsAry[1]);

    //5. Populate the dataStorageArray with the data from the input file
    populateDataArray(inputFilename, dataStorageAry, recordDetailsAry[0]);

    //6. Sort the dataStorageArray
    bubbleSort(dataStorageAry, recordDetailsAry[0]);

    //7. Save the Sorted data to the specified output file
    outputArrayDataToFile(outputFilename, dataStorageAry, recordDetailsAry[0]);

    //8. Assuming successful completion of the program...
    return 0;

}//End of main()

/**
 * Function getUserInput
 * This function is more of a template that I might use going forward to take care of error checking etc.
 * I'm thinking that this type of function would be more relevant for non-string input.
 *
 * @param question - The question being asked of the user
 * @param destinationArray - The location to store a valid user response
 * @param arraySize - The size of the destination array
 */
void getUserInput(const char* question, char *destinationArray, size_t arraySize){

    printf("%s", question); //Output the question to the user

    char lineBuffer[arraySize]; // A testBuffer to hold a lineBuffer of user input.
    char testBuffer[arraySize]; // a testBuffer to test if the input is valid

    if(fgets(lineBuffer, (int)arraySize, stdin)){   //fgets() returns NULL on EOF or Error

        //If I can format the input as required, then... (Probably redundant for string input)
        if(1 == sscanf(lineBuffer, "%s", testBuffer)){
            strcpy(destinationArray, testBuffer);       //Copy the input to the destinationArray
        }else{
            puts("Sorry - I had a problem understanding your input. Please try again.");
            exit(-1);   //Perhaps I could repeat until I get a valid response?
        }//End if/else
    }//End if

    ////Not sure if the above is any better then...
    //printf("%s", question);
    //scanf("%s", destinationArray);

}//End of function getUserInput

/**
 * Function assessRecordDetails
 * This function reads through the input File and quickly tots up the number of records and the length of the longest.
 *
 * @param inputFilename - The name of the file being assessed
 * @param recordDetailsAry - The array in which to store the assessment results. [0] #records, [1] MaxLength
 */
void assessRecordDetails(char *inputFilename, size_t recordDetailsAry[2]) {
    FILE *inputFilePtr; //A pointer to my input file.
    //Check that I can open the file...
    if((inputFilePtr = fopen(inputFilename, "r")) == NULL){ //If opening fails...
        fprintf(stderr,"Error opening input file '%s'. Please ensure you typed the filename in correctly and try again.",
                inputFilename);
        exit(-1); //This is a fatal error, so exit the program.
    }//End of if

    //Continuing if fopen was successful...
    //A temporary holder for each record. ## BUFSIZ is declared in stdio.h as 512 ##
    char temp[BUFSIZ] = "";

    //A local variable to hold the length of each assessed record. ## Saves having to call strlen twice ##
    size_t lengthOfRecord = 0;

    while(!feof(inputFilePtr)){ //Repeat this till we reach the End Of File
        fscanf(inputFilePtr, "%s", temp);   //Read in a record as a String and save it to temp
        lengthOfRecord = strlen(temp);      //Get the length of the record
        //If the record length is greater then out previous MAX, then update accordingly...
        if(lengthOfRecord > recordDetailsAry[1]) recordDetailsAry[1] = lengthOfRecord;
        //Increment the number of records identified.
        recordDetailsAry[0]++;

    }//Back to while condition

    fclose(inputFilePtr);   //Close the file

    //Print a summary for the user...
    printf("\nIdentified %u words. Largest word has %u characters.\n",
           (unsigned int)recordDetailsAry[0], (unsigned int)recordDetailsAry[1]);

}//End of function assessRecordDetails

/**
 * Function initDataStorageArray
 * This function initializes the dataStorageAry of Strings, when given the number of strings and the max length
 *
 * @param dataStorageAry - The array being initialized
 * @param numOfStrings - The number of char*'s we want to initialize our array with
 * @param maxLengthOfString - The number of bytes allocated to each char*
 * @return - A reference to our initialized array
 */
char** initDataStorageArray(char **dataStorageAry, size_t numOfStrings, size_t maxLengthOfString) {
    //Allocate the space for a char pointer, for the number of words identified...
    dataStorageAry = calloc(numOfStrings, sizeof(char*));

    //For each char pointer, allocate space for each string, using the size of the
    //largest detected string in the file to minimise waste...
    for(size_t i = 0; i < numOfStrings; i++){
        dataStorageAry[i] = calloc(maxLengthOfString + 1, sizeof(char)); //Max String size + NULL Char
    }//End for loop

    //Return my fully initialized array...
    return dataStorageAry;

}//End of function initDataStorageArray

/**
 * Function populateDataArray
 * This function populates my array with records from a file.
 *
 * @param inputFilename - The file with the data to input
 * @param dataStorageAry - The array that will store the data in memory
 * @param arraySize - The size of the array. ## i.e. The number of words it can hold ##
 */
void populateDataArray(char *inputFilename, char **dataStorageAry, size_t arraySize) {
    FILE *inputFilePtr;//A pointer to my input file.
    //Check that I can open the file...
    if((inputFilePtr = fopen(inputFilename, "r")) == NULL){ //If opening fails...
        fprintf(stderr,"Error opening input file '%s'. Please ensure that the file is not open by another program and try again.",
                inputFilename);
        exit(-1); //This is a fatal error, so exit the program.
    }//End of if

    //Continuing if fopen was successful...

    //Count / Iterate through the array...
    for(size_t i = 0; i < arraySize; i++){
        fscanf(inputFilePtr, "%s", dataStorageAry[i]);   //Read in a word and store it in the array

    }//End of for loop

    fclose(inputFilePtr); //Close the file

}//End of function populateDataArray

/**
 * Function bubbleSort
 * This function sorts an array in Ascending order and reports performance statistics to the user
 *
 * @param unsortedArray - The array to sort
 * @param arraySize - The size (#number of words) in the array
 */
void bubbleSort(char **unsortedArray, size_t arraySize) {

    clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    unsigned int swapCount = 0;     //Counter to track the number of data item swaps required to complete this task.
    unsigned int comparisonCount = 0;   //Counter to track the number of data item comparisons required to complete this task.

    size_t pass;    //Passes counter
    size_t i;       //Comparisons counter
    char *temp;     //Temporary location used to swap array elements

    // loop to control number of passes
    for (pass = 1; pass < arraySize; pass++) {

        // loop to control number of comparisons per pass
        for (i = 0; i < arraySize - 1; i++) {
            //For every comparison, increment the comparisonCount counter
            comparisonCount++;

            // compare adjacent elements and swap them if first
            // element is greater than second element
            if (strcmp(unsortedArray[i], unsortedArray[i + 1]) > 0) {
                //For every swap, increment the swapCount counter
                swapCount++;

                //Do the Swap...
                temp = unsortedArray[i];                    //Store item[i] in temp.
                unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

            } // end if
        } // end inner for
    } // end outer for

    clock_t finishTime = clock();   //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    printf("Bubble Sort completed after %f seconds, with %u data swaps and %u data comparisons.\n",
           (double) (finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

}//End of function bubbleSort

/**
 * Function outputArrayDataToFile
 * This function outputs the array data to a file, 1 record per line.
 *
 * @param outputFilename - The name of the file to be written
 * @param dataStorageAry - The data array that holds the data to be written to file
 * @param arraySize - The size (Number of elements) in the data array
 */
void outputArrayDataToFile(char outputFilename[32], char **dataStorageAry, size_t arraySize) {

    FILE *outputFilePtr;//A pointer to my input file.
    //Check that I can open the file...
    if((outputFilePtr = fopen(outputFilename, "w")) == NULL ){ //If opening fails...
        fprintf(stderr,"Error opening output file '%s'.\n", outputFilename);
        fprintf(stderr,"Please ensure that the filename is correct and that the file is not already opened by another program.");
        exit(-1); //This is a fatal error, so exit the program.
    }//End of if


    //Continuing if fopen was successful...
    fprintf(outputFilePtr, "%s", dataStorageAry[0]);    //Output the 1st element of data to file
    for (size_t x = 1; x < arraySize; x++){             //Iterate through the rest of the dataArray...
        fprintf(outputFilePtr, "\n%s", dataStorageAry[x]);  //Send a 'newline' and the next element of data to file
        // I do it this way so that I'm not left with an empty line at the end of my file.
    }//End of for

    fclose(outputFilePtr); //Close the file

}//End of function outputArrayDataToFile