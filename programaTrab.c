/*Autores:
 Juan Carlos Elias Obando Valdivia nro USP: 7487156
 Lucas Fernandes de Nóbrega nro USP: 9805320
 João Pedro Doimo Torrezan nro USP: 9806933
 Enrico Vicenzo Salvador Robazza nro USP: 9806738
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "registro.h"

#define FIELD1DOMAIN 50000
#define FIELD2DOMAIN 10700
#define FIELD3DOMAIN 32468
#define FIELD4DOMAIN 10956
#define NUMEROREGISTROSUBARQUIVO 1000

///////////////// //MÉTODOS PARA GERAR O ARQUIVO///////////////////////////////
unsigned int * generate_iucr(int N);
char ** generate_description(int N);
char ** generate_name(int N);
char ** generate_date(int N);
int gerar_arquivo( char *nomeArquivo, int N);
///////////////// //MÉTODOS PARA MOSTRAR O ARQUIVO//////////////////////////////////////////////////
registro ** ler_arquivo(char *nomeArquivo, int *counter);
///////////////// //MÉTODOS PARA REALIZAR A ORDENACÃO INTERNA///////////////////////////////////////
int ordenar_arquivo(char *nomeArquivoEntrada, char *nomeArquivoQueSeraGerado);
int particiona (registro **v, int p, int r);
void quicksort (registro** v, int p, int r);
void swap(registro **v, int j, int k);
int strcmp_datas (char *date1, char *date2);
int compara_datas (int dia1, int mes1, int ano1, int dia2, int mes2, int ano2);
///////////////// //MÉTODOS PARA REALIZAR O MERGING///////////////////////////////////////////////
int merging(char *nomeArquivoEntrada, char *nomeArquivoEntrada2, char *nomeArquivoQueSeraGerado);
///////////////// //MÉTODOS PARA REALIZAR O MATCHING///////////////////////////////////////////////
int matching(char *nomeArquivoEntrada, char *nomeArquivoEntrada2, char *nomeArquivoQueSeraGerado);
///////////////// //MÉTODOS PARA REALIZAR O MULTIWAY MERGING//////////////////////////////////////////////////////////
int multiway_merging(char listaArquivosEntrada[100][100], int numeroArquivosEntrada, char *nomeArquivoQueSeraGerado);
registro **encontrarMenor(registro* listRegistros[], int numeroArquivosEntrada, int **listaPosIguais, int *counter);
///////////////// //MÉTODOS PARA REALIZAR A ORDENAÇÃO EXTERNA//////////////////////////
int ordenar_externa_arquivo(char *nomeArquivoEntrada, char *nomeArquivoQueSeraGerado);
int criarSubArquivos (char* nomeArquivoEntrada, int *contNumFiles);
char *criarNomeSub(char *nomeArquivoEntrada, int contNumFiles);
char *criarNomeSubOrdenado(char *nomeArquivoEntrada, int contNumFiles);
char *criarNomeSubIntermediario(int contNumFiles,int pos, char * camada);
///////////////// //MÉTODOS AUXILIARES/////////////////////////////////////////////
int escrever_arquivo(char *nomeArquivo, int counter, registro** listaDeRegistros);
int escrever_registro (FILE *arquivo, registro *reg);
registro* ler_registro(FILE *arquivo);



//////////////////////////////////////////////////////////////////////////MÉTODOS PARA GERAR O ARQUIVO//////////////////////////////////////////////////////////////////////////////

/*O método generate_iucr recebe como parâmetro um inteiro N que representa o número de registros de um arquivo, assim devem ser gerados aleatoriamente N códigos iucr(campo 1).
O método devolve um vetor de interiros sem sinal com os iucrs gerados. O objetivo deste método é gerar N iucrs sendo que 30% de N estejam repetidos */
unsigned int * generate_iucr(int N) {

  unsigned int *iucrList;
  int *isInList;
  //iucrList representará o vetor de inteiros sem sinal com os iucrs gerados, por isso deve ter tamanho N
  iucrList = malloc (N * sizeof (unsigned int));
  //o vetor isInList de tamanho FIELD1DOMAIN = 50000 será preenchido com 1 caso o iucr já tenha sido gerado, e com 0 caso contrário
  isInList = calloc (FIELD1DOMAIN, sizeof (int));

  int max = N*0.7; //max representa 70% de N 
  int i = 0;
  int iucr;

  //para gerar os 70% iucrs diferentes
  while (i < max) {
    iucr = rand()%FIELD1DOMAIN; //iucr será gerado aleatoriamente entre 0 e FIELD1DOMAIN

    //se o iucr não está em inInList, coloco iucr em iucrList e o iucr ja terá sido usado em isInList
    if(isInList[iucr] == 0){
      iucrList[i] = iucr;
      isInList[iucr] = 1;
      i=i+1;
    }

  }

  //Já foram gerados os 70% iucrs diferentes, agora só falta gerar os 30% iucrs repetidos(i parou em 70% de N)
  while (i < N){  
    iucr= rand()%FIELD1DOMAIN;
    //se o iucr está na lista então ele é repetido, adicionamos ele a iucrList
    if(isInList[iucr] == 1){
      iucrList[i] = iucr;
      i=i+1;
    }

  }

  return iucrList;
}

/*O método generate_description recebe como parâmetro um inteiro N que representa o número de registros de um arquivo, assim devem ser geradas aleatoriamente N descrições do crime(campo 2).
O método devolve um vetor de strings com as descrições geradas. O objetivo deste método é gerar N descrições sendo que 25% de N estejam repetidas*/
char ** generate_description(int N) {

  char *description;
  //descriptionListComplete será um vetor de strings com tamanho FIELD2DOMAIN * 30 com todas as descrições do arquivo DescriptionsCrimes.csv
  char **descriptionListComplete= (char **) malloc (FIELD2DOMAIN * 30);
  //descriptionList representará o vetor de strings com as descrições dos crimes geradas, por isso deve ter tamanho N
  char **descriptionList= (char **) malloc (N * 30);
  int i;
  //o vetor isInList de tamanho FIELD2DOMAIN será preenchido com 1 caso a descrição já tenha sido gerada, e com 0 caso contrário
  int *isInList = calloc (FIELD2DOMAIN, sizeof (int));

  int max = N * 0.75; //max representa 75% de N 

  FILE *arquivo;
  //na hora de rodar o arquivo DescriptionsCrimes.csv tem que estar na mesma pasta do arquivo programaTrab.c
  arquivo = fopen("DescriptionsCrimes.csv", "r");

  if(!arquivo) {
    perror("Falha no processamento.\n");
    return NULL;
  } 
  else {
    //leitura caracter a caracter no arquivo DescriptionsCrimes.csv
    for(i = 0; i < FIELD2DOMAIN; i++) {
      //o campo 2(descrição do crime) tem 30 bytes
      description = (char *) malloc (30);
      int j = 0;
      char *c = (char *) malloc (1);
      fread(c, 1, 1, arquivo);

      while (*c != '\n') {
        description[j] = *c;
        j++;
        fread(c, 1, 1, arquivo);
      }

      //caso descriptions tenha menos de 30 caracteres, completar a descrição com espaços em branco
      while(j < 30){
        description[j] = ' ';
        j++;
      }

      //cada descrição lida do arquivo DescriptionsCrimes.csv eh adicionada a descriptionListComplete
      descriptionListComplete[i]=description;
    }
    fclose(arquivo);
  }

  i = 0;
  int id;

  //para gerar as 75% descrições diferentes
  while (i < max) {
    id = rand()%FIELD2DOMAIN;  //id será gerado aleatoriamente entre 0 e FIELD2DOMAIN, id representa posições nos vetores
    //se não está em inInList, coloco description em descriptionList e description já terá sido usada em isInList
    if(isInList[id] == 0){
      descriptionList[i] = descriptionListComplete[id];
      isInList[id] = 1;
      i = i + 1;
    }
  }
  //Já foram geradas as 75% descrições diferentes, agora só falta gerar as 25% descrições repetidas(i parou em 75% de N)
  while (i < N) {
    id = rand()%FIELD2DOMAIN;
    //se a posição id de isInList é igual a 1 então temos uma descrição repetida, adicionamos ela a descriptionList por meio de seu id
    if(isInList[id] == 1){
      descriptionList[i] = descriptionListComplete[id];
      i = i + 1;
    }
   }
  return descriptionList;
}

