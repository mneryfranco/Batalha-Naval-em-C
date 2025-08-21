// Desafio Batalha Naval - MateCheck
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int tamLinha = 10, tamColuna = 10; // quantidade de linhas e colunas do tabuleiro
char tabuleiro[14][14]; // matriz do tabuleiro que será mostrada na tela
int tabNavios[14][14]; // guarda a informação de onde estão os navios | 0 - não tem | 1 - escondido | 2 - encontrado
int countNavio = 0;
int numBombas = 0;
int numPowers = 0;

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

// Reseta o tabuleiro e a contagem de bombas e poderes
void inicializarTabuleiro (){
    countNavio = 0;
    numBombas = (tamLinha*tamColuna)/3;
    numPowers = (tamLinha*tamColuna)/(maiorIndice()*3);

    for(int i=0; i<tamLinha; i++){
        for (int j=0; j<tamLinha; j++){
            tabuleiro[i][j] = '~';
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
                else printf("\033[31m%d\033[0m  ", coluna);
            }
            else{
                if(coluna == -1) printf("\033[31m%d\033[0m  ", linha);
                else printf("%c  ", tabuleiro[linha][coluna]);
            }
            
        }
        printf("\n");
    }
    printf("Bombas: %d \tNavios: %d\tEspeciais: %d\n\n", numBombas, countNavio, numPowers);
}

// Procura um navio na posição informada e retorna 1 se tiver
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

