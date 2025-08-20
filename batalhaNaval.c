// Desafio Batalha Naval - MateCheck
#include <stdio.h>
#include <stdlib.h>

#define maxLinha 10 // quantidade de linhas do tabuleiro
#define maxColuna 10 // quantidade de colunas do tabuleiro
int tabNavios[maxLinha][maxColuna]; // guarda a informação de onde estão os navios | 0 - não tem | 1 - escondido | 2 - encontrado
char tabuleiro[maxLinha][maxColuna]; // matriz do tabuleiro que será mostrada na tela
int countNavio = 0;
int numAttacks = 0;
int numPowers = 0;

// Retorna o menor indice (linha ou coluna)
int menorIndice(){
    if(maxLinha < maxColuna)
        return maxLinha; 
    else return maxColuna;
}

// Retorna o maior indice (linha ou coluna)
int maiorIndice(){
    if(maxLinha > maxColuna)
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
    countNavio = 0;
    numAttacks = (maxLinha*maxColuna)/3;
    numPowers = (maxLinha*maxColuna)/(maiorIndice()*3);

    for(int i=0; i<maxLinha; i++){
        for (int j=0; j<maxLinha; j++){
            tabuleiro[i][j] = '~';
        }
    }
}

void printTabuleiro (){
    printf("\n\n");
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
    printf("Bombas: %d \tNavios: %d\tEspeciais: %d\n\n", numAttacks, countNavio, numPowers);
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
    if(direcao==1 && posY+tamNavio-1 >= maxLinha) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==2 && (posX+tamNavio-1 >= maxColuna || posY+tamNavio-1 >= maxLinha)) {
        printf("Erro: O Tamanho do navio excede o tabuleiro\n");
        return 1;
    } 
    if(direcao==3 && posX+tamNavio-1 >= maxColuna) {
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
        posX = rand() % (maxColuna-1);
        posY = rand() % (maxLinha-1);
        direcao = (rand() % 3) + 1;
        tamanho = rand() % (qtdMaximaNavios() / 2)+1;

        if(addNavio(tamanho, posX, posY, direcao))
            qtdNavios--;
    } while(qtdNavios > 0);
    return 1;
}

int attack(int linha, int coluna){
    char target = tabuleiro[linha][coluna]; //armazena o char da posição que foi atacado
    int hit = 0; // se acertou um navio se torna 1
    int checkNavioDestruido = 1; //será usada para checar se existe em torno desta posição outro char = target, ou seja, outra parte do navio

    numAttacks--;
    //Verifica se acertou um navio
    if(target >= '0' && target <= '0' + qtdMaximaNavios()){
        printf("Navio atingido!\n");
        hit = 1;
        tabuleiro[linha][coluna] = 'X';

        //Verifica se o navio foi destruído
        for(int l = linha-1 ; l <= linha+1 ; l++){
            for(int c = coluna-1 ; c <= coluna+1 ; c++){
                if(l >= 0 && l < maxLinha && c >= 0 && c < maxColuna){ //limita a checagem só para posições validas (dentro do tabuleiro)
                    if(tabuleiro[l][c] == target) {
                        checkNavioDestruido = 0;
                        break;
                    }
                }
            }
            if(checkNavioDestruido==0) break;
        }
        if(checkNavioDestruido == 1) {
            printf("NAVIO DESTRUIDO!");
            countNavio--;
        }
    
    } else {
        tabuleiro[linha][coluna] = '*';
        printf("Tiro na água!\n");
    }
    return hit;
}

int usePoderCone(int posX, int posY, int tam){}

int usePoderOcta(int posX, int posY, int tam){}

int usePoderCruz(int posX, int posY, int tam){}

void inicializarMenu(){
    int selectMenu=0;
    int tamanho=0, posX=0, posY=0, direcao=0;
    int qtdNavios=0, superpoder=0;

    do{
        system("clear");
        printTabuleiro();
        printf("BATALHA NAVAL: Escolha uma opção\n");
        printf("1 - Lançar Bomba\n");
        printf("2 - Usar Super Poder\n");
        //printf("6 - Adicionar navios aleatoriamente\n");
        //printf("7 - Adicionar um navio\n");
        //printf("8 - Limpar terminal\n");
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
                attack(posY,posX);
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
                printf("Tamanho (2 ou 3): ");
                do scanf("%d", &tamanho);
                while (tamanho < 2 || tamanho > 3);

                if(superpoder == 1) usePoderCone(posX, posY, tamanho);
                if(superpoder == 2) usePoderOcta(posX, posY, tamanho);
                if(superpoder == 3) usePoderCruz(posX, posY, tamanho);
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
                printf("Digite quantos navios quer no jogo (máximo %d): ", menorIndice());
                scanf("%d", &qtdNavios);
                inicializarTabuleiro();
                addNavioRandom(qtdNavios);
                break;
            
            case 0: 
                break;
            
            default:
                printf("Opcao invalida\n\n");
                break;
                       
        }
        //printf("Pressione ENTER para continuar...\n");
        //while (getchar() != '\n');
        sleep(2);
    } while (selectMenu != 0);
    
}

int main() {

    inicializarTabuleiro();
    addNavioRandom(10);
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
