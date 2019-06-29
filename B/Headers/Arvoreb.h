#ifndef ARVOREB_H
#define ARVOREB_H

#include<stdlib.h>
#include<stdio.h>
//Ávore B de ordem 5
#define M 2
#define MM (M * 2)

typedef struct TReg {
  long Chave;//número inteiro
  /*outros componentes*/
} TReg;//Estrutura que contém os atributos de um registro da árvore

typedef struct TPagina* Pont;//Renomeação do ponteiro para a struct TPagina para Pont

typedef struct TPagina {
  short n;//Quantidade de registros existentes no nó
  TReg r[MM];//Vetor de registros de tamanho 4 (M-1)
  Pont p[MM + 1];//Vetor de ponteiros para os filhos desses registros de tamanho 5 (M)
} TPagina;//Estrutura que contém os atributos de um nó da árvore


void Inicializa(Pont *Dicionario);//Incializa o ponteiro da árvore
void Pesquisa(TReg *x, Pont Ap);//Função que pesquisa um registro na árvore
void InsereNaPagina(Pont Ap,TReg Reg, Pont ApDir);//Insere um registro em um nó
void Ins(TReg Reg, Pont Ap, short *Cresceu, TReg *RegRetorno, Pont *ApRetorno);//Função que realiza a inserção de um registro na árvore
void Insere(TReg Reg, Pont *Ap);//Função que insere um registro na árvore caso a altura cresça
void Reconstitui(Pont ApPag, Pont ApPai,int PosPai, short *Diminuiu);//Função que reconstitui as propriedades da árvore B
void Antecessor(Pont Ap, int Ind,Pont ApPai, short *Diminuiu);//Função que encontra o antecessor do registro a ser removido
void Ret(long Ch, Pont *Ap, short *Diminuiu);//Função que retira um registro da árvore
void Retira(long Ch, Pont *Ap);//Função que organiza a árvore na raiz caso a retirada gere isso
void ImprimeI(Pont p, int nivel);//Função que imprime cada nível da árvore
void Imprime(Pont p);//Função que chama o procedimento de exibição da árvore
void TestaI(Pont p, int pai, short direita);//Função que testa a ordenação da árvore
void Testa(Pont p);//Função que chama o procedimento para testar a ordenação da árvore
void Ordem(Pont p);//Função que exibe os registros da árvore em ordem crescente

#endif
