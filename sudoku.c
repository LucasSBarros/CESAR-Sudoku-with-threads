#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <pthread.h>

int validador;

typedef struct {
  int **matriz;
  int tamanho;
  int linha;
  int coluna;
  int subgrade_linha;
  int subgrade_coluna;
} Sudoku;


typedef struct {
  Sudoku sudoku;
} ThreadArgs;


void *validarLinha(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;
  Sudoku sudoku = args->sudoku;
  int *valores = (int *)calloc(sudoku.tamanho, sizeof(int));

  for (int i = 0; i < sudoku.tamanho; i++) {
    int val = sudoku.matriz[sudoku.linha][i];
    if (val < 1 || val > sudoku.tamanho || valores[val - 1] == 1) {
      //printf("A linha %d é inválida\n", sudoku.linha + 1);
      validador++;
      pthread_exit(NULL);
    }
    valores[val - 1] = 1;
  }
  //printf("A linha %d é válida\n", sudoku.linha + 1);
  pthread_exit(NULL);
}


void *validarColuna(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;
  Sudoku sudoku = args->sudoku;
  int *valores = (int *)calloc(sudoku.tamanho, sizeof(int));

  for (int i = 0; i < sudoku.tamanho; i++) {
    int val = sudoku.matriz[i][sudoku.coluna];
    if (val < 1 || val > sudoku.tamanho || valores[val - 1] == 1) {
      //printf("A coluna %d é inválida\n", sudoku.coluna + 1);
      validador++;
      pthread_exit(NULL);
    }
    valores[val - 1] = 1;
  }
  //printf("A coluna %d é válida\n", sudoku.coluna + 1);
  pthread_exit(NULL);
}


void *validarSubgrade(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;
  Sudoku sudoku = args->sudoku;
  int *valores = (int *)calloc(sudoku.tamanho, sizeof(int));
  int num_subgrades_linha = sudoku.linha * sudoku.subgrade_linha;
  int num_subgrades_coluna = sudoku.coluna * sudoku.subgrade_coluna;

  for (int i = num_subgrades_linha; i < num_subgrades_linha + sudoku.subgrade_linha; i++) {
    for (int j = num_subgrades_coluna; j < num_subgrades_coluna + sudoku.subgrade_coluna; j++) {
      int val = sudoku.matriz[i][j];
      if (val < 1 || val > sudoku.tamanho || valores[val - 1] == 1) {
        //printf("A subgrade %d,%d é inválida\n", sudoku.linha + 1, sudoku.coluna + 1);
        validador++;
        pthread_exit(NULL);
      }
      valores[val - 1] = 1;
    }
  }
  //printf("A subgrade %d,%d é válida\n", sudoku.linha + 1, sudoku.coluna + 1);
  pthread_exit(NULL);
}