/*O método generate_name recebe como parâmetro um inteiro N que representa o número de registros de um arquivo, assim devem ser gerados aleatoriamente N nomes(campo 3).
O método devolve um vetor de strings com os nomes gerados. O objetivo deste método é gerar N nomes sendo que 20% de N estejam repetidos*/
char ** generate_name(int N) {

  char *nome;
  //nomeListComplete será um vetor de strings com tamanho FIELD3DOMAIN * 3 com todas as descrições do arquivo Nomes2017.csv
  char **nomeListComplete= (char **) malloc (FIELD3DOMAIN * 20);
  //nomeList representará o vetor de strings com os nomes gerados, por isso deve ter tamanho N
  char **nomeList= (char **) malloc (N * 20);
  int i;
  //o vetor isInList de tamanho FIELD3DOMAIN será preenchido com 1 caso o nome já tenha sido gerado, e com 0 caso contrário
  int *isInList = calloc (FIELD3DOMAIN, sizeof (int));
  int max = N * 0.80;  //max representa 80% de N 

  FILE *arquivo;
  arquivo = fopen("Nomes2017.csv", "r");

  if(!arquivo) {
    perror("Falha no processamento.\n");
    return NULL;
  }
  else {
    //leitura caracter a caracter no arquivo Nomes2017.csv
    for(i = 0; i< FIELD3DOMAIN; i++) {
      nome = (char *) malloc (20);
      int j = 0;
      char *c = (char *) malloc (1);
      fread(c, 1, 1, arquivo);
      while (*c != '\n'){
        nome[j] = *c;
        j++;
        fread(c, 1, 1, arquivo);
      }
      //caso nome tenha menos de 20 caracteres, completar a descrição com espaços em branco
      while(j < 20){
        nome[j] = ' ';
        j++;
      }
      //cada nome lido do arquivo Nomes2017.csv é adicionado a nomeListComplete
      nomeListComplete[i]=nome;
    }
    fclose(arquivo);
  }

  i = 0;
  int id;

  //para gerar os 80% de nomes diferentes
  while (i < max) {
    id = rand()%FIELD3DOMAIN; //id será gerado aleatoriamente entre 0 e FIELD3DOMAIN, id representa posições nos vetores
    //se não está em inInList, coloco nome em nomeList e nome já terá sido usado em isInList
    if(isInList[id] == 0) {
      nomeList[i] = nomeListComplete[id];
      isInList[id] = 1;
      i=i+1;
    }
  }
  //Já foram geradas as 80% nomes diferentes, agora só falta gerar os 20% nomes repetidos(i parou em 80% de N)
  while (i < N){
    id = rand()%FIELD3DOMAIN;

    //se a posição id de isInList é igual a 1 então temos um nome repetido, adicionamos ele a nomeList por meio de seu id
    if(isInList[id] == 1){
      nomeList[i] = nomeListComplete[id];
      i = i + 1;
    }
  }
  
  return nomeList;

}

/*O método generate_date recebe como parâmetro um inteiro N que representa o número de registros de um arquivo, assim devem ser geradas aleatoriamente N datas(campo 4).
O método devolve um vetor de strings com as datas geradas. O objetivo deste método é gerar N datas sendo que 15% de N estejam repetidas*/
char ** generate_date(int N) {
  char *date;
  //dateListComplete será um vetor de strings com tamanho FIELD4DOMAIN com todas as datas do arquivo datas.csv
  char **dateListComplete= (char **) malloc (FIELD4DOMAIN *10);
  //dateList representará o vetor de strings com as datas geradas, por isso deve ter tamanho N
  char **dateList= (char **) malloc (N * 10);
  int i;
  //o vetor isInList de tamanho FIELD4DOMAIN será preenchido com 1 caso a data já tenha sido gerada, e com 0 caso contrário
  int *isInList = calloc (FIELD4DOMAIN, sizeof (int));
  int max = N * 0.85; //max representa 85% de N

  FILE *arquivo;
  arquivo = fopen("datas.csv", "r");

  if(!arquivo) {
    perror("Falha no processamento.\n");
    return NULL;
  }
  else{
    //leitura caracter a caracter no arquivo datas.csv
    for(i = 0; i < FIELD4DOMAIN; i++)
    {
      date = (char *) malloc (10);
      int j = 0;
      char *c = (char *) malloc (1);
      fread(c, 1, 1, arquivo);
      while (*c != '\n'){
        date[j]=*c;
        j++;
        fread(c, 1, 1, arquivo);
      }
      //cada nome lido do arquivo datas.csv eh adicionada a dateListComplete
      dateListComplete[i] = date;
    }
    fclose(arquivo);
  }

  i = 0;
  int id;
  //para gerar os 85% de datas diferentes
  while (i<max) {
    id = rand()%FIELD4DOMAIN; //id será gerado aleatoriamente entre 0 e FIELD4DOMAIN, id representa posições nos vetores
    //se não está em inInList, coloco date em dateList date já terá sido usada em isInList
    if(isInList[id] == 0){
      dateList[i] = dateListComplete[id];
      isInList[id] = 1;
      i=i+1;

    }
  }
  //Já foram geradas as 85% datas diferentes, agora só falta gerar as 15% datas repetidas(i parou em 85% de N)
  while (i<N){
    id= rand()%FIELD4DOMAIN;
    //se a posição id de isInList é igual a 1 então temos uma data repetida, adicionamos ela a dateList por meio de seu id
    if(isInList[id] == 1){
      dateList[i] =  dateListComplete[id];
      i = i + 1;
    }
  }
  
  return dateList;

}


