#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mips.h"

void AddToMP(char *instrucaoBin, int endereco, long long int *MP)
{
  int i = 0;
  int j = 0;
  for(i = 31; i >= 0; i--)
    {
      MP[endereco] += (instrucaoBin[j] - '0') * pow( 2, i);
      j += 1;
    }
  printf(" MP[%i] - %lli\n", endereco, MP[endereco]);
}

void AddDataToMP(int dado, int endereco, long long int *MP)
{
  MP[endereco] = dado;
  printf(" MP[%i] - %lli\n", endereco, MP[endereco]);
}

char *getNameAssembly(char *fullLine)
{
	int i = 0;
	char *nome;
	nome = (char *)malloc(6); //tamanho maximo do nome das funcoes
	
	do
	{
		nome[i] = fullLine[i];
		i++;
	} while ( fullLine[i] != ' ' );    
	nome[i] = '\0';

	return (char *)nome;
}

char *DecodeReg(char *reg)
{
  if(!(strcmp(reg,"$zero")))
  {
    return "00000";
  }
  else if(!(strcmp(reg,"$t0")))
  {
    return "01000";
  }
  else if(!(strcmp(reg,"$t1")))
  {
    return "01001";
  }
  else if(!(strcmp(reg,"$t2")))
  {
    return "01010";
  }
  else if(!(strcmp(reg,"$t3")))
  {
    return "01011";
  }
  else if(!(strcmp(reg,"$t4")))
  {
    return "01100";
  }
  else if(!(strcmp(reg,"$t5")))
  {
    return "01101";
  }
  else if(!(strcmp(reg,"$t6")))
  {
    return "01110";
  }
  else if(!(strcmp(reg,"$t7")))
  {
    return "01111";
  }
  else if(!(strcmp(reg,"$s0")))
  {
    return "10000";
  }
  else if(!(strcmp(reg,"$s1")))
  {
    return "10001";
  }
  else if(!(strcmp(reg,"$s2")))
  {
    return "10010";
  }
  else if(!(strcmp(reg,"$s3")))
  {
    return "10011";
  }
  else if(!(strcmp(reg,"$s4")))
  {
    return "10100";
  }
  else if(!(strcmp(reg,"$s5")))
  {
    return "10101";
  }
  else if(!(strcmp(reg,"$s6")))
  {
    return "10110";
  }
  else if(!(strcmp(reg,"$s7")))
  {
    return "10111";
  }
  else if(!(strcmp(reg,"$t8")))
  {
    return "11000";
  }
  else if(!(strcmp(reg,"$t9")))
  {
    return "11001";
  }
  return "1";
}

//monta o valor do shift amm. das inst. do tipo r em binario
char *ShamtToBin(int a)
{
  char *shamt;
  shamt = (char *)malloc(6);
  int end = 4;

  strcpy(shamt, "00000");
  
  if(a == 0)
  {
    return  "00000";
  }
  else
  {
    do
    {
      shamt[end] = (a % 2) + '0';
      a = a / 2;
      end--;
    }while(a > 0);
    return (char *)shamt;
  }
  return "1";
}

//monta o valor imediato das inst. do tipo i em binario
char *ImmTIToBin(int a)
{
  char *Imm;
  Imm = (char *)malloc(17);
  int end = 15;

  strcpy(Imm, "0000000000000000");
  
  if(a == 0)
  {
    return  "0000000000000000";
  }
  else
  {
    do
    {
      Imm[end] = (a % 2) + '0';
      a = a / 2;
      end--;
    }while(a > 0);
    return (char *)Imm;
  }
  return "1";
}

//monta o valor imediato das inst. do tipo j em binario
char *ImmTJToBin(int a)
{
  char *Imm;
  Imm = (char *)malloc(27);
  int end = 25;

  strcpy(Imm, "00000000000000000000000000");
  
  if(a == 0)
  {
    return  "00000000000000000000000000";
  }
  else
  {
    do
    {
      Imm[end] = (a % 2) + '0';
      a = a / 2;
      end--;
    }while(a > 0);
    return (char *)Imm;
  }
  return "1";
}

