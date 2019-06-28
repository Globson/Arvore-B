#include "Headers/Arvoreb.h"
#include "Headers/menu.h"

int main(int argc, char *argv[])
{ TReg x;
  TPagina *D;
  Inicializa(&D);

  while(1){
    int a=0,b=2;
    menu();
    while(a!=1 && a!=2 && a!=3 && a!=4 && a!=9){
      printf("Entre com a opcao desejada:");
      scanf("%d",&a);
      getchar();
      if(a!=1 && a!=2 && a!=3 && a!=4 && a!=9){
        printf("\n\tErro! Opcao invalida!\n\n");}}

      if(a == 1){
        printf("Entre com a chave: ");
        scanf("%ld%*[^\n]", &x.Chave);
        getchar();
        Insere(x, &D);
        Imprime(D);
        Testa(D);}
      else if (a == 2){
        printf("Entre com a chave: ");
        scanf("%ld%*[^\n]", &x.Chave);
        getchar();
        Retira(x.Chave, &D);
        Imprime(D);
        Testa(D);}
      else if (a == 3){
        printf("Entre com a chave: ");
        scanf("%ld%*[^\n]", &x.Chave);
        getchar();
        Pesquisa(&x,D);
        Imprime(D);
        Testa(D);
      }
      else if (a == 4){
        Ordem(D);
      }
      else if (a == 9){
        confirmacao();
        while(b!=0 && b!=1){
          printf("Entre com a opcao desejada:");
          scanf("%d",&b);
          getchar();
          if(b!=0 && b!=1){
            printf("Erro!Opcao invalida!\n\n");}}
          if (b == 1){
            agradecimentos();
            break;}
    }
  }
  return 0;
}