/*O método gerar_arquivo recebe uma string com o nome do arquivo que será gerado(escrito) e o número de registros nesse arquivo representado pelo inteiro N. Devolve 1 caso o arquivo
tenha sido gerado com sucesso, 0 caso contrário. O objetivo do método é realizar a geração automática de N registros e escrever esses registros no arquivo de dados de entrada, 
sem qualquer ordenação dos dados.*/
int gerar_arquivo(char *nomeArquivo, int N) {
  int i;
  FILE *arquivo;
  
  //abre o arquivo nomeArquivo para escrita
  arquivo = fopen(nomeArquivo, "w");

  //gera 4 vetores cada um com: N iucrs, N descriptions, N nomes, N dates
  unsigned int *iucrList = generate_iucr(N);
  char **descriptionList = generate_description(N);
  char **nomeList = generate_name(N);
  char **dateList = generate_date(N);
 
  if(!arquivo || iucrList == NULL || descriptionList == NULL || nomeList == NULL || dateList ==NULL) {
    perror("Falha no processamento.\n");
    return 0;
  }

  else {
    //escreve cada um dos 4 campos no arquivo(N registros)
    for (i = 0; i < N; i++) {
      fwrite(&iucrList[i], sizeof(unsigned int), 1, arquivo); //campo1
      fwrite(descriptionList[i], 30, 1, arquivo);  //campo2
      fwrite(nomeList[i], 20, 1, arquivo);  //campo3
      fwrite(dateList[i], 10, 1, arquivo);  //campo4
    }
    fclose(arquivo);
  }
  return 1;
}

///////////////// //MÉTODOS PARA MOSTRAR O ARQUIVO///////////////////////////////
/*O metodo ler_arquivo recebe uma string com o nome do arquivo de entrada. Devolve uma lista de registros e um contador inteiro counter que representa o tamanho dessa lista
 O objetivo deste método é realizar a leitura de um arquivo que contem registros de tamanho fixo(4 campos de tamanho fixo)*/
registro ** ler_arquivo(char *nomeArquivo, int *counter){
  FILE *arquivo;
  registro *reg;
  registro **listaDeRegistros = NULL; 
  int i;
  *counter = 0;
  //abrimos o arquivo nomeArquivo para leitura
  arquivo = fopen(nomeArquivo, "r");
  if(!arquivo) {
    perror("Falha no processamento.\n");
    return NULL;
  }
  else {
    while (1) {
      //alocando espaco para cada registro reg lido
      reg = (registro *) malloc (sizeof (registro));
      //leitura do iucr que esta no registro reg
      if(!fread (&reg->iucr, sizeof(unsigned int), 1, arquivo)) {
        break;
      }
      //leitura da description que esta no registro reg
      for (i = 0; i < 30; i++) {
        fread(&reg->description[i], 1, 1, arquivo);
      }
      reg->description[30] = '\0';
      //leitura do nome que esta no registro reg
      for (i = 0; i < 20; i++) {
        fread(&reg->name[i], 1, 1, arquivo);
      }
      reg->name[20] = '\0';
      //leitura da data que esta no registro reg
      for (i = 0; i < 10; i++) {
        fread(&reg->date[i], 1, 1, arquivo);
      }
      reg->date[10] = '\0';
      (*counter)++;
      //colocando o registro reg que acabou de ser lido na lista de registros
      listaDeRegistros = (registro **) realloc (listaDeRegistros, sizeof(registro)*(*counter));
      listaDeRegistros[*counter-1] = reg;
    }

    fclose(arquivo);
    return listaDeRegistros; 
  }
}

///////////////// //MÉTODOS PARA REALIZAR A ORDENACÃO INTERNA///////////////////////////////

/*A função orrdenar_arquivo recebe uma string como o nome do arquivo que sera ordenado, e uma string com o nome do arquivo que estará ordenado. Devolve 1 caso o arquivo de saída esteja ordenado */
int ordenar_arquivo(char *nomeArquivoEntrada, char *nomeArquivoQueSeraGerado) {
  int counter = 0;
  //realiza a leitura de um arquivo, e armazena os os registros deste arquivo num vetor de registros
  registro **listaDeRegistros = ler_arquivo(nomeArquivoEntrada, &counter);
  //ordena os registros deste arquivo considerando a ordem de ordanação dos campos
  quicksort (listaDeRegistros, 0, counter-1);
  //escreve o arquivo ordenado no arquivo de saída
  escrever_arquivo(nomeArquivoQueSeraGerado, counter, listaDeRegistros);
  return 1;
}

/*O algoritmo quicksort da literatura foi implementado. O método quicksort recebe um vetor de registros v, as posições p e r(inteiros), tal que p <= r. O objetivo
é rearranjar o vetor v[p..r] em ordem crescente */
void quicksort (registro** v, int p, int r)
{
   int j;
   /*Se p < r, o código reduz a instância v[p..r] do problema ao par de instâncias v[p..j-1] e v[j+1..r], com p ≤ j ≤ r */
   if (p < r) {                   
      j = particiona (v, p, r);   
      quicksort (v, p, j-1);   //v[p..j-1] estará em ordem crescente   
      quicksort (v, j+1, r);   //v[j+1..r] estará em ordem crescente
      //Como v[p..j-1] ≤ v[j] < v[j+1..r] graças à função particiona, o vetor todo estará ordenado considerando a ordem dos campos nos registros
   }
}


/* Recebe vetor v de registros e os inteiros p, r que representam posições no vetor, tal que p <= r. O vetor v[p..r] é particionado e j devolvido tal que v[p..j-1] <= v[j] < v[j+1..r] */
int particiona (registro **v, int p, int r) {
   unsigned int pivo_iucr = v[r]->iucr; 
   char *pivo_description = v[r]->description;
   char *pivo_name = v[r]->name;
   char *pivo_date = v[r]->date;
   int j = p;
   int k;

   for (k = p; k < r; k++) {
      //ordena pelo campo 1
      if (v[k]->iucr < pivo_iucr) {
         swap(v, j, k);
         j++; 
      } 
      //se ocorre empate no campo1
      if (v[k]->iucr == pivo_iucr) {
        //ordena pelo campo 2
        if (strcmp(v[k]->description,pivo_description) < 0) {
          swap(v, j, k);
          j++; 
        }
        //se ocorre empate no campo2
        if (strcmp(v[k]->description,pivo_description) == 0) {
          //ordena pelo campo 3
          if (strcmp(v[k]->name,pivo_name) < 0) {
            swap(v, j, k);
            j++; 
          }
          //se ocorre empate no campo3
          if (strcmp(v[k]->name,pivo_name) == 0) {
            //ordena pelo campo 4
            if (strcmp_datas(v[k]->date,pivo_date) < 0) {
              swap(v, j, k);
              j++;
            }
          }
        } 
      }
   }
   swap(v, j, r);
   return j; 
}

