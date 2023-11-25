#include <stdio.h>
#include <stdlib.h>
#include "api.h"

void usage()
{
    printf("Usage: KBC [options] <plaintext or ciphertext>...\n");
    printf("Sample: KBC -e aes128 ecb \"With great power comes great responsibility\" \n")
}

void help()
{
    usage();
    printf("Options:\n");
    printf("-e encrypt\n");
    printf("-d decrypt\n\n");
    
    printf("ciphers:\n");
    printf("aes128\t");

    printf("\n\n");
    printf("modes of operation:\n");
    printf("ecb\tcbc\n");

}


int main(int argc, char *argv[]) {
    
    if (argc )
    /* 
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "rb");
    outputFile = fopen(outputFileName, "wb");
    

    if (inputFile == NULL || outputFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }
    */


    return 0;
}
