// Desafio Batalha Naval - MateCheck
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RED "\033[31m"
#define RESETCOLOR "\033[0m"

int tamLinha = 10, tamColuna = 10; // quantidade de linhas e colunas do tabuleiro
char tabuleiro[14][14]; // matriz do tabuleiro que será mostrada na tela
int tabNavios[14][14]; // guarda a informação de onde estão os navios
int configNumNavios = 6; // quantos navios vão ser adicionados no jogo
int qtdNaviosAtivos = 0; // quantos navios estão ativos em jogo, ou seja, ainda não foram destruidos
int numTiros = 30;
int numPowers = 4;

// Retorna o menor indice (linha ou coluna)
int menorIndice(){
    if(tamLinha < tamColuna)
        return tamLinha; 
    else return tamColuna;
}

// Retorna o maior indice (linha ou coluna)
int maiorIndice(){
    if(tamLinha > tamColuna)
        return tamLinha; 
    else return tamColuna;
}

// Retorna a capacidade máxima de navios do jogo, sendo 9 o máximo permitido
int qtdMaximaNavios(){
    menorIndice() > 9 ? 9 : menorIndice();
}

// Retornar o maior tamanho possível de cada navio
int tamanhoMaxNavio(){
    return menorIndice()/2;
}

// Reseta o tabuleiro e a contagem de tiros e poderes
void inicializarTabuleiro (){
    //qtdNaviosAtivos = 0;
    //numTiros = (tamLinha*tamColuna)/3;
    //numPowers = (tamLinha*tamColuna)/(maiorIndice()*3);

    for(int i=0; i<tamLinha; i++){
        for (int j=0; j<tamLinha; j++){
            tabuleiro[i][j] = '~';
            tabNavios[i][j] = '~';
        }
    }
}

// Imprime o tabuleiro do jogo no terminal
void printTabuleiro (){
    printf("\n\n");
    for(int linha=-1; linha<tamLinha; linha++){
        for (int coluna=-1; coluna<tamColuna; coluna++){
            if(linha == -1){
                if(coluna==-1) printf("   ");
                else printf(RED"%d  "RESETCOLOR, coluna);
            }
            else{
                if(coluna == -1) printf(RED"%d  "RESETCOLOR  , linha);
                else printf("%c  ", tabuleiro[linha][coluna]);
            }
            
        }
        printf("\n");
    }
    printf("Tiros: %d \tNavios: %d\tEspeciais: %d\n\n", numTiros, qtdNaviosAtivos, numPowers);
}

// Imprime o tabuleiro de navios escondidos
void printTabNavios (){
    printf("\n\n");
    for(int linha=-1; linha<tamLinha; linha++){
        for (int coluna=-1; coluna<tamColuna; coluna++){
            if(linha == -1){
                if(coluna==-1) printf("   ");
                else printf(RED"%d  "RESETCOLOR, coluna);
            }
            else{
                if(coluna == -1) printf(RED"%d  "RESETCOLOR  , linha);
                else printf("%c  ", tabNavios[linha][coluna]);
            }
            
        }
        printf("\n");
    }
    printf("Tiros: %d \tNavios: %d\tEspeciais: %d\n\n", numTiros, qtdNaviosAtivos, numPowers);
}