//A função swap recebe um vetor de registros v. Realiza a troca entre v[j] e v[k]
void swap(registro **v, int j, int k) {
  registro *aux = v[j];
  v[j] = v[k];
  v[k] = aux;
}

/*O método strcmp_datas recebe duas strings datas no formato dia/mes/ano e devolve o resultado de compara_datas a partir dos inteiros obtidos a partir das strings para dia, mes e ano*/
int strcmp_datas (char *date1, char *date2) {
 char dia1[3];
 char mes1[3];
 char ano1[5];
 char dia2[3];
 char mes2[3];
 char ano2[5];
 int i;
 for (i = 0; i < 2; i++) {
  dia1[i] = date1[i];
  dia2[i] = date2[i];
 }
 dia1[2] = '\0';
 dia2[2] = '\0';
 for (i = 0; i < 2; i++) {
  mes1[i] = date1[i+3];
  mes2[i] = date2[i+3];
 }
 mes1[2] = '\0';
 mes2[2] = '\0';
 for (i = 0; i < 4; i++) {
  ano1[i] = date1[i+6];
  ano2[i] = date2[i+6];
 }
 ano1[4] = '\0';
 ano2[4] = '\0';

 return compara_datas (atoi(dia1), atoi(mes1), atoi(ano1), atoi(dia2), atoi(mes2), atoi(ano2));

}

/*O método compara_datas recebe 6 inteiros que representam duas datas cada uma com dia, mes e ano. Devolve -1 caso a primeira data seja menor que a segunda data, 0 caso
as datas sejam iguais, e 1 caso a segunda data seja menor que a primeira data */
int compara_datas (int dia1, int mes1, int ano1, int dia2, int mes2, int ano2) {
  if (ano1 < ano2)
    return -1;
    
    else if (ano1 > ano2)
      return 1;

    if (ano1 == ano2) {
      if (mes1 < mes2)
        return -1;
      else if (mes1 > mes2)
        return 1;
      else if (dia1 < dia2)
        return -1;
      else if(dia1 > dia2)
        return 1;
      else
        return 0;
    }
}


///////////////// //MÉTODOS PARA REALIZAR O MERGING///////////////////////////////
/*O método merging recebe três strings: duas com os nomes dos arquivos de entrada ordenados, e outra com o nome do arquivo de saída que terá a operação de merging realizada com os dois 
arquivos de entrada. O método devolve 1 caso o arquivo de merging(união) tenha sido gerado, 0 caso contrário*/
int merging(char *nomeArquivoEntrada, char *nomeArquivoEntrada2, char *nomeArquivoQueSeraGerado) {
  FILE* arquivoEntrada;
  FILE* arquivoEntrada2;
  FILE* arquivoQueSeraGerado;

  //os dois primeiros arquivos são abertos para leitura
  arquivoEntrada = fopen (nomeArquivoEntrada, "r");
  arquivoEntrada2 = fopen (nomeArquivoEntrada2, "r");
  //o arquivo de saída é aberto para escrita 
  arquivoQueSeraGerado = fopen(nomeArquivoQueSeraGerado, "w");
  registro *reg1;
  registro *reg2;

  if(!arquivoEntrada || !arquivoEntrada2 || !arquivoQueSeraGerado) {
    perror("Falha no processamento.\n");
    return 0;
  }
  else {
    //lê o primeiro registro do primeiro arquivo
    reg1 = ler_registro(arquivoEntrada);
    //lê o primeiro registro do segundo arquivo
    reg2 = ler_registro(arquivoEntrada2);
    //enquanto existirem mais registros em cada um dos arquivos 
    while (reg1 != NULL && reg2 != NULL) {
      //se o iucr do registro do primeiro arquivo for menor que o iucr do segundo arquivo
      if (reg1->iucr < reg2->iucr) {
        //escrever o registro do primeiro arquivo no arquivo de saída
        escrever_registro(arquivoQueSeraGerado, reg1);
        //ler o próximo registro do primeiro arquivo
        reg1 = ler_registro(arquivoEntrada);
      }
      else {
        //se o iucr do registro do primeiro arquivo for maior que o iucr do segundo arquivo
        if (reg1->iucr > reg2->iucr) {
          //escrever o registro do segundoa rquivo no arquivo de saída
          escrever_registro(arquivoQueSeraGerado, reg2);
          //ler o próximo registro do segundo arquivo
          reg2 = ler_registro(arquivoEntrada2);
        }
        else { //se os iucrs do registros atuais de ambos arquivos forem iguais, então o critério de desempate será pelo segundo campo(description) 
          //se a descrição do crime do registro do primeiro arquivo for menor que a descrição do registro do segundo arquivo
          if (strcmp(reg1->description, reg2->description) < 0) {
            //escrever o registro do primeiro arquivo no arquivo de saída
            escrever_registro(arquivoQueSeraGerado, reg1);
            //ler o próximo registro do primeiro arquivo
            reg1 = ler_registro(arquivoEntrada);
          }
          else {
            //se a descrição do crime do registro do primeiro arquivo for maior que a descrição do registro do segundo arquivo
            if (strcmp(reg1->description, reg2->description) > 0) {
              //escrever o registro do segundo arquivo no arquivo de saída
              escrever_registro(arquivoQueSeraGerado, reg2);
              //ler o próximo registro do segundo arquivo
              reg2 = ler_registro(arquivoEntrada2);
            }
            else { //se as descriptions do registros atuais de ambos arquivos forem iguais, então o critério de desempate será pelo terceiro campo(name) 
              //se o nome do registro do primeiro arquivo for menor que o nome do registro do segundo arquivo
              if (strcmp(reg1->name, reg2->name) < 0) {
                //escrever o registro do primeiro arquivo no arquivo de saída
                escrever_registro(arquivoQueSeraGerado, reg1);
                //ler o próximo registro do primeiro arquivo
                reg1 = ler_registro(arquivoEntrada);
              }
              else {
                //se o nome do registro do primeiro arquivo for maior que o nome do registro do segundo arquivo
                if (strcmp(reg1->name, reg2->name) > 0) {
                  //escrever o registro do segundo arquivo no arquivo de saída
                  escrever_registro(arquivoQueSeraGerado, reg2);
                  //ler o próximo registro do segundo arquivo
                  reg2 = ler_registro(arquivoEntrada2);
                }
                else { //se o nomes dos registros atuais de ambos arquivos forem iguais, então o critério de desempate será pelo quarto campo(date) 
                  //se a data do registro do primeiro arquivo for menor que a data do registro do segundo arquivo
                  if (strcmp_datas(reg1->date, reg2->date) < 0) {
                    //escrever o registro do primeiro arquivo no arquivo de saída
                    escrever_registro(arquivoQueSeraGerado, reg1);
                    //ler o próximo registro do primeiro arquivo
                    reg1 = ler_registro(arquivoEntrada);
                  }
                  else {
                    //se a data do registro do primeiro arquivo for maior que o data do registro do segundo arquivo
                    if (strcmp_datas(reg1->date, reg2->date) > 0) {
                      //escrever o registro do segundo arquivo no arquivo de saída
                      escrever_registro(arquivoQueSeraGerado, reg2);
                      //ler o próximo registro do segundo arquivo
                      reg2 = ler_registro(arquivoEntrada2);
                    }
                    else {
                      //se as datas dos registros atuais de ambos arquivos forem iguais 
                      escrever_registro(arquivoQueSeraGerado, reg1);
                      //ler o próximo registro de ambos arquivos
                      reg1 = ler_registro(arquivoEntrada);
                      reg2 = ler_registro(arquivoEntrada2);
                    }
                  }

                }
              }

            }
          }

        }
      }
    }
    //se o segundo arquivo chegou ao fim, mas o primeiro não 
    if (reg2 == NULL) {
      //escrever todos os registros do  arquivo até que todos os registros faltantes desse arquivo tenham sido lidos 
      while (reg1 != NULL) {
        escrever_registro(arquivoQueSeraGerado, reg1);
        reg1 = ler_registro(arquivoEntrada);
      }
    }
    //se o primeiro arquivo chegou ao fim, mas o segundo não
    if (reg1 == NULL) {
      //escrever todos os registros do segundo arquivo até que todos os registros faltantes desse arquivo tenham sido lidos 
      while (reg2 != NULL) {
        escrever_registro(arquivoQueSeraGerado, reg2);
        reg2 = ler_registro(arquivoEntrada2);
      }
    }
  }

  //fechar os 3 arquivos
  fclose(arquivoEntrada);
  fclose(arquivoEntrada2);
  fclose(arquivoQueSeraGerado);

  return 1;
}


