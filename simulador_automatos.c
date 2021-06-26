/*  
    Simulador Universal de Autômatos Finitos
        Grupo:
        Geraldo Murilo Carrijo Viana Alves da Silva; 11849306; murilocarrijosilva@usp.br

        Iara Duarte Mainates; 11816143; iaramainates@usp.br

        Lucas Caetano Procópio; 11831338; lucascbsi020@usp.br

        Documentação disponível em:
        ---
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define MOSTRAR_ENTRADA

//Definicoes de Máximos
#define MAX_ESTADOS 10
#define MAX_SIMBOLOS 10
#define MAX_TRANSICOES 50
#define MAX_CADEIAS 10
#define MAX_COMPR_CADEIA 10

//Definicao do tipo do Automato
#define AFD 0
#define AFN 1

typedef struct automata
{
    int tipoAutomato;
    int qntEstados;
    int estadosIniciais;
    int qntEstadosFinais;
    int *estadosFinais;
    int **qntTransicoes;
    int ***transicoes;
    char *vocabulario;
} automata_t;

void leEntrada(automata_t *automata);
int avaliaAFD(automata_t *afd, char *str, int estadoAtual);
int avaliaAFN(automata_t *afn, char *str, int estadoAtual);
void imprimeResultados(int qtdCadeias,int *resultadosCadeia);

int main(int argc, char **argv)
{
    int numCadeias;
    char cadeia[4096];
    automata_t automato;
    int resultado;
    int *resultadosCadeia;
 
    leEntrada(&automato);

    scanf("%d", &numCadeias);
    resultadosCadeia=(int*)malloc(numCadeias*sizeof(int));
    
    for (int i = 0; i < numCadeias; i++)
    {
        resultadosCadeia[i]=0;
        scanf("%s", cadeia);
        
        if(strcmp(cadeia,"-")==0)
        {
            resultadosCadeia[i]=0;
            if(automato.tipoAutomato==AFD)
            {
                for(int j=0;j<automato.qntEstadosFinais;j++)
                {
                    if(automato.estadosFinais[j]==0)
                    {
                        resultadosCadeia[i]=1;
                        break;
                    }
                }
            }
            else
            {
                for(int j=0;j<automato.qntEstadosFinais;j++)
                {
                    for(int k=0;k<automato.estadosIniciais;k++)
                    {
                        if(automato.estadosFinais[j]==k)
                        {
                            resultadosCadeia[i]=1;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            if(automato.tipoAutomato==AFD)
            {
                resultado=avaliaAFD(&automato,cadeia,0);
                if(resultado==0)
                {
                    resultadosCadeia[i]=1;
                }
            }
            else if(automato.tipoAutomato==AFN)
            {
                for (int estadoInicial = 0; estadoInicial < automato.estadosIniciais; estadoInicial++)
                {
                    resultado = avaliaAFN(&automato, cadeia, estadoInicial);
                    if (resultado == 0)
                    {
                        resultadosCadeia[i]=1;
                        break;
                    }
                }
            }
        }
    }

    imprimeResultados(numCadeias,resultadosCadeia);
    return 0;
}

int avaliaAFD(automata_t *afd, char *str, int estadoAtual)
{
    char simboloAtual = *str;
    int indiceSimboloAtual;

    if (simboloAtual == '\0' || estadoAtual == -1)
    {
        for (int i = 0; i < afd->qntEstadosFinais; i++)
            if (afd->estadosFinais[i] == estadoAtual) return 0;
        return -1;
    }

    for (indiceSimboloAtual = 0; indiceSimboloAtual < MAX_SIMBOLOS; indiceSimboloAtual++)
        if (afd->vocabulario[indiceSimboloAtual] == simboloAtual) break;
    
    str++;
    return avaliaAFD(afd, str, afd->transicoes[estadoAtual][indiceSimboloAtual][0]);
}

int avaliaAFN(automata_t *afn, char *str, int estadoAtual)
{
    char simboloAtual = *str;
    int indiceSimboloAtual;

    if (simboloAtual == '\0')
    {
        for (int i = 0; i < afn->qntEstadosFinais; i++)
            if (afn->estadosFinais[i] == estadoAtual) return 0;
        return -1;
    }
    
    for (indiceSimboloAtual = 0; indiceSimboloAtual < MAX_SIMBOLOS; indiceSimboloAtual++)
        if (afn->vocabulario[indiceSimboloAtual] == simboloAtual) break;

    if (afn->qntTransicoes[estadoAtual][indiceSimboloAtual] == 0)
        return -1;

    for (int i = 0; i < afn->qntTransicoes[estadoAtual][indiceSimboloAtual]; i++)
    {
        if (avaliaAFN(afn, str + 1, afn->transicoes[estadoAtual][indiceSimboloAtual][i]) == 0)
            return 0;
    }
    return -1;
}

void leEntrada(automata_t *automata)
{
    int qntSimbolos;
    int qntTransicoes;

    // Estados
    scanf("%d", &automata->qntEstados);

    // Simbolos terminais
    scanf("%d", &qntSimbolos);
    automata->vocabulario = (char *) malloc(sizeof(char) * qntSimbolos);
    for (int i = 0; i < qntSimbolos; i++)
    {
        scanf(" %c", automata->vocabulario + i);
    }

    // Estados iniciais
    scanf("%d", &automata->estadosIniciais);
    if(automata->estadosIniciais==1)
    {
        automata->tipoAutomato=AFD;
    }
    else
    {
        automata->tipoAutomato=AFN;
    }

    // Estados finais
    scanf("%d", &automata->qntEstadosFinais);
    automata->estadosFinais = (int *) malloc(sizeof(int) * automata->qntEstadosFinais);
    for (int i = 0; i < automata->qntEstadosFinais; i++)
    {
        scanf(" %d", automata->estadosFinais + i);
    }

    // Transicoes
    scanf("%d", &qntTransicoes);
    // Alocando espaço para as transições
    automata->transicoes = (int ***) malloc(sizeof(int **) * automata->qntEstados);
    automata->qntTransicoes = (int **) malloc(sizeof(int *) * automata->qntEstados);
    for (int i = 0; i < automata->qntEstados; i++)
    {
        automata->transicoes[i] = (int **) malloc(sizeof(int *) * qntSimbolos);
        automata->qntTransicoes[i] = (int *) malloc(sizeof(int) * qntSimbolos);
    }
    // Quantidade de transições por par (estado, simbolo)
    for (int i = 0; i < automata->qntEstados; i++)
    {
        for (int j = 0; j < qntSimbolos; j++)
        {
            automata->transicoes[i][j] = NULL;
            automata->qntTransicoes[i][j] = 0;
        }
    }
    // Lendo as transições
    for (int i = 0; i < qntTransicoes; i++)
    {
        int q, qlinha;
        char simbolo;

        scanf("%d %c %d", &q, &simbolo, &qlinha);

        int indiceSimbolo;
        for (indiceSimbolo = 0; indiceSimbolo < qntSimbolos; indiceSimbolo++)
            if (automata->vocabulario[indiceSimbolo] == simbolo) break;

        int novaQnt = automata->qntTransicoes[q][indiceSimbolo] + 1;

        automata->qntTransicoes[q][indiceSimbolo] = novaQnt;
        automata->transicoes[q][indiceSimbolo] = (int *) realloc(automata->transicoes[q][indiceSimbolo], sizeof(int) * novaQnt);
        automata->transicoes[q][indiceSimbolo][novaQnt - 1] = qlinha;
    }
}

void imprimeResultados(int qtdCadeias,int *resultadosCadeia)
{
    for (int i = 0; i < qtdCadeias; i++)
    {
        if(resultadosCadeia[i]==1)
        {
            printf("%d. aceita\n",i);
        }
        else if(resultadosCadeia[i]==0)
        {
            printf("%d. rejeita\n",i);
        }
    }
}