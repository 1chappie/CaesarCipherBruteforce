//
// Created by chappie on 21.03.2022.
//
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define MAXLEN 350000

//read the distributon from distribution.txt
void readReference(float *frequency) {
    FILE *fp = fopen("distribution.txt", "r");
    float freq = 0;
    int index = 0;
    while (!feof(fp) && fscanf(fp, "%f", &freq))
        frequency[index++] = freq;
    fclose(fp);
}

//compute the frequency of each given character in a string
void createFrequencyTable(char *string, float *freqTable) {
    int letters = 0;
    for (int i = 0; i < strlen(string); i++)
        if (isalpha(tolower(string[i]))) {
            freqTable[tolower(string[i]) - 'a']++;
            letters++;
        }
    for (int i = 0; i < 26; i++)
        freqTable[i] = (float) ((100.0 * freqTable[i]) / letters); //frequency percentage out of 100
}

//compute chi distance between two histograms
float chiDistance(float *C, float *E) {
    float distance = 0;
    for (int i = 0; i < 26; i++) //sum from 'a' to 'z'
        distance += ((pow((C[i] - E[i]), 2)) / E[i]);
    return sqrtf(distance);
}

char *bruteForce(char *string, float *refFrequency) {
    float minDistance = 999;
    static char translation[MAXLEN];

    for (int shift = 0; shift < 26; shift++) {
        char shiftedString[MAXLEN];
        strcpy(shiftedString, string);
        //shifting every letter one by one
        for (int i = 0; i < strlen(string); i++)
            if (isalpha(tolower(shiftedString[i]))) {
                int newLetter = (int) tolower(shiftedString[i]) + shift;
                if (newLetter > 'z')
                    newLetter = newLetter - 'z' + 'a' - 1;
                if (newLetter < 'a')
                    newLetter = newLetter + 'z' - 'a' + 1;
                shiftedString[i] = (char) (newLetter);
            }
        float frequency[26] = {0};
        createFrequencyTable(shiftedString, frequency);
        float distance = chiDistance(frequency, refFrequency);
////        //memorizing the minimum chiDistance
        if (distance < minDistance) {
            minDistance = distance;
            strcpy(translation, shiftedString);
        }
    }
    return translation;
}

int main() {
    char string[MAXLEN];
    float refFrequency[26];
    readReference(refFrequency);

    printf("\nWhat would you like to decipher?\n");
    fgets(string, MAXLEN, stdin);

    char *deciphered = bruteForce(string, refFrequency);

    printf("\nThe deciphered text is:\n");
    for (int i = 0; i < strlen(deciphered); ++i) {
        printf("%c", deciphered[i]);
    }

}