///////////////////////////////////////////////////////MÉTODOS PARA REALIZAR O MATCHING//////////////////////////////////////////////////////////
/*O método matching recebe três strings: duas com os nomes dos arquivos de entrada ordenados, e outra com o nome do arquivo de saída que terá a operação de matching realizada com os dois 
arquivos de entrada. O método devolve 1 caso o arquivo de matching tenha sido gerado, 0 caso contrário*/
int matching(char *nomeArquivoEntrada, char *nomeArquivoEntrada2, char *nomeArquivoQueSeraGerado) {
  FILE* arquivoEntrada;
  FILE* arquivoEntrada2;
  FILE* arquivoQueSeraGerado;

  //os dois primeiros arquivos são abertos para leitura
  arquivoEntrada = fopen (nomeArquivoEntrada, "r");
  arquivoEntrada2 = fopen (nomeArquivoEntrada2, "r");
  //o arquivo de saída é aberto para escrita 
  arquivoQueSeraGerado = fopen(nomeArquivoQueSeraGerado, "w");
  registro *reg1;
  registro *reg2;

  if(!arquivoEntrada || !arquivoEntrada2 || !arquivoQueSeraGerado) {
    perror("Falha no processamento.\n");
    return 0;
  }
  else {
    //ler o primeiro registro de ambos arquivos
    reg1 = ler_registro(arquivoEntrada);
    reg2 = ler_registro(arquivoEntrada2);
    //enquanto existam mais registros a serem lidos no primeiro arquivo e no segundo arquivo
    while (reg1 != NULL && reg2 != NULL) {
      //se o iucr do registro do primeiro arquivo for menor que o iucr do segundo arquivo
      if (reg1->iucr < reg2->iucr) {
        //ler o proximo registro do primeiro arquivo
        reg1 = ler_registro(arquivoEntrada);
      }
      else {
        //se o iucr do registro do primeiro arquivo for maior que o iucr do segundo arquivo
        if (reg1->iucr > reg2->iucr) {
          //ler o proximo registro do segundo arquivo
          reg2 = ler_registro(arquivoEntrada2);
        }
        else { //caso os iucrs dos registros atuais de ambos arquivos forem iguais
          int counter;
          unsigned int id = reg1->iucr; //id é o iucr do registro atual, ou seja o que está repetido
          //uma lista de registros empatados pelo iucr(campo 1) deve ser criada
          registro ** listaIguais=NULL;
          listaIguais = (registro **) realloc(listaIguais,sizeof(registro)*(2)); 
          //as duas primeiras posições de listaIguais terá os registros atuais de ambos arquivos
          listaIguais[0] = reg1;
          listaIguais[1] = reg2;
          counter = 2; //representará o tamanho da listaIguais
          //enquanto existam ainda registros a serem lidos no primeiro arquivo
          while ((reg1 = ler_registro(arquivoEntrada)) != NULL){
            //se o iucr do registro atual do primeiro arquivo for igual a id
            if(reg1->iucr == id){
              //incrementar o counter
              counter++;
              //adicionar o registro atual a listaIguais
              listaIguais = (registro **) realloc(listaIguais,sizeof(registro)*(counter));
              listaIguais [counter-1] = reg1;
            }
            else {
              //se o iucr do registro atual do primeiro arquivo for diferente a id parar o laço 
              break;
            }
          }
          //enquanto existam ainda registros a serem lidos no segundo arquivo
          while ((reg2 = ler_registro(arquivoEntrada2)) != NULL) {
            //se o iucr do registro atual do segundo arquivo for igual a id
            if(reg2->iucr == id){
              //incrementar o counter
              counter++;
              //adicionar o registro atual a listaIguais
              listaIguais = (registro **) realloc(listaIguais,sizeof(registro)*(counter));
              listaIguais [counter-1] = reg2;
            }
            else {
              //se o iucr do registro atual do segundo arquivo for diferente a id parar o laço 
              break;
            }
          }
          //ordenar o vetor de registros listaIguais usando o quicksort implementado na funcioanalidade 3
          quicksort (listaIguais, 0, counter-1);
          int k;
          //escrever os registros de listaIguais ordenados no arquivo de saída
          for (k = 0;k < counter; k++){
            escrever_registro(arquivoQueSeraGerado, listaIguais[k]);
          }
        }
      }
    }
  }

  //fechar os arquivos
  fclose(arquivoEntrada);
  fclose(arquivoEntrada2);
  fclose(arquivoQueSeraGerado);

  return 1;
}


///////////////////////////////////////////////////////MÉTODOS PARA REALIZAR O MULTIWAY MERGING//////////////////////////////////////////////////
/*O método multiway_merging recebe uma vetor de strings com os nomes dos arquivos de entrada, um inteiro que representa o número de arquivos de entrada, uma
 string com o nome do arquivo de saída. O método retorna 1 caso o arquivo de saída referente a operação de multiway merging (união) de muitas listas seja gerado */
