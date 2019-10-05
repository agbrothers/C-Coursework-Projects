#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

bool decrypt(const char ciphertext[], const char crib[]);
bool isSubArray(const char ciphertext[], int n1, const char crib[], int n2, int& cribStart, int& cribEnd, char encryptedCrib[], char comparisonCrib[]);
bool patternCheck(const char ciphertext[], const char crib[], int n2, const int cribStart, const int cribEnd, char encryptedCrib[], char comparisonCrib[]);


int main()
{
    char cipher[] = "Zqysrsuu jysqjtsgj bw zrquucwcsx cgwbytqjcbg cu xqgesybmu.\nNrsqus ysrsqus qgo xbzmtsgju obm qzhmcys jb Icfcrsqfu.\nXbg'j ysrsqus qgojdcge stvqyyquucge jb mu; is EMvARRASS OmRSELkES hmITE iELL.\nzu 31 cu zdqrrsgecge!";
    char crib[] = "classified information";
    
    decrypt(cipher, crib);
}

bool decrypt(const char ciphertext[], const char crib[])
{
    int n1 = strlen(ciphertext);
    int n2 = strlen(crib);
    char encryptedCrib[81];
    char comparisonCrib[81];
    int cribStart = 0;
    int cribEnd = 0;
    
    if (n2 == 0 || n2 > n1 || n2 > 80) // ends if crib is > 80 char, crib has 0 char, or if the crib is longer than the ciphertext
        return false;
    
    bool g = isSubArray(ciphertext, n1, crib, n2, cribStart, cribEnd, encryptedCrib, comparisonCrib);  // CALLS ISSUBARRAY
    
    if (g == false)
        return false;
    
    // DECODER
    char alphabet[26] {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
    };
    char keyUpper[26];
    char keyLower[26];
    
    for (int i = 0; i < strlen(encryptedCrib); )
    {
        for (int j = 0; j < 26; j++)
        {
            if (toupper(comparisonCrib[i]) == alphabet[j]) // if part of the crib is a letter, it is added to a position corresponding with an alphabetic letter
            {
                keyUpper[j] = toupper(encryptedCrib[i]);
                keyLower[j] = tolower(encryptedCrib[i]);
                i++;
            }
            else if (!isalpha(comparisonCrib[i]))  //If part of the crib isnt a letter, it move on
                i++;
        }
    }
    
    // DECYPHERING
    char decodedMessage[5000];
    int posM = 0;
    
    for (int i = 0; i <= strlen(ciphertext); i++)  // What does this do?
    {
        bool knownChar = false;
        if (isalpha(ciphertext[i]))
            for (int j = 0; j < 26; j++) {
                if (ciphertext[i] == keyUpper[j] || ciphertext[i] == keyLower[j]) {
                    decodedMessage[posM] = alphabet[j];
                    posM++;
                    knownChar = true;
                }
            }
        if (knownChar == false) {
            decodedMessage[posM] = tolower(ciphertext[i]);
            posM++;
        }
    }
    cout << decodedMessage << endl;
    return(g);
}

bool isSubArray(const char ciphertext[], int n1, const char crib[], int n2, int& cribStart, int& cribEnd, char encryptedCrib[], char comparisonCrib[])
{
    // Creates an array for num of words & num of characters per word in ciphertext
    
    int cipherWordStartPos[100];  //holds the position of the first char in each word
    int loc1 = 0;
    int cipherWordEndPos[100];    //holds the position of the last char in each word
    int loc2 = 0;
    
    int arr1[100];     // number of char's per word
    int pos1 = 0;       // total num of words in the cipher
    int num1 = 0;
    for (int i = 0; i <= n1; )
    {
        if (isalpha(ciphertext[i])){
            cipherWordStartPos[loc1] = i;    // tracking where each word starts
            loc1++;
            for (int j = i; isalpha(ciphertext[j]); j++)
                num1++;
            arr1[pos1] = num1;
            pos1++;
            i += num1;
            //cout << i << endl;
        }
        else {
            //cout << i << ", ";
            if (isalpha(ciphertext[i - 1])) {
                cipherWordEndPos[loc2] = i - 1;  // tracking where each word ends
                loc2++;
            }
            i++;
        }
        num1 = 0;
    }
    // Creates an array for num of words & num of characters per word in crib
    int arr2[100];      // number of char's per word
    int pos2 = 0;       // total num of words int the crib
    int num2 = 0;
    for (int i = 0; i < n2; ) {
        if (isalpha(crib[i])) {
                for (int j = i; isalpha(crib[j]); j++)
                    num2++;
                arr2[pos2] = num2;
                pos2++;
                i += num2;
            }
        else
            i++;
        num2 = 0;
    }
    
    // Checks to see if crib can be considered a subarray by correct number of words/char only
    int matchingWordLocations[100];
    int loc3 = 0;
    int k = 0;
    for (int j = 0; j <= pos1; j++) {
        if (arr1[j] == arr2[k]) {
            matchingWordLocations[loc3] = j;
            loc3++;
            k++;
            if (k == pos2) {
                cribStart = cipherWordStartPos[matchingWordLocations[0]];
                cribEnd = cipherWordEndPos[matchingWordLocations[k-1]];

                if(patternCheck(ciphertext, crib, n2, cribStart, cribEnd, encryptedCrib, comparisonCrib) == true) // CALLS PATTERNCHECK
                    return true;
                else {
                    k = 0;
                    loc3 = 0;
                }
            }
        }
        else if (k > 0)
            return false;
    }
    return false;
}

bool patternCheck(const char ciphertext[], const char crib[], int n2, const int cribStart, const int cribEnd, char encryptedCrib[], char comparisonCrib[])
{
    // Builds the encrypted crib string
    int pos1 = 0;
    for (int i = cribStart; i <= cribEnd; i++) {
        if (isalpha(ciphertext[i])) { // Adds alpha characters from cipher into the the encrypted crib string
            encryptedCrib[pos1] = ciphertext[i];
            pos1++;
        }
        else if (ciphertext[i] == '\n' && i != cribEnd)  // Prevents a crib with a newline in the middle of it
            return false;
        else if (!isalpha(ciphertext[i]) && isalpha(ciphertext[i+1])) { //prevents more than 1 space/symbol between words in encrypted crib
            encryptedCrib[pos1] = ciphertext[i];
            pos1++;
        }
    }
    int pos2 = 0;
    for (int i = 0; i < n2; i++)
    {
        if (isalpha(crib[i])) { // Adds alpha characters from cipher into the the encrypted crib string
            comparisonCrib[pos2] = crib[i];
            pos2++;
        }
        else if (!isalpha(crib[i]) && isalpha(crib[i+1]) && pos2 != 0) { //prevents more than 1 space/symbol between words in encrypted crib
            comparisonCrib[pos2] = crib[i];
            pos2++;
        }
    }

    for (int i = 0; i < strlen(comparisonCrib); i++) {   // Tests the encrypted crib string for two dif letters being encrypted by the same char
        for (int j = i + 1; j < strlen(comparisonCrib); j++) {
            if (toupper(comparisonCrib[i]) == toupper(comparisonCrib[j]) && toupper(encryptedCrib[i]) != toupper(encryptedCrib[j]) && isalpha(encryptedCrib[i]))
                return false;
            if (toupper(comparisonCrib[i]) != toupper(comparisonCrib[j]) && toupper(encryptedCrib[i]) == toupper(encryptedCrib[j]) && isalpha(encryptedCrib[i]))
                return false;
        }
    }
    return true;
}
