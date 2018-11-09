// Função: Atividade parcial das disciplinas Linguagem de Programação e Algoritmos (UTFPR-PG)
// Autor: Cesar Mauricio Chauchuty, Gabriel da Cruz Rakovicz
// Data: 08/11/18
// Jogo: Pac-Man
// Versão: 1.0
// Compilador: TMD-GCC 4.9.2 64-bit Release

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <termios.h>
#include <fcntl.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

#include <sys/io.h>
#include <fcntl.h>
#ifndef _O_U16TEXT
  #define _O_U16TEXT 0x20000
#endif

#define tempoRandom() srand(time(NULL));
#define pt_br()  setlocale(LC_ALL,"");
#define corJogo() system("color f")
#define limpaTela() wprintf(L"\033[H")
#define linha() wprintf(L"\n---------------------------------------------")

wchar_t bolinha[] = L"\u2022";
wchar_t pac_a[] = L"C";
wchar_t pac_f[] = L"O";
wchar_t espaco[] = L" ";

struct Mapa{
	wchar_t *vet_mapa[13][20], vet_pontos[13][20];
	int dificuldade;
}mapa;

struct Player{
	char tecla, simbolo;
	int pos_i, pos_j, nav, pontos, m_pontos;
}player;

struct Fantasma{
	char simbolo;
	int pos_i, pos_j, nav, ia;
}fantasma[4];

int kbhit(void){
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}


// funcKbhit
void funcKbhit(){
	if(kbhit()){
			fflush(stdin);
			player.tecla = getchar();
		}
}

// Esconde o cursor
void esconderCursor(){
	/*
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
	 */
}

void pausar_jogo(){
	getchar();
}



// Arte Pac
void artePac(){
	wprintf(L"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= \n");
	wprintf(L"    _ __   __ _  ___ _ __ ___   __ _ _ __     \n");
	wprintf(L"   | '_ \\ / _  |/ __| '_ ` _ \\ / _` | '_ \\ \n");
	wprintf(L"   | |_) | (_| | (__| | | | | | (_| | | | |   \n");
	wprintf(L"   | .__/ \\__._|\\___|_| |_| |_|\\__,_|_| |_|\n");
	wprintf(L"   | |                                        \n");
	wprintf(L"   |_|                                        \n");
	wprintf(L"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= \n");
}

// Efeito de Loading
void loading(){
	int i;
	wprintf(L"\n\n\n\n\n\n\n\t\t   ");
	for(i = 0; i < 3 ; i++){
		wprintf(L". ");
		usleep(200000);
	}
	wprintf(L"\n");
	system("clear");
}

