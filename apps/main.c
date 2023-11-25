#include <stdio.h>
#include <stdlib.h>
#include "api.h"

void usage()
{
    printf("Usage: KBC [options] <plaintext or ciphertext> <key> <iv>...\n");
    printf("Sample: KBC -e aes128 ecb \"With great power comes great responsibility\" spiderman 0\n");
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
    
    /* 
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "rb");
    outputFile = fopen(outputFileName, "wb");
    

    if (inputFile == NULL || outputFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }
    */

    if (argc < 3)
    {
        usage();
        return 2;
    }
    if (strcmp(argv[1], "-e") == 0)
    {
        

    }else if (strcmp(argv[1], "-d") == 0)
    {


    }else if (strcmp(argv[1], "-h") == 0)
    {
        help();
    }else{
        usage();
        printf("-h is help option\n");
        return 1;
    }


    return 0;
}