int multiway_merging(char listaArquivosEntrada[100][100], int numeroArquivosEntrada, char *nomeArquivoQueSeraGerado) {
  int i;
  FILE* arquivoQueSeraGerado;
  FILE* listFilesEntrada[100];
  //Abrir todos os arquivos de entrada para leitura 
  for (i = 0; i < numeroArquivosEntrada; i++){
        listFilesEntrada[i] = fopen (listaArquivosEntrada[i], "r");    
  }
  //abrir o arquivo de saída para escrita
  arquivoQueSeraGerado = fopen(nomeArquivoQueSeraGerado, "w");
  
  registro *listRegistros[numeroArquivosEntrada]; //ponteiro para um vetor de registros
  for (i = 0; i < numeroArquivosEntrada; i++){
        if(!listFilesEntrada[i]) {
           perror("Falha no processamento.\n");
           return 0;
        }   
  }
  if(!arquivoQueSeraGerado) {
    perror("Falha no processamento.\n");
    return 0;
  }
  int k;
  //Ler o primeiro registro de cada arquivo coloca-lo na respectiva posição em listRegistros
  for (k = 0; k < numeroArquivosEntrada; k++){
    listRegistros[k] = ler_registro(listFilesEntrada[k]);
  }
  int fim = 0;
  int count = numeroArquivosEntrada;

  // Encontrar o menor registro (ou a lista dos menores registros iguais) enquanto existam 2 ou mais arquivos que não chegaram ao fim
  while (count >= 2) {
    count = 0;
    //count vai ter quantos arquivos de entrada ainda não chegaram ao fim
    for (k = 0; k < numeroArquivosEntrada; k++){
      if(listRegistros[k] != NULL) {
        count++;
      }
    }
    //se a quantidade de arquivos de entrada que não chegaram ao fim é maior igual que 2
    if(count >= 2){
      registro ** listaIguais = NULL;
      int *listaPosIguais = NULL;
      int numIguais=0;
      //Encontra a lista de registros menores que são iguais,
      listaIguais = encontrarMenor(listRegistros, numeroArquivosEntrada, &listaPosIguais, &numIguais);
      //ordenar listaIguais usando o quicksort da funcionalidade 3
      quicksort (listaIguais, 0, numIguais-1);
      int k;
      //escrever os registros que são iguais após ordenados no arquivo de saída a fazer a leitura dos próximos registros dos arquivos ao quais pertencem os registros de listaIguais
      for (k = 0; k < numIguais; k++) {
        escrever_registro(arquivoQueSeraGerado, listRegistros[listaPosIguais[k]]);
        listRegistros[listaPosIguais[0]]  = ler_registro(listFilesEntrada[listaPosIguais[k]]);
      }
    }
  }

  //Escrever os registros do final de cada arquivo que não chegou ao fim e ler o próximo registro do arquivo de entrada que acabou de escrever um registro 
  for (k = 0; k < numeroArquivosEntrada; k++){
    if(listRegistros[k]!=NULL) {
      while (listRegistros[k] != NULL) {
        escrever_registro(arquivoQueSeraGerado, listRegistros[k]);
        listRegistros[k] = ler_registro(listFilesEntrada[k]);
      } 
    }
  } 
  //Fechar os arquivos
  for (i = 0; i < numeroArquivosEntrada; i++) {
   fclose(listFilesEntrada[i]);
  }
  fclose(arquivoQueSeraGerado);
  return 1;
}


/*O método encontraMenor recebe uma vetor com os registros atuais de cada um dos arquivos de entrada, um inteiro com o número de arquivos de entrada, devolve um vetor de inteiros com
as posições dos arquivos de entrada que possuem o mesmo iucr. Além disso devolve a lista de registros com os registros iguais a esse menor segundo o campo 1 */
registro **encontrarMenor(registro* listRegistros[], int numeroArquivosEntrada, int **listaPosIguais, int *counter) {
  registro ** listaIguais=NULL;
  int i;
  unsigned int menor_id = 100000;
 
  //Encontra o menor menor_id
  for (i = 0; i < numeroArquivosEntrada;i++){
    if (listRegistros[i] == NULL){
        continue;
    }
    if(listRegistros[i]->iucr < menor_id) {
     menor_id = listRegistros[i]->iucr;
    }
 }
 //Encontra todos os registros que sao iguais a menor_id
 *counter=0;
 for (i = 0; i< numeroArquivosEntrada;i++){
  if (listRegistros[i] == NULL){
      continue;
  }
  if(listRegistros[i]->iucr == menor_id){
    (*counter)++;
    //criar um vetor com os registros que apresentam o mesmo id
    listaIguais = (registro **) realloc(listaIguais,sizeof(registro)*(*counter));
    listaIguais [(*counter)-1] = listRegistros[i];
    //é criado o vetor listPosIguais para saber quais arquivos de entrada tem o mesmo menor_id 
    (*listaPosIguais) = (int *) realloc(*listaPosIguais,sizeof(int)*(*counter));
    (*listaPosIguais) [(*counter)-1] = i;                             
  }
 }
 return listaIguais;
}

///////////////////////////////////////MÉTODOS PARA REALIZAR A ORDENAÇÃO EXTERNA///////////////////////////////

///////////////// //MÉTODOS PARA REALIZAR A ORDENAÇÃO EXTERNA///////////////////////////////

