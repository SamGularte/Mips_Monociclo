
char *getNameAssembly(char *fullLine);

int DecodeInstAddMP(char *instructionName, int endereco, long long int *MP);

int ExecutarInstrucao(long long int *MP, int endereco, long long int registradores[32], int ciclos);

int ChecarInst(long long int *MP, int endereco);