// Procura um navio na posição informada e retorna 1 se tiver
int buscaNavio(int tamNavio, int X, int Y, int direcao){
    int coluna = X, linha = Y, i=0;
    while (i < tamNavio){
        if(tabNavios[linha][coluna] != '~'){
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

// Revela a posição exata do navio quando ele é destruído
void revelaNavioDestruido(int Y, int X){
    char target = tabNavios[Y][X];

    //Procura o ID do navio nas redondezas
    for (int l = Y-1 ; l <= Y+1 ; l++){
        for (int c = X-1 ; c <= X+1 ; c++){
            if(l >= 0 && l < tamLinha && c >= 0 && c < tamColuna){ //limita a checagem só para posições validas (dentro do tabuleiro)
                if(tabNavios[l][c] == target && tabuleiro[l][c] != target){
                    tabuleiro[l][c] = target;
                    revelaNavioDestruido(l, c);
                }
            }
        }
    }
}

// Faz vários testes pra ver se é possível adicionar um navio com os dados fornecidos
int checkErrosAddNavio(int tamNavio, int X, int Y, int direcao){
    /*  
    tamNavio: Tamanho do navio a ser adicionado no tabuleiro
    X: posição X inicial do navio
    poxY: posição Y inicial do navio
    Direção: 1 = Vertical | 2 = Diagonal | 3 = Horizontal 
    */

    //Tratamento de erros
    //Se o usuário selecionar uma posição fora do tabuleiro, ou valores negativos, encerra.
    if(Y<0 || Y>=tamLinha){
        printf("Erro: Linha inexistente no tabuleiro\n");
        return 1;
    }
    if(X<0 || X>=tamColuna){
        printf("Erro: Coluna inexistente no tabuleiro\n");
        return 1;
    }  

    //Encerra pra valores invalidos no tamanho do navio
    if(tamNavio > tamanhoMaxNavio()) {
        printf("Erro: Navio muito grande. O tamanho maior é %d\n", tamanhoMaxNavio());
        return 1;
    }
    if(tamNavio <= 0) {
        printf("Erro: Tamanho inválido para o navio");
        return 1;
    }

    //Encerra pra valores inválidos de direcao do navio
    if(direcao <= 0 || direcao >= 4){
        printf("Erro: Valor inválido para direcao do navio\n");
        return 1;
    }

    //Se o tamanho do barco exceder o tamanho do tabuleiro, encerra.
    if(direcao==1 && Y+tamNavio-1 >= tamLinha) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==2 && (X+tamNavio-1 >= tamColuna || Y+tamNavio-1 >= tamLinha)) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==3 && X+tamNavio-1 >= tamColuna) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 

    //Verifica se já existe um navio na posição
    if(buscaNavio(tamNavio, X, Y, direcao)){
        printf("Erro ao tentar adicionar navio\n");
        return 1;
    }
    return 0;

}

// Adiciona um navio 
int addNavio(int tamNavio, int X, int Y, int direcao){
    /*  
    tamNavio: Tamanho do navio a ser adicionado no tabuleiro
    X: posição X inicial do navio
    poxY: posição Y inicial do navio
    Direção: 1 = Vertical | 2 = Diagonal | 3 = Horizontal 
    */
   
    //Verifica se o tabuleiro já está em sua capacidade máxima de navios
    if(qtdNaviosAtivos >= configNumNavios){
        printf("Tabuleiro lotado (%d navios)\n", qtdNaviosAtivos);
        return 0;
    }

    //Verifica se existe algum problema ao adicionar o navio
    if(checkErrosAddNavio(tamNavio, X, Y, direcao)){
        return 0;
    }

    //Adiciona o navio no tabuleiro
    int linha = Y, coluna = X, i=0;
    while (i < tamNavio){
        tabNavios[linha][coluna] = qtdNaviosAtivos + '0';
        if (direcao == 1) linha++;
        if (direcao == 2){
            linha++;
            coluna++;
        }
        if (direcao == 3) coluna++;
        i++;
    }
    qtdNaviosAtivos++;
    printTabuleiro();
    return 1;
}

// Adiciona uma quantidade específica de navios aleatoriamente pelo tabuleiro
int addNavioRandom (int qtdNavios){
    int X, Y, direcao, tamanho;

    if(qtdNavios > configNumNavios) qtdNavios = configNumNavios;
    
    srand(time(NULL)); // inicializa a semente com o tempo atual
    do{
        //Verifica se o tabuleiro já está em sua capacidade máxima de navios
        if(qtdNaviosAtivos >= configNumNavios){
            printf("Tabuleiro lotado (%d navios)\n", qtdNaviosAtivos);
            return 0;
            break;
        }
        X = rand() % (tamColuna-1);
        Y = rand() % (tamLinha-1);
        direcao = (rand() % 3) + 1;
        tamanho = (configNumNavios - qtdNaviosAtivos) / 2;
        if(tamanho < 2) tamanho = 2; // garante que o tamanho mínimo do navio seja 2

        if(addNavio(tamanho, X, Y, direcao))
            qtdNavios--;
    } while(qtdNavios > 0);
    return 1;
}

// Ataca uma posição do tabuleiro. Retorna 1 se atingir um navio.
int attack(int linha, int coluna){
    char target = tabNavios[linha][coluna]; //armazena o char da posição que foi atacado
    int hit = 0; // se acertou um navio se torna 1
    int checkNavioDestruido = 1; //será usada para checar se existe em torno desta posição outro char = target, ou seja, outra parte do navio

    if(linha < 0 || coluna < 0 || linha >= tamLinha || coluna >= tamColuna){
        printf("Fora do tabuleiro!\n");
        return 0;
    }

    //Verifica se acertou um navio
    if(target >= '0' && target <= '0' + configNumNavios){
        printf("Navio atingido!\n");
        hit = 1;
        tabuleiro[linha][coluna] = 'X';

        //Verifica se o navio foi destruído
        for(int l = linha-1 ; l <= linha+1 ; l++){
            for(int c = coluna-1 ; c <= coluna+1 ; c++){
                if(l >= 0 && l < tamLinha && c >= 0 && c < tamColuna){ //limita a checagem só para posições validas (dentro do tabuleiro)
                    if(tabNavios[l][c] == target && tabuleiro[l][c] != 'X') {
                        checkNavioDestruido = 0;
                        break;
                    }
                }
            }
            if(checkNavioDestruido==0) break;
        }
        if(checkNavioDestruido == 1) {
            printf("NAVIO DESTRUIDO!\n");
            qtdNaviosAtivos--;
            revelaNavioDestruido(linha, coluna);
        }
    
    } else{
        tabuleiro[linha][coluna] = '*';
    }
    
    return hit;
}

// Função de ataque com a bomba. Retorna 1 se tiver sucesso
int attackBomba(int linha, int coluna){
    numTiros--;
    if(attack(linha,coluna)){
        return 1;
    }
    else {
        printf("Tiro na água!\n");
        return 0;
    }
}

// Usada pelas funções de super poder para atacar
int attackPoder(int linha, int coluna, int tam){    
    if(attack(linha,coluna)){
        return 1;
    }
    else return 0;
}

int initAttackSuper(int superpoder, int X, int Y, int tam){
    if(numPowers < tam){
        printf("Você não tem poder especial suficiente\n");
        return 0;
    }
    if(superpoder == 1) usePoderCone(X, Y, tam);
    if(superpoder == 2) usePoderOcta(X, Y, tam);
    if(superpoder == 3) usePoderCruz(X, Y, tam);
    
    numPowers = numPowers - tam;
    return 1;
}

// Executa um super poder no tabuleiro em forma de cone
int usePoderCone(int X, int Y, int tam){
    int largura = 0, altura = 0;
    int linha = Y-tam;
    int coluna = X;

    while(altura <= tam+1){
        for(int c = -largura ; c <= largura ; c++){
            attackPoder(linha, coluna+c, tam);
        }
        linha++;
        altura++;
        largura++;
    }
}

// Executa um super poder no tabuleiro em forma de octaedro
int usePoderOcta(int X, int Y, int tam){
    int largura;
    int linha = Y-tam;
    int coluna = X;

    while(linha <= Y+tam){
        largura = tam - abs(Y-linha);
        for(int c = -largura ; c <= largura ; c++){
            attackPoder(linha, coluna+c, tam);
        }
        linha++;
    }
}

// Executa um super poder no tabuleiro em forma de cruz
int usePoderCruz(int X, int Y, int tam){
    tam++; // só pra deixar a cruz maior mesmo e ser mais interessante por jogardor usar
    int linha = Y-tam;

    for(int linha = Y-tam; linha <= Y+tam; linha++){
        if(linha == Y){
            for(int c = -tam ; c <= tam ; c++){
                attackPoder(linha, X+c, tam);
            }
        }
        else attackPoder(linha, X, tam);
    }
}

//Menu inicial 
void menuPrincipal(){
    int selectMenu=0;
    
    do{
        system("clear");

        printf(
            "================================\n"
            RED"\tBATALHA NAVAL\n"RESETCOLOR
            "================================\n\n"
            "Este é um jogo Single Player.\n"
            "Seu objetivo é destruir todos os navios no tabuleiro.\n"
            "Os navios podem estar posicinados de forma horizontal, vertical ou diagonal, e possuem tamanho de 2 a 5.\n"
            "Descubra a posição dos navios e use seus tiros e poderes para destrui-los! \n\n"
            
            RED"MENU PRINCIPAL:\n"RESETCOLOR
            "1 - Iniciar Jogo\n"
            "0 - Finalizar\n");

        scanf("%d", &selectMenu);
        printf("\n");

        switch (selectMenu){
            case 1:
                inicializarTabuleiro();
                addNavioRandom(configNumNavios);
                menuJogo();
                break;
            
            case 0:
                break;
            
            default:
                printf("Opção Inválida\n");
        }
        sleep(1);
    } while(selectMenu < 0 || selectMenu > 1);

}

//Menu com o jogo ativo
void menuJogo(){
    int selectMenu=0;
    int tamanho=0, X=0, Y=0, direcao=0;
    int qtdNavios=0, superpoder=0;

    do{
        system("clear");
        printTabNavios();
        printTabuleiro();
        printf("MENU DO JOGO:\n");
        printf("1 - Atirar\n");
        if(numPowers > 0) printf("2 - Usar Especial\n");
        printf("9 - Reiniciar Jogo\n");
        printf("0 - Revelar tabuleiro e Finalizar\n");
        scanf("%d", &selectMenu);
        printf("\n");

        switch (selectMenu){
            case 1:
                printf("Linha: ");
                scanf("%d", &Y);
                printf("Coluna: ");
                scanf("%d", &X);
                attackBomba(Y,X);
                break;

            case 2:
                printf("Escolha o super poder:\n");
                printf("1 - Cone\n");
                printf("2 - Octaedro\n");
                printf("3 - Cruz\n");
                printf("0 - Cancelar\n");
                do scanf("%d", &superpoder);
                while (superpoder < 0 || superpoder > 3);
                if(superpoder == 0) break;

                printf("Defina o ponto central:\n");
                printf("Linha: ");
                scanf("%d", &Y);
                printf("Coluna: ");
                scanf("%d", &X);
                printf("Tamanho (1 ou 2): ");
                do scanf("%d", &tamanho);
                while (tamanho < 1 || tamanho > 2);

                initAttackSuper(superpoder, X, Y, tamanho);
                break;
            
            /*
            case 6:
                printf("Digite quantos navios quer adicionar (máximo %d): ", menorIndice());
                scanf("%d", &qtdNavios);
                addNavioRandom(qtdNavios);
                break;
            

            case 7:
                printf("Digite as coordenadas iniciais de onde quer adicionar o navio:\n");
                printf("Linha: ");
                scanf("%d", &Y);
                printf("Coluna: ");
                scanf("%d", &X);
                printf("Tamanho do Navio (máximo %d): ", tamanhoMaxNavio());
                scanf("%d", &tamanho);
                printf("\nDirecao do Navio: \n1 - Vertical \n2 - Diagonal \n3 - Horizontal\n");
                scanf("%d", &direcao);
                printf("\n");

                if(addNavio(tamanho, X, Y, direcao)){
                    printf("Navio adicionado\n");
                }
                break;
            
            */

            case 9:
                inicializarTabuleiro();
                addNavioRandom(configNumNavios);
                system("clear");
                break;
            
            case 0: 
                printTabNavios();
                break;
            
            default:
                printf("Opcao invalida\n\n");
                break;
                       
        }

        sleep(1);

    } while (selectMenu != 0 && ((numTiros > 0 || numPowers > 0) && qtdNaviosAtivos > 0));
    
    if(qtdNavios > 0) printf("\nVocê perdeu! =/\n");
    if(qtdNavios <= 0) printf("\nParabéns! Você venceu! =/\n");

    menuPrincipal();
}


int main() {
    menuPrincipal();
    return 0;
}