//separa registradores das instrucões (add,sub,and,or,nor)
void Tipo1(char *fullLine, char *instrucaoBin)
{
  int i = 0, j = 0, z = 0;
	char *rd, *rs, *rt;
  rd = (char *)malloc(5);
  rs = (char *)malloc(5);
  rt = (char *)malloc(5);

  do
    {
      if(fullLine[i] == '$')
      {
        z = 0;
        if(j == 0)
        {
          do
	        {
		        rd[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ' '); 
          rd[z] = '\0';
          j++;
        }
        else if(j == 1)
        {
          do
	        {
		        rs[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ' '); 
          rs[z] = '\0';
          j++;
        }
        else if(j == 2)
        {
          do
	        {
		        rt[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != '\n'); 
          rt[z] = '\0';
          j++;
        }
      }
      i++;
    }while (i < 32);

  strcat(instrucaoBin, DecodeReg(rs));
  strcat(instrucaoBin, DecodeReg(rt));
  strcat(instrucaoBin, DecodeReg(rd));
}

//separa registradores das instrucões (sll,srl)
void Tipo2(char *fullLine, char *instrucaoBin)
{
  int a = 0, i = 0, j = 0, z = 0;
	char *rd, *rs, *shamt;
  rd = (char *)malloc(5);
  rs = (char *)malloc(5);
  shamt = (char *)malloc(9);

  do
    {
      if(fullLine[i] == '$')
      {
        z = 0;
        if(j == 0)
        {
          do
	        {
		        rd[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ' '); 
          rd[z] = '\0';
          j++;
        }
        else if(j == 1)
        {
          do
	        {
		        rs[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ' '); 
          rs[z] = '\0';
          j++;
        }
      }
      else if(j == 2)
      {
        z = 0;
        do
	        {
		        shamt[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != '\n'); 
        a = atoi(shamt);
        j++;
      }
      i++;
    }while (i < 32);
  strcat(instrucaoBin, DecodeReg(rs));
  strcat(instrucaoBin, "00000");
  strcat(instrucaoBin, DecodeReg(rd));
  strcat(instrucaoBin, ShamtToBin(a));
}

//separa registradores das instrucões (jr)
void Tipo3(char *fullLine, char *instrucaoBin)
{
  int i = 0, j = 0, z = 0;
	char *rs;
  rs = (char *)malloc(5);

  do
    {
      if(fullLine[i] == '$')
      {
        z = 0;
        if(j == 0)
        {
          do
	        {
		        rs[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != '\n'); 
          rs[z] = '\0';
          j++;
        }
      }
      i++;
    }while (i < 32);

  strcat(instrucaoBin, DecodeReg(rs));
}

//separa registradores das instrucões (lw,sw)
void Tipo4(char *fullLine, char *instrucaoBin)
{
  int a = 0, i = 0, j = 0, z = 0;
	char *rt, *rs, *Imm;
  rt = (char *)malloc(5);
  rs = (char *)malloc(5);
  Imm = (char *)malloc(5);

  do
    {
      if(fullLine[i] == '$')
      {
        z = 0;
        if(j == 0)
        {
          do
	        {
		        rt[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ' '); 
          rt[z] = '\0';
          j++;
        }
        else if(j == 2)
        {
          do
	        {
		        rs[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ')'); 
          rs[z] = '\0';
          j++;
        }
      }
      else if(j == 1)
      {
        z = 0;
        do
	        {
		        Imm[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != '('); 
        a = atoi(Imm);
        j++;
      }
      i++;
    }while (i < 32);
  strcat(instrucaoBin, DecodeReg(rs));
  strcat(instrucaoBin, DecodeReg(rt));
  strcat(instrucaoBin, ImmTIToBin(a));
}

void Tipo5(char *fullLine, char *instrucaoBin)
{
    int a = 0, i = 0, j = 0, z = 0;
	char *Imm;
  Imm = (char *)malloc(5);

  do
    {
      if(fullLine[i] == ' ')
      {
        i++;
        z = 0;
        do
	        {
		        Imm[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != '\n'); 
        a = atoi(Imm); 
        i = 32;
      }
      i++;
    }while (i < 32);
  strcat(instrucaoBin, ImmTJToBin(a));
}

//separa registradores das instrucões (sll,srl)
void Tipo6(char *fullLine, char *instrucaoBin)
{
  int a = 0, i = 0, j = 0, z = 0;
	char *rt, *rs, *shamt;
  rt = (char *)malloc(5);
  rs = (char *)malloc(5);
  shamt = (char *)malloc(17);

  do
    {
      if(fullLine[i] == '$')
      {
        z = 0;
        if(j == 0)
        {
          do
	        {
		        rs[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ' '); 
          rs[z] = '\0';
          j++;
        }
        else if(j == 1)
        {
          do
	        {
		        rt[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != ' '); 
          rt[z] = '\0';
          j++;
        }
      }
      else if(j == 2)
      {
        z = 0;
        do
	        {
		        shamt[z] = fullLine[i];
            z++;
		        i++;
	        } while ( fullLine[i] != '\n'); 
        a = atoi(shamt);
        j++;
      }
      i++;
    }while (i < 32);
  strcat(instrucaoBin, DecodeReg(rs));
  strcat(instrucaoBin, DecodeReg(rt));
  strcat(instrucaoBin, ImmTIToBin(a));
}

int DecodeInstAddMP(char *inputLine, int endereco, long long int *MP)
{
  int tipo;
  char instrucaoBin [34] = {"00000000000000000000000000000000"};
  char instructionName[6];      // guarda o nome da instrução
  
  strcpy(instructionName, getNameAssembly(inputLine));
  
  if (!(strcmp(instructionName,"lw")))
  {
    strcpy(instrucaoBin, "100011");
    Tipo4(inputLine, instrucaoBin);
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"sw")))
  {		
    strcpy(instrucaoBin, "101011");
    Tipo4(inputLine, instrucaoBin);
    printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"beq")))
  {  
		strcpy(instrucaoBin, "000100");
    Tipo6(inputLine, instrucaoBin);
    printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	}     
	else if (!(strcmp(instructionName,"add")))
  {
    strcpy(instrucaoBin, "000000");
    Tipo1(inputLine, instrucaoBin);
    strcat(instrucaoBin, "00000100000");
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"sub")))
  {
	  strcpy(instrucaoBin, "000000");
    Tipo1(inputLine, instrucaoBin);
    strcat(instrucaoBin, "00000100010");
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"and")))
  {
		strcpy(instrucaoBin, "000000");
    Tipo1(inputLine, instrucaoBin);
    strcat(instrucaoBin, "00000100100");
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	}
  else if (!(strcmp(instructionName,"or")))
  {
		strcpy(instrucaoBin, "000000");
    Tipo1(inputLine, instrucaoBin);
    strcat(instrucaoBin, "00000100101");
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"nor")))
  {
		strcpy(instrucaoBin, "000000");
    Tipo1(inputLine, instrucaoBin);
    strcat(instrucaoBin, "00000100111");
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"slt")))
  {
		strcpy(instrucaoBin, "000000");
    Tipo1(inputLine, instrucaoBin);
    strcat(instrucaoBin, "00000101010");
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	}  
  else if (!(strcmp(instructionName,"j")))
  {
    strcpy(instrucaoBin, "000010");
    Tipo5(inputLine, instrucaoBin);
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"jr")))
  {  
    strcpy(instrucaoBin, "000000");
    Tipo3(inputLine, instrucaoBin);
    strcat(instrucaoBin, "000000000000000000100");
		printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	}  
  else if (!(strcmp(instructionName,"sll")))
  {
    strcpy(instrucaoBin, "000000");
    Tipo2(inputLine, instrucaoBin);
    strcat(instrucaoBin, "000000");
    printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	} 
  else if (!(strcmp(instructionName,"srl")))
  {
		strcpy(instrucaoBin, "000000");
    Tipo2(inputLine, instrucaoBin);
    strcat(instrucaoBin, "000010");
    printf("\n intrucao: %s\n", instrucaoBin);
    AddToMP(instrucaoBin, endereco, MP);
    return 1;
	}
	else 
  {
		printf("\nERRO! - Funcao nao existente!\n");
    return 0;
	}	
  return 0;
}

