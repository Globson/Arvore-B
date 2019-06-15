#include "../Headers/Arvoreb.h"


void Inicializa(Pont *Dicionario)
{  *Dicionario = NULL; }

void Pesquisa(TReg *x, Pont Ap)
{ long i = 1;
  if (Ap == NULL)
  { printf("Registro não presente na Arvore!\n");
    return;
  }
  while (i < Ap->n && x->Chave > Ap->r[i-1].Chave) i++;
  if (x->Chave == Ap->r[i-1].Chave)
  { *x = Ap->r[i-1];
    return;
  }
  if (x->Chave < Ap->r[i-1].Chave)
  Pesquisa(x, Ap->p[i-1]);
  else Pesquisa(x, Ap->p[i]);
}

void InsereNaPagina(Pont Ap,TReg Reg, Pont ApDir)
{ short NaoAchouPosicao;
  int k;
  k = Ap->n;  NaoAchouPosicao = (k > 0);
  while (NaoAchouPosicao)
    { if (Reg.Chave >= Ap->r[k-1].Chave)
      { NaoAchouPosicao = 0;
        break;
      }
      Ap->r[k] = Ap->r[k-1];
      Ap->p[k+1] = Ap->p[k];
      k--;
      if (k < 1) NaoAchouPosicao = 0;
    }
  Ap->r[k] = Reg;
  Ap->p[k+1] = ApDir;
  Ap->n++;
}

void Ins(TReg Reg, Pont Ap, short *Cresceu,TReg *RegRetorno,  Pont *ApRetorno)
{ long i = 1; long j;
  Pont ApTemp;
  if (Ap == NULL)
  { *Cresceu = 1; (*RegRetorno) = Reg; (*ApRetorno) = NULL;
    return;
  }
  while (i < Ap->n && Reg.Chave > Ap->r[i-1].Chave)  i++;
  if (Reg.Chave == Ap->r[i-1].Chave)
  { printf(" Erro: Registro ja esta presente em arvore!\n"); *Cresceu = 0;
    return;
  }
  if (Reg.Chave < Ap->r[i-1].Chave) i--;
  Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
  if (!*Cresceu) return;
  if (Ap->n < MM)   /* Pagina tem espaco */
    { InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
      *Cresceu = 0;
      return;
    }
  /* Overflow: Pagina tem que ser dividida */
  ApTemp = (Pont)malloc(sizeof(TPagina));
  ApTemp->n = 0;  ApTemp->p[0] = NULL;
  if (i < M + 1)
  { InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  }
  else InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  for (j = M + 2; j <= MM; j++)
    InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
  Ap->n = M;  ApTemp->p[0] = Ap->p[M+1];
  *RegRetorno = Ap->r[M];  *ApRetorno = ApTemp;
}

