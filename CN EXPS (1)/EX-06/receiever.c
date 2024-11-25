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

void binaryToChar(char* binary, char* result) {
    int value = 0;
    for (int i = 0; i < 8; i++) {
        value = value * 2 + (binary[i] - '0');
    }
    *result = (char)value;
}

void detectAndCorrectHammingCode(char* hammingCode, int totalBits, int redundantBits, char* correctedData) {
    int totalLength = totalBits + redundantBits;
    int errorPosition = 0;

    // Check parity bits
    for (int i = 0; i < redundantBits; i++) {
        int pos = (1 << i);
        int xorSum = 0;

        for (int j = pos; j <= totalLength; j++) {
            if ((j & pos) && hammingCode[j - 1] != 'r') {
                xorSum ^= (hammingCode[j - 1] - '0');
            }
        }

        if (xorSum != 0) {
            errorPosition += pos;
        }
    }

    if (errorPosition != 0) {
        printf("Error detected at position: %d\n", errorPosition);
        if (errorPosition <= totalLength) {
            hammingCode[errorPosition - 1] = (hammingCode[errorPosition - 1] == '0') ? '1' : '0';
            printf("Corrected bit at position %d: %c\n", errorPosition, hammingCode[errorPosition - 1]);
        }
    } else {
        printf("No errors detected.\n");
    }

    // Extract data from corrected Hamming code
    int j = 0;
    for (int i = 1; i <= totalLength; i++) {
        if ((i & (i - 1)) != 0) { // Exclude redundant bit positions
            correctedData[j++] = hammingCode[i - 1];
        }
    }
    correctedData[j] = '\0'; // Null-terminate the extracted data
}

void binaryToString(char* binaryData, char* result) {
    int length = strlen(binaryData);
    char binaryChar[9];
    binaryChar[8] = '\0';

    int index = 0;
    for (int i = 0; i < length; i += 8) {
        strncpy(binaryChar, &binaryData[i], 8); // Take 8 bits at a time
        binaryToChar(binaryChar, &result[index]);
        index++;
    }
    result[index] = '\0'; // Null-terminate the result string
}

int main() {
    char hamming[900], correctedData[800], resultString[100];
    FILE *channelFile = fopen("channel.txt", "r");

    if (channelFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    fgets(hamming, sizeof(hamming), channelFile);
    fclose(channelFile);

    int totalLength = strlen(hamming);
    if (hamming[totalLength - 1] == '\n') {
        hamming[totalLength - 1] = '\0';
        totalLength--;
    }

    int redundantBits = 0;
    while ((1 << redundantBits) < totalLength) {
        redundantBits++;
    }

    int totalBits = totalLength - redundantBits;

    detectAndCorrectHammingCode(hamming, totalBits, redundantBits, correctedData);

    printf("\nCorrected binary data: %s\n", correctedData);
    binaryToString(correctedData, resultString);
    printf("\nCorrected string data: %s\n", resultString);

    return 0;
}