// Faz vários testes pra ver se é possível adicionar um navio com os dados fornecidos
int checkErrosAddNavio(int tamNavio, int posX, int posY, int direcao){
    /*  
    tamNavio: Tamanho do navio a ser adicionado no tabuleiro
    posX: posição X inicial do navio
    poxY: posição Y inicial do navio
    Direção: 1 = Vertical | 2 = Diagonal | 3 = Horizontal 
    */

    //Tratamento de erros
    //Se o usuário selecionar uma posição fora do tabuleiro, ou valores negativos, encerra.
    if(posY<0 || posY>=tamLinha){
        printf("Erro: Linha inexistente no tabuleiro\n");
        return 1;
    }
    if(posX<0 || posX>=tamColuna){
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
    if(direcao==1 && posY+tamNavio-1 >= tamLinha) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==2 && (posX+tamNavio-1 >= tamColuna || posY+tamNavio-1 >= tamLinha)) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==3 && posX+tamNavio-1 >= tamColuna) {
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

// Adiciona um navio 
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
    if(checkErrosAddNavio(tamNavio, posX, posY, direcao)){
        return 0;
    }

    //Adiciona o navio no tabuleiro
    int linha = posY, coluna = posX, i=0;
    while (i < tamNavio){
        tabuleiro[linha][coluna] = countNavio + '0';
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

// Adiciona uma quantidade específica de navios aleatoriamente pelo tabuleiro
int addNavioRandom (int qtdNavios){
    int posX, posY, direcao, tamanho;

    if(qtdNavios > qtdMaximaNavios()+1) qtdNavios = qtdMaximaNavios()+1;
    
    srand(time(NULL)); // inicializa a semente com o tempo atual
    do{
        //Verifica se o tabuleiro já está em sua capacidade máxima de navios
        if(countNavio == qtdMaximaNavios()){
            printf("Tabuleiro lotado (%d navios)\n", countNavio);
            return 0;
            break;
        }
        posX = rand() % (tamColuna-1);
        posY = rand() % (tamLinha-1);
        direcao = (rand() % 3) + 1;
        //tamanho = rand() % (qtdMaximaNavios() / 2)+1;
        tamanho = (qtdMaximaNavios()+1 - countNavio) / 2;
        if(tamanho < 2) tamanho = 2; // garante que o tamanho mínimo do navio seja 2

        if(addNavio(tamanho, posX, posY, direcao))
            qtdNavios--;
    } while(qtdNavios > 0);
    return 1;
}

// Ataca uma posição do tabuleiro. Retorna 1 se atingir um navio.
int attack(int linha, int coluna){
    char target = tabuleiro[linha][coluna]; //armazena o char da posição que foi atacado
    int hit = 0; // se acertou um navio se torna 1
    int checkNavioDestruido = 1; //será usada para checar se existe em torno desta posição outro char = target, ou seja, outra parte do navio

    if(linha < 0 || coluna < 0 || linha >= tamLinha || coluna >= tamColuna){
        printf("Fora do tabuleiro!\n");
        return 0;
    }

    //Verifica se acertou um navio
    if(target >= '0' && target <= '0' + qtdMaximaNavios()){
        printf("Navio atingido!\n");
        hit = 1;
        tabuleiro[linha][coluna] = 'X';

        //Verifica se o navio foi destruído
        for(int l = linha-1 ; l <= linha+1 ; l++){
            for(int c = coluna-1 ; c <= coluna+1 ; c++){
                if(l >= 0 && l < tamLinha && c >= 0 && c < tamColuna){ //limita a checagem só para posições validas (dentro do tabuleiro)
                    if(tabuleiro[l][c] == target) {
                        checkNavioDestruido = 0;
                        break;
                    }
                }
            }
            if(checkNavioDestruido==0) break;
        }
        if(checkNavioDestruido == 1) {
            printf("NAVIO DESTRUIDO!\n");
            countNavio--;
        }
    
    } else{
        tabuleiro[linha][coluna] = '*';
    }
    
    return hit;
}

// Função de ataque com a bomba. Retorna 1 se tiver sucesso
int attackBomba(int linha, int coluna){
    numBombas--;
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

// Executa um super poder no tabuleiro em forma de cone
int usePoderCone(int posX, int posY, int tam){
    int largura = 0, altura = 0;
    int linha = posY-tam;
    int coluna = posX;

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
int usePoderOcta(int posX, int posY, int tam){
    int largura;
    int linha = posY-tam;
    int coluna = posX;

    while(linha <= posY+tam){
        largura = tam - abs(posY-linha);
        for(int c = -largura ; c <= largura ; c++){
            attackPoder(linha, coluna+c, tam);
        }
        linha++;
    }
}

// Executa um super poder no tabuleiro em forma de cruz
int usePoderCruz(int posX, int posY, int tam){
    tam++; // só pra deixar a cruz maior mesmo e ser mais interessante por jogardor usar
    int linha = posY-tam;

    for(int linha = posY-tam; linha <= posY+tam; linha++){
        if(linha == posY){
            for(int c = -tam ; c <= tam ; c++){
                attackPoder(linha, posX+c, tam);
            }
        }
        else attackPoder(linha, posX, tam);
    }
}

void menu(){
    int selectMenu=0;
    int tamanho=0, posX=0, posY=0, direcao=0;
    int qtdNavios=0, superpoder=0;

    do{
        system("clear");
        printTabuleiro();
        printf("BATALHA NAVAL: Escolha uma opção\n");
        printf("1 - Lançar Bomba\n");
        if(numPowers > 0) printf("2 - Usar Super Poder\n");
        printf("9 - Reiniciar Jogo\n");
        printf("0 - Sair\n");
        scanf("%d", &selectMenu);
        printf("\n");
        
        switch (selectMenu){
            case 1:
                printf("Linha: ");
                scanf("%d", &posY);
                printf("Coluna: ");
                scanf("%d", &posX);
                attackBomba(posY,posX);
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
                scanf("%d", &posY);
                printf("Coluna: ");
                scanf("%d", &posX);
                printf("Tamanho (1 ou 2): ");
                do scanf("%d", &tamanho);
                while (tamanho < 1 || tamanho > 2);

                if(numPowers >= tamanho){
                    if(superpoder == 1) usePoderCone(posX, posY, tamanho);
                    if(superpoder == 2) usePoderOcta(posX, posY, tamanho);
                    if(superpoder == 3) usePoderCruz(posX, posY, tamanho);
                    numPowers = numPowers - tamanho;
                } 
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
                scanf("%d", &posY);
                printf("Coluna: ");
                scanf("%d", &posX);
                printf("Tamanho do Navio (máximo %d): ", tamanhoMaxNavio());
                scanf("%d", &tamanho);
                printf("\nDirecao do Navio: \n1 - Vertical \n2 - Diagonal \n3 - Horizontal\n");
                scanf("%d", &direcao);
                printf("\n");

                if(addNavio(tamanho, posX, posY, direcao)){
                    printf("Navio adicionado\n");
                }
                break;
            
            */

            case 9:
                inicializarTabuleiro();
                addNavioRandom(qtdMaximaNavios());
                break;
            
            case 0: 
                break;
            
            default:
                printf("Opcao invalida\n\n");
                break;
                       
        }
        //printf("Pressione ENTER para continuar...\n");
        //while (getchar() != '\n');
        sleep(1);
    } while (selectMenu != 0 && numBombas > 0 && numBombas > 0);
    if(qtdNavios > 0) printf("\nVocê perdeu! =/\n");
    
}

int main() {

    inicializarTabuleiro();
    addNavioRandom(qtdMaximaNavios());
    menu();
    
    return 0;
}
