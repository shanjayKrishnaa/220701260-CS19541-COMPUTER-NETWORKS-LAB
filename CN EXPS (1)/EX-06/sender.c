#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void charToBinary(char ch, char* binary) {
    for (int i = 7; i >= 0; --i) {
        binary[7 - i] = ((ch >> i) & 1) ? '1' : '0';
    }
    binary[8] = '\0';
}

int calculateRedundantBits(int m) {
    int r = 0;
    while ((1 << r) < (m + r + 1)) {
        r++;
    }
    return r;
}

void applyHammingToEntireData(char* data, int totalBits, char* hammingCode, int redundantBits) {
    int totalLength = totalBits + redundantBits;
    int j = 0, k = 0;

    printf("\nPositions of redundant bits: ");
    
    for (int i = 1; i <= totalLength; i++) {
        if (i == (1 << k)) {
            hammingCode[i - 1] = 'r';
            printf("%d ", i);  
            k++;
        } else {
            hammingCode[i - 1] = data[j++];
        }
    }

    printf("\n\nCalculating and inserting redundant bits:\n");

    for (int i = 0; i < redundantBits; i++) {
        int pos = (1 << i); 
        int xorSum = 0;
        
        for (int j = pos; j <= totalLength; j++) {
            if (j & pos) {
                if (hammingCode[j - 1] != 'r') { 
                    xorSum ^= (hammingCode[j - 1] - '0');  
                }
            }
        }
        hammingCode[pos - 1] = xorSum + '0';  
        printf("Redundant bit at position %d: %c\n", pos, hammingCode[pos - 1]);
    }

    hammingCode[totalLength] = '\0';  
}

int main() {
    char input[100], binary[9], totalBinary[800] = "", hamming[900];
    FILE *channelFile = fopen("channel.txt", "w");

    printf("Enter the text to send: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 

    for (int i = 0; i < strlen(input); i++) {
        charToBinary(input[i], binary);
        strcat(totalBinary, binary);
    }

    printf("\nEntire binary data of the input: %s\n", totalBinary);

    int totalBits = strlen(totalBinary);
    printf("Total number of data bits: %d\n", totalBits);

    int redundantBits = calculateRedundantBits(totalBits);
    printf("Number of redundant bits required: %d\n", redundantBits);

    applyHammingToEntireData(totalBinary, totalBits, hamming, redundantBits);

    printf("\nHamming encoded data: %s\n", hamming);

    fprintf(channelFile, "%s", hamming);
    fclose(channelFile);

    printf("\nData sent and stored in channel.txt.\n");

    return 0;
}
