#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>

typedef struct Args
{
    int L;
    int Z;
    int K;
    int TL;
    int TB;
} Argumenty;

int L;
int Z;
int K;
int TL;
int TB;

int *Action = NULL;

void SKIBUS()
{
    srand(getpid() + time(NULL));
    int idZ = 1;
    /*
    Po spuštění vypíše: A: BUS: started
    • (#) idZ=1 (identifikace zastávky)
    • (*) Jede na zastávku idZ---čeká pomocí volání usleep náhodný čas v intervalu <0,TB>.
    • Vypíše: A: BUS: arrived to idZ

    • Nechá nastoupit všechny čekající lyžaře do kapacity autobusu
    • Vypíše: A: BUS: leaving idZ

    • Pokud idZ<Z, tak idZ=idZ+1 a pokračuje bodem (*)
    • Jinak jede na výstupní zastávku---čeká pomocí volání usleep náhodný čas v intervalu <0,TB>.
    • Vypíše: A: BUS: arrived to final

    • Nechá vystoupit všechny lyžaře
    • Vypíše: A: BUS: leaving final

    • Pokud ještě nějací lyžaři čekají na některé ze zastávek/mohou přijít na zastávku, tak pokračuje
    bodem (#)
    • Jinak vypíše: A: BUS: finish

    • Proces končí
    */

    printf("%d: BUS: started\n", ++(*Action));
    usleep(rand() % TB);
    printf("%d: BUS: arrived to %d\n", ++(*Action), idZ);
    printf("%d: BUS: leaving %d\n", ++(*Action));
    if (idZ < Z)
        idZ++;
    usleep(rand() % TB);
    printf("%d: BUS: arrived to final\n", ++(*Action));
    printf("%d: BUS: leaving final\n", ++(*Action));
    printf("%d: BUS: finish\n", ++(*Action));
}

void LYZAR()
{
}

int main(int argc, char **argv)
{
    L = atoi(argv[1]);
    Z = atoi(argv[2]);
    K = atoi(argv[3]);
    TL = atoi(argv[4]);
    TB = atoi(argv[5]);

    Action = mmap(NULL, sizeof(*Action), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    printf("Parent id is -> %d\n", getpid());
    pid_t skibusID = fork();
    if (skibusID < 0)
        printf("Error fork skibus\n");
    else if (skibusID == 0)
    {
        printf("Skibus (%d) nastartoval.\n", getpid());
        return 0;
    }

    for (int i = 0; i < L; i++)
    {

        pid_t lyzarID = fork();
        if (lyzarID < 0)
            printf("Error fork lyzar\n");
        else if (lyzarID == 0)
        {
            srand(getpid() + time(NULL));
            int zastavka = (rand() % Z) + 1;
            printf("Lyzar (%d) se spawnul a jde na zastavku %d\n", getpid(), zastavka);
            return 0;
        }
    }

    while (wait(NULL) > 0)
        ;

    Action = munmap(NULL, sizeof(*Action));
    return 0;
}