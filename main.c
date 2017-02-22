#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

size_t numItems = 0;
size_t maxItemLength = 0;

void bubbleSort(char **unsortedArray, size_t arraySize);

int main() {

    char inputFilename[32] = "";
    char outputFilename[32] = "";
    char **dataStorageAry;

    printf("%s", "Input filename :");
    scanf("%s", inputFilename);
    printf("%s", "Output filename :");
    scanf("%s", outputFilename);



    FILE *inputFilePtr;
    if((inputFilePtr = fopen(inputFilename, "r")) == NULL){

        fprintf(stderr,"Error opening file");
        return(-1);
    }


    char temp[32] = "";
    size_t recordCount = 0;

    while(!feof(inputFilePtr)){

        fscanf(inputFilePtr, "%s", temp);
        recordCount++;

    }
    rewind(inputFilePtr);
    printf("The count was :%u.\n", recordCount);


    // allocate space for 5 pointers to strings
    dataStorageAry = (char**)malloc(recordCount*sizeof(char*));

    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(int i = 0; i < recordCount; i++){
        //printf("%d\n", i);
        dataStorageAry[i] = (char*)malloc(50*sizeof(char));
    }


    for(size_t count = 0; count < recordCount; count++){
        fscanf(inputFilePtr, "%s", temp);
        sprintf(dataStorageAry[count], temp);

    }


    fclose(inputFilePtr);



    for (size_t x = 0; x < recordCount; x++){
        printf("Array element %u, has data: %s.\n", x, dataStorageAry[x]);
    }

    bubbleSort(dataStorageAry, recordCount);

    for (size_t x = 0; x < recordCount; x++){
        printf("Array element %u, has data: %s.\n", x, dataStorageAry[x]);
    }

    ///////////////////////////////////
    ///////////////////////////////////

    FILE *outputFilePtr;
    if((outputFilePtr = fopen(outputFilename, "w")) == NULL ){
        fprintf(stderr,"Error opening file");
        return(-1);
    }
    for (size_t x = 0; x < recordCount; x++){
        fprintf(outputFilePtr, "%s\n", dataStorageAry[x]);
        //printf("Array element %u, has data: %s.\n", x, dataStorageAry[x]);
    }

    fclose(outputFilePtr);



    return 0;
}


void bubbleSort(char **unsortedArray, size_t arraySize) {


    clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    unsigned int swapCount = 0;     //Counter to track the number of data item swaps required to complete this task.
    unsigned int comparisonCount = 0;   //Counter to track the number of data item comparisons required to complete this task.

    // initialize a
    size_t pass; // passes counter
    size_t i; // comparisons counter
    char *temp; // temporary location used to swap array elements

    // bubble sort
    // loop to control number of passes
    for (pass = 1; pass < arraySize; pass++) {

        // loop to control number of comparisons per pass
        for (i = 0; i < arraySize - 1; i++) {
            comparisonCount++;  //For every comparison, increment the comparisonCount counter

            // compare adjacent elements and swap them if first
            // element is greater than second element
            if (strcmp(unsortedArray[i], unsortedArray[i + 1]) > 0) {
                swapCount++;    //For every swap, increment the swapCount counter

                temp = unsortedArray[i];
                unsortedArray[i] = unsortedArray[i + 1];
                unsortedArray[i + 1] = temp;


            } // end if

        } // end inner for
    } // end outer for

    clock_t finishTime = clock();   //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    printf("Bubble Sort completed after \t%f seconds, with %7u data swaps and  %8u data comparisons.\n",
           (double) (finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

}