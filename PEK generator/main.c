/*
 * File:   main.c
 * Author: Tomáš Marek
 *
 * Created on 23. září 2014, 19:37
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    int name;
    int value;
    int row;
    int col;
    int neigh[6];
    int count;
    int prev;
} node;

int getCount(int n) {
    return (n*(n+1))/2;
}

node setNeighbors(node mainNode, int n) {
    int rowStarts[n];
    int i;
    int rowStart = 0;
    for ( i = 0; i < n; i++) {
        rowStart += i;
        rowStarts[i] = rowStart;
    }
    if ( mainNode.row < (n-1) ) {
        mainNode.neigh[mainNode.count + 0] = rowStarts[mainNode.row + 1]+mainNode.col;
        mainNode.neigh[mainNode.count + 1] = rowStarts[mainNode.row + 1]+mainNode.col + 1;
        mainNode.count += 2;
    }
    
    if ( mainNode.col > 0 ) {
        mainNode.neigh[mainNode.count + 0] = mainNode.name - 1;
        mainNode.neigh[mainNode.count + 1] = rowStarts[mainNode.row - 1]+mainNode.col-1;
        mainNode.count += 2;
    }
    
    if ( mainNode.col != mainNode.row ) {
        mainNode.neigh[mainNode.count + 0] = mainNode.name + 1;
        mainNode.neigh[mainNode.count + 1] = rowStarts[mainNode.row - 1]+mainNode.col;
        mainNode.count += 2;
    }
    
    return mainNode;
}

void generateNew(node * nodes, int n) {
    int i;
    int j;
    int k = 0;
    for ( i = 0; i < n; i++ ) {
        for ( j = 0; j <= i; j++  ) {
            nodes[k].name = k;
            nodes[k].value = k;
            nodes[k].row = i;
            nodes[k].col = j;
            nodes[k].count = 0;
            int t = 0;
            for ( t = 0; t < 6; t++ ) {
                nodes[k].neigh[t] = -1;
            }
            nodes[k] = setNeighbors(nodes[k], n);
            k++;
        }
    }
}

void printTree(node * nodes, int n) {
    int i, j, k = 0;
    for ( i = 0; i < n; i++ ) {
        for ( j = 0; j <= i; j++ ) {
            printf("%i\t", nodes[k].value);
            k++;
        }
        printf("\n");
    }
}

int shuffleOnce( node * nodes, int pos ) {
    int neigh;
    int random = rand();
    do {
        neigh = nodes[pos].neigh[random % nodes[pos].count];
        random+= 3;
    } while( neigh == nodes[pos].prev );
    int newPos = nodes[neigh].name;
    nodes[pos].value = nodes[newPos].value;
    nodes[pos].prev = nodes[newPos].name;
    nodes[newPos].value = 0;
    return nodes[newPos].name;
}

void shuffle( node * nodes, int t, int pos ) {
    int i = 0;
    srand((unsigned)time(NULL));
    int oldPos = pos;
    int newPos;
    for ( i = 0; i < t; i++  ) {
        newPos = shuffleOnce(nodes, pos);
        oldPos = pos;
        pos = newPos;
    }
}

void printToFile( node * nodes, char * name, int n) {
    FILE * output;
    output = fopen(name, "w");
    int count = getCount(n);
    int i;
    for ( i = 0; i < count; i++) {
        if ( i == count-1 ) {
            fprintf(output, "%i", nodes[i].value);
        } else {
            fprintf(output, "%i\n", nodes[i].value);
        }
        
    }
    fclose(output);
}
/*
 *
 */
int main(int argc, char* argv[]) {
    if ( argc < 4 ) {
        printf("Zadejte tri parametry: 1. vyska grafu, 2. pocet tahu, 3. nazev vystupniho souboru.");
    } else {
        int n, q, freeNode = 0;
        n = atoi(argv[1]);
        q = atoi(argv[2]);
        char * name = argv[3];
        int count = getCount(n);
        node * nodes;
        nodes = (node*)malloc(count * sizeof(node));
        generateNew(nodes, n);
        shuffle(nodes, q,freeNode);
        printTree(nodes, n);
        printToFile(nodes, name, n);
        free(nodes);
    }
    return (EXIT_SUCCESS);
}