// Opções da telaInicial()
int opcoesMenu(char tecla, int nav){
	char dificuldade[10];
	linha();
	switch(mapa.dificuldade){
		case 200000:
			strcpy(dificuldade, "Fácil");
			break;
		case 150000:
			strcpy(dificuldade, "Médio");
			break;
		case 100000:
			strcpy(dificuldade, "Béééé");
	}
	switch(nav){
		case 0:
			wprintf(L"\n\t      >  Iniciar       ");
			wprintf(L"\n\t          Instruções   ");
			wprintf(L"\n\t          Dificuldade  ");
			wprintf(L"\n\t          Pontuação    ");
			wprintf(L"\n\t          Créditos     ");
			wprintf(L"\n\t          Sair         ");
			wprintf(L"\n\t\t\t\tNível: %s", dificuldade);
			if(tecla == '\n'){
				return 0;
			}
			break;
		case 1:
			wprintf(L"\n\t          Iniciar         ");
			wprintf(L"\n\t      >  Instruções       ");
			wprintf(L"\n\t          Dificuldade     ");
			wprintf(L"\n\t          Pontuação       ");
			wprintf(L"\n\t          Créditos        ");
			wprintf(L"\n\t          Sair         ");
			wprintf(L"\n\t\t\t\tNível: %s", dificuldade);
			if(tecla == '\n'){
				return 1;
			}
			break;
		case 2:
			wprintf(L"\n\t          Iniciar         ");
			wprintf(L"\n\t          Instruções      ");
			wprintf(L"\n\t      >  Dificuldade      ");
			wprintf(L"\n\t          Pontuação       ");
			wprintf(L"\n\t          Créditos        ");
			wprintf(L"\n\t          Sair         ");
			wprintf(L"\n\t\t\t\tNível: %s", dificuldade);
			if(tecla == '\n'){
				return 2;
			}
			break;
		case 3:
			wprintf(L"\n\t          Iniciar         ");
			wprintf(L"\n\t          Instruções      ");
			wprintf(L"\n\t          Dificuldade     ");
			wprintf(L"\n\t      >  Pontuação        ");
			wprintf(L"\n\t          Créditos        ");
			wprintf(L"\n\t          Sair         ");
			wprintf(L"\n\t\t\t\tNível: %s", dificuldade);
			if(tecla == '\n'){
				return 3;
			}
			break;
		case 4:
			wprintf(L"\n\t          Iniciar         ");
			wprintf(L"\n\t          Instrucões      ");
			wprintf(L"\n\t          Dificuldade     ");
			wprintf(L"\n\t          Pontuação       ");
			wprintf(L"\n\t      >  Créditos         ");
			wprintf(L"\n\t          Sair         ");
			wprintf(L"\n\t\t\t\tNível: %s", dificuldade);
			if(tecla == '\n'){
				return 4;
			}
			break;
		case 5:
			wprintf(L"\n\t          Iniciar         ");
			wprintf(L"\n\t          Instrucões      ");
			wprintf(L"\n\t          Dificuldade     ");
			wprintf(L"\n\t          Pontuação       ");
			wprintf(L"\n\t          Créditos        ");
			wprintf(L"\n\t      >  Sair             ");
			wprintf(L"\n\t\t\t\tNível: %s", dificuldade);
			if(tecla == '\n'){
				return 5;
			}
		}
		linha();
		wprintf(L"\n\t          Versão 1.0          ");
}

// Tela de Instruções
void instrucoes(){
	system("clear");
	loading();
	artePac();
	wprintf(L"\t\t- INSTRUÇÕES -");
	linha();
	wprintf(L"\n Controle:\n");
	wprintf(L"\t   [W] Cima\t [A] Esquerda\n\t   [S] Baixo\t [D] Direita");
	linha();
	wprintf(L"\n Objetivo: \n");
	wprintf(L"\t   Coma todas as bolinhas sobre o \n\t   tabuleiro.\n");
	wprintf(L"\n\t   Não deixe que os fantasmas o \n\t   toquem!");
	linha();
	wprintf(L"\n\n Pressione tecla Enter para voltar...");
	pausar_jogo();
	system("clear");
	loading();
}

//Tela Dificuldade

int dificuldade(){
		system("clear");
		loading();
		player.nav = 0;
		player.tecla = ' ';
	do{
		artePac();
		wprintf(L"\t\t- DIFICULDADE -");
		linha();
		funcKbhit();
		switch(player.tecla){
			case 'w':
				if(player.nav > 0){
					player.nav--;
					player.tecla = ' ';
				}
				break;
			case 's':
				if(player.nav < 2){
					player.nav++;
					player.tecla = ' ';
				}
		}
		switch(player.nav){
			case 0:
				wprintf(L"\n\t         >  Fácil     ");
				wprintf(L"\n\t             Médio    ");
				wprintf(L"\n\t             Béééé  ");
				funcKbhit();
				if(player.tecla == '\n'){
					mapa.dificuldade = 200000;
					system("clear");
					loading();
					return 0;
				}
				break;
			case 1:
				wprintf(L"\n\t             Fácil    ");
				wprintf(L"\n\t         >  Médio     ");
				wprintf(L"\n\t             Béééé  ");
				funcKbhit();
				if(player.tecla == '\n'){
					mapa.dificuldade = 150000;
					system("clear");
					loading();
					return 0;
				}
				break;
			case 2:
				wprintf(L"\n\t             Fácil    ");
				wprintf(L"\n\t             Médio    ");
				wprintf(L"\n\t         >  Béééé     ");
				funcKbhit();
				if(player.tecla == '\n'){
					mapa.dificuldade = 100000;
					system("clear");
					loading();
					return 0;
				}
				break;
		}
		wprintf(L"\n");
		usleep(200000);
		limpaTela();
	}while(1);
		pausar_jogo();
		system("clear");
		loading();

}