void Insere(TReg Reg, Pont *Ap)
{ short Cresceu;
  TReg RegRetorno;
  TPagina *ApRetorno, *ApTemp;
  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
  if (Cresceu)  /* Arvore cresce na altura pela raiz */
  { ApTemp = (TPagina *)malloc(sizeof(TPagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    ApTemp->p[0] = *Ap;  *Ap = ApTemp;
  }
}

void Reconstitui(Pont ApPag, Pont ApPai,int PosPai, short *Diminuiu)
{ TPagina *Aux;  long DispAux, j;
  if (PosPai < ApPai->n)  /* Aux = TPagina a direita de ApPag */
  { Aux = ApPai->p[PosPai+1];  DispAux = (Aux->n - M + 1) / 2;
    ApPag->r[ApPag->n] = ApPai->r[PosPai];
    ApPag->p[ApPag->n + 1] = Aux->p[0];  ApPag->n++;
    if (DispAux > 0)  /* Existe folga: transfere de Aux para ApPag */
    { for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
      ApPai->r[PosPai] = Aux->r[DispAux-1];  Aux->n -= DispAux;
      for (j = 0; j < Aux->n; j++) Aux->r[j] = Aux->r[j + DispAux];
      for (j = 0; j <= Aux->n; j++) Aux->p[j] = Aux->p[j + DispAux];
      *Diminuiu = 0;
    }
    else /* Fusao: intercala Aux em ApPag e libera Aux */
      { for (j = 1; j <= M; j++)
          InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
        free(Aux);
        for (j = PosPai + 1; j < ApPai->n; j++)
          { ApPai->r[j-1] = ApPai->r[j];
	    ApPai->p[j] = ApPai->p[j+1];
	  }
        ApPai->n--;
        if (ApPai->n >= M) *Diminuiu = 0;
      }
  }
  else /* Aux = TPagina a esquerda de ApPag */
    { Aux = ApPai->p[PosPai-1]; DispAux = (Aux->n - M + 1) / 2;
      for (j = ApPag->n; j >= 1; j--) ApPag->r[j] = ApPag->r[j-1];
      ApPag->r[0] = ApPai->r[PosPai-1];
      for (j = ApPag->n; j >= 0; j--) ApPag->p[j+1] = ApPag->p[j];
      ApPag->n++;
      if (DispAux > 0) /* Existe folga: transf. de Aux para ApPag */
      { for (j = 1; j < DispAux; j++)
          InsereNaPagina(ApPag, Aux->r[Aux->n - j],
	                 Aux->p[Aux->n - j + 1]);
        ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
        ApPai->r[PosPai-1] = Aux->r[Aux->n - DispAux];
        Aux->n -= DispAux;  *Diminuiu = 0;
      }
      else /* Fusao: intercala ApPag em Aux e libera ApPag */
        { for (j = 1; j <= M; j++)
            InsereNaPagina(Aux, ApPag->r[j-1], ApPag->p[j]);
          free(ApPag);  ApPai->n--;
          if (ApPai->n >= M)  *Diminuiu = 0;
        }
    }
}

void Antecessor(Pont Ap, int Ind,Pont ApPai, short *Diminuiu)
{ if (ApPai->p[ApPai->n] != NULL)
  { Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
    if (*Diminuiu)
    Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu);
    return;
  }
  Ap->r[Ind-1] = ApPai->r[ApPai->n - 1];
  ApPai->n--;  *Diminuiu = (ApPai->n < M);
}

void Ret(long Ch, Pont *Ap, short *Diminuiu)
{ long j, Ind = 1;
  Pont Pag;
  if (*Ap == NULL)
  { printf("Erro: registro nao esta na arvore!\n"); *Diminuiu = 0;
    return;
  }
  Pag = *Ap;
  while (Ind < Pag->n && Ch > Pag->r[Ind-1].Chave) Ind++;
  if (Ch == Pag->r[Ind-1].Chave)
  { if (Pag->p[Ind-1] == NULL)   /* TPagina folha */
    { Pag->n--;
      *Diminuiu = (Pag->n < M);
      for (j = Ind; j <= Pag->n; j++)
        { Pag->r[j-1] = Pag->r[j];  Pag->p[j] = Pag->p[j+1]; }
      return;
    }
    /* TPagina nao e folha: trocar com antecessor */
    Antecessor(*Ap, Ind, Pag->p[Ind-1], Diminuiu);
    if (*Diminuiu)
    Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);
    return;
  }
  if (Ch > Pag->r[Ind-1].Chave) Ind++;
  Ret(Ch, &Pag->p[Ind-1], Diminuiu);
  if (*Diminuiu) Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);
}

void Retira(long Ch, Pont *Ap)
{ short Diminuiu;
  Pont Aux;
  Ret(Ch, Ap, &Diminuiu);
  if (Diminuiu && (*Ap)->n == 0)  /* Arvore diminui na altura */
  { Aux = *Ap;   *Ap = Aux->p[0];
    free(Aux);
  }
}

void ImprimeI(Pont p, int nivel)
{ long i;
  if (p == NULL) return;
  printf("Nivel %d : ", nivel);
  for (i = 0; i < p->n; i++)
    printf("%ld ",(long)p->r[i].Chave);
  putchar('\n');
  nivel++;
  for (i = 0; i <= p->n; i++)
    ImprimeI(p->p[i], nivel);
}

void Imprime(Pont p)
{ int  n = 0; ImprimeI(p, n); }

void TestaI(Pont p, int pai, short direita)
{ int i;
  int antecessor = 0;
  if (p == NULL) return;
  if (p->r[0].Chave > pai && direita == 0)
  { printf("Erro: filho %12ld maior que pai %d\n", p->r[0].Chave, pai);
    return;
  }
  for (i = 0; i < p->n; i++)
    { if (p->r[i].Chave <= antecessor)
      { printf("Erro: irmao %ld maior que irmao a esquerda %d\n",
        (long)p->r[i].Chave, antecessor);
        return;
      }
      antecessor = p->r[i].Chave;
    }
  for (i = 0; i < p->n; i++)
    TestaI(p->p[i], p->r[i].Chave, 0);
  TestaI(p->p[p->n], p->r[i].Chave, 1);
}

void Testa(Pont p)
{ int i;
  if (p == NULL) return;
  for (i = 0; i < p->n; i++)
    TestaI(p->p[i], p->r[i].Chave, 0);
  TestaI(p->p[p->n], p->r[i].Chave, 1);
}
