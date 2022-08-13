 // https://github.com/daltonbr/MIPS
// https://github.com/toorajtaraz/mips-c
// https://github.com/JB-toriel/MIPS_Emulator
// https://www.youtube.com/watch?v=OjaAToVkoTw

#include "mips.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  int opcao, tamanho, i = 0;
  int end;
  int iniMemDados, fimMem;          // inicio e fim da memoria de dados
  long long int dado;               // dado a ser posto na MP
  int qtdeInst = 0;                 // quantidade de linhas do programa
  int inicio, fim;                  // usadas para ler intervalo da MP
  long long int *MP;                // 0 - 511 instruções e 512 - 1023 dados
  long long int registradores[32];  // registradores do mips
  char arquivo_leitura[50];         // guarda o nome do arquivo de leitura
  char inputLine[199];              // guarda as linhas do arquivo que sera lido
  FILE *inputFile;                  // arquivos de entrada de dados
  int pc = 0;                       // program counter
  int totalc = 0;                   // guarda o total de ciclos da execucao do programa
  long long int *CacheInst;         // cache de instrucoes
  long long int *CacheDados;        // cache de dados
  int *endGuardadoCacheinst;
  int *endGuardadoCachedado;
  int conjuntoscacheInst;
  int conjuntoscachedados;
  int associatividadeInst;
  int associatividadeDados;
  int qtdePalavrasInst = 0;
  int qtdePalavrasDados = 0;
  
  

  for(i = 0; i < 32; i++)
    {
      registradores[i] = 0;
    }
  
  printf("******************Simulador MIPS****************\n\n\n");
  printf("Digite o tamanho da memoria principal do sistema: ");
  scanf("%i", &tamanho);

  MP = (long long int *)malloc(tamanho * sizeof(long long int));

  for(i = 0; i < tamanho; i++)
    {
      MP[i] = 0;
    }

  iniMemDados = tamanho / 2;
  fimMem = tamanho - 1;
  printf("\ntamanho: %i \n", tamanho);
  printf("instrucoes: %i - %i \n", tamanho - tamanho, (tamanho / 2) - 1);
  printf("dados: %i - %i \n", iniMemDados, fimMem);
  printf("OBS: todos os valores de memoria sao inicializados com o valor = 0\n\n\n");
  do {
    printf("*****************OPCOES*******************\n");
    printf("1 - Ler Aqruivo para memoria\n");
    printf("2 - Adicionar dado na memoria principal\n");
    printf("3 - Executar programa direto na MP\n");
    printf("4 - Executar programa com caches\n");
    printf("5 - Imprimir intervalo da MP\n");
    printf("6 - Mostrar registradores\n");
    printf("7 - Sair\n");
    printf("Digite sua escolha: ");
    scanf("%i", &opcao);
    fflush(stdin);

    switch (opcao) {
    case 1:
      printf("digite o nome do arquivo que deseja executar: ");
      scanf("%s", &arquivo_leitura);
      fflush(stdin);

      inputFile = fopen(arquivo_leitura, "r");

      if (!inputFile) 
      {
        printf("\nerro ao abrir arquivo de Leitura!\n");
      } 
      else 
      {
        printf("\nAberto com sucesso\n");
        while (!feof(inputFile))
        {
          if (fgets(inputLine, 100, inputFile))
          {
            //obs: ele le o \n!!!!!!
            if(DecodeInstAddMP(inputLine, qtdeInst, MP) == 1)
            {
              printf(" instrucao adicionada com sucesso a mp!\n");
              qtdeInst += 1;
            }
          }
        }
      }
      fclose(inputFile);
      break;
      
    case 2:
      printf("\nInicio da memoria de dados: %i\n", iniMemDados);
      printf("Fim da memoria de dados: %i\n", fimMem);
      printf("digite o endereco do dado que sera adicionado: ");
      scanf("%i", &end);
      fflush(stdin);
      printf("digite o dado que deseja adicionar: ");
      scanf("%lli", &dado);
      fflush(stdin);

      MP[end] = dado;

      printf("\nDado adicionado com sucesso!!\n");
      break;

    case 3:
      totalc = 0;
      for(pc = 0; pc < qtdeInst; pc++)
        { 
          pc = ExecutarInstrucao( MP, pc, registradores, totalc);
          totalc += ChecarInst(MP, pc);
          totalc += 1;
        }
      printf("\no programa foi executado em %i ciclos\n", totalc);
      break;

    case 4:
      printf("digite o numero de conjuntos da cache de instrucoes: ");
      scanf("%i", &conjuntoscacheInst);
      fflush(stdin);
      CacheInst = (long long int *)malloc(conjuntoscacheInst * sizeof(long long int));
      endGuardadoCacheinst = (int *)malloc(conjuntoscacheInst * sizeof(int));

      printf("digite o numero de conjuntos da cache de dados: ");
      scanf("%i", &conjuntoscachedados);
      fflush(stdin);
      CacheDados = (long long int *)malloc(conjuntoscachedados * sizeof(long long int));
      endGuardadoCachedado = (int *)malloc(conjuntoscacheInst * sizeof(int));

      for(i = 0; i < conjuntoscacheInst; i++)
        {
          CacheInst[i] = MP[i];
          endGuardadoCacheinst[i] = i;
        }

       for(i = 512; i < (conjuntoscachedados + 512); i++)
        {
          CacheDados[i] = MP[i];
          endGuardadoCachedado[i] = i;
        }

      for(i = 512; i < (conjuntoscachedados + 512); i++)
        {
          printf("%lli\n", CacheDados[i]);
        }

      totalc = 0;
      for(pc = 0; pc < qtdeInst; pc++)
        { 
          pc = ExecutarInstrucao( MP, pc, registradores, totalc);
          totalc += ChecarInst(MP, pc);
          totalc += 1;
        }
      printf("\n");
      break;
      
    case 5:
      printf("digite o endereco do inicio da leitura: ");
      scanf("%i", &inicio);
      fflush(stdin);
      printf("digite o endereco do fim da leitura: ");
      scanf("%i", &fim);
      fflush(stdin);
      printf("\n");
      for(i = inicio; i <= fim; i++)
        {
          printf("MP[%i]: %lli\n", i, MP[i]);
        }
      break;

    case 6:
      printf("\n\n$zero - %lli", registradores[0]);
      printf("\n$at - %lli", registradores[1]);
      printf("\n$vo - %lli", registradores[2]);
      printf("\n$v1 - %lli", registradores[3]);
      printf("\n$a0 - %lli", registradores[4]);
      printf("\n$a1 - %lli", registradores[5]);
      printf("\n$a2 - %lli", registradores[6]);
      printf("\n$a3 - %lli", registradores[7]);
      printf("\n$t0 - %lli", registradores[8]);
      printf("\n$t1 - %lli", registradores[9]);
      printf("\n$t2 - %lli", registradores[10]);
      printf("\n$t3 - %lli", registradores[11]);
      printf("\n$t4 - %lli", registradores[12]);
      printf("\n$t6 - %lli", registradores[14]);
      printf("\n$t7 - %lli", registradores[15]);
      printf("\n$s0 - %lli", registradores[16]);
      printf("\n$s1 - %lli", registradores[17]);
      printf("\n$s2 - %lli", registradores[18]);
      printf("\n$s3 - %lli", registradores[19]);
      printf("\n$s4 - %lli", registradores[20]);
      printf("\n$s5 - %lli", registradores[21]);
      printf("\n$s6 - %lli", registradores[22]);
      printf("\n$s7 - %lli", registradores[23]);
      printf("\n$t8 - %lli", registradores[24]);
      printf("\n$t9 - %lli", registradores[25]);
      printf("\n$k0 - %lli", registradores[26]);
      printf("\n$k1 - %lli", registradores[27]);
      printf("\n$gp - %lli", registradores[28]);
      printf("\n$sp - %lli", registradores[29]);
      printf("\n$fp - %lli", registradores[30]);
      printf("\n$ra - %lli", registradores[31]);
      printf("\n\n");
      break;

    case 7:
      printf("\nPrograma encerrado!!!\n");
      break;
    default:
      printf("\nOpcao invalida!!!\n");
      break;
    }

  } while (opcao != 7);
  return 0;
}