// Tela de Pontuação
void pontuacao(int pontos){
	system("clear");
	loading();
	artePac();
	wprintf(L"\t\t- PONTUAÇÃO -");
	linha();
	if(player.pontos > player.m_pontos){
		player.m_pontos = player.pontos - 20;
	}
	wprintf(L"\n\t     Pontuação Máxima: %d", player.m_pontos);
	wprintf(L"\n\n   Pressione tecla Enter para voltar...");
	pausar_jogo();
	system("clear");
	loading();
}

// Tela de Créditos
void creditos(){
	system("clear");
	loading();
	artePac();
	wprintf(L"\t\t- CRÉDITOS -");
	linha();
	wprintf(L"\n\t   Cesar Mauricio Chauchuty");
	wprintf(L"\n\t   Gabriel Da Cruz Rakovicz");
	linha();
	wprintf(L"\n\t\t- VERSÃO LINUX -");
	linha();
	wprintf(L"\n\t   Gabriel Pena");
	wprintf(L"\n\n Pressione tecla Enter para voltar...");
	pausar_jogo();
	system("clear");
	loading();
}

// Saindo...
void sair(int cheat){
	int i;
	wprintf(L"\n");
	system("clear");
	usleep(500000);
	wprintf(L"\n\n\n\t\t   Saindo");
	for(i = 0; i < 3; i++){
		wprintf(L". ");
		wprintf(L"\n");
		usleep(500000);
	}
	// "Cheat"
	if(cheat == 1){
		wprintf(L"\n\n");
		wprintf(L"\t  Béééééé!!!                            ");
		wprintf(L"\n                  _.-..                 ");
		wprintf(L"\n                ,'9 )\\)`-.,.--.        ");
		wprintf(L"\n                `-.|           `.       ");
		wprintf(L"\n                   \\,      ,    \\)    ");
		wprintf(L"\n                    `.  )._\\   (\\     ");
		wprintf(L"\n                      |//   `-,//       ");
		wprintf(L"\n                      ]||    //""       ");
		wprintf(L"\n                      ""    ""          ");
		sleep(2);
	}
	wprintf(L"\n\n");
	sleep(1);
	exit(0);
}

// Tela vocePerdeu()
void vocePerdeu(){
	sleep(1);
	system("clear");
	loading();
	artePac();
	wprintf(L"\n\t\tVocê perdeu!");
	wprintf(L"\n\n Pressione tecla Enter para continuar...");
	pausar_jogo();
	system("clear");
	loading();
	telaInicial();
}

// Tela voceGanhou()
void voceGanhou(){
	sleep(1);
	system("clear");
	loading();
	artePac();
	artePac();
	wprintf(L"\n\t   Parabéns! Você ganhou!");
	wprintf(L"\n\n Pressione tecla Enter para continuar...");
	pausar_jogo();
	system("clear");
	loading();
	telaInicial();
}

// Controlador:
/*
   Dentro do procedimento gamePlay() temos a função kbhit() combinada com getchar().
   O resultado é capturar as telas W, S, D, A e de acordo com a tecla capturada,
   movimentar o Pac dentro da Matriz (i++, i--, j++, j--)...
*/

