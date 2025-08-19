// Desafio Batalha Naval - MateCheck

#include <stdio.h>
#define maxLinha 10 // quantidade de linhas do tabuleiro
#define maxColuna 10 // quantidade de colunas do tabuleiro
#define qtdTiposNavios 3 // quantidade de navios diferentes no jogo
int navio[qtdTiposNavios] = {1, 3, 5}; // vetor de navios, onde cada numero representa seu tamanho
int tabuleiro[maxLinha][maxColuna];

void inicializarTabuleiro (){
    for(int i=0; i<maxLinha; i++){
        for (int j=0; j<maxLinha; j++){
            tabuleiro[i][j] = 0;
        }
    }
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

int checkErrosAddNavio(int tamNavio, int posX, int posY, int direcao){
    /*  
    tamNavio: Tamanho do navio a ser adicionado no tabuleiro
    posX: posição X inicial do navio
    poxY: posição Y inicial do navio
    Direção: 1 = Vertical | 2 = Diagonal | 3 = Horizontal 
    */
    int temErro = 0;

    //Tratamento de erros
    //Se o usuário selecionar uma posição fora do tabuleiro, ou valores negativos, encerra.
    if(posY<0 || posY>=maxLinha){
        printf("Erro: Linha inexistente no tabuleiro\n");
        temErro = 1;
    }
    if(posX<0 || posX>=maxColuna){
        printf("Erro: Coluna inexistente no tabuleiro\n");
        temErro = 1;
    }  

    //Encerra pra valores invalidos no tamanho do navio
    for(int i=0; i<qtdTiposNavios; i++){
        if(navio[i] == tamNavio){
            break;
        }
        if(i == qtdTiposNavios-1) {
            temErro = 1;
            printf("Erro: Tamanho do navio inválido\n");
        }
    }

    //Encerra pra valores inválidos de direcao do navio
    if(direcao <= 0 || direcao >= 4){
        printf("Erro: Valor inválido para direcao do navio\n");
        temErro = 1;
    }

    //Se o tamanho do barco exceder o tamanho do tabuleiro, encerra.
    if(direcao==1 && posY+tamNavio-1 > maxLinha) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        temErro = 1;
    } 
    if(direcao==2 && (posX+tamNavio-1 >= maxColuna || posY+tamNavio-1 >= maxLinha)) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        temErro = 1;
    } 
    if(direcao==3 && posX+tamNavio-1 > maxColuna) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        temErro = 1;
    } 

    //Verifica se já existe um navio na posição
    if(buscaNavio(tamNavio, posX, posY, direcao)){
        temErro = 1;
    }
    return temErro;

}

int addNavio(int tamNavio, int posX, int posY, int direcao){
    
    //Verifica se existe algum problema ao adicionar o navio
    if(checkErrosAddNavio(tamNavio, posX, posY, direcao)) 
        return 0;

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
    return 1;
}

void inicializarMenu(){
    int selectMenu=0;
    int tamNavio=0, posX=0, posY=0, direcao=0;
    do{
        printf("\nMENU | BATALHA NAVAL:\n");
        printf("1 - Ver Tabuleiro\n");
        printf("2 - Adicionar navio\n");
        printf("0 - Sair\n");
        scanf("%d", &selectMenu);
        
        switch (selectMenu){
        case 1: 
            printTabuleiro();
            break;
        
            case 2:
            printTabuleiro();
            printf("Digite as coordenadas iniciais de onde quer adicionar o navio:\n");
            printf("Linha: ");
            scanf("%d", &posY);
            printf("Coluna: ");
            scanf("%d", &posX);
            printf("Tamanho do Navio | Escolha entre ");
            for(int i=0; i<qtdTiposNavios; i++) printf("%d ", navio[i]);
            printf(": ");
            scanf("%d", &tamNavio);
            printf("\nDirecao do Navio: \n1 - Vertical \n2 - Diagonal \n3 - Horizontal\n");
            scanf("%d", &direcao);
            printf("\n");

            if(addNavio(tamNavio, posX, posY, direcao))
                printf("Navio adicionado\n");

            break;
        
        case 0: break;
        
        default:
            printf("Opcao invalida\n\n");
            break;
        }

    } while (selectMenu != 0);
    
}

int main() {

    inicializarTabuleiro();
    inicializarMenu();
    //addNavio(5, 1, 1, 1); // Add Navio tam=5, linha=1, coluna=1, Vertical
    //addNavio(4, 6, 6, 2); // Add Navio tam=4, linha=6, coluna=6, Diagonal
    //addNavio(3, 2, 5, 3); // Add Navio tam=3, linha=2, coluna=5, Horizontal
    //addNavio(3, 2, 5, 3); // Add Navio tam=3, linha=2, coluna=3, Horizontal
    //printTabuleiro();

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
