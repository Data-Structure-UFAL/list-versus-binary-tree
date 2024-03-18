#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct tree_node {
    int data;
    struct tree_node* left;
    struct tree_node* right;
} tree_node;

typedef struct list_node {
    int data;
    struct list_node* next;
} list_node;

int isLinkedListEmpty(list_node*head){
    return head == NULL;
}

int beginInsertLinkedList(list_node** head, int value) {
    list_node* newNode = (list_node*)malloc(sizeof(list_node));
    if (newNode == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        return 0;
    }
    newNode->data = value;
    newNode->next = *head;
    *head = newNode;
    return 0;
}

int endInsertLinkedList(list_node** head, int value){
    int comparisons = 0;
    list_node* newNode = (list_node*)malloc(sizeof(list_node));

    if (newNode == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        return 0;
    }

    if(isLinkedListEmpty(*head)){
        //comparisons++;
        *head = newNode;
    }else{
        list_node *aux = *head;
        while(aux->next != NULL){
            comparisons++;
            aux = aux->next;
        }
        aux->next = newNode;
    }

    return comparisons;
}

int sortedInsertLinkedList(list_node** head, int value){
    int comparisons = 0;
    list_node* newNode = (list_node*)malloc(sizeof(list_node));

    if (newNode == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        return 0;
    }

    if(isLinkedListEmpty(*head)){
        newNode->next = NULL;
        *head = newNode;

    }else if(newNode->data < (*head)->data){
        comparisons++;
        newNode->next = *head;
        *head = newNode;
    }else{
        list_node *aux = *head;

        while(aux->next != NULL && newNode->data > aux->next->data){
            comparisons++;
            aux = aux->next;
        }

        newNode->next = aux->next;
        aux->next = newNode;
    }

    return comparisons;
}

int insertBinarySearchTree(tree_node** root, int value) {
    int comparisons = 0;

    if (*root == NULL) {
        *root = (tree_node*)malloc(sizeof(tree_node));
        (*root)->data = value;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return comparisons;
    }

    comparisons++;

    if (value < (*root)->data) {
        comparisons += insertBinarySearchTree(&((*root)->left), value);
    } else if (value > (*root)->data) {
        comparisons += insertBinarySearchTree(&((*root)->right), value);
    } else {
        return comparisons;
    }

    return comparisons;
}

int searchLinkedList(list_node*head, int data){
    int comparisons = 0;

    while (head != NULL) {
        comparisons++;
        if (head->data == data) {
            comparisons++;
        }
        head = head->next;
    }

    return comparisons;
}


int searchBinarySearchTree(tree_node* bt, int data) {
    int comparisons = 0;

    while (bt != NULL) {
        comparisons++;
        if (bt->data == data) {
            return comparisons; 
        } else if (data < bt->data) {
            bt = bt->left; 
        } else {
            bt = bt->right; 
        }
    }
    return comparisons;
}


void freeLinkedList(list_node* head) {
    list_node* current = head;
    while (current != NULL) {
        list_node* temp = current;
        current = current->next;
        free(temp);
    }
}

void freeBinarySearchTree(tree_node* root) {
    if (root == NULL) return;
    freeBinarySearchTree(root->left);
    freeBinarySearchTree(root->right);
    free(root);
}

void printTree(tree_node* root) {
    if (root != NULL) {
        printTree(root->left);
        printf("%d ", root->data);
        printTree(root->right);
    }
}

void printLinkedList(list_node* root){
    while (root != NULL){
        printf("%d -> ");
        root = root->next;
    }
    printf("\n");
}

void red(){
    printf("\033[1;31m");
}

void yellow(){
    printf("\033[1;33m");
}

void resetColors(){
    printf("\033[0m");
}

void purple(){
    printf("\033[0;35m");
}

void blue(){
    printf("\033[0;34m");
}

void green(){
    printf("\033[0;32m");
}

void plotGraph(int* x, int* y1, int* y2, char** names, int n, char* xLabel, char* yLabel, char* title, int highestComparison, char* fileName) {
    FILE* fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("Error: Unable to create data file.\n");
        return;
    }
    int i;
    for (i = 0; i < n; i++) {
        //puts(names[i]);
        fprintf(fp, "%d %d %d %s\n", x[i], y1[i], y2[i], names[i]);
    }
    fclose(fp);

    //puts(fileName);

    char command[1000];

    if (n > 20) {
        sprintf(command, "octave --no-gui --eval 'data = dlmread(\"%s\"); plot(data(:,1), data(:,2), \"r\", data(:,1), data(:,3), \"b\"); xlabel(\"%s\"); ylabel(\"%s\"); title(\"%s\"); xlim([0, %d]); ylim([0, %d]); pause;'", fileName, xLabel, yLabel, title, n, highestComparison*2);
    } else {
        sprintf(command, "octave --no-gui --eval 'fid = fopen(\"%s\", \"r\"); data = textscan(fid, \"%%d %%d %%d %%s\"); fclose(fid); names = cellstr(data{4}); plot(data{1}, data{2}, \"r\", data{1}, data{3}, \"b\"); text(data{1}, data{2} + 0.5, names); xlabel(\"%s\"); ylabel(\"%s\"); title(\"%s\"); xlim([0, %d]); ylim([0, %d]); pause;'", fileName, xLabel, yLabel, title, n, highestComparison*2);
    }

    system(command);
}