void controle(int pos_i, int pos_j, int tam_i, int tam_j){
	switch(player.tecla){
			case 'w':
				if(mapa.vet_mapa[pos_i-1][pos_j] == L"|"){
					break;
				}
				if(player.pos_i != 0){
					player.pos_i--;
				}

				break;
			case 's':
				if(mapa.vet_mapa[pos_i+1][pos_j] == L"|"){
					break;
				}
				if(player.pos_i != tam_i){
					player.pos_i++;
				}
				break;
			case 'a':
				if(mapa.vet_mapa[pos_i][pos_j-1] == L"|"){
					break;
				}
				if(player.pos_j != 0){
					player.pos_j--;
				}
				break;
			case 'd':
				if(mapa.vet_mapa[pos_i][pos_j+1] == L"|"){
					break;
				}
				if(player.pos_j != tam_j){
					player.pos_j++;
				}
				break;
		}
}

void fantasma_nav(int nav, int pos_i, int pos_j, int ia, int tam_i, int tam_j){
	switch(nav){
		case 0:
			if(nav == 1 || nav == 2 || nav == 3){
				break;
			}
		case 1:
			switch(rand() % ia){
				case 0:
					if(mapa.vet_mapa[pos_i+1][pos_j] != L"|" && pos_i != tam_i){
						fantasma[nav].pos_i++;
					}
					break;
				case 1:
					if(mapa.vet_mapa[pos_i-1][pos_j] != L"|" && pos_i != 0){
						fantasma[nav].pos_i--;
					}
					break;
				case 2:
					if(mapa.vet_mapa[pos_i][pos_j+1] != L"|" && pos_i != tam_j){
						fantasma[nav].pos_j++;
					}
					break;
				case 3:
					if(mapa.vet_mapa[pos_i][pos_j-1] != L"|" && pos_i != 0){
						fantasma[nav].pos_j--;
					}
					break;
			}
			break;
		case 2:
		case 3:
			switch(rand() % ia){
			case 0:
			case 1:
			case 2:
			case 3:
				if(player.pos_i < fantasma[nav].pos_i){
					if(mapa.vet_mapa[pos_i-1][pos_j] != L"|"){
						fantasma[nav].pos_i--;
					} else {
						switch(rand() % 2){
							case 0:
								fantasma[nav].pos_j--;
								break;
							case 1:
								fantasma[nav].pos_j++;
						}
					}
				} else if(player.pos_j < fantasma[nav].pos_j){
					if(mapa.vet_mapa[pos_i][pos_j-1] != L"|"){
						fantasma[nav].pos_j--;
					} else {
						switch(rand() % 2){
							case 0:
								fantasma[nav].pos_i--;
								break;
							case 1:
								fantasma[nav].pos_i++;
						}
					}
				} else if(player.pos_i > fantasma[nav].pos_i){
					if(mapa.vet_mapa[pos_i+1][pos_j] != L"|"){
						fantasma[nav].pos_i++;
					} else {
						switch(rand() % 2){
							case 0:
								fantasma[nav].pos_j--;
								break;
							case 1:
								fantasma[nav].pos_j++;
						}
					}
				} else if(player.pos_j > fantasma[nav].pos_j){
					if(mapa.vet_mapa[pos_i][pos_j+1] != L"|"){
						fantasma[nav].pos_j++;
					} else {
						switch(rand() % 2){
							case 0:
								fantasma[nav].pos_i;
								break;
							case 1:
								fantasma[nav].pos_i++;
						}
					}
				}
		}
	}
}

