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

int insertBinarySearchTree(tree_node** root, int value, int comparisons){

  if (*root == NULL) {
    *root = (tree_node*)malloc(sizeof(tree_node));
    (*root)->data = value;
    (*root)->left = NULL;
    (*root)->right = NULL;
    return comparisons;
  }

  comparisons++;

  if (value < (*root)->data) {
    insertBinarySearchTree(&((*root)->left), value, comparisons);
  } else if (value > (*root)->data) {
    insertBinarySearchTree(&((*root)->right), value, comparisons);
  } else {
    return comparisons;
  }
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

void plotGraph(int* x, int* y1, int* y2, int n, char* xLabel, char* yLabel, char* title, int highestComparison) {

    FILE* fp = fopen("data.txt", "w");
    if (fp == NULL) {
        printf("Error: Unable to create data file.\n");
        return;
    }
    int i;
    for (i = 0; i < n; i++) {
        fprintf(fp, "%d %d %d\n", x[i], y1[i], y2[i]);
    }
    fclose(fp);

    char command[100];
    sprintf(command, "octave --no-gui --eval 'data = load(\"data.txt\"); plot(data(:,1), data(:,2), \"r\", data(:,1), data(:,3), \"b\"); xlabel(\"%s\"); ylabel(\"%s\"); title(\"%s\"); ylim([0, %d]); pause;'", xLabel, yLabel, title, highestComparison*2);
    system(command);
}


void beginInsertionTest(list_node*linkedList, tree_node*binarySearchTree, int insertions){

    int* x = malloc(insertions * sizeof(int));
    int* y_linked_list = malloc(insertions * sizeof(int));
    int* y_binary_search_tree = malloc(insertions * sizeof(int));

    if (x == NULL || y_linked_list == NULL || y_binary_search_tree == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    int highestComparision = 0;

    int i;
    for (i = 0; i < insertions; i++) {
        int data = rand() % 100;

        y_linked_list[i] = beginInsertLinkedList(&linkedList, data);
        y_binary_search_tree[i] = insertBinarySearchTree(&binarySearchTree, data, 0);
        x[i] = i + 1;

        int comparisonsLL = y_linked_list[i];
        int comparisonsBST = y_binary_search_tree[i];

        if(comparisonsLL > highestComparision || comparisonsBST > highestComparision){
            if(comparisonsBST > comparisonsLL){
                highestComparision = comparisonsBST;
            }else if(comparisonsLL > highestComparision){
                highestComparision = comparisonsLL;
            }
        }

    }

    plotGraph(x, y_linked_list, y_binary_search_tree, insertions, "Number of Insertions", "Comparisons", "Comparison of Linked List and Binary Search Tree", highestComparision);

}

void endInsertionTest(list_node*linkedList, tree_node*binarySearchTree, int insertions){

    int* x = malloc(insertions * sizeof(int));
    int* y_linked_list = malloc(insertions * sizeof(int));
    int* y_binary_search_tree = malloc(insertions * sizeof(int));

    if (x == NULL || y_linked_list == NULL || y_binary_search_tree == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    int comparisons_linked_list = 0;
    int comparisons_binary_search_tree = 0;
    int highestComparision = 0;

}

void showSubMenu(list_node*linkedList, tree_node*binarySearchTree){
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
            beginInsertionTest(linkedList, binarySearchTree, insertions);
            break;
        case 2:

            break;

        case 3:
        
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
        showSubMenu(linkedList, binarySearchTree);

        break;
    case 2:

        break;
    default:
        break;
    }

    return 0;
}
