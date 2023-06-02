#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
#include <windows.h>
using namespace std;

#define ESC    	27

/*
	Colocar MostrarColetaveisMissao() para mostrar as moedas ou doces das respectiveis fases,
	Criar fun��o para receber item ao completar
*/
//local para os structs
typedef struct _blocosColisao
{
  int x, y, altura, largura, tipo;
  bool colidido, cliqueMouse, coletado;
  void *spriteMascara;
  void *sprite;
}BlocoDeColisao;

//criando o vetor para blocos de colis�o
BlocoDeColisao *blocosColisao;
int qntBlocos = 40;

//=> Fases
int fases = 0;
bool pegouMissao = false;
bool inventario = true;
bool entrou = false;
void *inventarioImagem;

//checagem de itens coletados
int qntItensColetados = 0;
int qntMoedasColetadas = 0;
int qntDocesColetados = 0;
 
//vari�vel para controlar qual personagem ter� no mapa 
int variavelDeControle = 18;

//=====================> Tempo de Espera para Clique do Mouse <=====================
int Espera = 500;
double TempoDecorrido;
long long Inicio, Agora;
					
//===============================> Mouse <===============================
HWND janela;//coletando a janela
POINT P;//posi��o do Mouse
int indexItemColidido = 0;//coletandoBlocoColidido
bool colisaoMouse = false;//verificando se o mouse colidiu com algum bloco
  
//===============================> Invent�rio <===============================
int InvLargura = 400;
int InvAltura = 720;
int xInv = 1280 - InvLargura;
int yInv = 0;
 
//=====================> Fun��es <=====================
//=> �rea para Colis�es
bool ChecagemDeColisao(int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu);

void ChecagemDeColisaoDoMouse();

void ZerandoColisoes();

//= > �rea para Desenhar e Criar
void CriandoInvetario(int IndexInicial, int IndexFinal, int index);

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara);

void* carregarImagem(const char *sprite, int largura, int altura , int x, int y);

void CriandoPersonagem(int index);

void MostrarColetaveisMissao(int indexMenor, int indexMaior);

void MostrarColetaveis(int index);

//= > �rea para Lidar com Leveis
void LidandoComFases(void *cenario, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal, bool TemPersonagem);

void Menu(void *cenario, int &fases);

void SaindoDoTutorial();

void AtivandoFinal();

void CaixaDeTextos(int indexMenor, int indexMaior);

//= > �rea para Lidar com Personagens
void LidandoComPersonagem(int &index);

void LidandoComAsFolhas(int &index);

void LidandoComABruxa(int primeiroItemMissao, int ultimoItemMissao);

void LidandoComMissoes(int tipo);

void FinalizandoMissoes();

void FinalGuaxinim();

void FinalCat();


//= > �rea para Coletas de Itens
void ColetarItensFase();

void ColetarItensMissoes(int tipo, int quantidade);
  