void gamePlay(){
	int i, j, tam_i = 12, tam_j = 19;
	mapa.vet_mapa[13][20], mapa.vet_pontos[14][20];
	player.simbolo = 67, player.pos_i = 1, player.pos_j = 2, player.nav = 0, player.pontos = 0, player.m_pontos = 0;
	fantasma[0].simbolo = 35,fantasma[0].pos_i = 6, fantasma[0].pos_j = 9, fantasma[0].nav = 0, fantasma[0].ia = 4;
	fantasma[1].simbolo = 36,fantasma[1].pos_i = 6, fantasma[1].pos_j = 10, fantasma[1].nav = 1, fantasma[1].ia = 4;
	fantasma[2].simbolo = 37,fantasma[2].pos_i = 7, fantasma[2].pos_j = 9, fantasma[2].nav = 2, fantasma[2].ia = 5;
	fantasma[3].simbolo = 38,fantasma[3].pos_i = 7, fantasma[3].pos_j = 10, fantasma[3].nav = 3, fantasma[3].ia = 6;

	loading();

	// Limpar Mapa Pontos
	for(i = 0; i <= tam_i; i++){
		for(j = 0; j <= tam_j; j++){
				mapa.vet_pontos[i][j] = ' ';
		}
	}
	// Fim Limpar Mapa Pontos

	do{

		limpaTela();

		// Captura Tecla

		funcKbhit();

		// Fim Captura Tela

		// Controle
		controle(player.pos_i, player.pos_j, tam_i, tam_j);
		// Fim Controle

		// Fantasmas
		fantasma_nav(fantasma[0].nav, fantasma[0].pos_i, fantasma[0].pos_j, fantasma[0].ia, tam_i, tam_j);
		fantasma_nav(fantasma[1].nav, fantasma[1].pos_i, fantasma[1].pos_j, fantasma[1].ia, tam_i, tam_j);
		fantasma_nav(fantasma[2].nav, fantasma[2].pos_i, fantasma[2].pos_j, fantasma[2].ia, tam_i, tam_j);
		fantasma_nav(fantasma[3].nav, fantasma[3].pos_i, fantasma[3].pos_j, fantasma[3].ia, tam_i, tam_j);


		// Fantasmas

	// Construção Mapa
		wprintf(L" -------------------------------------\n");
		for(i = 0; i <= tam_i; i++){
			for(j = 0; j <= tam_j; j++){
				if(mapa.vet_pontos[i][j] == 'X'){
					mapa.vet_mapa[i][j] = L" ";
				} else {

					if(j == 0){
						mapa.vet_mapa[i][j] = L"|";
					} else if(j == 3 && (i >= 4 && i <= 8)){
						mapa.vet_mapa[i][j] = L"|";
					} else if(j == 6 && (i >= 3 && i <= 9)){
						mapa.vet_mapa[i][j] = L"|";
					} else if(j == 8 && (i >= 6 && i <= 7)){
						mapa.vet_mapa[i][j] = L"|";
					} else if(j == 11 && (i >= 6 && i <= 7)){
						mapa.vet_mapa[i][j] = L"|";
					} else if(j == 13 && (i >= 3 && i <= 9)){
						mapa.vet_mapa[i][j] = L"|";
					} else if(j == 16 && (i >= 4 && i <= 8)){
						mapa.vet_mapa[i][j] = L"|";
					} else if(j == tam_j){
						mapa.vet_mapa[i][j] = L"|";
					} else {
						mapa.vet_mapa[i][j] = L"\u2022";
					}
				}

				if(player.simbolo == 67 && player.pontos > 1){
					mapa.vet_mapa[player.pos_i][player.pos_j] = L"O";
				} else {
					mapa.vet_mapa[player.pos_i][player.pos_j] = L"C";
				}


				mapa.vet_mapa[fantasma[0].pos_i][fantasma[0].pos_j] = L"#";
				mapa.vet_mapa[fantasma[1].pos_i][fantasma[1].pos_j] = L"$";
				mapa.vet_mapa[fantasma[2].pos_i][fantasma[2].pos_j] = L"%";
				mapa.vet_mapa[fantasma[3].pos_i][fantasma[3].pos_j] = L"&";

				if(mapa.vet_pontos[player.pos_i][player.pos_j] != 'X'){
					mapa.vet_pontos[player.pos_i][player.pos_j] = 'X';
						player.pontos+= 10;
						// Pontuação Máxima 2060
						if(player.pontos >= 2060){
							voceGanhou();
						}
				}
			}
		}

		for(i = 0; i <= tam_i; i++){
			for(j = 0; j <= tam_j; j++){
				fwprintf(stdout, L"%ls ", mapa.vet_mapa[i][j]);
			}
			wprintf(L"\n");
		}

		if((player.pos_i == fantasma[0].pos_i && player.pos_j == fantasma[0].pos_j)){
				vocePerdeu();
		} else if((player.pos_i == fantasma[1].pos_i && player.pos_j == fantasma[1].pos_j)) {
				vocePerdeu();
		} else if((player.pos_i == fantasma[2].pos_i && player.pos_j == fantasma[2].pos_j)) {
				vocePerdeu();
		} else if((player.pos_i == fantasma[3].pos_i && player.pos_j == fantasma[3].pos_j)) {
				vocePerdeu();
		}


		wprintf(L" -------------------------------------");

		// Pontuação

		wprintf(L"\n\t\t\t   Pontos: %04d  ", player.pontos-10);

		// Fim Pontuação

		if(player.simbolo == 67 && player.pontos > 1){
			player.simbolo = 79;
		} else {
			player.simbolo = 67;
		}
		wprintf(L"\n");
		usleep(mapa.dificuldade);
		// Fim Construção Mapa

	}while(1);
}

