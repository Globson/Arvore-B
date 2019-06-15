#ifndef ARVOREB_H
#define ARVOREB_H

#include<stdlib.h>
#include<stdio.h>

#define M 2
#define MM (M * 2)

typedef struct TReg {
  long Chave;
  /*outros componentes*/
} TReg;

typedef struct TPagina* Pont;

typedef struct TPagina {
  short n;
  TReg r[MM];
  Pont p[MM + 1];
} TPagina;


void Inicializa(Pont *Dicionario);
void Pesquisa(TReg *x, Pont Ap);
void InsereNaPagina(Pont Ap,TReg Reg, Pont ApDir);
void Ins(TReg Reg, Pont Ap, short *Cresceu,TReg *RegRetorno,  Pont *ApRetorno);
void Insere(TReg Reg, Pont *Ap);
void Reconstitui(Pont ApPag, Pont ApPai,int PosPai, short *Diminuiu);
void Antecessor(Pont Ap, int Ind,Pont ApPai, short *Diminuiu);
void Ret(long Ch, Pont *Ap, short *Diminuiu);
void Retira(long Ch, Pont *Ap);
void ImprimeI(Pont p, int nivel);
void Imprime(Pont p);
void TestaI(Pont p, int pai, short direita);
void Testa(Pont p);

#endif
