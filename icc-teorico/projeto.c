/*
Insere produto
IP_<nome>_<quantidade>_<preço>

Aumenta estoque
AE_<código>_<quantidade>

Modifica preço
MP_<código>_<preço>

Venda
VE_<código>_<código>_..._<código>_<-1>

Consulta estoque
CE

Consulta saldo
CS

Finaliza a execução
FE
*/

#include <stdio.h>
#include <stdlib.h>

#define LINHA "--------------------------------------------------"

typedef struct {
    int codigo;
    char *nome;
    double preco;
    int quantidade;
} produto_t;

typedef struct {
    produto_t *produtos;
    int numero;
} inventario_t;

typedef struct {
    produto_t *produtos;
    int total;
} venda_t;

inventario_t *insere_produto(char *nome, int quantidade, double preco, inventario_t *inventario);
inventario_t *aumenta_estoque(int codigo, int quantidade, inventario_t *inventario);
int modifica_preco(int codigo, double preco, inventario_t *inventario);
venda_t *venda(int *codigos, int numero_vendidos, inventario_t *inventario);
int consulta_estoque(inventario_t *inventario);
int colsulta_saldo(inventario_t *inventario);
int finaliza_execucao(inventario_t *inventario);

inventario_t *insere_produto(char *nome, int quantidade, double preco, inventario_t *inventario) {
    produto_t produto;
    produto.codigo = inventario->numero;
    produto.nome = nome;
    produto.preco = preco;
    produto.quantidade = quantidade;

    inventario->numero++;

    inventario->produtos = realloc(inventario->produtos, inventario->numero * sizeof(produto_t)); 
    inventario->produtos[inventario->numero - 1] = produto;

    return inventario;
}

int main() {
    produto_t *produtos = NULL;
    
    return 0;
}