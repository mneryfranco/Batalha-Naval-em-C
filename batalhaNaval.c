// Desafio Batalha Naval - MateCheck
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RED "\033[31m"
#define DARKBLUE "\033[34m"
#define DARKBLACK "\033[1m"
#define GRAY "\033[97m"
#define RESETCOLOR "\033[0m"

int tamLinha = 10, tamColuna = 10; // quantidade de linhas e colunas do tabuleiro
char tabuleiro[14][14]; // matriz do tabuleiro que será mostrada na tela
int tabNavios[14][14]; // guarda a informação de onde estão os navios
int visitado[14][14]; // usado em algoritmos de busca e checagem de navios
int configNumNavios; // quantos navios vão ser adicionados no jogo
int qtdNaviosAtivos; // quantos navios estão ativos em jogo, ou seja, ainda não foram destruidos
int numTiros ;
int numPowers;

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

// Reseta o tabuleiro e a contagem de navios, tiros e poderes
void inicializarTabuleiro (){
    qtdNaviosAtivos = 0;
    numTiros = 20;
    numPowers = 6;
    configNumNavios = 6;

    for(int i=0; i<tamLinha; i++){
        for (int j=0; j<tamLinha; j++){
            tabuleiro[i][j] = '~';
            tabNavios[i][j] = '~';
            visitado[i][j] = '~';
        }
    }
}

// Imprime o tabuleiro do jogo no terminal
void printTabuleiro (){
    printf("\n\n");
    for(int l=-1; l<tamLinha; l++){
        for (int c=-1; c<tamColuna; c++){
            if(l == -1){
                if(c==-1) printf("   ");
                else printf(RED"%d  "RESETCOLOR, c);
            }
            else{
                if(c == -1) printf(RED"%d  "RESETCOLOR  , l);
                else {
                    // Mar = azul escuro 
                    if(tabuleiro[l][c] == '~'){
                        printf(DARKBLUE"%c  "RESETCOLOR, tabuleiro[l][c]);
                    }

                    // Número = cinza 
                    else if(tabuleiro[l][c] > '0' && tabuleiro[l][c] <= '9'){
                        printf(GRAY"%c  "RESETCOLOR, tabuleiro[l][c]);
                    }

                    // Navio = preto negrito 
                    else if(tabuleiro[l][c] >= 'A' && tabuleiro[l][c] < 'Z'){
                        printf(DARKBLACK"%c  "RESETCOLOR, tabuleiro[l][c]);
                    }

                    else printf("%c  ", tabuleiro[l][c]);
                }
            }
            
        }
        printf("\n");
    }
    printf(RESETCOLOR);
    printf("Tiros: %d \tNavios: %d\tEspeciais: %d\n\n", numTiros, qtdNaviosAtivos, numPowers);
}

// Imprime o tabuleiro de navios escondidos
void printTabGabarito (){
    printf("\n\nGABARITO\n");
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
}

// Imprime o tabuleiro de checagem de navios
void printTabChecagem (){
    printf("\n\nCHECAGEM\n");
    for(int linha=-1; linha<tamLinha; linha++){
        for (int coluna=-1; coluna<tamColuna; coluna++){
            if(linha == -1){
                if(coluna==-1) printf("   ");
                else printf(RED"%d  "RESETCOLOR, coluna);
            }
            else{
                if(coluna == -1) printf(RED"%d  "RESETCOLOR  , linha);
                else printf("%c  ", visitado[linha][coluna]);
            }
            
        }
        printf("\n");
    }
}

// Retorna a quantidade de navios ao redor da posicao fornecida
int getQtdNavioRedor(int Y, int X){
    int qtd=0;

    for (int l = Y-1 ; l <= Y+1 ; l++){
        for (int c = X-1 ; c <= X+1 ; c++){
            if(l >= 0 && l < tamLinha && c >= 0 && c < tamColuna && !(l == Y && c == X)){ //limita a checagem só para posições validas (dentro do tabuleiro)
                if(tabNavios[l][c] >= 'A' && tabNavios[l][c] <= 'Z'){
                    qtd++;
                }
            }
        }
    }
    return qtd;
}