//realiza a ordenação externa do arquivo de entrada e escreve o resultado no arquivo de saída 
int ordenar_externa_arquivo(char *nomeArquivoEntrada, char *nomeArquivoQueSeraGerado) {
  int contNumFiles;
  char *nomeSubArquivoOrdenado=NULL;
  char *nomeSubArquivo = NULL;
  char *listNomeArquivosSaida[12];
  int i; 

  // cria os subArquivos
  int criou=criarSubArquivos (nomeArquivoEntrada, &contNumFiles);

  //ordena os subArquivos
  for (i = 0; i < contNumFiles; i++){
    nomeSubArquivo=criarNomeSub(nomeArquivoEntrada,i);
    nomeSubArquivoOrdenado=criarNomeSubOrdenado(nomeArquivoEntrada,i);
    int ordenou = ordenar_arquivo(nomeSubArquivo, nomeSubArquivoOrdenado);
    printf("ordenou %s \n",nomeSubArquivoOrdenado);
    listNomeArquivosSaida[i]= nomeSubArquivoOrdenado;
  }

  int newContNumFiles=contNumFiles;
  char *nomeIntermediario; 
  int pos; 
  char camada[5] = "";

  //Faz o merge de 2 em 2 até ter apenas 2 ou 1 arquivo
  while (newContNumFiles/2.0 >= 1.5){
     pos=0; 
     //fazer o merge de 2 em 2 e colocar o nome do arquivo intermediario criado na lista de nomes de arquivos de saida
     for (i = 0; i < newContNumFiles - 1; i = i + 2){
       nomeIntermediario=criarNomeSubIntermediario(i,pos,camada);
       merging(listNomeArquivosSaida[i], listNomeArquivosSaida[i+1], nomeIntermediario);
       printf("merging %s %s em %s \n",listNomeArquivosSaida[i], listNomeArquivosSaida[i+1],nomeIntermediario);
       listNomeArquivosSaida[pos]= nomeIntermediario;
       pos++;
     }//endfor
     // se temos apenas um arquivo sobrando fazer a copia do nome para a lista de arquivos de saida para ser processado na próxima camada 
     if(newContNumFiles%2 == 1){ 
        printf("sobrando 1 arquivo\n");
        listNomeArquivosSaida[pos]= listNomeArquivosSaida[i];
        pos++;  
        newContNumFiles++;
     } 
     strcat(camada,"_");
     newContNumFiles=newContNumFiles/2;
   }//endwhile

  //fazer o merge se sobraram 2 arquivos 
  if (newContNumFiles == 2){
    merging(listNomeArquivosSaida[0], listNomeArquivosSaida[1], nomeArquivoQueSeraGerado);
    printf("merging %s %s em %s \n",listNomeArquivosSaida[0], listNomeArquivosSaida[1],nomeArquivoQueSeraGerado);
  }
  //escrever diretamente no arquivo de saida se sobrou apenas 1 arquivo (newContNumFiles=1)
  else{ 
     int counter;
     registro** listaDeRegistros = ler_arquivo(listNomeArquivosSaida[0], &counter);
     int escreveu=escrever_arquivo(nomeArquivoQueSeraGerado, counter, listaDeRegistros);     
   }
   return 1;
}

//Cria subArquivos de tamanho máximo NUMEROREGISTROSUBARQUIVO a partir do Arquivo de Entrada
int criarSubArquivos (char* nomeArquivoEntrada, int *contNumFiles){
  
  FILE** listFilesSaida= (FILE **) malloc (sizeof(FILE)*12);
  char *nomeArquivoSaida;
  int cont=0; //contador de registros
  *contNumFiles=0; //contados do número de arquivos 

 //abrir o arquivo de entrada
  FILE *arquivoEntrada = fopen (nomeArquivoEntrada, "r");
  if( !arquivoEntrada) {
    return 0;
  }

  //ler o primeiro registro e abrir o primeiro sub arquivo
  registro *reg1 = ler_registro(arquivoEntrada);
  nomeArquivoSaida=criarNomeSub(nomeArquivoEntrada,*contNumFiles);
  listFilesSaida[*contNumFiles] = fopen( nomeArquivoSaida , "w");

 // enquanto não seja o final do arquivo de entrada
  while (reg1 != NULL) {
    // se chegou em uma quantidade de registros múltiplo de NUMEROREGISTROSUBARQUIVO fechar o sub arquivo e abrir um novo sub arquivo
    if (cont%NUMEROREGISTROSUBARQUIVO==0){
      if (cont!=0){
        fclose(listFilesSaida[*contNumFiles]);
        printf("Criando %s \n",nomeArquivoSaida);     
        (*contNumFiles)++; 
        nomeArquivoSaida=criarNomeSub(nomeArquivoEntrada,*contNumFiles);
        //strcpy(listNomeArquivosSaida[*contNumFiles], nomeArquivoSaida);
        listFilesSaida[*contNumFiles] = fopen( nomeArquivoSaida , "w");
      }  
    }
    // escrever o registro no sub arquivo e ler o próximo registro do arquivo de entrada
    escrever_registro(listFilesSaida[*contNumFiles], reg1);
    //printf("Escrevendo em %s reg %d\n",nomeArquivoSaida, cont);   
      reg1 = ler_registro(arquivoEntrada);
      cont++;
  }
   
   // fechar o último sub arquivo criado 
     fclose(listFilesSaida[*contNumFiles]);
     (*contNumFiles)++;
     printf("Criando %s\n",nomeArquivoSaida);     
     fclose(arquivoEntrada);
     return 1;
}

//cria uma string para ser usada como o nome do sub arquivo não ordenado
// o nome é sub_numNomeArquivoEntrada
char *criarNomeSub(char *nomeArquivoEntrada, int contNumFiles){
  char *nomeArquivoSaida=(char *) malloc (100);
  strcpy(nomeArquivoSaida,"");
  strcat(nomeArquivoSaida,"sub_");
  char numFiles[12];
  snprintf (numFiles, sizeof(numFiles), "%d",contNumFiles); 
  strcat(nomeArquivoSaida,numFiles);   
  strcat(nomeArquivoSaida,nomeArquivoEntrada);
  return nomeArquivoSaida;
}

//cria uma string para ser usada como o nome do sub arquivo ordenado
//o nome é sub_numOrdNomeArquivoEntrada
char *criarNomeSubOrdenado(char *nomeArquivoEntrada, int contNumFiles){
  char *nomeArquivoSaida=(char *) malloc (100);
  strcpy(nomeArquivoSaida,"");
  strcat(nomeArquivoSaida,"sub_");
  char numFiles[12];
  snprintf (numFiles, sizeof(numFiles), "%d",contNumFiles); 
  strcat(nomeArquivoSaida,numFiles);
  strcat(nomeArquivoSaida,"Ord");
  strcat(nomeArquivoSaida,nomeArquivoEntrada);
  return nomeArquivoSaida;
}
//cria uma string para ser usada como o nome dos arquivos intermediarios
//o nome é sub_numPosCamada
char *criarNomeSubIntermediario(int contNumFiles,int pos, char * camada){
  char *nomeArquivoSaida=(char *) malloc (100);
  strcpy(nomeArquivoSaida,"");
  strcat(nomeArquivoSaida,"sub_");
  char numFiles[12];
  snprintf (numFiles, sizeof(numFiles), "%d",contNumFiles); 
  strcat(nomeArquivoSaida,numFiles);
  char posFiles[12];
  snprintf (posFiles, sizeof(posFiles), "%d",pos); 
  strcat(nomeArquivoSaida,posFiles);
  strcat(nomeArquivoSaida,camada);
  return nomeArquivoSaida;
}


