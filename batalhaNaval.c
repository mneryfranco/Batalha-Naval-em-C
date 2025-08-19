// Desafio Batalha Naval - MateCheck
// Este código inicial serve como base para o desenvolvimento do sistema de Batalha Naval.
// Siga os comentários para implementar cada parte do desafio.

#include <stdio.h>
#define maxLinha 10 // quantidade de linhas do tabuleiro
#define maxColuna 10 // quantidade de colunas do tabuleiro
int tabuleiro[maxLinha][maxColuna];

void inicializarTabuleiro (){
    for(int i=0; i<maxLinha; i++){
        for (int j=0; j<maxLinha; j++){
            tabuleiro[i][j] = 0;
        }
    }
    return tabuleiro;
}

void printTabuleiro (){

    for(int linha=-1; linha<maxLinha; linha++){
        for (int coluna=-1; coluna<maxColuna; coluna++){
            if(linha == -1){
                if(coluna==-1) printf("\t");
                else printf("\033[31m%d\033[0m\t", coluna);
            }
            else{
                if(coluna == -1) printf("\033[31m%d\033[0m\t", linha);
                else printf("%d\t", tabuleiro[linha][coluna]);
            }
            
        }
        printf("\n");
    }
    printf("\n");
}

int buscaNavio(int tamNavio, int posX, int posY, int direcao){
    int linha = posX, coluna = posY, i=0;
    while (i < tamNavio){
        if(tabuleiro[linha][coluna] == 1){
            printf("Navio encontrado em (%d, %d)\n", linha, coluna);
            return 1;
        }
        if (direcao == 1) linha++;
        if (direcao == 2){
            linha++;
            coluna++;
        }
        if (direcao == 3) coluna++;
        i++;
    }
    return 0;
}

void addNavio(int tamNavio, int posX, int posY, int direcao){
    /*  
    tamNavio: Tamanho do navio a ser adicionado no tabuleiro
    posX: posição X inicial do navio
    poxY: posição Y inicial do navio
    Direção: 1 = Vertical | 2 = Diagonal | 3 = Horizontal 
    */


    //Tratamento de erros
    //Se o usuário selecionar uma posição fora do tabuleiro, ou valores negativos, encerra.
    if(posX<0 || posY<0 || posX>=maxColuna || posY>=maxLinha){
        printf("Posicao inexistente no tabuleiro\n");
        return;
    } 
    //Encerra pra valores invalidos no tamanho do navio
    if(tamNavio <= 0){
        printf("Tamanho do navio inválido\n");
        return;
    }
    //Se o tamanho do barco exceder o tamanho do tabuleiro, encerra.
    if(direcao==1 && posY+tamNavio-1 > maxLinha) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return;
    } 
    if(direcao==2 && (posX+tamNavio-1 >= maxColuna || posY+tamNavio-1 >= maxLinha)) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return;
    } 
    if(direcao==3 && posX+tamNavio-1 > maxColuna) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return;
    } 
    //Verifica se já existe um navio na posição
    if(buscaNavio(tamNavio, posX, posY, direcao)){
        return;
    }


    //Adiciona o navio no tabuleiro
    int linha = posX, coluna = posY, i=0;
    while (i < tamNavio){
        tabuleiro[linha][coluna] = 1;
        if (direcao == 1) linha++;
        if (direcao == 2){
            linha++;
            coluna++;
        }
        if (direcao == 3) coluna++;
        i++;
    }
}

int main() {

    inicializarTabuleiro();
    addNavio(5, 1, 1, 1); // Add Navio tam=5, linha=1, coluna=1, Vertical
    addNavio(4, 6, 6, 2); // Add Navio tam=4, linha=6, coluna=6, Diagonal
    addNavio(3, 2, 5, 3); // Add Navio tam=3, linha=2, coluna=5, Horizontal
    addNavio(3, 2, 5, 3); // Add Navio tam=3, linha=2, coluna=3, Horizontal
    printTabuleiro();
    
    
    // Sugestão: Posicione dois navios no tabuleiro, um verticalmente e outro horizontalmente.
    // Sugestão: Utilize `printf` para exibir as coordenadas de cada parte dos navios.

    // Nível Aventureiro - Expansão do Tabuleiro e Posicionamento Diagonal
    // Sugestão: Expanda o tabuleiro para uma matriz 10x10.
    // Sugestão: Posicione quatro navios no tabuleiro, incluindo dois na diagonal.
    // Sugestão: Exiba o tabuleiro completo no console, mostrando 0 para posições vazias e 3 para posições ocupadas.

    // Nível Mestre - Habilidades Especiais com Matrizes
    // Sugestão: Crie matrizes para representar habilidades especiais como cone, cruz, e octaedro.
    // Sugestão: Utilize estruturas de repetição aninhadas para preencher as áreas afetadas por essas habilidades no tabuleiro.
    // Sugestão: Exiba o tabuleiro com as áreas afetadas, utilizando 0 para áreas não afetadas e 1 para áreas atingidas.

    // Exemplos de exibição das habilidades:
    // Exemplo para habilidade em cone:
    // 0 0 1 0 0
    // 0 1 1 1 0
    // 1 1 1 1 1
    
    // Exemplo para habilidade em octaedro:
    // 0 0 1 0 0
    // 0 1 1 1 0
    // 0 0 1 0 0

    // Exemplo para habilidade em cruz:
    // 0 0 1 0 0
    // 1 1 1 1 1
    // 0 0 1 0 0

    return 0;
}