int telaInicial(){
	int pac = 0;
	wchar_t *vet_efeito[23];
	int 	i, pos_pac = 0, pontos = 0, cheat = 0;
	player.tecla = ' ';
	player.nav = 0;

	do{

		// Arte Pacman
		artePac();
		// Fim Arte Pacman

		// Efeito Pacman

		for(i = 0; i <= 22; i++){
			if(pos_pac == i){
				if(pac == 0){
					pac = 1;
					vet_efeito[i] = L"O";
				} else {
					pac = 0;
					vet_efeito[i] =  L"C";
				}
			} else{
				if(i >= pos_pac){
					vet_efeito[i] = L"\u2022";
				} else {
					vet_efeito[pos_pac-1] = L" ";
				}
			}
		}

		for(i = 0; i <= 21; i++){
		  fwprintf(stdout, L" %ls", vet_efeito[i]);
		}

		pos_pac++;

		if(pos_pac > 22){
			pos_pac = 0;
		}
		// Fim Efeito Pacman

		// Menu

		// Controle
		if(kbhit()){
			fflush(stdin);
			player.tecla = getchar();
			if((player.tecla == 'w') && player.nav > 0){
				player.nav--;
			} else if((player.tecla == 's') && player.nav < 5){
				player.nav++;
			}
			if(player.tecla == 'c'){
				cheat = 1;
			}
		}

		// Opções Menu
		switch(opcoesMenu(player.tecla, player.nav)){
			case 0:
				system("clear");
				gamePlay();
				player.tecla = ' ';
				break;
			case 1:
				instrucoes();
				player.tecla = ' ';
				break;
			case 2:
				dificuldade();
				player.tecla = ' ';
				break;
			case 3:
				pontuacao(pontos);
				player.tecla = ' ';
				break;
			case 4:
				creditos();
				player.tecla = ' ';
				break;
			case 5:
				sair(cheat);
				break;
		}

		// Fim Controle
		wprintf(L"\n");
		// Fim Menu
		usleep(200000);
		limpaTela();

	}while(1);

	return 0;
}

int main(int argc, char *argv[]){
	int i;
	mapa.dificuldade = 200000;

	//_setmode(_fileno(stdout), _O_U16TEXT);
	// Procedimentos Gerais
	esconderCursor();
	tempoRandom();
	pt_br();
	corJogo();
	// Fim Procedimentos Gerais

	// Tela Inicial
	system("clear");
	telaInicial();
	// Fim Tela Inicial

	return 0;
}
