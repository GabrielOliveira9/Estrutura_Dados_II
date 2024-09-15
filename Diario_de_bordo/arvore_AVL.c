#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct NO {
  int info;
  int altura;
  struct NO *esq;
  struct NO *dir;
};

typedef struct NO *ArvAVL;

ArvAVL *raiz;

void imprimeCaminho(int caminho[], int tamanho);
int isBST(ArvAVL *raiz);
int isBSTAux(ArvAVL *raiz, long min, long max);
void liberaNO(struct NO *no);
struct NO *procuraMenor(struct NO *atual);

int main(void) {
  printf("Hello World\n");
  return 0;
}

ArvAVL *criaArvore() {
  ArvAVL *raiz = (ArvAVL *)malloc(sizeof(ArvAVL));
  if (raiz != NULL) {
    *raiz = NULL;
  }
  return raiz;
}

void liberaArvore(ArvAVL *raiz) {
  if (raiz == NULL) {
    return;
  }
  liberaNO(*raiz);
  free(raiz);
}

void liberaNO(struct NO *no) {
  if (no == NULL) {
    return;
  }
  liberaNO(no->esq);
  liberaNO(no->dir);
  free(no);
  no = NULL;
}

int alturaNO(struct NO *no) {
  if (no == NULL) {
    return -1;
  } else {
    return no->altura;
  }
}

int fatorBalanceamento(struct NO *no) {
  return labs(alturaNO(no->esq) - alturaNO(no->dir));
}

