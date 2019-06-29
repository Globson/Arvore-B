#include "../Headers/Arvoreb.h"


void Inicializa(Pont *Dicionario)//Incializa o ponteiro da árvore
{  *Dicionario = NULL; }//Ponteiro para a árvore recebe NULL

void Pesquisa(TReg *x, Pont Ap)//Função que pesquisa um registro na árvore
{ long i = 1;
  if (Ap == NULL)//Caso o ponteiro chegue em uma posição vazia
  { printf("Registro não presente na Arvore!\n");
    return;//Fim da chamada
  }
  while (i < Ap->n && x->Chave > Ap->r[i-1].Chave) i++;//Avança com o contador enquanto não atingir o número atual de chaves
  //E enquanto a chave não for menor ou igual ao registro correspondente a posição do contador menos um
  if (x->Chave == Ap->r[i-1].Chave)//Se a chave procurada é igual ao registro contido na árvore:
  { *x = Ap->r[i-1];//Chave procurada pertence à árvore
    return;//Fim da chamada
  }
  if (x->Chave < Ap->r[i-1].Chave)//Se a chave procurada é menor ao registro contido na árvore:
  Pesquisa(x, Ap->p[i-1]);//Chamada recursiva para a esquerda do registro atual
  else Pesquisa(x, Ap->p[i]);//Chamada recursiva para a direita do registro atual
}

void InsereNaPagina(Pont Ap,TReg Reg, Pont ApDir)//Insere um registro em um nó
{ short NaoAchouPosicao;
  int k;
  k = Ap->n;  NaoAchouPosicao = (k > 0);
  while (NaoAchouPosicao)//Enquanto não achar uma posição válida
    { if (Reg.Chave >= Ap->r[k-1].Chave)//Se a chave a ser inserida for maior ou igual que o registro atual no nó:
      { NaoAchouPosicao = 0;//Não existe posição para esse registro
        break;//Fim while
      }//Se houver posição para esse registro, quer dizer que ele é menor que o registro presente no nó
      Ap->r[k] = Ap->r[k-1];//Desloca o registro existente para a direita
      Ap->p[k+1] = Ap->p[k];//Desloca o filho desse registro deslocado
      k--;//Diminui o contador
      if (k < 1) NaoAchouPosicao = 0;//Caso o contador chegue na primeira posição, já não há posição para achar
    }
  Ap->r[k] = Reg;//Atribui o registro a ser inserido na posição correta definida pelo contador
  Ap->p[k+1] = ApDir;//Atribui o filho desse registro inserido na posição correta definida pelo contador
  Ap->n++;//Aumenta o número de chaves desse nó
}

void Ins(TReg Reg, Pont Ap, short *Cresceu, TReg *RegRetorno, Pont *ApRetorno)//Função que realiza a inserção de um registro na árvore
{ long i = 1; long j;
  Pont ApTemp;
  if (Ap == NULL)//Se o ponteiro chegar em uma posição vazia:
  { *Cresceu = 1; (*RegRetorno) = Reg; (*ApRetorno) = NULL;//Árvore cresce pela raiz
    return;//Fim da chamada
  }
  while (i < Ap->n && Reg.Chave > Ap->r[i-1].Chave)  i++;//Avança com o contador enquanto não atingir o número atual de chaves
  //E enquanto a chave não for menor ou igual ao registro correspondente a posição do contador menos um
  if (Reg.Chave == Ap->r[i-1].Chave)//Caso o registro presente na árvore seja igual ao que será inserido:
  { printf(" Erro: Registro ja esta presente em arvore!\n"); *Cresceu = 0;//Sem necessidade de inserção
    return;//Fim da chamada
  }
  if (Reg.Chave < Ap->r[i-1].Chave) i--;//Se o registro a ser inserido é menor que o registro contido na árvore:
  //Decrementa o contador para acessar os filhos à esquerda desse pai
  Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);//Chamada recursiva para os filhos da esquerda do registro atual
  if (!*Cresceu) return;//Verifica se houve crescimento da altura na árvore
  if (Ap->n < MM)//Se houver espaço no nó atual:
    { InsereNaPagina(Ap, *RegRetorno, *ApRetorno);//Insere o registro nesse nó
      *Cresceu = 0;//Sem necessidade de crescimento
      return;//Fim da chamada
    }
  /* Overflow: Pagina tem que ser dividida *///Processo de split em um nó
  ApTemp = (Pont)malloc(sizeof(TPagina));//Criação de um nó temporário para abrigar o valor do meio do split
  ApTemp->n = 0;  ApTemp->p[0] = NULL;//Inicializações dos atributos desse nó
  if (i < M + 1)//Se chegar no meio do nó:
  { InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM]);//Insere nesse nó temporário e sobe o mesmo
    Ap->n--;//Diminui o número de registros após o split no nó dividido
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);//Realiza inserções necessárias no nó divido caso precise
  }
  else InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);//Se não chegar no meio do nó já houve splits
  //Basta o nó temporário receber esses valores retornados
  for (j = M + 2; j <= MM; j++)//Realiza splits sucessivos caso precise
    InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);//Nó temporário recebe os valores
  Ap->n = M;  ApTemp->p[0] = Ap->p[M+1];//Atualiza os atributos do nó divido e do temporário
  *RegRetorno = Ap->r[M];  *ApRetorno = ApTemp;//Atualiza o registro e apontador a serem retornados
}

