#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>

#define mxlen 30

#define BASE 117L
#define MOD 1000000009L

long long calculateCurrentHash(wchar_t *word) {
    long long ret = 0;
    for(int i = 0; i < wcslen(word); i ++ ) {
        ret = (ret * BASE + word[i]) % MOD;
    }
    return ret;
}

// sanity bound 1e6
#define mxn 1000000L
#define M 70000L

long long hashMap[M][2 * (int)(mxn/M)];
int lens[M];

bool was(long long hash) {
    int id = hash % M;
    for (int i = 0; i < lens[id]; i ++) {
        if (hashMap[id][i] == hash) {
            return true;
        }
    }
    return false;
}

void add(long long hash) {
    int id = hash % M;
    hashMap[id][lens[id]] = hash;
    lens[id]++;
}


int main() {
    setlocale(LC_ALL, "");

    FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");
    if (inputFile == NULL) {
        perror("Error opening input.txt");
        return 1;
    }

    wchar_t longestWord[mxlen] = L"";
    wchar_t currentWord[mxlen] = L"";
    
    wchar_t longestWords[mxlen][mxlen];
    int numLongestWords = 0;

    while (fwscanf(inputFile, L"%99ls", currentWord) != EOF) {
        int currentLength = wcslen(currentWord);
        int longestLength = wcslen(longestWord);

        long long hash = calculateCurrentHash(currentWord);

        if (currentLength > longestLength) {
            numLongestWords = 0;
            wcscpy(longestWord, currentWord);
            add(hash);
            wcscpy(longestWords[numLongestWords++], currentWord);
        } else if (currentLength == longestLength) {
            if (!was(hash)) {
                wcscpy(longestWords[numLongestWords++], currentWord);
                add(hash);
            }
        }
    }

    fclose(inputFile);

    if (wcslen(longestWord) > 0) {
        fprintf(outputFile, "The longest unique word(s) in the file:\n");
        for (int i = 0; i < numLongestWords; i++) {
            fwprintf(outputFile, L"%ls\n", longestWords[i]);
        }
    } else {
        fprintf(outputFile, "Input is empty.\n");
    }

    fclose(outputFile);

    return 0;
}