int makeInsertions(list_node**linkedList, tree_node**binarySearchTree, int insertions, int(*linkedListFunction)(list_node**, int), int*x, int*y_linked_list, int*y_binary_search_tree, int isSearch, char**names){

    int highestComparision = 0, i;

    for (i = 0; i < insertions; i++) {
        int data = rand() % 100;
        //printf("dangerous %d\n", data);

        int comparisonsLL, comparisonsBST;
        //printf("eh de search? %d\n", isSearch);
        x[i] = i + 1;
        char result[50];
        //printf("inserido: %d\n", data);

        if(isSearch){

            linkedListFunction(linkedList, data);
            insertBinarySearchTree(binarySearchTree, data);

            int random = rand() % 100;
            char original[] = "Searched:";
            sprintf(result, "%s%d", original, random);


            comparisonsLL = searchLinkedList(*linkedList, random);
            comparisonsBST = searchBinarySearchTree(*binarySearchTree, random);
        }else{

            char original[] = "Inserted:";
            sprintf(result, "\"%s%d\"", original, data);
            comparisonsLL = linkedListFunction(linkedList, data);
            comparisonsBST = insertBinarySearchTree(binarySearchTree, data);
        }

        y_linked_list[i] = comparisonsLL;
        y_binary_search_tree[i] = comparisonsBST;
        names[i] = malloc(strlen(result) + 1); // Allocate memory for the string
        if (names[i] != NULL) {
            strcpy(names[i], result); // Copy the contents of result into names[i]
        }

        if(comparisonsLL > highestComparision || comparisonsBST > highestComparision){
            if(comparisonsBST > comparisonsLL){
                highestComparision = comparisonsBST;
            }else if(comparisonsLL > highestComparision){
                highestComparision = comparisonsLL;
            }
        }

    }

    return highestComparision;
}

void searchTest(list_node*linkedList, tree_node*binarySearchTree, int insertions){

    int* xInsertions = malloc(insertions * sizeof(int));
    int* yLLInsertions = malloc(insertions * sizeof(int));
    int* yBSTInsertions = malloc(insertions * sizeof(int));
    char** names = malloc(insertions * sizeof(char*));

    for (int i = 0; i < insertions; i++) {
        names[i] = malloc(20 * sizeof(char));
    }

    if (xInsertions == NULL || yLLInsertions == NULL || yBSTInsertions == NULL) {
            printf("Error: Memory allocation failed.\n");
            return;
    }

    int highestComparision = makeInsertions(&linkedList, &binarySearchTree, insertions, beginInsertLinkedList, xInsertions, yLLInsertions, yBSTInsertions, 1, names);

    plotGraph(xInsertions, yLLInsertions, yBSTInsertions,names, insertions, "Número de Elementos Inseridos", "Número de Comparações", "Plotagem de Busca - Vermelho = Lista Encadeada, Azul = Árvore de Busca Binária", highestComparision, "search.txt");

}

void insertTest(list_node**linkedList, tree_node**binarySearchTree, int insertions, int(*linkedListFunction)(list_node**, int), int(*binarySearchTreeFunction)(tree_node**, int)){

    int* x = malloc(insertions * sizeof(int));
    int* y_linked_list = malloc(insertions * sizeof(int));
    int* y_binary_search_tree = malloc(insertions * sizeof(int));
    char** names = malloc(insertions * sizeof(char*));

    if (x == NULL || y_linked_list == NULL || y_binary_search_tree == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    int highestComparision = makeInsertions(linkedList, binarySearchTree, insertions, linkedListFunction, x, y_linked_list, y_binary_search_tree, 0, names);

    //printLinkedList(*linkedList);
    //printTree(*binarySearchTree);

    plotGraph(x, y_linked_list, y_binary_search_tree, names, insertions, "Número de Inserções", "Número de Comparações", "Plotagem de Inserção Vermelho = Lista Encadeada, Azul = Árvore de Busca Binária", highestComparision, "insertion.txt");

}


void showInsertionSubMenu(list_node**linkedList, tree_node**binarySearchTree){
    printf("\n- Para a Lista Encadeada, a inserção será:\n");
    printf("\t1. No início\n");
    printf("\t2. No final\n");
    printf("\t3. Ordenada\n");

    int subMenuOption = 0, insertions;
    scanf("%d", &subMenuOption);

    printf("Insira quantas inserções deseja fazer: \n");
    scanf("%d", &insertions);

    switch (subMenuOption){

        case 1:
            insertTest(linkedList, binarySearchTree, insertions, beginInsertLinkedList, insertBinarySearchTree);
            break;
        case 2:
            
            insertTest(linkedList, binarySearchTree, insertions, endInsertLinkedList, insertBinarySearchTree);
            break;

        case 3:
            insertTest(linkedList, binarySearchTree, insertions, sortedInsertLinkedList, insertBinarySearchTree);
            break;

        default:
            break;
    }
}

void showMenu(){
    printf("\n- Menu:\n");
    printf("\t1. Comparar inserção\n");
    printf("\t2. Comparar busca\n");
}

int main(int argc, char const *argv[]){

    printf("\n");
    //yellow();
    printf("X1 DE ");
    yellow();
    printf("ESTRUTURAS DE DADOS");
    resetColors();
    printf(" APRESENTA:\n");
    printf("\n");

    red();
    printf("LISTA ENCADEADA"); 
    yellow(); 
    printf(" vs "); 
    red();
    printf("ARVORE DE BUSCA BINARIA\n"); 
    resetColors();

    list_node* linkedList = NULL;
    tree_node* binarySearchTree = NULL;

    showMenu();

    int menuOption = 0;
    scanf("%d", &menuOption);

    switch (menuOption){
    case 1:
        showInsertionSubMenu(&linkedList, &binarySearchTree);

        break;
    case 2:

        int insertions = 0;

        printf("Insira quantas inserções deseja fazer para que um número sorteado seja buscado: \n");
        scanf("%d", &insertions);

        searchTest(linkedList, binarySearchTree, insertions);

        break;
    default:
        break;
    }

    return 0;
}