// Procura um navio na posição informada e retorna 1 se tiver
int buscaNavio(int tamNavio, int X, int Y, int direcao){
    int coluna = X, linha = Y, i=0;
    while (i < tamNavio){
        if(tabNavios[linha][coluna] >= 'A' && tabNavios[linha][coluna] <= 'Z'){
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
        //printf("Erro: Linha inexistente no tabuleiro\n");
        return 1;
    }
    if(X<0 || X>=tamColuna){
        //printf("Erro: Coluna inexistente no tabuleiro\n");
        return 1;
    }  

    //Encerra pra valores invalidos no tamanho do navio
    if(tamNavio > tamanhoMaxNavio()) {
        //printf("Erro: Navio muito grande. O tamanho maior é %d\n", tamanhoMaxNavio());
        return 1;
    }
    if(tamNavio <= 0) {
        //printf("Erro: Tamanho inválido para o navio");
        return 1;
    }

    //Encerra pra valores inválidos de direcao do navio
    if(direcao <= 0 || direcao >= 4){
        //printf("Erro: Valor inválido para direcao do navio\n");
        return 1;
    }

    //Se o tamanho do barco exceder o tamanho do tabuleiro, encerra.
    if(direcao==1 && Y+tamNavio-1 >= tamLinha) {
        //printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==2 && (X+tamNavio-1 >= tamColuna || Y+tamNavio-1 >= tamLinha)) {
        //printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==3 && X+tamNavio-1 >= tamColuna) {
        //printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 

    //Verifica se já existe um navio na posição
    if(buscaNavio(tamNavio, X, Y, direcao)){
        //printf("Erro ao tentar adicionar navio\n");
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
        //printf("Tabuleiro lotado (%d navios)\n", qtdNaviosAtivos);
        return 0;
    }

    //Verifica se existe algum problema ao adicionar o navio
    if(checkErrosAddNavio(tamNavio, X, Y, direcao)){
        return 0;
    }

    //Adiciona o navio no tabuleiro
    int linha = Y, coluna = X, i=0;
    while (i < tamNavio){
        tabNavios[linha][coluna] = qtdNaviosAtivos + 'A';
        if (direcao == 1) linha++;
        if (direcao == 2){
            linha++;
            coluna++;
        }
        if (direcao == 3) coluna++;
        i++;
    }
    qtdNaviosAtivos++;
    return 1;
}

// Adiciona uma quantidade específica de navios aleatoriamente pelo tabuleiro
int addNavioRandom (){
    int X, Y, direcao, tamanho;
    int countAdd = configNumNavios;
    srand(time(NULL)); // inicializa a semente com o tempo atual
    do{
        X = rand() % (tamColuna-1);
        Y = rand() % (tamLinha-1);
        direcao = (rand() % 3) + 1;
        tamanho = (menorIndice() - qtdNaviosAtivos) / 2;
        if(tamanho < 2) tamanho = 2; // garante que o tamanho mínimo do navio seja 2

        if(addNavio(tamanho, X, Y, direcao))
            countAdd--;
    } while(countAdd > 0);
    return 1;
}

// Retorna o tamanho de um navio dado uma posição inicial dele
int getTamanhoNavio(int Y, int X){
    int tam=0;
    char target;

    if (Y < 0 || Y >= tamLinha || X < 0 || X >= tamColuna)
        return 0;
    
    // Verifica se tem navio na posição Y,X
    if (tabNavios[Y][X] == '~')
        return 0;
    else {
        target = tabNavios[Y][X];
        tam++;
    }

    // Verifica se já foi visitado
    if (visitado[Y][X] == target)
        return 0;
    
    // Verifica se é parte do navio
    if (tabNavios[Y][X] != target)
        return 0;
    
    // Marca como visitado
    visitado[Y][X] = target;

    //Procura o ID do navio nas redondezas
    for (int l = Y-1 ; l <= Y+1 ; l++){
        for (int c = X-1 ; c <= X+1 ; c++){
            if(l >= 0 && l < tamLinha && c >= 0 && c < tamColuna && !(l == Y && c == X)){ //limita a checagem só para posições validas (dentro do tabuleiro)
                if(tabNavios[l][c] == target && visitado[l][c] != target){
                    tam += getTamanhoNavio(l, c);
                }
            }
        }
    }
    visitado[Y][X] = '~';
    return tam;
}

// Conta quantas partes do navio foram destruidas
int countPartesDestruidasNavio(int Y, int X){
    char target;
    int countDestruido=0;

    if (Y < 0 || Y >= tamLinha || X < 0 || X >= tamColuna)
        return 0;
    
    // Verifica se tem navio na posição Y,X
    if (tabNavios[Y][X] == '~')
        return 0;
    else target = tabNavios[Y][X];
    
    // Verifica se já foi visitado
    if (visitado[Y][X] == target)
        return 0;
    
    // Verifica se é parte do navio
    if (tabNavios[Y][X] != target)
        return 0;

    // Marca como visitado
    visitado[Y][X] = target;

    // Conta como parte destruída se foi atingida
    countDestruido = (tabuleiro[Y][X] == '#') ? 1 : 0;

    // Verifica vizinhos 
    //Procura o ID do navio nas redondezas
    for (int l = Y-1 ; l <= Y+1 ; l++){
        for (int c = X-1 ; c <= X+1 ; c++){
            if(l >= 0 && l < tamLinha && c >= 0 && c < tamColuna && !(l == Y && c == X)){ //limita a checagem só para posições validas (dentro do tabuleiro)
                if(tabNavios[l][c] == target && visitado[l][c] != target){
                    countDestruido += countPartesDestruidasNavio(l, c);
                }
            }
        }
    }
    
    visitado[Y][X] = '~';
    return countDestruido;
}

// Verifica se o navio foi destruido, ou seja, se todas suas partes foram atacadas
int checkNavioDestruido(int Y, int X){
    if (Y < 0 || Y >= tamLinha || X < 0 || X >= tamColuna)
        return 0;
    
    // Verifica se tem navio na posição Y,X
    if (tabNavios[Y][X] == '~')
        return 0;
    
    if(getTamanhoNavio(Y,X) == countPartesDestruidasNavio(Y,X))
        return 1;
    else return 0;
    
}

// Ataca uma posição do tabuleiro. Retorna 1 se atingir um navio.
int attack(int linha, int coluna){
    char target = tabNavios[linha][coluna]; //armazena o char da posição que foi atacado
    int hit = 0; // se acertou um navio se torna 1

    if(linha < 0 || coluna < 0 || linha >= tamLinha || coluna >= tamColuna){
        printf("\Fora do tabuleiro\n");
        return 0;
    }
    
    if(tabuleiro[linha][coluna] != '~'){
        printf("Alvo repetido. Tiro desperdiçado.\n");
        return 1;
    }

    //Verifica se acertou um navio
    if(target >= 'A' && target < 'Z' + configNumNavios){
        printf("Navio atingido!\n");
        hit = 1;

        //Não deixa substituir por X se acertar novamente um navio já destruido 
        if(tabuleiro[linha][coluna] != target)
            tabuleiro[linha][coluna] = '#';

        //Verifica se o navio foi destruído
        if(checkNavioDestruido(linha, coluna)) {
            printf(RED"NAVIO DESTRUIDO!\n"RESETCOLOR);
            qtdNaviosAtivos--;
            revelaNavioDestruido(linha, coluna);
        }
    
    } 
    else if(getQtdNavioRedor(linha, coluna) > 0){
        tabuleiro[linha][coluna] = '0' + getQtdNavioRedor(linha,coluna);
        printf("Tiro na água\n");
    }
    else{
        tabuleiro[linha][coluna] = '*';
        printf("Tiro na água\n");
        return 1;
    }
    
    return hit;
}

// Função de ataque simples. Retorna 1 se tiver sucesso
int tiroSimples(int linha, int coluna){
    if(numTiros <= 0){
        printf("Tiros insuficientes");
        return 0;
    }
    if(attack(linha,coluna)){
        numTiros--;
        return 1;
    }
    else {
        return 0;
    }
}

// Usada pelas funções de super poder para atacar uma célula do tabuleiro
int attackPoder(int linha, int coluna, int tam){    
    if(attack(linha,coluna)){
        return 1;
    }
    else return 0;
}

// Executa um super poder no tabuleiro em forma de cone
int usarPoderTriangulo(int X, int Y, int tam){
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
int usarPoderLosangulo(int X, int Y, int tam){
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

// Faz a seleção do super poder e chama a função respectiva
int initAttackSuper(int superpoder, int X, int Y, int tam){
    if(numPowers < tam){
        printf("Você não tem poder especial suficiente\n");
        return 0;
    }
    if(superpoder == 1) usarPoderTriangulo(X, Y, tam);
    if(superpoder == 2) usarPoderLosangulo(X, Y, tam);
    if(superpoder == 3) usePoderCruz(X, Y, tam);
    
    numPowers = numPowers - tam;
    return 1;
}

// Função auxiliar para ler um inteiro com validação
int lerInteiro(const char *mensagem) {
    int valor;
    char c;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1) {
            // Limpa o buffer após leitura válida
            while ((c = getchar()) != '\n' && c != EOF);
            return valor;
        }
        printf("Digite uma opção válida: ");
        // Limpa o buffer após leitura inválida
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

// Função auxiliar para ler um inteiro com validação
int lerInteiroBetween(const char *mensagem, int min, int max) {
    int valor;
    char c;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1 && valor >= min && valor <= max) {
            // Limpa o buffer após leitura válida
            while ((c = getchar()) != '\n' && c != EOF);
            return valor;
        }
        printf("Entrada inválida! Digite um número entre %d e %d.\n\n", min, max);
        // Limpa o buffer após leitura inválida
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

//Menu com o jogo ativo
void menuJogo(){
    int selectMenu=-1;
    int tamanho=-1, X=-1, Y=-1, direcao=-1;
    int qtdNavios=-1, superpoder=-1;
    char confirma=-1;

    do{
        system("clear");
        printTabuleiro();
        printf("MENU DO JOGO:\n");
        if(numTiros > 0) printf("1 - Atirar\n");
        if(numPowers > 0) printf("2 - Usar Especial\n");
        printf("9 - Reiniciar\n");
        printf("0 - Finalizar\n");
        selectMenu = lerInteiro("");
        printf("\n");

        switch (selectMenu){
            case 1:
                if(numTiros <= 0){
                    printf("Você não tem tiros suficientes");
                    break;
                }
                Y = lerInteiroBetween("Linha: ", 0, tamLinha - 1);
                X = lerInteiroBetween("Coluna: ", 0, tamColuna - 1);
                printf("\n");
                tiroSimples(Y,X);
                break;

            case 2:
                if(numPowers <= 0){
                    printf("Você não tem especiais suficientes");
                    break;
                }
                printf("Escolha o super poder:\n");
                printf("1 - Triangulo\n");
                printf("2 - Losangulo\n");
                printf("3 - Cruz\n");
                printf("0 - Cancelar\n");
                superpoder = lerInteiroBetween("Opção: ", 0, 3);
                if(superpoder == 0) break;

                printf("\nDefina o ponto central:\n");
                Y = lerInteiroBetween("Linha: ", 0, tamLinha - 1);
                X = lerInteiroBetween("Coluna: ", 0, tamColuna - 1);
                tamanho = lerInteiroBetween("Tamanho (1 ou 2): ", 1, 2);

                printf("\n");
                initAttackSuper(superpoder, X, Y, tamanho);
                break;

            case 9:
                confirma = lerInteiroBetween("Você tem certeza que deseja reiniciar o jogo? \n1 - Sim \n2 - Não\n", 1, 2);
                if(confirma == 2) break;
                inicializarTabuleiro();
                addNavioRandom();
                system("clear");
                break;
            
            case 0: 
                confirma = lerInteiroBetween("Você tem certeza que deseja encerrar o jogo? \n1 - Sim \n2 - Não\n", 1, 2);
                if(confirma == 2) selectMenu = -1;
                break;
            
            default:
                printf("Opcao invalida\n\n");
                break;
                       
        }
        sleep(1);

    } while (selectMenu != 0 && ((numTiros > 0 || numPowers > 0) && qtdNaviosAtivos > 0));
    
    // Se o jogo acabou sem o usuário pedir pra finalizar, mostrar os resultados
    if(selectMenu != 0){

        system("clear");
        printTabuleiro();
        printTabGabarito();

        if(qtdNaviosAtivos > 0) printf("\nVocê perdeu! =/\n");
        if(qtdNaviosAtivos <= 0) printf("\nParabéns! Você venceu!\n");

        int exit = lerInteiro("Digite 1 para continuar... ");
    }
    
    menuPrincipal();
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
            "Cada navio é representado por uma letra de A a J.\n"
            "Os navios podem estar posicinados de forma horizontal, vertical ou diagonal, e possuem tamanho de 2 a 5.\n"
            "Sempre que você acertar uma parte do navio será marcado com #.\n"
            "Números aparecerão no tabuleiro indicando quantos navios existem em sua redondeza.\n"
            "Você pode usar tiros especiais em forma de triângulo, losangulo ou cruz para acertar vários alvos de uma vez.\n"
            "Descubra a posição dos navios e use seus tiros e poderes para destrui-los! \n\n"
            
            RED"MENU PRINCIPAL:\n"RESETCOLOR
            "1 - Iniciar Jogo\n"
            "0 - Finalizar\n");

        selectMenu = lerInteiro("");
        printf("\n");

        switch (selectMenu){
            case 1:
                inicializarTabuleiro();
                addNavioRandom();
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


int main() {
    menuPrincipal();
    return 0;
}