char *MPPBin(long long int *MP, int endereco)
{
  char instrucaoBin [34] = {"00000000000000000000000000000000"};
  unsigned long int inst = MP[endereco];
  int indice = 31;
  
  do
  {
    if((inst % 2) == 1)
    {
      instrucaoBin[indice] = '1';
    }
    inst = inst / 2;
    indice--;
  }while(inst > 0);

  printf("\n%s\n", instrucaoBin);
  
  return (char *)instrucaoBin;
}

char *RegBin(int num)
{
  char *instrucaoBin;
  instrucaoBin = (char *)malloc(5);
  int indice = 4;

  strcpy(instrucaoBin, "00000");
  
  do
  {
    if((num % 2) == 1)
    {
      instrucaoBin[indice] = '1';
    }
    num = num / 2;
    indice--;
  }while(num > 0);

  printf("\n%s\n", instrucaoBin);
  
  return (char *)instrucaoBin;
}

int ConversorRegistroInteiro(char registrador[5])
{
  int i = 0;
  int j = 0;
  int registradorInt;
  for(i = 4; i >= 0; i--)
    {
      registradorInt += (registrador[j] - '0') * pow( 2, i);
      j += 1;
    }
  return registradorInt;
}

int ExecutarInstrucao(long long int *MP, int endereco, long long int registradores[32], int ciclos)
{
  char instrucaoBin[34];
  char *opcode, *funct, *rs, *rt, *rd, *immTI, *shamt, *logicas;
  int i, registradorRs, registradorRt, registradorRd, imm, shiftA, indicert, indicertrs, indicerd, ind = 16;
  i = registradorRs = registradorRt = registradorRd = imm = 0;

  opcode = (char *)malloc(6);
  rs = (char *)malloc(5);
  rt = (char *)malloc(5);
  rd = (char *)malloc(5);
  logicas = (char *)malloc(6);
  funct = (char *)malloc(6);
  immTI = (char *)malloc(16);
  shamt = (char *)malloc(5);
  
  strcpy(instrucaoBin, MPPBin(MP, endereco));
  for(i = 0; i < 6; i ++)
  {
    opcode[i] = instrucaoBin[i];
    funct[i] = instrucaoBin[i + 26];
  }
  
  if(!(strcmp(opcode,"000000")))
  {
    for(i = 0; i < 5; i ++)
    {
      rs[i] = instrucaoBin[i + 6];
      rt[i] = instrucaoBin[i + 11];
      rd[i] = instrucaoBin[i + 16];
      shamt[i] = instrucaoBin[i + 21];
    }
    registradorRs = (int) strtol(rs, NULL, 2);
    registradorRt = (int) strtol(rt, NULL, 2);
    registradorRd = (int) strtol(rd, NULL, 2);
    shiftA = (int) strtol(shamt, NULL, 2);
    if(!(strcmp(funct,"100000")))
    {
      registradores[registradorRd] = registradores[registradorRs] + registradores[registradorRt];
      printf("\nregistrador[%i] = %lli\n",registradorRd, registradores[registradorRd]);
      return endereco;
    }
    else if(!(strcmp(funct,"100010")))
    {
      registradores[registradorRd] = registradores[registradorRs] - registradores[registradorRt];
      printf("\nregistrador[%i] = %lli\n",registradorRd, registradores[registradorRd]);
      return endereco;
    }
    else if(!(strcmp(funct,"100100")))
    {
      printf("\and\n");
      return endereco;
    }
    else if(!(strcmp(funct,"100101")))
    {
      printf("\nor\n");
      return endereco;
    }
    else if(!(strcmp(funct,"100111")))
    {
      printf("\nnor\n");
      return endereco;
    }
    else if(!(strcmp(funct,"101010")))
    {
      if(registradores[registradorRs] < registradores[registradorRt])
      {
        registradores[registradorRd] = 1;
      }
      else
      {
        registradores[registradorRd] = 0;
      }
      printf("\nslt realizado e registrador[%i] =  %lli\n",registradorRd,registradores[registradorRd]);
      return endereco;
    }
    else if(!(strcmp(funct,"000100")))
    {
      endereco = (registradores[registradorRs] - 1);
      printf("\npulou para pc: %i\n",endereco+1);
      return endereco;
    }
    else if(!(strcmp(funct,"000000")))
    {
      registradores[registradorRd] = registradores[registradorRs] * pow(2, shiftA);
      printf("\nsll realizado e registrador[%i] =  %lli\n",registradorRd,registradores[registradorRd]);
      return endereco;
    }
    else if(!(strcmp(funct,"000010")))
    {
      registradores[registradorRd] = registradores[registradorRs] / pow(2, shiftA);
      printf("\nsrl realizado e registrador[%i] =  %lli\n",registradorRd,registradores[registradorRd]);
      return endereco;
    }
  }
  else if(!(strcmp(opcode,"000010")))
  {
    for(i = 0; i < 26; i++)
    {
      immTI[i] = instrucaoBin[i + 6];
    }
    printf("Chegou");
    imm = (int) strtol(immTI, NULL, 2);
    printf("Chegou");
    endereco = (imm - 1);
    printf("\npulou para pc: %i\n",imm);
    return endereco;
  }
  else if(!(strcmp(opcode,"000100")))
  {
    for(i = 0; i < 5; i ++)
    {
      rs[i] = instrucaoBin[i + 6];
      rt[i] = instrucaoBin[i + 11];
    }
    
    for(i = 0; i < 16; i++)
    {
      immTI[i] = instrucaoBin[i + 17];
    }
    registradorRs = (int) strtol(rs, NULL, 2);
    registradorRt = (int) strtol(rt, NULL, 2);
    imm = (int) strtol(immTI, NULL, 2);
    if(registradores[registradorRs] == registradores[registradorRt])
    {
      endereco = (imm - 1);
      printf("\npulou para pc: %i\n",imm);
    }
    return endereco;
  }
  else if(!(strcmp(opcode,"100011")))
  {
    for(i = 0; i < 5; i ++)
    {
      rs[i] = instrucaoBin[i + 6];
      rt[i] = instrucaoBin[i + 11];
    }
    
    for(i = 0; i < 16; i++)
    {
      immTI[i] = instrucaoBin[i + 17];
    }
    registradorRs = (int) strtol(rs, NULL, 2);
    registradorRt = (int) strtol(rt, NULL, 2);
    imm = (int) strtol(immTI, NULL, 2);
    indicert = imm + registradores[registradorRs];
    registradores[registradorRt] = MP[indicert];
    
    printf("\n%lli adicionado ao registrador[%i]\n",MP[indicert], registradorRt);
    return endereco;
    ciclos += 50;
  }
  else if(!(strcmp(opcode,"101011")))
  {
    for(i = 0; i < 5; i ++)
    {
      rs[i] = instrucaoBin[i + 6];
      rt[i] = instrucaoBin[i + 11];
    }
    
    for(i = 0; i < 16; i++)
    {
      immTI[i] = instrucaoBin[i + 17];
    }
    registradorRs = (int) strtol(rs, NULL, 2);
    registradorRt = (int) strtol(rt, NULL, 2);
    imm = (int) strtol(immTI, NULL, 2);
    indicert = imm + registradores[registradorRs];
    MP[indicert] = registradores[registradorRt];
    
    printf("\nvalor do registrador[%i] adicionado ao endereco %i de memoria\n",registradorRt, indicert);
    return endereco;
  } 
  return endereco;
}

int ChecarInst(long long int *MP, int endereco)
{
  char instrucaoBin[34];
  char *opcode;
  int i;

  opcode = (char *)malloc(6);
  
  strcpy(instrucaoBin, MPPBin(MP, endereco));

  for(i = 0; i < 6; i ++)
  {
    opcode[i] = instrucaoBin[i];
  }

  if(!(strcmp(opcode,"100011")))
  {
    return 50;
  }
  else if(!(strcmp(opcode,"101011")))
  {
    return 50;
  }
  return 0;
}
