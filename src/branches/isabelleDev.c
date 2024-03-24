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
    //printf("valor a ser inserido: %d\n", value);

    int comparisons = 0;
    list_node* newNode = (list_node*)malloc(sizeof(list_node));
    newNode->data = value;

    if (newNode == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        return 0;
    }

    if(isLinkedListEmpty(*head)){
        newNode->next = NULL;
        *head = newNode;

    }else if(value < (*head)->data){
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
    //abre o arquivo ou cria ele
    //caso seja para inserções, o arquivo se chamará "insertion.txt"
    //caso seja para buscas, o arquivo se chamará "search.txt"
    FILE* fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("Error: Unable to create data file.\n");
        return;
    }

    int i;
    for (i = 0; i < n; i++) {
        //escreve no arquivo aberto ou criado, em cada coluna, os pontos do eixo x, do eixo y para a lista encadeada e do eixo y para a árvore de busca binária, respectivamente
        //caso haja strings para nomear os pontos, também serão escritas
        fprintf(fp, "%d %d %d %s\n", x[i], y1[i], y2[i], names[i]);
    }
    //fecha-se o arquivo
    fclose(fp);

    //string que armazenará o comando para o Octave
    char command[1000];

    //caso o número de inserções (n) seja maior ou igual a 20, será plotado somente com os pontos, sem as strings (rótulos para os pontos)
    if (n >= 20) {
        // sprintf formata uma string e armazena ela na variável "command"
        // o comando começa verdadeiramente após as aspas simples
        // data é uma variável que recebe a leitura do arquivo txt que foi gerado acima
        // a função plot(), nativa do octave, plota os dados da primeira coluna versus o da segunda coluna do arquivo (data(:,1), data(:,2)) usando o comando "r", que indica que esse gráfico seja vermelho (red) => lista encadeada
        // depois, plota os dados da primeira coluna versus o da terceira coluna usando o comando "b", que indica que esse gráfico seja azul (blue) => árvore de busca binária
        // xlabel e ylabel definem rótulos para o eixo x e eixo y, respectivamente
        // title define um título que ficará lá em cima do gráfico
        // xlim e ylim definem limites para o eixo x e para o eixo y
        // pause é usado para que o Octave pause a execução após a criação do gráfico. Esse comando foi necessário porque, sem ele, após abrir o gráfico, o próprio C terminaria o programa, fechando ele em seguida
        sprintf(command, "octave --no-gui --eval 'data = dlmread(\"%s\"); plot(data(:,1), data(:,2), \"r\", data(:,1), data(:,3), \"b\"); xlabel(\"%s\"); ylabel(\"%s\"); title(\"%s\"); xlim([0, %d]); ylim([0, %d]); pause;'", fileName, xLabel, yLabel, title, n-1, highestComparison*2);
    } else {
        //para mostrar os rótulos dos pontos, diferencia-se a função para ler o arquivo
        //usa-se fopen() para ler o arquivo e guarda-se as informações dele na variável "fid"
        // usa-se textscan para ler as informações do arquivo em 4 colunas, sendo a quarta coluna, a coluna de string
        // uma variável "names" recebe as strings da coluna 4
        // a função text() plota a variável names na ordem do eixo x, mas um pouco acima (+ 0.5)
        sprintf(command, "octave --no-gui --eval 'fid = fopen(\"%s\", \"r\"); data = textscan(fid, \"%%d %%d %%d %%s\"); fclose(fid); names = cellstr(data{4}); plot(data{1}, data{2}, \"r\", data{1}, data{3}, \"b\"); text(data{1}, data{2} + 0.5, names); xlabel(\"%s\"); ylabel(\"%s\"); title(\"%s\"); xlim([0, %d]); ylim([0, %d]); pause;'", fileName, xLabel, yLabel, title, n-1, highestComparison*2);
    }

    //invoca um comando pro S.O.
    system(command);
}

