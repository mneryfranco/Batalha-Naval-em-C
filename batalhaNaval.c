// Desafio Batalha Naval - MateCheck
#include <stdio.h>
#include <stdlib.h>

#define maxLinha 10 // quantidade de linhas do tabuleiro
#define maxColuna 10 // quantidade de colunas do tabuleiro
int tabNavios[maxLinha][maxColuna]; // guarda a informação de onde estão os navios | 0 - não tem | 1 - escondido | 2 - encontrado
char tabuleiro[maxLinha][maxColuna]; // matriz do tabuleiro que será mostrada na tela
int countNavio = 0;

void printTabuleiro (){
    for(int linha=-1; linha<maxLinha; linha++){
        for (int coluna=-1; coluna<maxColuna; coluna++){
            if(linha == -1){
                if(coluna==-1) printf("\t");
                else printf("\033[31m%d\033[0m\t", coluna);
            }
            else{
                if(coluna == -1) printf("\033[31m%d\033[0m\t", linha);
                else printf("%c\t", tabuleiro[linha][coluna]);
            }
            
        }
        printf("\n");
    }
    printf("Navios no jogo: %d\n\n", countNavio);
}

// Retorna o menor indice (linha ou coluna)
int menorIndice(){
    if(maxLinha < maxColuna)
        return maxLinha; 
    else return maxColuna;
}

// Retorna a capacidade máxima de navios do jogo, sendo 9 o máximo permitido
int qtdMaximaNavios(){
    menorIndice() > 9 ? 9 : menorIndice();
}

int tamanhoMaxNavio(){
    return menorIndice()/2;
}

void inicializarTabuleiro (){
    countNavio=0;
    for(int i=0; i<maxLinha; i++){
        for (int j=0; j<maxLinha; j++){
            tabuleiro[i][j] = '~';
        }
    }
}

int buscaNavio(int tamNavio, int posX, int posY, int direcao){
    int coluna = posX, linha = posY, i=0;
    while (i < tamNavio){
        if(tabuleiro[linha][coluna] != '~'){
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

    //Tratamento de erros
    //Se o usuário selecionar uma posição fora do tabuleiro, ou valores negativos, encerra.
    if(posY<0 || posY>=maxLinha){
        printf("Erro: Linha inexistente no tabuleiro\n");
        return 1;
    }
    if(posX<0 || posX>=maxColuna){
        printf("Erro: Coluna inexistente no tabuleiro\n");
        return 1;
    }  

    //Encerra pra valores invalidos no tamanho do navio
    if(tamNavio > tamanhoMaxNavio()) {
        printf("Erro: Navio muito grande. O tamanho maior é %d\n", tamanhoMaxNavio());
        return 1;
    }

    //Encerra pra valores inválidos de direcao do navio
    if(direcao <= 0 || direcao >= 4){
        printf("Erro: Valor inválido para direcao do navio\n");
        return 1;
    }

    //Se o tamanho do barco exceder o tamanho do tabuleiro, encerra.
    if(direcao==1 && posY+tamNavio-1 > maxLinha) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==2 && (posX+tamNavio-1 >= maxColuna || posY+tamNavio-1 >= maxLinha)) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==3 && posX+tamNavio-1 > maxColuna) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 

    //Verifica se já existe um navio na posição
    if(buscaNavio(tamNavio, posX, posY, direcao)){
        printf("Erro ao tentar adicionar navio\n");
        return 1;
    }
    return 0;

}

int addNavio(int tamNavio, int posX, int posY, int direcao){
    /*  
    tamNavio: Tamanho do navio a ser adicionado no tabuleiro
    posX: posição X inicial do navio
    poxY: posição Y inicial do navio
    Direção: 1 = Vertical | 2 = Diagonal | 3 = Horizontal 
    */
   
    //Verifica se o tabuleiro já está em sua capacidade máxima de navios
    if(countNavio == qtdMaximaNavios()){
        printf("Tabuleiro lotado (%d navios)\n", countNavio);
        return 0;
    }

    //Verifica se existe algum problema ao adicionar o navio
    if(checkErrosAddNavio(tamNavio, posX, posY, direcao)) 
        return 0;

    //Adiciona o navio no tabuleiro
    int linha = posY, coluna = posX, i=0;
    while (i < tamNavio){
        tabuleiro[linha][coluna] = countNavio + '1';
        if (direcao == 1) linha++;
        if (direcao == 2){
            linha++;
            coluna++;
        }
        if (direcao == 3) coluna++;
        i++;
    }
    countNavio++;
    printTabuleiro();
    return 1;
}

int addNavioRandom (int qtdNavios){
    int posX, posY, direcao, tamanho;

    if(qtdNavios > menorIndice()) qtdNavios = menorIndice();

    do{
        //Verifica se o tabuleiro já está em sua capacidade máxima de navios
        if(countNavio == qtdMaximaNavios()){
            printf("Tabuleiro lotado (%d navios)\n", countNavio);
            return 0;
            break;
        }
        posX = rand() % (maxColuna-1);
        posY = rand() % (maxLinha-1);
        direcao = (rand() % 3) + 1;
        tamanho = rand() % (menorIndice() / 2);
        if(addNavio(tamanho, posX, posY, direcao))
            qtdNavios--;
    } while(qtdNavios > 0);
    return 1;
}

void inicializarMenu(){
    int selectMenu=0;
    int tamNavio=0, posX=0, posY=0, direcao=0;
    int qtdNavios=0;

    do{
        printTabuleiro();
        printf("MENU | BATALHA NAVAL:\n");
        printf("1 - Adicionar um navio\n");
        printf("2 - Adicionar navios aleatoriamente\n");
        printf("8 - Limpar terminal\n");
        printf("9 - Reset Tabuleiro\n");
        printf("0 - Sair\n");
        scanf("%d", &selectMenu);
        
        switch (selectMenu){
        case 1:
            printf("Digite as coordenadas iniciais de onde quer adicionar o navio:\n");
            printf("Linha: ");
            scanf("%d", &posY);
            printf("Coluna: ");
            scanf("%d", &posX);
            printf("Tamanho do Navio (máximo %d): ", tamanhoMaxNavio());
            scanf("%d", &tamNavio);
            printf("\nDirecao do Navio: \n1 - Vertical \n2 - Diagonal \n3 - Horizontal\n");
            scanf("%d", &direcao);
            printf("\n");

            if(addNavio(tamNavio, posX, posY, direcao)){
                printf("Navio adicionado\n");
            }
            break;
        
        case 2:
            printf("Digite quantos navios quer adicionar (máximo %d): ", menorIndice());
            scanf("%d", &qtdNavios);
            addNavioRandom(qtdNavios);
            break;
        case 8:
            system("clear");
            break;
        case 9:
            inicializarTabuleiro();
            break;
        
        case 0: 
            break;
        
        default:
            printf("Opcao invalida\n\n");
            break;
        }

    } while (selectMenu != 0);
    
}

int main() {

    inicializarTabuleiro();
    inicializarMenu();
    

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