//=================================================> JOGO <=================================================
int main()
{
  //===============================> Carrega som<=========================
    mciSendString("open .\\sons\\Kartelles_Seele.mp3 type MPEGVideo alias jogo", NULL, 0, 0); 
    
  //===============================> Lidando com a janela <===============================
  int pg = 1;
  char tecla;	
  unsigned long long gt1, gt2;//Clocks do Computador
  int fps = 60;
  
  initwindow(1280, 720, "Kartelles Seele");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do la�o para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  tecla = 0;//index do teclado

  //===============================> Blocos de Colis�o para o Mouse <===============================
  //Vetor que est� guardando os blocos
  blocosColisao = NULL;
  blocosColisao = (BlocoDeColisao *) malloc(sizeof(BlocoDeColisao) * qntBlocos);
  
  //===============================> Bot�es para troca de fases <===============================
  //bot�o para cima
  blocosColisao[0].x = 640;
  blocosColisao[0].y = 20;
  blocosColisao[0].altura = 64;
  blocosColisao[0].largura = 64;
  blocosColisao[0].tipo = 0;
  blocosColisao[0].colidido = false;
  blocosColisao[0].cliqueMouse = false;
  blocosColisao[0].coletado = false;
  blocosColisao[0].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Cima_Mascara.bmp", blocosColisao[0].largura, blocosColisao[0].altura, 0, 0);
  blocosColisao[0].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Cima.bmp", blocosColisao[0].largura, blocosColisao[0].altura, 0, 0);
  
  //bot�o para direita
  blocosColisao[1].altura = 64;
  blocosColisao[1].largura = 64;
  blocosColisao[1].x = 1250 - blocosColisao[1].largura;
  blocosColisao[1].y = 360;
  blocosColisao[1].tipo = 1;
  blocosColisao[1].colidido = false;
  blocosColisao[1].cliqueMouse = false;
  blocosColisao[1].coletado = false;
  blocosColisao[1].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Direita_Mascara.bmp", blocosColisao[1].largura, blocosColisao[1].altura, 0, 0);
  blocosColisao[1].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Direita.bmp", blocosColisao[1].largura, blocosColisao[1].altura, 0, 0);
  
  //bot�o para esquerda
  blocosColisao[2].altura = 64;
  blocosColisao[2].largura = 64;
  blocosColisao[2].x = 30;
  blocosColisao[2].y = 360;
  blocosColisao[2].tipo = 2;
  blocosColisao[2].colidido = false;
  blocosColisao[2].cliqueMouse = false;
  blocosColisao[2].coletado = false;
  blocosColisao[2].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Esquerda_Mascara.bmp", blocosColisao[2].largura, blocosColisao[2].altura, 0, 0);
  blocosColisao[2].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Esquerda.bmp", blocosColisao[2].largura, blocosColisao[2].altura, 0, 0);
  
  //bot�o para baixo
  blocosColisao[3].altura = 64;
  blocosColisao[3].largura = 64;
  blocosColisao[3].x = 640;
  blocosColisao[3].y = 720 - blocosColisao[3].altura - 20;
  blocosColisao[3].tipo = 3;
  blocosColisao[3].colidido = false;
  blocosColisao[3].cliqueMouse = false;
  blocosColisao[3].coletado = false;
  blocosColisao[3].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Baixo_Mascara.bmp", blocosColisao[3].largura, blocosColisao[3].altura, 0, 0);
  blocosColisao[3].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Baixo.bmp", blocosColisao[3].largura, blocosColisao[3].altura, 0, 0);
  
  //invent�rio
  blocosColisao[4].altura = 64;
  blocosColisao[4].largura = 64;
  blocosColisao[4].tipo = 4;
  blocosColisao[4].x = 1280 - blocosColisao[4].largura;
  blocosColisao[4].y = 30;
  blocosColisao[4].colidido = false;
  blocosColisao[4].cliqueMouse = false;
  blocosColisao[4].coletado = false;
  blocosColisao[4].spriteMascara = carregarImagem(".//Artes//Botoes//icone_Inventario_Mascara.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[4].sprite = carregarImagem(".//Artes//Botoes//icone_Inventario.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //===============================> ITENS <===============================
  //=>Ervas
  blocosColisao[5].x = 320;
  blocosColisao[5].y = 230;
  blocosColisao[5].altura = 54;
  blocosColisao[5].largura = 96;
  blocosColisao[5].tipo = 5;
  blocosColisao[5].colidido = false;
  blocosColisao[5].cliqueMouse = false;
  blocosColisao[5].coletado = false;
  blocosColisao[5].sprite = carregarImagem(".//Artes//Itens//ervas.bmp", blocosColisao[5].largura, blocosColisao[5].altura, 0, 0);
  blocosColisao[5].spriteMascara = carregarImagem(".//Artes//Itens//ervas_masc.bmp", blocosColisao[5].largura, blocosColisao[5].altura, 0, 0);
  
  //=>Pote de Sal
  blocosColisao[6].x = 900;
  blocosColisao[6].y = 540;
  blocosColisao[6].altura = 54;
  blocosColisao[6].largura = 96;
  blocosColisao[6].tipo = 5;
  blocosColisao[6].colidido = false;
  blocosColisao[6].cliqueMouse = false;
  blocosColisao[6].coletado = false;
  blocosColisao[6].sprite = carregarImagem(".//Artes//Itens//pote_de_sal.bmp", blocosColisao[6].largura, blocosColisao[6].altura, 0, 0);
  blocosColisao[6].spriteMascara = carregarImagem(".//Artes//Itens//pote_de_sal_masc.bmp", blocosColisao[6].largura, blocosColisao[6].altura, 0, 0);
  
  //=>Relic�rio
  blocosColisao[7].x = 670;
  blocosColisao[7].y = 240;
  blocosColisao[7].altura = 54;
  blocosColisao[7].largura = 96;
  blocosColisao[7].tipo = 5;
  blocosColisao[7].colidido = false;
  blocosColisao[7].cliqueMouse = false;
  blocosColisao[7].coletado = false;
  blocosColisao[7].sprite = carregarImagem(".//Artes//Itens//relicario.bmp", blocosColisao[7].largura, blocosColisao[7].altura, 0, 0);
  blocosColisao[7].spriteMascara = carregarImagem(".//Artes//Itens//relicario_masc.bmp", blocosColisao[7].largura, blocosColisao[7].altura, 0, 0);
  
  //=>Boneca
  blocosColisao[8].x = 1000;
  blocosColisao[8].y = 400;
  blocosColisao[8].altura = 54;
  blocosColisao[8].largura = 96;
  blocosColisao[8].tipo = 5;
  blocosColisao[8].colidido = false;
  blocosColisao[8].cliqueMouse = false;
  blocosColisao[8].coletado = false;
  blocosColisao[8].sprite = carregarImagem(".//Artes//Itens//boneca.bmp", blocosColisao[8].largura, blocosColisao[8].altura, 0, 0);
  blocosColisao[8].spriteMascara = carregarImagem(".//Artes//Itens//boneca_masc.bmp", blocosColisao[8].largura, blocosColisao[8].altura, 0, 0);
  
  //=>Estrela Judaica
  blocosColisao[9].x = 1100;
  blocosColisao[9].y = 130;
  blocosColisao[9].altura = 54;
  blocosColisao[9].largura = 96;
  blocosColisao[9].tipo = 5;
  blocosColisao[9].colidido = false;
  blocosColisao[9].cliqueMouse = false;
  blocosColisao[9].coletado = false;
  blocosColisao[9].sprite = carregarImagem(".//Artes//Itens//caixa_estrela.bmp", blocosColisao[9].largura, blocosColisao[9].altura, 0, 0);
  blocosColisao[9].spriteMascara = carregarImagem(".//Artes//Itens//caixa_estrela_masc.bmp", blocosColisao[9].largura, blocosColisao[9].altura, 0, 0);
  
  //Parte depois do Tutorial
  //=>COGUMELO
  blocosColisao[10].x = 900;
  blocosColisao[10].y = 255;
  blocosColisao[10].altura = 54;
  blocosColisao[10].largura = 96;
  blocosColisao[10].tipo = 5;
  blocosColisao[10].colidido = false;
  blocosColisao[10].cliqueMouse = false;
  blocosColisao[10].coletado = true;
  blocosColisao[10].sprite = carregarImagem(".//Artes//Itens//cogumelo.bmp", blocosColisao[10].largura, blocosColisao[10].altura, 0, 0);
  blocosColisao[10].spriteMascara = carregarImagem(".//Artes//Itens//cogumelo_masc.bmp", blocosColisao[10].largura, blocosColisao[10].altura, 0, 0);
  
  //=>Caveira
  blocosColisao[11].x = 595;
  blocosColisao[11].y = 255;
  blocosColisao[11].altura = 96;
  blocosColisao[11].largura = 54;
  blocosColisao[11].tipo = 5;
  blocosColisao[11].colidido = false;
  blocosColisao[11].cliqueMouse = false;
  blocosColisao[11].coletado = true;
  blocosColisao[11].sprite = carregarImagem(".//Artes//Itens//caveira.bmp", blocosColisao[11].largura, blocosColisao[11].altura, 0, 0);
  blocosColisao[11].spriteMascara = carregarImagem(".//Artes//Itens//caveira_masc.bmp", blocosColisao[11].largura, blocosColisao[11].altura, 0, 0);
  
  //=>Olho M�gico
  blocosColisao[12].x = 900;
  blocosColisao[12].y = 300;
  blocosColisao[12].altura = 54;
  blocosColisao[12].largura = 96;
  blocosColisao[12].tipo = 5;
  blocosColisao[12].colidido = false;
  blocosColisao[12].cliqueMouse = false;
  blocosColisao[12].coletado = true;
  blocosColisao[12].sprite = carregarImagem(".//Artes//Itens//olho_magico.bmp", blocosColisao[12].largura, blocosColisao[12].altura, 0, 0);
  blocosColisao[12].spriteMascara = carregarImagem(".//Artes//Itens//olho_magico_masc.bmp", blocosColisao[12].largura, blocosColisao[12].altura, 0, 0);
  
  //=>C�lice
  blocosColisao[13].x = 800;
  blocosColisao[13].y = 600;
  blocosColisao[13].altura = 54;
  blocosColisao[13].largura = 96;
  blocosColisao[13].tipo = 5;
  blocosColisao[13].colidido = false;
  blocosColisao[13].cliqueMouse = false;
  blocosColisao[13].coletado = true;
  blocosColisao[13].sprite = carregarImagem(".//Artes//Itens//calice.bmp", blocosColisao[13].largura, blocosColisao[13].altura, 0, 0);
  blocosColisao[13].spriteMascara = carregarImagem(".//Artes//Itens//calice_masc.bmp", blocosColisao[13].largura, blocosColisao[13].altura, 0, 0);
  
  //=>Giz
  blocosColisao[14].x = 1150;
  blocosColisao[14].y = 500;
  blocosColisao[14].altura = 54;
  blocosColisao[14].largura = 96;
  blocosColisao[14].tipo = 5;
  blocosColisao[14].colidido = false;
  blocosColisao[14].cliqueMouse = false;
  blocosColisao[14].coletado = true;
  blocosColisao[14].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  blocosColisao[14].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  
  //=>incenso
  blocosColisao[15].x = 1150;
  blocosColisao[15].y = 425;
  blocosColisao[15].altura = 64;
  blocosColisao[15].largura = 64;
  blocosColisao[15].tipo = 5;
  blocosColisao[15].colidido = false;
  blocosColisao[15].cliqueMouse = false;
  blocosColisao[15].coletado = false;
  blocosColisao[15].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  blocosColisao[15].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  
  //=>vela
  blocosColisao[16].x = 1200;
  blocosColisao[16].y = 300;
  blocosColisao[16].altura = 64;
  blocosColisao[16].largura = 64;
  blocosColisao[16].tipo = 5;
  blocosColisao[16].colidido = false;
  blocosColisao[16].cliqueMouse = false;
  blocosColisao[16].coletado = false;
  blocosColisao[16].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  blocosColisao[16].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  
  //=>adaga
  blocosColisao[17].x = 1050;
  blocosColisao[17].y = 100;
  blocosColisao[17].altura = 64;
  blocosColisao[17].largura = 64;
  blocosColisao[17].tipo = 5;
  blocosColisao[17].colidido = false;
  blocosColisao[17].cliqueMouse = false;
  blocosColisao[17].coletado = false;
  blocosColisao[17].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  blocosColisao[17].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  
  //===============================> PERSONAGENS <===============================
  //=>Folhas
  blocosColisao[18].x = 500;
  blocosColisao[18].y = 300;
  blocosColisao[18].altura = 173;
  blocosColisao[18].largura = 343;
  blocosColisao[18].tipo = 6;
  blocosColisao[18].colidido = false;
  blocosColisao[18].cliqueMouse = false;
  blocosColisao[18].coletado = false;
  blocosColisao[18].sprite = carregarImagem(".//Artes//Personagens//folhas.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  blocosColisao[18].spriteMascara = carregarImagem(".//Artes//Personagens//folhas_masc.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  
  //=>Folhas Outline
  blocosColisao[19].x = blocosColisao[18].x;
  blocosColisao[19].y = blocosColisao[18].y;
  blocosColisao[19].altura = blocosColisao[18].altura;
  blocosColisao[19].largura = blocosColisao[18].largura;
  blocosColisao[19].tipo = blocosColisao[18].tipo;
  blocosColisao[19].colidido = false;
  blocosColisao[19].cliqueMouse = false;
  blocosColisao[19].coletado = false;
  blocosColisao[19].sprite = carregarImagem(".//Artes//Personagens//folhas_out.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  blocosColisao[19].spriteMascara = carregarImagem(".//Artes//Personagens//folhas_out_masc.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  
  //=>Lontra
  blocosColisao[20].x = blocosColisao[18].x + 100;
  blocosColisao[20].y = blocosColisao[18].y;
  blocosColisao[20].altura = 255;
  blocosColisao[20].largura = 255;
  blocosColisao[20].tipo = 7;
  blocosColisao[20].colidido = false;
  blocosColisao[20].cliqueMouse = false;
  blocosColisao[20].coletado = false;
  blocosColisao[20].sprite = carregarImagem(".//Artes//Personagens//lontra.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  blocosColisao[20].spriteMascara = carregarImagem(".//Artes//Personagens//lontra_masc.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  
  //=>Lontra Outline
  blocosColisao[21].x = blocosColisao[20].x;
  blocosColisao[21].y = blocosColisao[20].y;
  blocosColisao[21].altura = blocosColisao[20].altura;
  blocosColisao[21].largura = blocosColisao[20].largura;
  blocosColisao[21].tipo = blocosColisao[20].tipo;
  blocosColisao[21].colidido = false;
  blocosColisao[21].cliqueMouse = false;
  blocosColisao[21].coletado = false;
  blocosColisao[21].sprite = carregarImagem(".//Artes//Personagens//lontra_outline.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  blocosColisao[21].spriteMascara = carregarImagem(".//Artes//Personagens//lontra_outline_masc.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  
  //=>Bruxa
  blocosColisao[22].x = 1100;
  blocosColisao[22].y = 130;
  blocosColisao[22].altura = 300;
  blocosColisao[22].largura = 300;
  blocosColisao[22].tipo = 8;
  blocosColisao[22].colidido = false;
  blocosColisao[22].cliqueMouse = false;
  blocosColisao[22].coletado = false;
  blocosColisao[22].sprite = carregarImagem(".//Artes//Personagens//bruxa.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  blocosColisao[22].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa_masc.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  
  //=>Bruxa Outline
  blocosColisao[23].x = blocosColisao[22].x;
  blocosColisao[23].y = blocosColisao[22].y;
  blocosColisao[23].altura = blocosColisao[22].altura;
  blocosColisao[23].largura = blocosColisao[22].largura;
  blocosColisao[23].tipo = blocosColisao[22].tipo;
  blocosColisao[23].colidido = false;
  blocosColisao[23].cliqueMouse = false;
  blocosColisao[23].coletado = false;
  blocosColisao[23].sprite = carregarImagem(".//Artes//Personagens//bruxa_outline.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  blocosColisao[23].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa_outline_masc.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  
  //=>Bruxa2
  blocosColisao[24].x = 600;
  blocosColisao[24].y = 130;
  blocosColisao[24].altura = 137;
  blocosColisao[24].largura = 137;
  blocosColisao[24].tipo = 7;
  blocosColisao[24].colidido = false;
  blocosColisao[24].cliqueMouse = false;
  blocosColisao[24].coletado = false;
  blocosColisao[24].sprite = carregarImagem(".//Artes//Personagens//bruxa2.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  blocosColisao[24].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa2_masc.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  
  //=>Bruxa2 Outline
  blocosColisao[25].x = blocosColisao[24].x;
  blocosColisao[25].y = blocosColisao[24].y;
  blocosColisao[25].altura = blocosColisao[24].altura;
  blocosColisao[25].largura = blocosColisao[24].largura;
  blocosColisao[25].tipo = blocosColisao[24].tipo;
  blocosColisao[25].colidido = false;
  blocosColisao[25].cliqueMouse = false;
  blocosColisao[25].coletado = false;
  blocosColisao[25].sprite = carregarImagem(".//Artes//Personagens//bruxa2_outline.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  blocosColisao[25].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa2_out_masc.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  
  //=>Guaxinim
  blocosColisao[26].x = 500;
  blocosColisao[26].y = 300;
  blocosColisao[26].altura = 255;
  blocosColisao[26].largura = 255;
  blocosColisao[26].tipo = 9;
  blocosColisao[26].colidido = false;
  blocosColisao[26].cliqueMouse = false;
  blocosColisao[26].coletado = false;
  blocosColisao[26].sprite = carregarImagem(".//Artes//Personagens//guaxi.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  blocosColisao[26].spriteMascara = carregarImagem(".//Artes//Personagens//guaxi_masc.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  
  //=>Guaxinim Outline
  blocosColisao[27].x = blocosColisao[26].x;
  blocosColisao[27].y = blocosColisao[26].y;
  blocosColisao[27].altura = blocosColisao[26].altura;
  blocosColisao[27].largura = blocosColisao[26].largura;
  blocosColisao[27].tipo = blocosColisao[26].tipo;
  blocosColisao[27].colidido = false;
  blocosColisao[27].cliqueMouse = false;
  blocosColisao[27].coletado = false;
  blocosColisao[27].sprite = carregarImagem(".//Artes//Personagens//guaxi_outline.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  blocosColisao[27].spriteMascara = carregarImagem(".//Artes//Personagens//guaxi_outline_masc.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  
  //=>Cat
  blocosColisao[28].x = 1100;
  blocosColisao[28].y = 130;
  blocosColisao[28].altura = 400;
  blocosColisao[28].largura = 170;
  blocosColisao[28].tipo = 10;
  blocosColisao[28].colidido = false;
  blocosColisao[28].cliqueMouse = false;
  blocosColisao[28].coletado = false;
  blocosColisao[28].sprite = carregarImagem(".//Artes//Personagens//cat.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  blocosColisao[28].spriteMascara = carregarImagem(".//Artes//Personagens//cat_masc.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  
  //=>Cat Outline
  blocosColisao[29].x = blocosColisao[28].x;
  blocosColisao[29].y = blocosColisao[28].y;
  blocosColisao[29].altura = blocosColisao[28].altura;
  blocosColisao[29].largura = blocosColisao[28].largura;
  blocosColisao[29].tipo = blocosColisao[28].tipo;
  blocosColisao[29].colidido = false;
  blocosColisao[29].cliqueMouse = false;
  blocosColisao[29].coletado = false;
  blocosColisao[29].sprite = carregarImagem(".//Artes//Personagens//cat_out.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  blocosColisao[29].spriteMascara = carregarImagem(".//Artes//Personagens//cat_out_masc.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  
  //===============================> ITENS DE MISS�ES <===============================
  //=>Moedas 1
  blocosColisao[30].x = 900;
  blocosColisao[30].y = 130;
  blocosColisao[30].altura = 64;
  blocosColisao[30].largura = 64;
  blocosColisao[30].tipo = 11;
  blocosColisao[30].colidido = false;
  blocosColisao[30].cliqueMouse = false;
  blocosColisao[30].coletado = true;
  blocosColisao[30].sprite = carregarImagem(".//Artes//Personagens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[30].spriteMascara = carregarImagem(".//Artes//Personagens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 2
  blocosColisao[31].x = blocosColisao[30].x;
  blocosColisao[31].y = blocosColisao[30].y;
  blocosColisao[31].altura = blocosColisao[30].altura;
  blocosColisao[31].largura = blocosColisao[30].largura;
  blocosColisao[31].tipo = blocosColisao[30].tipo;
  blocosColisao[31].colidido = false;
  blocosColisao[31].cliqueMouse = false;
  blocosColisao[31].coletado = true;
  blocosColisao[31].sprite = carregarImagem(".//Artes//Personagens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[31].spriteMascara = carregarImagem(".//Artes//Personagens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 3
  blocosColisao[32].x = blocosColisao[30].x;
  blocosColisao[32].y = blocosColisao[30].y;
  blocosColisao[32].altura = blocosColisao[30].altura;
  blocosColisao[32].largura = blocosColisao[30].largura;
  blocosColisao[32].tipo = blocosColisao[30].tipo;
  blocosColisao[32].colidido = false;
  blocosColisao[32].cliqueMouse = false;
  blocosColisao[32].coletado = true;
  blocosColisao[32].sprite = carregarImagem(".//Artes//Personagens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[32].spriteMascara = carregarImagem(".//Artes//Personagens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 4
  blocosColisao[33].x = blocosColisao[30].x;
  blocosColisao[33].y = blocosColisao[30].y;
  blocosColisao[33].altura = blocosColisao[30].altura;
  blocosColisao[33].largura = blocosColisao[30].largura;
  blocosColisao[33].tipo = blocosColisao[30].tipo;
  blocosColisao[33].colidido = false;
  blocosColisao[33].cliqueMouse = false;
  blocosColisao[33].coletado = true;
  blocosColisao[33].sprite = carregarImagem(".//Artes//Personagens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[33].spriteMascara = carregarImagem(".//Artes//Personagens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 5
  blocosColisao[34].x = blocosColisao[30].x;
  blocosColisao[34].y = blocosColisao[30].y;
  blocosColisao[34].altura = blocosColisao[30].altura;
  blocosColisao[34].largura = blocosColisao[30].largura;
  blocosColisao[34].tipo = blocosColisao[30].tipo;
  blocosColisao[34].colidido = false;
  blocosColisao[34].cliqueMouse = false;
  blocosColisao[34].coletado = true;
  blocosColisao[34].sprite = carregarImagem(".//Artes//Personagens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[34].spriteMascara = carregarImagem(".//Artes//Personagens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Doce 1
  blocosColisao[35].x = 900;
  blocosColisao[35].y = 130;
  blocosColisao[35].altura = 64;
  blocosColisao[35].largura = 64;
  blocosColisao[35].tipo = 12;
  blocosColisao[35].colidido = false;
  blocosColisao[35].cliqueMouse = false;
  blocosColisao[35].coletado = true;
  blocosColisao[35].sprite = carregarImagem(".//Artes//Personagens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[35].spriteMascara = carregarImagem(".//Artes//Personagens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 2
  blocosColisao[36].x = blocosColisao[35].x;
  blocosColisao[36].y = blocosColisao[35].y;
  blocosColisao[36].altura = blocosColisao[35].altura;
  blocosColisao[36].largura = blocosColisao[35].largura;
  blocosColisao[36].tipo = blocosColisao[35].tipo;
  blocosColisao[36].colidido = false;
  blocosColisao[36].cliqueMouse = false;
  blocosColisao[36].coletado = true;
  blocosColisao[36].sprite = carregarImagem(".//Artes//Personagens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[36].spriteMascara = carregarImagem(".//Artes//Personagens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 3
  blocosColisao[37].x = blocosColisao[35].x;
  blocosColisao[37].y = blocosColisao[35].y;
  blocosColisao[37].altura = blocosColisao[35].altura;
  blocosColisao[37].largura = blocosColisao[35].largura;
  blocosColisao[37].tipo = blocosColisao[35].tipo;
  blocosColisao[37].colidido = false;
  blocosColisao[37].cliqueMouse = false;
  blocosColisao[37].coletado = true;
  blocosColisao[37].sprite = carregarImagem(".//Artes//Personagens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[37].spriteMascara = carregarImagem(".//Artes//Personagens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 4
  blocosColisao[38].x = blocosColisao[35].x;
  blocosColisao[38].y = blocosColisao[35].y;
  blocosColisao[38].altura = blocosColisao[35].altura;
  blocosColisao[38].largura = blocosColisao[35].largura;
  blocosColisao[38].tipo = blocosColisao[35].tipo;
  blocosColisao[38].colidido = false;
  blocosColisao[38].cliqueMouse = false;
  blocosColisao[38].coletado = true;
  blocosColisao[38].sprite = carregarImagem(".//Artes//Personagens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[38].spriteMascara = carregarImagem(".//Artes//Personagens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 5
  blocosColisao[39].x = blocosColisao[35].x;
  blocosColisao[39].y = blocosColisao[35].y;
  blocosColisao[39].altura = blocosColisao[35].altura;
  blocosColisao[39].largura = blocosColisao[35].largura;
  blocosColisao[39].tipo = blocosColisao[35].tipo;
  blocosColisao[39].colidido = false;
  blocosColisao[39].cliqueMouse = false;
  blocosColisao[39].coletado = true;
  blocosColisao[39].sprite = carregarImagem(".//Artes//Personagens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[39].spriteMascara = carregarImagem(".//Artes//Personagens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //===============================> CAIXAS DE TEXTO <===============================
  //Texto tutorial 1
  blocosColisao[39].x = 0;
  blocosColisao[39].y = 601;
  blocosColisao[39].altura = 120;
  blocosColisao[39].largura = 1280;
  blocosColisao[39].tipo = 13;
  blocosColisao[39].colidido = false;
  blocosColisao[39].cliqueMouse = false;
  blocosColisao[39].coletado = true;
  blocosColisao[39].sprite = carregarImagem(".//Artes//Personagens//doce.bmp", blocosColisao[39].largura, blocosColisao[39].altura, 0, 0);
  blocosColisao[39].spriteMascara = carregarImagem(".//Artes//Personagens//doce_masc.bmp", blocosColisao[39].largura, blocosColisao[39].altura, 0, 0);
  
  //Texto tutorial 2
  
  //===============================> Cen�rios <===============================
  void **cenarios;
  int qntDeCenarios = 13;
  cenarios = (void **) malloc(sizeof(void *) * qntDeCenarios);

  cenarios[0] = carregarImagem(".//Artes//Cenarios//sala_tutas.bmp", 1280, 600, 0, 0);
  
  cenarios[1] = carregarImagem(".//Artes//Cenarios//quarto_tutas.bmp", 1280, 600, 0, 0);
  
  cenarios[2] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_1.bmp", 1280, 600, 0, 0);
  
  cenarios[3] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_2.bmp", 1280, 600, 0, 0);
  
  cenarios[4] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_3.bmp", 1280, 600, 0, 0);
  
  cenarios[5] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_4.bmp", 1280, 600, 0, 0);
  
  cenarios[6] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_5.bmp", 1280, 600, 0, 0);
  
  cenarios[7] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_6.bmp", 1280, 600, 0, 0);
  
  cenarios[8] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_7.bmp", 1280, 600, 0, 0);
  
  cenarios[9] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_8.bmp", 1280, 600, 0, 0);
  
  cenarios[10] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_9.bmp", 1280, 600, 0, 0);
  
  cenarios[11] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_10.bmp", 1280, 600, 0, 0);
  
  cenarios[12] = carregarImagem(".//Artes//Cenarios//menu.bmp", 1280, 720, 0, 0);
  
  //Imagem do Inventario
  inventarioImagem = carregarImagem(".//Artes//Itens//inventario.bmp", InvLargura, InvAltura, 0, 0);
  
  //=======================> Salvando o tick do computador Inicialmente <=======================
  gt1 = GetTickCount();//registrando o tick inicial do computador para intervalo de frames
  Inicio = GetTickCount();//registrando o tick inicial do computador para tempo de espera do click
  
  while(tecla != ESC)
  {
    //===============================> Coletando o Tick ao passar do tempo <===============================
    gt2 = GetTickCount();//coletando o tick atual do computador
	
    if (gt2-gt1 > 1000/fps)//setando teto para frames
	{
      gt1 = gt2;
      TempoDecorrido = gt2 - Inicio;
	  if(pg == 1) pg = 2; else pg = 1;//t�cnica de pagina��o
	  setactivepage(pg);
	  cleardevice();
  
      //=================> Lidando com a troca de fases <=================
      if(fases == 0)//menu
		Menu(cenarios[12], fases);
	  if(fases == 1)
      {
        LidandoComFases(cenarios[0], fases, false, false, true, false, inventario, 5, 6, 5, 9, false);
        SaindoDoTutorial();//Verificando se o Player coletou todos os itens para mudar de etapa
	  }//n�o h� segunda fase pela l�gica que criei de transi��o de fase
	  else if(fases == 3)
	  {
	    LidandoComFases(cenarios[1], fases, false, false, false, true, inventario, 7, 9, 5, 9, false);
	    SaindoDoTutorial();//Verificando se o Player coletou todos os itens para mudar de etapa
	  }
	  else if(fases == 4)
	  {
	    LidandoComFases(cenarios[2], fases, false, false, true, false, inventario, 10, 10, 10, 17, true);
	  }
	
	  else if(fases == 6)
	  {
	    variavelDeControle = 18;
	    LidandoComFases(cenarios[3], fases, true, false, pegouMissao, true, inventario, 11, 11, 10, 17, false);
	  }
	  else if(fases == 7)
	  {
	    LidandoComFases(cenarios[4], fases, false, true, true, false, inventario, 12, 12, 10, 17, false);
	  }
	  else if(fases == 8)
	  {
	    LidandoComFases(cenarios[6], fases, false, false, true, true, inventario, 14, 14, 10, 17, false);
	  }
	  else if(fases == 9)
	  {
	  	if(!entrou)
	  	{	
	      pegouMissao = true;
	      entrou = true;
		}
	    variavelDeControle = 22;
	    LidandoComFases(cenarios[5], fases, false, false, false, true, inventario, 13, 13, 10, 17, true);
	  }
	  else if(fases == 10)
	  {
	    variavelDeControle = 24;
	    LidandoComFases(cenarios[7], fases, false, false, true, true, inventario, 7, 9, 10, 17, true);
	  }
	  else if(fases == 12)
	  {
	    LidandoComFases(cenarios[8], fases, true, false, true, true, inventario, 7, 9, 10, 17, false);
	  }
	  else if(fases == 13)
	  {
	    variavelDeControle = 26;
	    LidandoComFases(cenarios[9], fases, false, true, true, false, inventario, 15, 15, 10, 17, true);
	  }
	  else if(fases == 14)
	  {
	    variavelDeControle = 28;
	    LidandoComFases(cenarios[11], fases, false, false, false, true, inventario, 17, 17, 10, 17, true);
	  }
	  else if(fases == 15)
	  {
	    LidandoComFases(cenarios[10], fases, false, false, false, true, inventario, 16, 16, 10, 17, false);
	  }
	  
      setvisualpage(pg);
    }//Fim do Condicional para Barramento de Frames
    
    //===============================> Input para sa�da do la�o <===============================
    fflush(stdin);
    if (kbhit())
	{
      tecla = getch();
    }
  }//Fim do La�o
  
  printf("\n\nFim do Programa");
  
  //===>liberando mem�ria dos blocos
  for(int i = 0; i < qntBlocos; i++)
  {
    free(blocosColisao[i].sprite);
    free(blocosColisao[i].spriteMascara);
  }
  free(blocosColisao);
  
  //===>liberando mem�ria dos cen�rios
  for(int i = 0; i < qntDeCenarios; i++)
  {
    free(cenarios[i]);
  }
  free(cenarios);
  
  closegraph();
  return 0;
}

//= > �rea para Desenhar e Criar
void CriandoPersonagem(int index)
{
  ChecagemDeColisao( P.x, P.y, blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].largura, blocosColisao[index].altura, blocosColisao[index].colidido);
  ChecagemDeColisaoDoMouse();
  
  if(!blocosColisao[index].colidido)//Detectando colis�o para por outline
    DesenhandoBotao( blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].sprite, blocosColisao[index].spriteMascara);
  else
    DesenhandoBotao( blocosColisao[index+1].x, blocosColisao[index+1].y, blocosColisao[index+1].sprite, blocosColisao[index+1].spriteMascara);
}

void CriandoInvetario(int IndexInicial, int IndexFinal, int index)
{
  int controleItensInventario = InvAltura/5;//Criando vari�vel para controlar a localidade de cada item no invent�rio.
  if(blocosColisao[index].tipo == 4 && blocosColisao[index].coletado == true)
  {
    putimage(xInv, yInv, inventarioImagem, COPY_PUT);//desenhando o invent�rio
    for(int j = IndexInicial; j < IndexFinal + 1; j++)//pegando quais itens tem pela fase para aparecer no invent�rio
    {
	  if(!blocosColisao[j].coletado)//modificando a imagem com base se o item foi coletado ou n�o
	  {
	  	if((j - IndexInicial + 1) <= 5)
	  	{
	      DesenhandoBotao(xInv + 50, controleItensInventario*(j - IndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), blocosColisao[j].sprite, blocosColisao[j].spriteMascara);	
		}
	    else if((j - IndexInicial + 1) > 5)
	    {
	      DesenhandoBotao(xInv + 200, controleItensInventario*(j - (IndexInicial + 5) + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), blocosColisao[j].sprite, blocosColisao[j].spriteMascara);
        }
      }
    }	
  }
}

void* carregarImagem(const char *sprite, int largura, int altura , int x, int y)
{	
  void *image;//vetor para bits brutos
  int aux = imagesize(x, y, largura, altura);//tamanho da imagem
  image = malloc(aux);//abrindo espa�o de mem�ria para o vetor com base no tamanho da imagem
  readimagefile(sprite, x, y, largura, altura);//lendo a imagem que queremos
  getimage(x, y, largura, altura, image);
  
  return image;
}

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara)
{
  putimage(xImagem, yImagem, spritesMascara, AND_PUT);	
  putimage(xImagem, yImagem, sprites, OR_PUT);
}

void MostrarColetaveis(int index)
{
  if(!blocosColisao[index].coletado)
	DesenhandoBotao( blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].sprite, blocosColisao[index].spriteMascara);
}

void MostrarColetaveisMissao(int indexMenor, int indexMaior)
{
	for(int i = indexMenor; i < indexMaior + 1; i++)
	{
		MostrarColetaveis(i);
	}
}

//= > �rea para Lidar com Leveis
void LidandoComFases(void *cenario, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal, bool TemPersonagem)
{
  //Desenhando Cen�rio
  putimage(0, 0, cenario, COPY_PUT);
  
  //Lidando com Personagem da fase, se houver
  if(TemPersonagem == true)
    LidandoComPersonagem(variavelDeControle);
  //===============================> Desenhando os Itens <===============================
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo quais itens tem na fase
  {
    MostrarColetaveis(i);
  }
  //===============================> Lidando com os Blocos <===============================
  for(int i = 0; i <= 4; i++)//pegando os bot�es bases para troca de fase e invent�rio
  { 
    //===============================> Blocos para Mudan�a de Fase <===============================
    if(blocosColisao[i].tipo == 0 && fasePraCima == true)//conferindo para quais lados ser�o as pr�ximas fases / fases anteriores
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);//Coletando a informa��o de colis�o e guardando dentro do item.
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
      
    if(blocosColisao[i].tipo == 1 && fasePraDireita == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
      
    if(blocosColisao[i].tipo == 2 && fasePraEsquerda == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
      
    if(blocosColisao[i].tipo == 3 && fasePraBaixo == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
 
    //===============================> Bot�o do Invent�rio <===============================
    if(i == 4 && inventario == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}

    //==================> A��es dos Bloco de Colis�o que n�o sejam colet�veis (Bot�es de mudan�a de fases e invent�rio) <==================
    if(blocosColisao[i].tipo == 0 && blocosColisao[i].cliqueMouse == true && fasePraCima == true)//bot�o fase Acima
	{
	  fase += 1;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 1 && blocosColisao[i].cliqueMouse == true && fasePraDireita == true)//bot�o fase para � Direita
	{	
	  fase += 2;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 2 && blocosColisao[i].cliqueMouse == true && fasePraEsquerda == true)//bot�o fase � Esquerda
	{	
	  fase -= 2;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
    else if(blocosColisao[i].tipo == 3 && blocosColisao[i].cliqueMouse == true && fasePraBaixo == true)//bot�o fase Abaixo
    {
	  fase -= 1;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 4 && inventario == true && blocosColisao[i].cliqueMouse == true)//bot�o para invent�rio
    {
      if(!blocosColisao[i].coletado)//abrindo o invent�rio
      {
        blocosColisao[i].x = xInv - blocosColisao[i].largura;//mudando a posi��o do �cone do invent�rio
        blocosColisao[i].coletado = true;
	  }
	  else//fechando o invent�rio
	  {
	    blocosColisao[i].x = 1280 - blocosColisao[i].largura;//reajustando a posi��o do �cone do invent�rio
	    blocosColisao[i].coletado = false;
	  }
	  blocosColisao[i].cliqueMouse = false;
	}
	
	//Criando o Invent�rio
    CriandoInvetario(InvIndexInicial, InvIndexFinal, i);
  }
  //===============================> Colis�o do Mouse <===============================
  (GetCursorPos(&P));
  (ScreenToClient(janela, &P));
      
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo a colis�o para cada Bloco de Colis�o
  {	
    ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);//Coletando a informa��o de colis�o e guardando dentro do item.
  }
  ChecagemDeColisaoDoMouse();
}

void Menu(void *cenario, int &fases)
{
  putimage(0, 0, cenario, COPY_PUT);
  if(GetKeyState(VK_LBUTTON)&0x80)
  {
    mciSendString("play jogo repeat", NULL, 0, 0); 
    fases ++;
  }
}

void SaindoDoTutorial()//Verificando se o Player coletou todos os itens para mudar de etapa
{
  if(qntItensColetados >= 5)
  {
    inventario = false;
    fases = 4;
    ZerandoColisoes();
  }
}

void AtivandoFinal()
{
  if(fases == 9)
  {
    //Caixas de Texto();
  }
}

void CaixaDeTextos(int indexMenor, int indexMaior)
{
  for(int i = indexMenor; i > indexMaior; i++)
  {
  	//blocos de texto
  }
}

//= > �rea para Lidar com Colis�es
void ChecagemDeColisaoDoMouse()
{
  if(colisaoMouse == false)
  {
    for(int i = 0; i < qntBlocos; i++)//checagem dos Itens para ver se h� algum colidido e Qual foi.
	{
	  colisaoMouse = blocosColisao[i].colidido;
	  if(colisaoMouse == true)//se h�, coleta qual item
	  {
	    indexItemColidido = i;
	    break;
	  }
	}
  }
  if(colisaoMouse == true)
  {
	for(int i = 0; i < qntBlocos; i++)//checagem se n�o h� mais colis�o.
	{
	  if(blocosColisao[i].colidido == true)//se h�, ent�o ignora a verifica��o
		break;
	  else if(i == qntBlocos - 1)//se n�o h�. ent�o mouse n�o est� colidido
	  {
		if(blocosColisao[i].colidido == false) colisaoMouse = false;
	  }
	}
  }
  
  //===============================> Captura de Inputs <===============================
  if(TempoDecorrido > Espera)
  {
	if(GetKeyState(VK_LBUTTON)&0x80 && colisaoMouse == true)//Input do Mouse
	{
	  Inicio = GetTickCount();//resetando o tempo de espera
	  blocosColisao[indexItemColidido].cliqueMouse = true;
	  ColetarItensFase();
	  ColetarItensMissoes(11, qntMoedasColetadas);
	  ColetarItensMissoes(12, qntDocesColetados);
	}
  }
}

void ZerandoColisoes()
{
  //Fazendo com que todos os itens desverifiquem as colis�es(�til para situa��es em que o item acabou saindo da tela com a informa��o q est� c�lidido e bug o jogo)
  for(int i = 0; i < qntBlocos; i++)
    blocosColisao[i].colidido = false;
}

bool ChecagemDeColisao( int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )//checagem se o mouse est� � direita e em baixo do v�rtice superior esquerdo
  {
    if( xColisor <= (xColidido + larguraColidido) && yColisor <= (yColidido + alturaColidido) )//checagem se o mouse est� � esquerda e em cima do v�rtice inferior direito
      colidiu = true;
    else
      colidiu = false;
  }
  else
    colidiu = false;
}

//= > �rea para Lidar com Personagens
void LidandoComPersonagem(int &index)//Colentando o tipo de personagem que teve intera��o e reagindo conforme devido
{
  CriandoPersonagem(index);
  if(blocosColisao[index].cliqueMouse == true && blocosColisao[index].coletado == false)
  {
	switch(blocosColisao[index].tipo)
	{
	  case 6:
	    LidandoComAsFolhas(index);//Intera��o com as folhas de onde sai a Lontra
	    break;
	  case 7:
	    //Caixas de Texto()
	    break;
	  case 8:
	    LidandoComABruxa(10, 17);
	    break;
	  case 9:
	    LidandoComMissoes(11);
	    break;
	  case 10:
	    LidandoComMissoes(12);
	    break;
	}
	blocosColisao[index].coletado = true;
  }
}

void LidandoComAsFolhas(int &index)//Intera��o com as folhas de onde sai a Lontra
{
  index += 2;
  ZerandoColisoes();
}

void LidandoComABruxa(int primeiroItemMissao, int ultimoItemMissao)//Ativando os itens a serem coletados
{
  for(int i = primeiroItemMissao; i < ultimoItemMissao; i++)
  {
    blocosColisao[i].coletado = false;
  }
  inventario = true;
}

void LidandoComMissoes(int tipo)//Ativando os itens a serem coletados adicionalmente para recompensa no final
{
  for(int i = 0; i < qntBlocos; i++)
  {
    if(blocosColisao[i].tipo == tipo)
      blocosColisao[i].coletado = false;
  }
}

void FinalizandoMissoes()
{
  if(qntItensColetados >= 15)
    AtivandoFinal();
  else if(qntDocesColetados >= 5)
    FinalCat();
  else if(qntMoedasColetadas >= 5)
    FinalGuaxinim();
}

void FinalGuaxinim()
{
  if(fases == 14)
  {
    //Caixas de texto()
    qntItensColetados++;
	qntMoedasColetadas = 0;
  }
}

void FinalCat()
{
  if(fases == 15)
  {
    //Caixas de texto()
    qntItensColetados++;
	qntMoedasColetadas = 0;
  }
}
//= > �rea para Coletas de Itens
void ColetarItensFase()
{
  if(blocosColisao[indexItemColidido].tipo == 5 && blocosColisao[indexItemColidido].coletado == false)//Captura de Colet�veis da Fase
  {	
    qntItensColetados++;//registrando itens coletados
	blocosColisao[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
  }
}

void ColetarItensMissoes(int tipo, int quantidade)
{
  if(blocosColisao[indexItemColidido].tipo == tipo && blocosColisao[indexItemColidido].coletado == false)//Captura de Colet�veis da Miss�o
  {
  	quantidade++;
    blocosColisao[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
  }
}