int makeInsertions(list_node**linkedList, tree_node**binarySearchTree, int insertions, int(*linkedListFunction)(list_node**, int), int*x, int*y_linked_list, int*y_binary_search_tree, int isSearch, char**names){

    //o maior número de comparações feito durante o loop será usado para a plotagem
    int highestComparision = 0, i;

    for (i = 0; i < insertions; i++) {
        //número aleatório gerado que será inserido
        int data = rand() % 100;

        //número de comparações para cada estrutura
        int comparisonsLL, comparisonsBST;
        //o eixo x será populado com o número de inserções feito (i que será incrementado no loop for)
        x[i] = i;
        //string que é usada, caso o número de inserções seja menor que 20
        char result[50];

        //caso a função seja chamada num teste de busca
        if(isSearch){
            //popula-se as estruturas de dados com o número aleatório gerado
            linkedListFunction(linkedList, data);
            insertBinarySearchTree(binarySearchTree, data);

            //cria-se outro número aleatório para se buscar na estrutura (leva, na maioria das vezes ao pior caso, de não ter aquele número, deixando o gráfico ainda mais preciso)
            int random = rand() % 100;
            //string para concatenar (nome dos pontos, caso insertions < 20)
            if(insertions < 20){
                char original[] = "Searched:";
                sprintf(result, "%s%d", original, random);
            }

            //o número de comparações será, então, das operações de busca em cada uma das estruturas
            comparisonsLL = searchLinkedList(*linkedList, random);
            comparisonsBST = searchBinarySearchTree(*binarySearchTree, random);
        }else{
            //caso não seja o teste de busca, o número de comparações virá das operações de inserção
            if(insertions< 20){
                char original[] = "Inserted:";
                sprintf(result, "\"%s%d\"", original, data);
            }
            
            comparisonsLL = linkedListFunction(linkedList, data);
            //printLinkedList(*linkedList);
            //printf("comparacoes %d\n", comparisonsLL);
            comparisonsBST = insertBinarySearchTree(binarySearchTree, data);
        }

        //o número de comparações, enfim, será atribuído às arrays de coordenada y para cada uma das estruturas, para posterior plotagem
        y_linked_list[i] = comparisonsLL;
        y_binary_search_tree[i] = comparisonsBST;

        if(insertions<20){
            //maneira de se atribuir à array de strings inicializada na função de teste a nova string (nome do ponto que foi buscado ou inserido)
            //o resultado é parecido com => "Inserted:x" ou "Searched:x"
            names[i] = malloc(strlen(result) + 1); 
            if (names[i] != NULL) {
                strcpy(names[i], result); 
            }
        }

        //elenca-se o maior número de comparações que houve durante o loop, para colocar limite no eixo y do gráfico e não ficar muito pequeno ou muito grande
        //poder-se-ia fazer percorrer toda a array posteriormente buscando por esse maior número, mas evita aumentar a complexidade
        if(comparisonsLL > highestComparision || comparisonsBST > highestComparision){
            if(comparisonsBST > comparisonsLL){
                highestComparision = comparisonsBST;
            }else if(comparisonsLL > highestComparision){
                highestComparision = comparisonsLL;
            }
        }

    }

    //retorna o maior número de comparações feita durante o loop
    return highestComparision;
}

void searchTest(list_node*linkedList, tree_node*binarySearchTree, int insertions){

    //arrays do gráfico (número de comparações x número de buscas)
    int* xSearches = malloc(insertions * sizeof(int)); // eixo x
    int* yLLComparisons = malloc(insertions * sizeof(int)); // pontos do eixo y da lista encadeada
    int* yBSTComparisons = malloc(insertions * sizeof(int)); // pontos do eixo y da árvore de busca binária
    char** names = malloc(insertions * sizeof(char*)); // nomes dos pontos

    if(insertions < 20){
        //não haverá nomes para os pontos gerados no gráfico, porque fica muito apertado (uma bagunça)
        for (int i = 0; i < insertions; i++) {
            names[i] = malloc(20 * sizeof(char));
        }
    }

    //verificação do processo de alocação
    if (xSearches == NULL || yLLComparisons == NULL || yBSTComparisons == NULL || names == NULL) {
            printf("Error: Memory allocation failed.\n");
            return;
    }
    
    //faz-se as inserções de números aleatórios gerados na função makeInsertions, que retorna o maior número de comparações feito durante o processo
    //envia-se o binário 1 para indicar que está chamando ela com finalidade de teste de busca
    //como teste de busca, ela vai inserir e, a cada inserção, fazer uma busca por um outro número aleatório gerado
    int highestComparision = makeInsertions(&linkedList, &binarySearchTree, insertions, beginInsertLinkedList, xSearches, yLLComparisons, yBSTComparisons, 1, names);

    //função de plotagem do gráfico
    plotGraph(xSearches, yLLComparisons, yBSTComparisons,names, insertions, "Número de Elementos Inseridos", "Número de Comparações", "Plotagem de Busca - Vermelho = Lista Encadeada, Azul = Árvore de Busca Binária", highestComparision, "search.txt");

}

void insertTest(list_node**linkedList, tree_node**binarySearchTree, int insertions, int(*linkedListFunction)(list_node**, int), int(*binarySearchTreeFunction)(tree_node**, int)){

    //arrays do gráfico (número de comparações x número de inserções)
    int* x = malloc(insertions * sizeof(int)); // eixo x do gráfico
    int* y_linked_list = malloc(insertions * sizeof(int)); // pontos do eixo y da lista encadeada
    int* y_binary_search_tree = malloc(insertions * sizeof(int)); // pontos do eixo y da arvore de busca binaria
    char** names = malloc(insertions * sizeof(char*)); // nomes para os pontos do gráfico

    if(insertions < 20){
        //não haverá nomes para os pontos gerados no gráfico, porque fica muito apertado (uma bagunça)
        for (int i = 0; i < insertions; i++) {
            names[i] = malloc(20 * sizeof(char));
        }
    }

    //verificação do processo de alocação
    if (x == NULL || y_linked_list == NULL || y_binary_search_tree == NULL || names == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    //faz-se as inserções de números aleatórios gerados na função makeInsertions, que retorna o maior número de comparações feito durante o processo
    //envia-se o binário 0 para indicar que não se está chamando essa função com finalidade de um teste de busca
    int highestComparision = makeInsertions(linkedList, binarySearchTree, insertions, linkedListFunction, x, y_linked_list, y_binary_search_tree, 0, names);

    //função de plotagem do gráfico
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

        printf("Insira quantas inserções aleatórias deseja que sejam feitas para que um número sorteado seja buscado: \n");
        scanf("%d", &insertions);

        searchTest(linkedList, binarySearchTree, insertions);

        break;
    default:
        break;
    }

    return 0;
}
