#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
#include <windows.h>
using namespace std;

#define ESC    	27

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
int qntBlocos = 30;

//checagem de itens coletados
int qntItensColetados = 0;
 
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

//= > �rea para Lidar com leveis
void LidandoComFases(void *cenario, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal);

void Menu(void *cenario, int &fases);

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
int InvLargura = 90;
int InvAltura = 720;
int xInv = 1280 - InvLargura;
int yInv = 0;
  
//=================================================> JOGO <=================================================
int main()
{
  //===============================> Carrega som<=========================
    mciSendString("open .\\sons\\my.mp3 type MPEGVideo alias fundo2", NULL, 0, 0); 
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
  blocosColisao[5].altura = 64;
  blocosColisao[5].largura = 64;
  blocosColisao[5].tipo = 5;
  blocosColisao[5].colidido = false;
  blocosColisao[5].cliqueMouse = false;
  blocosColisao[5].coletado = false;
  blocosColisao[5].sprite = carregarImagem(".//Artes//Itens//ervas.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[5].spriteMascara = carregarImagem(".//Artes//Itens//ervas_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>Pote de Sal
  blocosColisao[6].x = 900;
  blocosColisao[6].y = 540;
  blocosColisao[6].altura = 64;
  blocosColisao[6].largura = 64;
  blocosColisao[6].tipo = 5;
  blocosColisao[6].colidido = false;
  blocosColisao[6].cliqueMouse = false;
  blocosColisao[6].coletado = false;
  blocosColisao[6].sprite = carregarImagem(".//Artes//Itens//pote_de_sal.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[6].spriteMascara = carregarImagem(".//Artes//Itens//pote_de_sal_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>Relic�rio
  blocosColisao[7].x = 670;
  blocosColisao[7].y = 240;
  blocosColisao[7].altura = 64;
  blocosColisao[7].largura = 64;
  blocosColisao[7].tipo = 5;
  blocosColisao[7].colidido = false;
  blocosColisao[7].cliqueMouse = false;
  blocosColisao[7].coletado = false;
  blocosColisao[7].sprite = carregarImagem(".//Artes//Itens//relicario.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[7].spriteMascara = carregarImagem(".//Artes//Itens//relicario_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>Boneca
  blocosColisao[8].x = 1000;
  blocosColisao[8].y = 400;
  blocosColisao[8].altura = 64;
  blocosColisao[8].largura = 64;
  blocosColisao[8].tipo = 5;
  blocosColisao[8].colidido = false;
  blocosColisao[8].cliqueMouse = false;
  blocosColisao[8].coletado = false;
  blocosColisao[8].sprite = carregarImagem(".//Artes//Itens//boneca.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[8].spriteMascara = carregarImagem(".//Artes//Itens//boneca_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>Estrela Judaica
  blocosColisao[9].x = 1100;
  blocosColisao[9].y = 130;
  blocosColisao[9].altura = 64;
  blocosColisao[9].largura = 64;
  blocosColisao[9].tipo = 5;
  blocosColisao[9].colidido = false;
  blocosColisao[9].cliqueMouse = false;
  blocosColisao[9].coletado = false;
  blocosColisao[9].sprite = carregarImagem(".//Artes//Itens//caixa_estrela.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[9].spriteMascara = carregarImagem(".//Artes//Itens//caixa_estrela_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>COGUMELO
  blocosColisao[10].x = 900;
  blocosColisao[10].y = 255;
  blocosColisao[10].altura = 64;
  blocosColisao[10].largura = 64;
  blocosColisao[10].tipo = 5;
  blocosColisao[10].colidido = false;
  blocosColisao[10].cliqueMouse = false;
  blocosColisao[10].coletado = false;
  blocosColisao[10].sprite = carregarImagem(".//Artes//Itens//cogumelo.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[10].spriteMascara = carregarImagem(".//Artes//Itens//cogumelo_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>Caveira
  blocosColisao[11].x = 595;
  blocosColisao[11].y = 255;
  blocosColisao[11].altura = 64;
  blocosColisao[11].largura = 64;
  blocosColisao[11].tipo = 5;
  blocosColisao[11].colidido = false;
  blocosColisao[11].cliqueMouse = false;
  blocosColisao[11].coletado = false;
  blocosColisao[11].sprite = carregarImagem(".//Artes//Itens//caveira.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[11].spriteMascara = carregarImagem(".//Artes//Itens//carveira_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>Olho M�gico
  blocosColisao[12].x = 900;
  blocosColisao[12].y = 300;
  blocosColisao[12].altura = 64;
  blocosColisao[12].largura = 64;
  blocosColisao[12].tipo = 5;
  blocosColisao[12].colidido = false;
  blocosColisao[12].cliqueMouse = false;
  blocosColisao[12].coletado = false;
  blocosColisao[12].sprite = carregarImagem(".//Artes//Itens//olho_magico.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[12].spriteMascara = carregarImagem(".//Artes//Itens//olho_magico_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>C�lice
  blocosColisao[13].x = 800;
  blocosColisao[13].y = 600;
  blocosColisao[13].altura = 64;
  blocosColisao[13].largura = 64;
  blocosColisao[13].tipo = 5;
  blocosColisao[13].colidido = false;
  blocosColisao[13].cliqueMouse = false;
  blocosColisao[13].coletado = false;
  blocosColisao[13].sprite = carregarImagem(".//Artes//Itens//calice.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[13].spriteMascara = carregarImagem(".//Artes//Itens//calice_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //=>Giz
  blocosColisao[14].x = 1150;
  blocosColisao[14].y = 500;
  blocosColisao[14].altura = 64;
  blocosColisao[14].largura = 64;
  blocosColisao[14].tipo = 5;
  blocosColisao[14].colidido = false;
  blocosColisao[14].cliqueMouse = false;
  blocosColisao[14].coletado = false;
  blocosColisao[14].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[14].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //
  blocosColisao[15].x = 312;
  blocosColisao[15].y = 432;
  blocosColisao[15].altura = 64;
  blocosColisao[15].largura = 64;
  blocosColisao[15].tipo = 5;
  blocosColisao[15].colidido = false;
  blocosColisao[15].cliqueMouse = false;
  blocosColisao[15].coletado = false;
  blocosColisao[15].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[15].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  blocosColisao[16].x = 1100;
  blocosColisao[16].y = 130;
  blocosColisao[16].altura = 64;
  blocosColisao[16].largura = 64;
  blocosColisao[16].tipo = 5;
  blocosColisao[16].colidido = false;
  blocosColisao[16].cliqueMouse = false;
  blocosColisao[16].coletado = false;
  blocosColisao[16].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[16].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  blocosColisao[17].x = 1100;
  blocosColisao[17].y = 130;
  blocosColisao[17].altura = 64;
  blocosColisao[17].largura = 64;
  blocosColisao[17].tipo = 5;
  blocosColisao[17].colidido = false;
  blocosColisao[17].cliqueMouse = false;
  blocosColisao[17].coletado = false;
  blocosColisao[17].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[17].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //===============================> PERSONAGENS <===============================
  //=>Folhas
  blocosColisao[18].x = 500;
  blocosColisao[18].y = 300;
  blocosColisao[18].altura = 173;
  blocosColisao[18].largura = 343;
  blocosColisao[18].tipo = 5;
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
  blocosColisao[20].tipo = 5;
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
  blocosColisao[22].tipo = 5;
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
  blocosColisao[23].tipo = 5;
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
  blocosColisao[24].tipo = 5;
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
  blocosColisao[26].tipo = 5;
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
  blocosColisao[28].tipo = 5;
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

  //===============================> Fases <===============================
  int fases = 0;
  bool pegouMissao = false;
  bool inventario = true;
  
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
        LidandoComFases(cenarios[0], fases, false, false, true, false, inventario, 5, 6, 5, 9);
        if(qntItensColetados >= 5)
        {
	      inventario = false;
          fases = 4;
          ZerandoColisoes();
		}
	  }//n�o h� segunda fase pela l�gica que criei de transi��o de fase
	  
	  else if(fases == 3)
	  {
	    LidandoComFases(cenarios[1], fases, false, false, false, true, inventario, 7, 9, 5, 9);
	    if(qntItensColetados >= 5)
	    {
	      inventario = false;
          fases = 4;
          ZerandoColisoes();
		}
	  }
	  
	  else if(fases == 4)
	  {
	    LidandoComFases(cenarios[2], fases, false, false, true, false, inventario, 10, 10, 10, 17);
	    if(!blocosColisao[18].cliqueMouse)
	      CriandoPersonagem(18);
	    else
	    {	
	      blocosColisao[18].colidido = false;
		  CriandoPersonagem(20);
		}
	  }
	  
	  else if(fases == 6)
	  {
	    LidandoComFases(cenarios[3], fases, true, false, pegouMissao, true, inventario, 11, 11, 10, 17);
	  }
	  
	  else if(fases == 7)
	  {
	    LidandoComFases(cenarios[4], fases, false, true, true, false, inventario, 12, 12, 10, 17);
	  }
	  
	  else if(fases == 8)
	  {
	    LidandoComFases(cenarios[6], fases, false, false, true, true, inventario, 14, 14, 10, 17);
	  }
	  
	  else if(fases == 9)
	  {
	    LidandoComFases(cenarios[5], fases, false, false, false, true, inventario, 13, 13, 10, 17);
	    CriandoPersonagem(22);
	    pegouMissao = true;
	  }
	  
	  else if(fases == 10)
	  {
	    LidandoComFases(cenarios[7], fases, false, false, true, true, inventario, 7, 9, 10, 17);
	    inventario = true;
	    CriandoPersonagem(24);
	  }
	  
	  else if(fases == 12)
	  {
	    LidandoComFases(cenarios[8], fases, true, false, true, true, inventario, 7, 9, 10, 17);
	  }
	  else if(fases == 13)
	  {
	    LidandoComFases(cenarios[9], fases, false, true, true, false, inventario, 7, 9, 5, 9);
	    CriandoPersonagem(26);
	  }
	  
	  else if(fases == 14)
	  {
	    LidandoComFases(cenarios[11], fases, false, false, false, true, inventario, 7, 9, 5, 9);
	    CriandoPersonagem(28);
	  }
	  
	  else if(fases == 15)
	  {
	    LidandoComFases(cenarios[10], fases, false, false, false, true, inventario, 7, 9, 5, 9);
	  }
	  
      setvisualpage(pg);
    }
    
    //===============================> Input para sa�da do la�o <===============================
    fflush(stdin);
    if (kbhit())
	{
      tecla = getch();
    }
  }
  
  printf("\n\nFim do Programa");
  
  for(int i = 0; i<qntBlocos; i++)//liberando mem�ria dos blocos
  {
    free(blocosColisao[i].sprite);
  	free(blocosColisao[i].spriteMascara);
  }
  free(blocosColisao);
  
  for(int i = 0; i < qntDeCenarios; i++)//liberando mem�ria dos cen�rios
  {
    free(cenarios[i]);
  }
  free(cenarios);
  
  closegraph();
  return 0; 
}

void CriandoPersonagem(int index)
{
  ChecagemDeColisao( P.x, P.y, blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].largura, blocosColisao[index].altura, blocosColisao[index].colidido);
  ChecagemDeColisaoDoMouse();
  
  if(!blocosColisao[index].colidido)//Detectando colis�o para por outline
    DesenhandoBotao( blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].sprite, blocosColisao[index].spriteMascara);
  else
    DesenhandoBotao( blocosColisao[index+1].x, blocosColisao[index+1].y, blocosColisao[index+1].sprite, blocosColisao[index+1].spriteMascara);
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

void LidandoComFases(void *cenario, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal)
{
  //Desenhando Cen�rio
  putimage(0, 0, cenario, COPY_PUT);
  
  //===============================> Desenhando os Itens <===============================
  setfillstyle(1,RGB(255, 255, 0));
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo quais itens tem na fase
  {
	if(!blocosColisao[i].coletado)
	  DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
  }

  //===============================> Lidando com os Blocos <===============================
  setfillstyle(1,RGB(255, 0, 0));
  for(int i = 0; i <= 4; i++)//pegando os bot�es bases para troca de fase e invent�rio
  {
    //Coletando a informa��o de colis�o e guardando dentro do item.
    
    //===============================> Blocos para Mudan�a de Fase <===============================
    if(blocosColisao[i].tipo == 0 && fasePraCima == true)//conferindo para quais lados ser�o as pr�ximas fases / fases anteriores
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
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

void CriandoInvetario(int IndexInicial, int IndexFinal, int index)
{
  int controleItensInventario = InvAltura/(IndexFinal - IndexInicial + 1);//Criando vari�vel para controlar a localidade de cada item no invent�rio.
  if(blocosColisao[index].tipo == 4 && blocosColisao[index].coletado == true)
  {
    setfillstyle(1,RGB(50, 125, 255));
    bar(blocosColisao[index].x + blocosColisao[index].largura, 0, 1280, 720);//desenhando o invent�rio
    for(int j = IndexInicial; j < IndexFinal + 1; j++)//pegando quais itens tem pela fase para aparecer no invent�rio
    {
	  if(!blocosColisao[j].coletado)//modificando a imagem com base se o item foi coletado ou n�o
	  {
	    setfillstyle(1,RGB(50, 125, 50));
	    bar(xInv + (InvLargura/2) - (blocosColisao[index].largura/2), controleItensInventario*(j - IndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), xInv + (InvLargura/2) + (blocosColisao[index].largura/2), controleItensInventario*(j - IndexInicial + 1) - (controleItensInventario/2) + (blocosColisao[j].largura/2));
	  }
	  else
      {
	    setfillstyle(1,RGB(0, 0, 0));
	    bar(xInv + (InvLargura/2) - (blocosColisao[index].largura/2), controleItensInventario*(j - IndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), xInv + (InvLargura/2) + (blocosColisao[index].largura/2), controleItensInventario*(j - IndexInicial + 1) - (controleItensInventario/2) + (blocosColisao[j].largura/2));
      }
    }
  }	
}

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
  if(GetKeyState(VK_LBUTTON)&0x80 && colisaoMouse == true)//Input do Mouse
  {
    if(TempoDecorrido > Espera)
    {
      Inicio = GetTickCount();//resetando o tempo de espera
      blocosColisao[indexItemColidido].cliqueMouse = true;
      if(blocosColisao[indexItemColidido].tipo == 5 && blocosColisao[indexItemColidido].coletado == false)//Captura de Colet�veis
      {	
        qntItensColetados++;//registrando itens coletados
        blocosColisao[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
	  }
	}
  }
}

void ZerandoColisoes()
{
  //Fazendo com que todos os itens desverifiquem as colis�es(�til para situa��es em que o item acabou saindo da tela com a informa��o q est� c�lidido e bug o jogo)
  for(int i = 0; i < qntBlocos; i++)
    blocosColisao[i].colidido = false;
}



void Menu(void *cenario, int &fases){
  mciSendString("play fundo2", NULL, 0, 0);
  putimage(0, 0, cenario, COPY_PUT);
  if(GetKeyState(VK_LBUTTON)&0x80)
  {
    mciSendString("stop fundo2", NULL, 0, 0);
    mciSendString("play jogo", NULL, 0, 0); 
    fases ++;
  }
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

/*
largura de uma letra na tela = 4 pixeis;
strcpy() -> Copia a string para dentro da vari�vel
strcat() -> Junta duas strings
strlen() -> Nos fala o tamanho de uma string
strcmp() -> Compara duas strings
gets()   -> Input para strings, muito melhor que scanf
puts()   -> Print para string, mais f�cil que printf
*/
 
