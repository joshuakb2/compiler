#include <stdio.h>

void printLeadLines(int tabs) {
    for(int i = 0; i < tabs - 1; i++) {
        printf("|\t");
    }
}

void printLines(int tabs) {
    printLeadLines(tabs);
    if(tabs > 0)
        printf("|------ ");
}

void printTabs(int tabs) {
    for(int i = 0; i < tabs; i++)
        printf("\t");
}