///////////////// //MÉTODOS AUXILIARES///////////////////////////////
//O método ler_registro recebe um ponteiro para um arquivo já aberto e devolve o proximo registro lido desse arquivo
registro* ler_registro(FILE *arquivo) {
  int i;
  //alocando espaco para o primeiro registro
  registro *reg = (registro *) malloc (sizeof (registro));
  //leitura do iucr que esta no registro reg
  if(!fread (&reg->iucr, sizeof(unsigned int), 1, arquivo)) {
    return NULL;
  }
  //leitura da description que esta no registro reg
  for (i = 0; i < 30; i++) {
    fread(&reg->description[i], 1, 1, arquivo);
  }
  reg->description[30] = '\0';
  //leitura do name que esta no registro reg
  for (i = 0; i < 20; i++) {
    fread(&reg->name[i], 1, 1, arquivo);
  }
  reg->name[20] = '\0';
  //leitura da date que esta no registro reg
  for (i = 0; i < 10; i++) {
    fread(&reg->date[i], 1, 1, arquivo);
  }
  reg->date[10] = '\0';
  return reg;
}

//O método escreve_registro recebe um ponteiro para um arquivo de saída já aberto de saida e um registro reg, devolve 1 caso o registro reg tenha sido escrito no arquivo
int escrever_registro (FILE *arquivo, registro *reg) {
  fwrite(&reg->iucr, sizeof(unsigned int), 1, arquivo);
  fwrite(&reg->description, 30, 1, arquivo);
  fwrite(&reg->name, 20, 1, arquivo);
  fwrite(&reg->date, 10, 1, arquivo); 
}


/*O método escrever_arquivo recebe uma string com o nome de um arquivo de saída, um inteiro counter que representa quantos registros serão escritos e um vetor com os registros que devem 
ser escritos. Devolve 1 caso os registros tenham sido escritos no arquivo, 0 caso contrário */

int escrever_arquivo(char *nomeArquivo, int counter, registro** listaDeRegistros){
  int i;
  FILE *arquivo = NULL;

  //abre o arquivo para escrita
  arquivo = fopen(nomeArquivo, "w");
  
  if(!arquivo) {
    perror("Falha no processamento.\n");
    return 0;
  }

  else {
    //escreve os campos no arquivo de saída
    for (i = 0; i < counter; i++) {
      fwrite(&listaDeRegistros[i]->iucr, sizeof(unsigned int), 1, arquivo);
      fwrite(&listaDeRegistros[i]->description, 30, 1, arquivo);
      fwrite(&listaDeRegistros[i]->name, 20, 1, arquivo);
      fwrite(&listaDeRegistros[i]->date, 10, 1, arquivo); 
    }
    //fecha o arquivo
    fclose(arquivo);
  }
  return 1;
}


////////////////////////////////////MAIN////////////////////////////////////////////////////////
int main(int argc, char const *argv[]) {

  srand(time(NULL));

  int i;
  char  nomeArquivo[100];
  char nomeArquivoEntrada[100];
  char nomeArquivoEntrada2[100];
  char nomeArquivoQueSeraGerado[100];
  char listaArquivosEntrada [100][100];
  int opt = atoi(argv[1]);
  int N;
  int k;
  
  switch(opt) {

    //FUNCIONALIDADE 1: GERAR ARQUIVO
    case 1: 
    if(argc!=4) {
      printf("Faltam argumentos: a opção 1 tem 2 argumentos\n");
    }
    else {
      strncpy(nomeArquivo,argv[2],100);
      N = atoi(argv[3]);
      int gerou = gerar_arquivo(nomeArquivo,N);
      if (gerou) printf("Arquivo gerado.\n");
    }
    break;

   //FUNCIONALIDADE 2:MOSTRAR O ARQUIVO
   case 2: 
    if(argc!=3) {
      printf("Faltam argumentos: a opção 2 tem 1 argumento\n");
    }
    else
    { 
      strncpy(nomeArquivo,argv[2],100);
      int counter;
      registro** listaDeRegistros = ler_arquivo(nomeArquivo, &counter);
      for (i = 0; i < counter; i++) {
        char buffer[6];
        snprintf(buffer, sizeof(buffer), "%d", listaDeRegistros[i]->iucr);
        printf("%s ", buffer);
        printf("%s ", listaDeRegistros[i]->description);
        printf("%s ", listaDeRegistros[i]->name);
        printf("%s\n", listaDeRegistros[i]->date);
      }
      if (counter == 0)
        printf("Arquivo vazio.\n");
    }
    break;

    //FUNCIONALIDADE 3: ORDENAÇÃO INTERNA
     case 3: 
      if(argc!=4) {
       printf("Faltam argumentos: a opção 3 tem 2 argumentos\n");
      }
    else
    {
      strncpy(nomeArquivoEntrada,argv[2],100);
      strncpy(nomeArquivoQueSeraGerado,argv[3],100);
      int ordenou = ordenar_arquivo(nomeArquivoEntrada, nomeArquivoQueSeraGerado);
      if (ordenou) printf("Arquivo gerado.\n");
    }
    break;

    //FUNCIONALIDADE 4: MERGING
    case 4:
    if(argc!=5) {
      printf("Faltam argumentos: a opção 4 tem 3 argumentos\n");
    }
    else
    {
      strncpy(nomeArquivoEntrada,argv[2],100);
      strncpy(nomeArquivoEntrada2,argv[3],100);
      strncpy(nomeArquivoQueSeraGerado,argv[4],100);

      int juntou = merging(nomeArquivoEntrada, nomeArquivoEntrada2,nomeArquivoQueSeraGerado);
      if (juntou) printf("Arquivo gerado.\n");
    }

    break;

    //FUNCIONALIDADE 5: MATCHING
    case 5: 
    if(argc!=5) {
      printf("Faltam argumentos: a opção 4 tem 3 argumentos\n");
    }
    else
    {
      strncpy(nomeArquivoEntrada,argv[2],100);
      strncpy(nomeArquivoEntrada2,argv[3],100);
      strncpy(nomeArquivoQueSeraGerado,argv[4],100);
      int interceptou = matching(nomeArquivoEntrada, nomeArquivoEntrada2,nomeArquivoQueSeraGerado);
      if (interceptou) printf("Arquivo gerado.\n");
    }
    break;

    //FUNCIOANLIDADE 6: MULTIWAY MERGING
    case 6: 
    for (k = 0; k < argc-3; k++) {
       strncpy(listaArquivosEntrada[k], argv[k+2],100);
    }
    strncpy(nomeArquivoQueSeraGerado,argv[argc-1],100);
    int multi_juntou = multiway_merging(listaArquivosEntrada, argc-3,nomeArquivoQueSeraGerado);
    if (multi_juntou) printf("Arquivo gerado.\n");
    break;

    //FUNCIONALIDADE 7: ORDENAÇÃO EXTERNA
    case 7:
    if(argc!=4) {
      printf("Faltam argumentos: a opção 7 tem 2 argumentos\n");
    }
    else
    {
      strncpy(nomeArquivoEntrada,argv[2],100);
      strncpy(nomeArquivoQueSeraGerado,argv[3],100);
      int ordenou = ordenar_externa_arquivo(nomeArquivoEntrada, nomeArquivoQueSeraGerado);
      if (ordenou) printf("Arquivo gerado.\n");
    }  
    break;

  }

  return 0;

}
