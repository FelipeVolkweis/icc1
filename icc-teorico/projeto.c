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
#define SALDO_INICIAL 100

typedef struct {
    int codigo;
    char *nome;
    double preco;
    int quantidade;
} produto_t;

typedef struct {
    produto_t *produtos;
    int tamanho;
    double saldo;
} inventario_t;

typedef struct {
    produto_t *produtos;
    double total;
    int tamanho;
} venda_t;

typedef struct {
    int tamanho;
    int *codigos;
    int quantidade_venda;
} codigos_venda_t;

void insere_produto(char *nome, int quantidade, double preco, inventario_t *inventario);
void aumenta_estoque(int codigo, int quantidade, inventario_t *inventario);
void modifica_preco(int codigo, double preco, inventario_t *inventario);
codigos_venda_t scan_codigos_venda();
venda_t venda(int *codigos, int tamanho_codigos, int quantidade_vendidos, inventario_t *inventario);
void mostra_venda(venda_t venda);
void consulta_estoque(inventario_t inventario);
void consulta_saldo(inventario_t inventario);
int finaliza_execucao();
int executa_comando(char *comando, inventario_t *inventario);
int compara_comando(char *input, char *comando);
char *scan_nome(char *nome);
void free_inventario(inventario_t *inventario);

void insere_produto(char *nome, int quantidade, double preco, inventario_t *inventario) {
    produto_t produto;
    produto.codigo = inventario->tamanho;
    produto.nome = nome;
    produto.preco = preco;
    produto.quantidade = quantidade;

    inventario->tamanho++;

    inventario->produtos = realloc(inventario->produtos, inventario->tamanho * sizeof(produto_t)); 
    inventario->produtos[inventario->tamanho - 1] = produto;
}

void aumenta_estoque(int codigo, int quantidade, inventario_t *inventario) {
    inventario->produtos[codigo].quantidade += quantidade;
}

void modifica_preco(int codigo, double preco, inventario_t *inventario) {
    inventario->produtos[codigo].preco = preco;
}

codigos_venda_t scan_codigos_venda() {
    int *codigos = NULL;
    int codigo;
    int tamanho = 0;
    codigos_venda_t codigos_venda;

    scanf("%d", &codigo);
    codigos = malloc(sizeof(int));
    codigos[tamanho] = codigo;
    tamanho++;
    while(1) {
        scanf("%d", &codigo);
        if(codigo < 0)
            break;
        
        codigos = realloc(codigos, (tamanho + 1) * sizeof(int));
        codigos[tamanho] = codigo;
        tamanho++;
    }
    codigos_venda.tamanho = tamanho; 
    codigos_venda.codigos = codigos; 
    codigos_venda.quantidade_venda = codigo;

    return codigos_venda;
}

venda_t venda(int *codigos, int tamanho_codigos, int quantidade_vendidos, inventario_t *inventario) {
    produto_t *produtos = NULL;
    venda_t venda;
    venda.total = 0;
    venda.tamanho = tamanho_codigos;

    for(int i = 0; i < tamanho_codigos; i++) {
        int index = codigos[i];
        produto_t *produto_atual = &inventario->produtos[index];
        produto_atual->quantidade += quantidade_vendidos;

        produto_t produto;
        produto.codigo = produto_atual->codigo;
        produto.nome = produto_atual->nome;
        produto.preco = produto_atual->preco;
        produto.quantidade = quantidade_vendidos * -1; 

        produtos = realloc(produtos, (i + 1) * sizeof(produto_t));
        produtos[i] = produto;

        venda.total += produto.quantidade * produto.preco;
    }
    venda.produtos = produtos;

    inventario->saldo += venda.total;

    return venda;
}

void mostra_venda(venda_t venda) {
    for(int i = 0; i < venda.tamanho; i++) {
        printf("%s %.2lf\n", venda.produtos[i].nome, venda.produtos[i].preco);
    }
    printf("Total: %.2lf\n", venda.total);
    printf("%s\n", LINHA);
}

void consulta_estoque(inventario_t inventario) {
    for(int i = 0; i < inventario.tamanho; i++) {
        printf("%d %s %d\n", 
            inventario.produtos[i].codigo, 
            inventario.produtos[i].nome,
            inventario.produtos[i].quantidade);
    }
    printf("%s\n", LINHA);
}

void consulta_saldo(inventario_t inventario) {
    printf("Saldo: %.2lf\n", inventario.saldo);
    printf("%s\n", LINHA);
}

int finaliza_execucao() {
    return 0;
}

int compara_comando(char *input, char *comando) {
    return input[0] == comando[0] && input[1] == comando[1];
}

char *scan_nome(char *nome) {
    char c;
    int tamanho = 0;

    while(((c = getchar()) != '\0') && c != ' ' && c != '\n' && c != EOF) {
        tamanho++;
        nome = realloc(nome, tamanho * sizeof(char));
        nome[tamanho - 1] = c;
    }
    nome = realloc(nome, (tamanho + 1) * sizeof(char));

    nome[tamanho] = '\0';
    //printf("comando: %s ( %d, %d, %d )[%d]\n", nome, nome[0], nome[1], nome[2], tamanho);

    return nome;
}

void free_inventario(inventario_t *inventario) {
    for(int i = 0; i < inventario->tamanho; i++) {
        free(inventario->produtos[i].nome);
        inventario->produtos[i].nome = NULL;
    }
    
    free(inventario->produtos);
    inventario->produtos = NULL;
}

int executa_comando(char *comando, inventario_t *inventario) {    
    if(compara_comando(comando, "IP")) {
        char *nome = NULL;
        int quantidade;
        double preco;

        nome = scan_nome(nome);
        scanf("%d %lf ", &quantidade, &preco);

        insere_produto(nome, quantidade, preco, inventario);

    } else if(compara_comando(comando, "AE")) {
        int codigo;
        int quantidade;
        scanf("%d %d ", &codigo, &quantidade);

        aumenta_estoque(codigo, quantidade, inventario);

    } else if(compara_comando(comando, "MP")) {
        int codigo;
        double preco;
        scanf("%d %lf ", &codigo, &preco);

        modifica_preco(codigo, preco, inventario);

    } else if(compara_comando(comando, "VE")) {
        codigos_venda_t codigos_venda = scan_codigos_venda();
        venda_t venda_ = venda(codigos_venda.codigos, codigos_venda.tamanho, codigos_venda.quantidade_venda, inventario);
        mostra_venda(venda_);
        
        free(codigos_venda.codigos);
        codigos_venda.codigos = NULL;

        free(venda_.produtos);
        venda_.produtos = NULL;

    } else if(compara_comando(comando, "CE")) {
        consulta_estoque(*inventario);

    } else if(compara_comando(comando, "CS")) {
        consulta_saldo(*inventario);

    } else if(compara_comando(comando, "FE")) {
        return finaliza_execucao();
    }
    return 1;
}

int main() {
    char *comando = NULL;
    int loop = 1;

    inventario_t inventario;
    inventario.produtos = NULL;
    inventario.saldo = SALDO_INICIAL;
    inventario.tamanho = 0;

    while(loop) {
        comando = scan_nome(comando);
        loop = executa_comando(comando, &inventario);

        free(comando);
        comando = NULL;
    }

    free_inventario(&inventario);

    return 0;
}