int main(int argc, char *argv[]) {

  // DECLARAÇÃO DE VARIÁVEIS
  
  char *qnt_lin_sudoku, *qnt_coluna_sudoku, *qnt_lin_subgrade, *qnt_col_subgrade;
  char tamanho1[10], tamanho2[10], primeiro_char, *verificar_x;
  int ultimo_char, penultimo_char;
  Sudoku sudoku;

  // VERIFICAÇÃO DE ERRO NO NÚMERO DE ARGUMENTOS

  if (argc != 2) {
    printf("Invalid number of parameters");
    return 1;
  }

  // VERIFICAÇÃO DO ARQUIVO

  FILE *arq;

  // VERIFICAÇÃO DE ERRO NA ABERTURA DO ARQUIVO (ARQUIVO INEXISTENTE)

  arq = fopen(argv[1], "r"); // Abertura do arquivo para leitura

  if (arq == NULL) {
    printf("Error to read file");
    return 1;
  }
  
 // VERIFICAÇÃO DE ARQUIVO VAZIO (ERRO DE FORMATO)

  primeiro_char = fgetc(arq);

  if (primeiro_char == EOF) {
    printf("File out of format");
    return 1;
  } 

 //VERIFICAÇÃO DA EXISTÊNCIA DE ESPAÇO OU QUEBRA DE LINHA NO FINAL DO ARQUIVO

  fseek(arq, -1, SEEK_END);
  ultimo_char = fgetc(arq);

  if (ultimo_char == '\n') {
    printf("File out of format");
    return 1;
  } else if (ultimo_char == ' ') {
    printf("File out of format");
    return 1;
  }

  fclose(arq);

  
  //LEITURA DOS DADOS DO ARQUIVO

  // ATRIBUIÇÃO DO TAMANHO DA MATRIZ PRINCIPAL

  arq = fopen(argv[1], "r"); 

  if (arq == NULL) {
    printf("Error to read file");
    return 1;
  }

  fgets(tamanho1, 10, arq); // Coleta da primeira string do arquivo

  // VERIFICAR A EXISTÊNCIA DO X NA PRIMEIRA LINHA

  verificar_x = strchr(tamanho1, 'x');

  if (verificar_x == NULL) {
    printf("File out of format");
    return 1;
  }

  // SEPARAÇÃO DA PRIMEIRA PARTE DA PRIMEIRA STRING E VERIFICAÇÃO DA EXISTÊNCIA DE ARGUMENTO

  qnt_lin_sudoku = strtok(tamanho1, "x");

  if (qnt_lin_sudoku == NULL) {
    printf("File out of format");
    return 1;
  }

  sudoku.linha = atoi(qnt_lin_sudoku); // Obtenção da quantidade de linhas da matriz do Sudoku

  // SEPARAÇÃO DA SEGUNDA PARTE DA PRIMEIRA STRING E VERIFICAÇÃO DA EXISTÊNCIA DE ARGUMENTO

  qnt_coluna_sudoku = strtok(NULL, "\n");

  if (qnt_coluna_sudoku == NULL) {
    printf("File out of format");
    return 1;
  }

  sudoku.coluna = atoi(qnt_coluna_sudoku); // Obtenção da quantidade de colunas da matriz do Sudoku

  sudoku.tamanho = sudoku.linha;

  //VERIFICAÇÃO DE ERRO DE TAMANHO MÍNIMO

  if (sudoku.tamanho < 9){
    printf("File out of format");
    return 1;
  }

    // VERIFICAÇÃO DE ERRO DE FORMATO - NxN

  if (sudoku.linha != sudoku.coluna){
    printf("File out of format");
    return 1;
  }

  // ATRIBUIÇÃO DO TAMANHO DAS SUBMATRIZES

  fgets(tamanho2, 10, arq); // Coleta da segunda string do arquivo

  // VERIFICAR A EXISTÊNCIA DO X NA SEGUNDA LINHA

  verificar_x = strchr(tamanho2, 'x');
  
  if (verificar_x == NULL) {
    printf("File out of format");
    return 1;
  }

  // SEPARAÇÃO DA PRIMEIRA PARTE DA SEGUNDA STRING E VERIFICAÇÃO DA EXISTÊNCIA DE ARGUMENTO

  qnt_lin_subgrade = strtok(tamanho2, "x");

  if (qnt_lin_subgrade == NULL) {
    printf("File out of format");
    return 1;
  }

  sudoku.subgrade_linha = atoi(qnt_lin_subgrade); // Obtenção da quantidade de linhas da submatriz do Sudoku

  // SEPARAÇÃO DA SEGUNDA PARTE DA SEGUNDA STRING E VERIFICAÇÃO DA EXISTÊNCIA DE ARGUMENTO

  qnt_col_subgrade = strtok(NULL, "\n");

  if (qnt_col_subgrade == NULL) {
    printf("File out of format");
    return 1;
  }
  sudoku.subgrade_coluna = atoi(qnt_col_subgrade); // Obtenção da quantidade de colunas da submatriz do Sudoku

    //VERIFICAÇÃO DE ERRO DE FORMATO - AxB = N

  if ((sudoku.subgrade_linha * sudoku.subgrade_coluna) != sudoku.linha){
    printf("File out of format");
    return 1;
  }

  // PREENCHIMENTO DA MATRIZ DO SUDOKU

  sudoku.matriz = (int **)malloc(sudoku.tamanho * sizeof(int *));

  for (int i = 0; i < sudoku.tamanho; i++) {
    sudoku.matriz[i] = (int *)malloc(sudoku.tamanho * sizeof(int));
    for (int j = 0; j < sudoku.tamanho; j++) {
      fscanf(arq, "%d", &sudoku.matriz[i][j]);
    }
  }

  fclose(arq);

  // VERIFICAÇÃO DOS INCLUSÃO DE VALORES MAIORES QUE 0 E MENORES OU IGUAIS AO TAMANHO DA MATRIZ

  for (int i = 0; i < sudoku.linha; i++) {
    for (int j = 0; j < sudoku.coluna; j++) {    
      if (sudoku.matriz[i][j] <= 0 || sudoku.matriz[i][j] > sudoku.tamanho){
        printf("File out of format");
        return 1;
      }                  
    }
  }

  // VERIFICAÇÃO DO TAMANHO DE LINHAS E COLUNAS DA MATRIZ CONSTANTE NO ARQUIVO

  arq = fopen(argv[1], "r");

  char teste_1linha[100];
  int teste_linhas = 0, teste_colunas = 0;

  while (fgets(teste_1linha, sizeof(teste_1linha), arq)) {
        teste_colunas = 0;
        char *token = strtok(teste_1linha, " ");
        while (token != NULL) {
            teste_colunas++;
            token = strtok(NULL, " ");
        }
        teste_linhas++;
    }

    //printf("O arquivo tem %d linhas e %d colunas.\n", teste_linhas, teste_colunas);

    if(teste_linhas>sudoku.tamanho+2 || teste_colunas != sudoku.tamanho){
      printf("File out of format");
      return 1;
    }

  fclose(arq);

  // VERIFICAÇÃO DA EXISTÊNCIA DE ESPAÇOS EM BRANCO NO INÍCIO E NO FINAL DE CADA LINHA DO ARQUIVO

  arq = fopen(argv[1], "r");

  char teste_2linha[100];

  int num_linha = 1;
    while (fgets(teste_2linha, sizeof(teste_2linha), arq)) {
      int i = 0;
      while (teste_2linha[i] == ' ') {
        i++;
        }
        if (i > 0) {
        //printf("Espaço em branco no início da linha %d.\n", num_linha);
          printf("File out of format");
          return 1;
        }

        int j = strlen(teste_2linha) - 2;

        if (teste_2linha[j+1] != '\n') {
          j = strlen(teste_2linha) - 1;
        }
       
        while (j >= 0 && (teste_2linha[j] == ' ')) {
          j--;
        }
        if (j < strlen(teste_2linha) - 2) {
        //printf("Espaço em branco no final da linha %d.\n", num_linha);
          printf("File out of format");
          return 1;
        }

        num_linha++;
    }

  fclose(arq);

  // INICIALIZAÇÃO DAS THREADS

  int num_subgrades_linha = sudoku.tamanho / sudoku.subgrade_linha;
  int num_subgrades_coluna = sudoku.tamanho / sudoku.subgrade_coluna;
  int count = 0;
  int num_threads = sudoku.tamanho * 3;

  pthread_t threads[num_threads];

  for (int i = 0; i < sudoku.linha; i++) {
    ThreadArgs *args_linha = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    args_linha->sudoku = sudoku;
		args_linha->sudoku.linha = i;
    pthread_create(&threads[i], NULL, validarLinha, (void *)args_linha);
  }

  for (int i = 0; i < sudoku.coluna; i++) {
    ThreadArgs *args_coluna = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    args_coluna->sudoku = sudoku;
		args_coluna->sudoku.coluna = i;
    pthread_create(&threads[i + sudoku.coluna], NULL, validarColuna, (void *)args_coluna);
  }

  ThreadArgs *args_subgrade = (ThreadArgs *)malloc(num_subgrades_linha * num_subgrades_coluna * sizeof(ThreadArgs));

  for (int i = 0; i < num_subgrades_linha; i++) {
    for (int j = 0; j < num_subgrades_coluna; j++) {
      args_subgrade[count].sudoku = sudoku;
      args_subgrade[count].sudoku.linha = i;
      args_subgrade[count].sudoku.coluna = j;
      pthread_create(&threads[count+sudoku.linha+sudoku.coluna], NULL, validarSubgrade, (void *)&args_subgrade[count]);
      count++;
    }
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

// VALIDAÇÃO E IMPRESSÃO DA RESPOSTA NO ARQUIVO

  FILE *resposta;

  resposta = fopen("sudoku_lsb5.out", "w");

  if (resposta == NULL) {
    printf("Error to read file.");
    return 1;
  }

  if (validador > 0) {
    fprintf(resposta, "FAIL");
    //printf("%d\n", validador);
  } else {
    fprintf(resposta, "SUCCESS");
    //printf("%d\n", validador);
  }

  fclose(resposta);

  return 0;
}