void Insere(TReg Reg, Pont *Ap)//Função que insere um registro na árvore caso a altura cresça
{ short Cresceu;
  TReg RegRetorno;
  TPagina *ApRetorno, *ApTemp;
  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);//Chamada da função que insere o registro
  if (Cresceu)  /* Arvore cresce na altura pela raiz */
  { ApTemp = (TPagina *)malloc(sizeof(TPagina));//Criação da nova raiz
    ApTemp->n = 1;//Atualiza o número de registros da raiz para o termo a ser inserido
    ApTemp->r[0] = RegRetorno;//Recebe o registro retornado pela função Ins
    ApTemp->p[1] = ApRetorno;//Recebe o apontador retornado pela função Ins
    ApTemp->p[0] = *Ap;  *Ap = ApTemp;//Organiza os apontadores
  }
}

void Reconstitui(Pont ApPag, Pont ApPai,int PosPai, short *Diminuiu)//Função que reconstitui as propriedades da árvore B
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

void Antecessor(Pont Ap, int Ind,Pont ApPai, short *Diminuiu)//Função que encontra o antecessor do registro a ser removido
{ if (ApPai->p[ApPai->n] != NULL)//Se o pai possui filhos:
  { Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);//Chama recursivamente para o filho da esquerda desse pai
    if (*Diminuiu)//Caso a propriedade de quantidade mínima de filhos por nó seja quebrada:
    Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu);//Chama o procedimento que reconstitui as propriedades da árvore
    return;//Fim da chamada
  }
  Ap->r[Ind-1] = ApPai->r[ApPai->n - 1];//Recebe o antecessor do registro a ser removido
  ApPai->n--;  *Diminuiu = (ApPai->n < M);//Diminui a quantidade de registros no nó do antecessor encontrado
  //Verifica se a quantidade mínima de filhos por nó foi quebrada
}

void Ret(long Ch, Pont *Ap, short *Diminuiu)//Função que retira um registro da árvore
{ long j, Ind = 1;
  Pont Pag;
  if (*Ap == NULL)//Caso o ponteiro da árvore seja vazio:
  { printf("Erro: registro nao esta na arvore!\n"); *Diminuiu = 0;//Não há o que remover
    return;//Fim da chamada
  }
  Pag = *Ap;//Apontador auxiliar para percorrer a árvore
  while (Ind < Pag->n && Ch > Pag->r[Ind-1].Chave) Ind++;//Avança com o índice enquanto ele for menor que a quantidade de registros no nó
  //E até o registro a ser removido ser menor ou igual ao registro atual do índice menos um
  if (Ch == Pag->r[Ind-1].Chave)//Se o registro a ser removido for achado:
  { if (Pag->p[Ind-1] == NULL)   /* TPagina folha *///O nó pode ser um nó folha, caso for:
    { Pag->n--;//Diminui o número de registros desse nó
      *Diminuiu = (Pag->n < M);//Verifica se a quantidade mínima de registros por nó foi quebrada
      for (j = Ind; j <= Pag->n; j++)//Percorre os registros deslocando-os para suas novas posições
        { Pag->r[j-1] = Pag->r[j];  Pag->p[j] = Pag->p[j+1]; }//Desloca os registros
      return;//Fim da chamada
    }//Se o nó atual não é um nó folha:
    /* TPagina nao e folha: trocar com antecessor */
    Antecessor(*Ap, Ind, Pag->p[Ind-1], Diminuiu);//Chama o procedimento que acha o antecessor do registro a ser removido
    if (*Diminuiu)//Caso a quantidade mínima de registros por nó seja quebrada:
    Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);//Chama o procedimento que reconstitui as propriedades da árvore
    return;//Fim da chamada
  }
  if (Ch > Pag->r[Ind-1].Chave) Ind++;//Se o registro a ser removido for maior que o registro atual do nó
  Ret(Ch, &Pag->p[Ind-1], Diminuiu);//Chama recursivamente para o filho da direita desse registro
  if (*Diminuiu) Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);//Caso a quantidade mínima de registros por nó seja quebrada:
  //Chama o procedimento que reconstitui as propriedades da árvore
}