int maior(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

void rotacaoLL(ArvAVL *raiz) {
  struct NO *no = (*raiz)->esq;
  (*raiz)->esq = no->dir;
  no->dir = *raiz;
  (*raiz)->altura = maior(alturaNO((*raiz)->esq), alturaNO((*raiz)->dir)) + 1;
  no->altura = maior(alturaNO(no->esq), (*raiz)->altura) + 1;
  *raiz = no;
}

void rotacaoRR(ArvAVL *raiz) {
  struct NO *no = (*raiz)->dir;
  (*raiz)->dir = no->esq;
  no->esq = *raiz;
  (*raiz)->altura = maior(alturaNO((*raiz)->esq), alturaNO((*raiz)->dir)) + 1;
  no->altura = maior(alturaNO(no->dir), (*raiz)->altura);
  *raiz = no;
}

void rotacaoRL(ArvAVL *raiz) {
  rotacaoLL(&(*raiz)->dir);
  rotacaoRR(raiz);
}

void rotacaoLR(ArvAVL *raiz) {
  rotacaoRR(&(*raiz)->esq);
  rotacaoLL(raiz);
}

int insereAVL(ArvAVL *raiz, int valor) {
  int res;
  if (raiz == NULL) {
    return 0;
  }
  if (*raiz == NULL) {
    struct NO *novo = (struct NO *)malloc(sizeof(struct NO));
    if (novo == NULL) {
      return 0;
    }
    novo->info = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 0;
    *raiz = novo;
    return 1;
  }
  struct NO *atual = *raiz;
  if (valor < atual->info) {
    if ((res = (insereAVL(&((*raiz)->esq), valor)) == 1)) {
      if (fatorBalanceamento(atual) >= 2) {
        if (valor < atual->esq->info) {
          rotacaoLL(raiz);
        } else {
          rotacaoLR(raiz);
        }
      }
    }
  } else {
    if (valor > atual->info) {
      if ((res = (insereAVL(&((*raiz)->dir), valor)) == 1)) {
        if (fatorBalanceamento(atual) >= 2) {
          if (valor > atual->dir->info) {
            rotacaoRR(raiz);
          } else {
            rotacaoRL(raiz);
          }
        }
      }
    } else {
      printf("Valor duplicado! Inserção cancelada.");
      return 0;
    }
  }
  atual->altura = maior(atual->esq->altura, atual->dir->altura) + 1;
  return res;
}

int removeAVL(ArvAVL *raiz, int valor) {
  if (*raiz == NULL) {
    return 0;
  }
  int res;
  if (valor < (*raiz)->info) {
    if (((res = removeAVL(&(*raiz)->esq, valor)) == 1)) {
      if (fatorBalanceamento(*raiz) >= 2) {
        if (alturaNO((*raiz)->dir->esq) <= alturaNO((*raiz)->dir->dir)) {
          rotacaoRR(raiz);
        } else {
          rotacaoRL(raiz);
        }
      }
    }
  }
  if (valor > (*raiz)->info) {
    if (((res = removeAVL(&(*raiz)->dir, valor)) == 1)) {
      if (fatorBalanceamento(*raiz) >= 2) {
        if (alturaNO((*raiz)->esq->dir) <= alturaNO((*raiz)->esq->esq)) {
          rotacaoLL(raiz);
        } else {
          rotacaoLR(raiz);
        }
      }
    }
  }
  if (valor == (*raiz)->info) {
    if (((*raiz)->esq == NULL) || (*raiz)->dir == NULL) {
      struct NO *oldNode = (*raiz);
      if ((*raiz)->esq != NULL)
        *raiz = (*raiz)->esq;
      else {
        *raiz = (*raiz)->dir;
      }
      free(oldNode);
    } else {
      struct NO *temp = procuraMenor((*raiz)->dir);
      (*raiz)->info = temp->info;
      removeAVL(&(*raiz)->dir, (*raiz)->info);
      if (fatorBalanceamento(*raiz) >= 2) {
        if (alturaNO((*raiz)->esq->dir) <= alturaNO((*raiz)->esq->esq)) {
          rotacaoLL(raiz);
        } else {
          rotacaoLR(raiz);
        }
      }
    }
    if (*raiz != NULL) {
      (*raiz)->altura =
          maior(alturaNO((*raiz)->esq), alturaNO((*raiz)->dir)) + 1;
    }
    return 1;
  }
  (*raiz)->altura = maior(alturaNO((*raiz)->esq), alturaNO((*raiz)->dir)) + 1;
  return res;
}

struct NO *procuraMenor(struct NO *atual) {
  struct NO *no1 = atual;
  struct NO *no2 = atual->esq;
  while (no2 != NULL) {
    no1 = no2;
    no2 = no2->esq;
  }
  return no1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int isAVL(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 1;
  if (!isBST(raiz))
    return 0;
  if (fatorBalanceamento(*raiz) >= 3)
    return 0;
  return isAVL(&(*raiz)->esq) && isAVL(&(*raiz)->dir);
}

int isBST(ArvAVL *raiz) { 
  return isBSTAux(raiz, LONG_MIN, LONG_MAX); 
}

int isBSTAux(ArvAVL *raiz, long min, long max) {
  if (raiz == NULL)
    return 1;
  if ((*raiz)->info >= max || (*raiz)->info <= min)
    return 0;
  return isBSTAux(&(*raiz)->esq, min, (*raiz)->info) &&
         isBSTAux(&(*raiz)->dir, (*raiz)->info, max);
}

int mesmaArvore(ArvAVL *p, ArvAVL *q) {
  if (p == NULL && q == NULL)
    return 1;
  if (*p == NULL && *q == NULL)
    return 1;
  if (p == NULL || q == NULL)
    return 0;
  if ((*p)->info != (*q)->info)
    return 0;
  return mesmaArvore(&(*p)->esq, &(*q)->esq) &&
         mesmaArvore(&(*q)->dir, &(*p)->dir);
}

char tipoArvore(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 'V'; // Vazia
  if (isBST(raiz))
    return 'B'; // BST
  if (isAVL(raiz))
    return 'A'; // AVL
  return 'O';   // outro tipo
}

int checaBalanceamento(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 1;
  if (fatorBalanceamento(*raiz) >= 2)
    return 0;
  return checaBalanceamento(&(*raiz)->esq) && checaBalanceamento(&(*raiz)->dir);
}

int buscaValor(ArvAVL *raiz, int valor) {
  if (raiz == NULL || *raiz == NULL)
    return 0;
  if (valor < (*raiz)->info)
    return buscaValor(&(*raiz)->esq, valor);
  if (valor > (*raiz)->info)
    return buscaValor(&(*raiz)->dir, valor);
  return 1;
}

int numFolhas(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 0;
  int numFolhasEsq = numFolhas(&(*raiz)->esq);
  int numFolhasDir = numFolhas(&(*raiz)->dir);

  if ((*raiz)->esq == NULL && (*raiz)->dir == NULL)
    return 1;
  return numFolhasEsq + numFolhasDir;
}

int arvoreSimilar(ArvAVL *p, ArvAVL *q) {
  if (p == NULL && q == NULL)
    return 1;
  if (*p == NULL && q == NULL)
    return 1;
  if (p == NULL || q == NULL)
    return 0;
  if (*p == NULL || *q == NULL)
    return 0;
  return arvoreSimilar(&(*p)->esq, &(*q)->esq) &&
         arvoreSimilar(&(*p)->dir, &(*q)->dir);
}

int arvoreSemelhante(ArvAVL *p, ArvAVL *q) {
  if (p == NULL && q == NULL)
    return 1;
  if (*p == NULL && q == NULL)
    return 1;
  if (!buscaValor(q, (*p)->info))
    return 0;
  return arvoreSemelhante(&(*p)->esq, q) &&
         arvoreSemelhante(&(*p)->dir, q);
}

int alturaArvore(ArvAVL *raiz) {
  if (raiz == NULL)
    return -1;
  if (*raiz == NULL)
    return 0;
  return maior(alturaArvore(&(*raiz)->esq), alturaArvore(&(*raiz)->dir)) + 1;
}

int somaNos(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 0;
  return (*raiz)->info + somaNos(&(*raiz)->esq) + somaNos(&(*raiz)->dir);
}

int contaNosPositivos(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 0;
  if ((*raiz)->info >= 0)
    return 1 + contaNosPositivos(&(*raiz)->esq) +
           contaNosPositivos(&(*raiz)->dir);
  return contaNosPositivos(&(*raiz)->esq) + contaNosPositivos(&(*raiz)->dir);
}

int contaUmFilho(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 0;
  if ((*raiz)->esq == NULL && (*raiz)->dir == NULL)
    return 0;
  if ((*raiz)->esq == NULL || (*raiz)->dir == NULL)
    return contaUmFilho(&(*raiz)->esq) + contaUmFilho(&(*raiz)->dir) + 1;
  return contaUmFilho(&(*raiz)->esq) + contaUmFilho(&(*raiz)->dir);
}

void espelhaArvore(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return;
  struct NO *aux = (*raiz)->esq;
  (*raiz)->esq = (*raiz)->dir;
  (*raiz)->dir = aux;
  espelhaArvore(&(*raiz)->esq);
  espelhaArvore(&(*raiz)->dir);
}

int checaBin(ArvAVL *raiz) {
  if (raiz == NULL || *raiz == NULL)
    return 1;
  if((*raiz)-> esq == NULL && (*raiz)->dir == NULL)
    return 1;
  if ((*raiz)->esq == NULL || (*raiz)->dir == NULL)
    return 0;
  return checaBin(&(*raiz)->esq) && checaBin(&(*raiz)->dir);
}

void encontrarCaminhos(ArvAVL *raiz, int caminho[], int tamanho) {
  if (raiz == NULL || *raiz == NULL)
    return;
  caminho[tamanho] = (*raiz)->info;
  ++tamanho; // precisa ser inicializado como 0 na main
  if ((*raiz)->esq == NULL && (*raiz)->dir == NULL)
    imprimeCaminho(caminho, tamanho);
  encontrarCaminhos(&(*raiz)->esq, caminho, tamanho);
  encontrarCaminhos(&(*raiz)->dir, caminho, tamanho);
}

void imprimeCaminho(int caminho[], int tamanho) {
  printf("Caminho para a folha: ");
  for (int i = 0; i < tamanho; ++i) {
    printf("%d", caminho[i]);
    if (i < tamanho - 1)
      printf(" -> ");
  }
  printf("\n");
}

int nivelNO(ArvAVL *raiz, int valor, int nivelAtual) {
  if (raiz == NULL || *raiz == NULL)
    return -1;
  if (valor == (*raiz)->info)
    return nivelAtual;
  int nivelEsquerdo = nivelNO(&(*raiz)->esq, valor, nivelAtual + 1);
  int nivelDireito = nivelNO(&(*raiz)->dir, valor, nivelAtual + 1);
  return (nivelEsquerdo != -1) ? nivelEsquerdo : nivelDireito;
}