void Retira(long Ch, Pont *Ap)//Função que organiza a árvore na raiz caso a retirada gere isso
{ short Diminuiu;
  Pont Aux;
  Ret(Ch, Ap, &Diminuiu);//Chama o procedimento que retira um registro da árvore
  if (Diminuiu && (*Ap)->n == 0)  /* Arvore diminui na altura */
  { Aux = *Ap;   *Ap = Aux->p[0];//Diminui a altura da árvore na raiz
    free(Aux);//Libera espaço na memória apagando o apontador auxiliar
  }
}

void ImprimeI(Pont p, int nivel)//Função que imprime cada nível da árvore
{ long i;
  if (p == NULL) return;//Se o ponteiro da árvore é vazio, não há o que exibir
  printf("Nivel %d : ", nivel);//Exibe o nível atual da árvore que está sendo exibido
  for (i = 0; i < p->n; i++)//Percorre os registros do nó atual
    printf("%ld ",(long)p->r[i].Chave);//Exibe cada registro existente nesse nó
  putchar('\n');
  nivel++;//Incrementa o contador que exibe em que nível a impressão está
  for (i = 0; i <= p->n; i++)//Percorre cada filho dos registros do nó atual
    ImprimeI(p->p[i], nivel);//Chama recursivamente para cada filho
}

void Imprime(Pont p)//Função que chama o procedimento de exibição da árvore
{ int  n = 0; ImprimeI(p, n); }//Chama o procedimento que imprime cada nível da árvore

void TestaI(Pont p, int pai, short direita)//Função que testa a ordenação da árvore
{ int i;
  int antecessor = 0;
  if (p == NULL) return;//Se chegar em um nó vazio, fim da chamada
  if (p->r[0].Chave > pai && direita == 0)//Se o filho da esquerda é maior que o pai:
  { printf("Erro: filho %12ld maior que pai %d\n", p->r[0].Chave, pai);//Mensagem de erro
    return;//Fim da chamada
  }//Se não, analisar a ordenação entre os registros do nó
  for (i = 0; i < p->n; i++)//Percorre os registros do nó atual
    { if (p->r[i].Chave <= antecessor)//Se o registro atual é menor que seu antecessor:
      { printf("Erro: irmao %ld menor que irmao a esquerda %d\n",
        (long)p->r[i].Chave, antecessor);//Mensagem de erro
        return;//Fim da chamada
      }
      antecessor = p->r[i].Chave;//Recebe o antecessor do próximo registro
    }
  for (i = 0; i < p->n; i++)//Avança com os ponteiros de filhos de cada nó
    TestaI(p->p[i], p->r[i].Chave, 0);//Avança para os filhos à direita
  TestaI(p->p[p->n], p->r[i].Chave, 1);//Avança para os filhos à esquerda
}

void Testa(Pont p)//Função que chama o procedimento para testar a ordenação da árvore
{ int i;
  if (p == NULL) return;//Se a árvore é vazia não há o que testar
  for (i = 0; i < p->n; i++)//Percorre os filhos de cada registro
    TestaI(p->p[i], p->r[i].Chave, 0);//Testa para a esquerda
  TestaI(p->p[p->n], p->r[i].Chave, 1);//Testa para a direita
}

void Ordem(Pont p){//Função que exibe os registros da árvore em ordem crescente
  int i;
  if (p != NULL){//Se o ponteiro da árvore não é vazio
    for (i = 0; i < p->n; i++){//Percorre os registros do nó atual
        Ordem(p->p[i]);//Chama recursivamente até chegar em um nó vazio
        printf("%ld ", p->r[i].Chave);//Exibe as chaves de um nó não vazio
        printf("\n");
    }
        Ordem(p->p[i]);//Chama recursivamente para o filho existente nessa posição
    }
    //Final do procedimento: Todos os registros da árvore exibidos em ordem crescente
}
