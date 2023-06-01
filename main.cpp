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
int qntBlocos = 31;

//checagem de itens coletados
int qntItensColetados = 0;
 
//Area para funcoes
void* carregarImagem(const char *sprite, int largura, int altura , int x, int y);

void ChecagemDeColisaoDoBloco(int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu);

void LidandoComFases(void *cenario, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal);

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara);

void ChecagemDeColisaoDoMouse(int indexInicial, int indexFinal);

void CapturaDeInput_e_Acao();

//=====================> Tempo de Espera para Clique do Mouse <=====================
int Espera = 500;//Tempo de Espera do "CoolDown"
double TempoDecorrido;//Registro do Tempo Decorrido para compara��o com o de Espera
long long Inicio, Agora;//Vari�veis para contagem do Tempo Decorrido
					
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
  //===============================> Lidando com a janela <===============================
  int pg = 1;
  char tecla;	
  unsigned long long gt1, gt2;//Clocks do Computador
  int fps = 60;//quantidade M�xima de frames por segundo
  
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
  
  //Itens
  blocosColisao[5].x = 320;
  blocosColisao[5].y = 230;
  blocosColisao[5].altura = 64;
  blocosColisao[5].largura = 64;
  blocosColisao[5].tipo = 5;
  blocosColisao[5].colidido = false;
  blocosColisao[5].cliqueMouse = false;
  blocosColisao[5].coletado = false;
  blocosColisao[5].sprite = NULL;
  blocosColisao[5].spriteMascara = NULL;
  
  blocosColisao[6].x = 900;
  blocosColisao[6].y = 540;
  blocosColisao[6].altura = 64;
  blocosColisao[6].largura = 64;
  blocosColisao[6].tipo = 5;
  blocosColisao[6].colidido = false;
  blocosColisao[6].cliqueMouse = false;
  blocosColisao[6].coletado = false;
  blocosColisao[6].sprite = NULL;
  blocosColisao[6].spriteMascara = NULL;
  
  blocosColisao[7].x = 670;
  blocosColisao[7].y = 240;
  blocosColisao[7].altura = 64;
  blocosColisao[7].largura = 64;
  blocosColisao[7].tipo = 5;
  blocosColisao[7].colidido = false;
  blocosColisao[7].cliqueMouse = false;
  blocosColisao[7].coletado = false;
  blocosColisao[7].sprite = NULL;
  blocosColisao[7].spriteMascara = NULL;
  
  blocosColisao[8].x = 1000;
  blocosColisao[8].y = 400;
  blocosColisao[8].altura = 64;
  blocosColisao[8].largura = 64;
  blocosColisao[8].tipo = 5;
  blocosColisao[8].colidido = false;
  blocosColisao[8].cliqueMouse = false;
  blocosColisao[8].coletado = false;
  blocosColisao[8].sprite = NULL;
  blocosColisao[8].spriteMascara = NULL;
  
  blocosColisao[9].x = 1100;
  blocosColisao[9].y = 130;
  blocosColisao[9].altura = 64;
  blocosColisao[9].largura = 64;
  blocosColisao[9].tipo = 5;
  blocosColisao[9].colidido = false;
  blocosColisao[9].cliqueMouse = false;
  blocosColisao[9].coletado = false;
  blocosColisao[9].sprite = NULL;
  blocosColisao[9].spriteMascara = NULL;
  
  blocosColisao[10].x = 1100;
  blocosColisao[10].y = 130;
  blocosColisao[10].altura = 64;
  blocosColisao[10].largura = 64;
  blocosColisao[10].tipo = 5;
  blocosColisao[10].colidido = false;
  blocosColisao[10].cliqueMouse = false;
  blocosColisao[10].coletado = false;
  blocosColisao[10].sprite = NULL;
  blocosColisao[10].spriteMascara = NULL;
  
  blocosColisao[11].x = 1100;
  blocosColisao[11].y = 130;
  blocosColisao[11].altura = 64;
  blocosColisao[11].largura = 64;
  blocosColisao[11].tipo = 5;
  blocosColisao[11].colidido = false;
  blocosColisao[11].cliqueMouse = false;
  blocosColisao[11].coletado = false;
  blocosColisao[11].sprite = NULL;
  blocosColisao[11].spriteMascara = NULL;
  
  blocosColisao[12].x = 1100;
  blocosColisao[12].y = 130;
  blocosColisao[12].altura = 64;
  blocosColisao[12].largura = 64;
  blocosColisao[12].tipo = 5;
  blocosColisao[12].colidido = false;
  blocosColisao[12].cliqueMouse = false;
  blocosColisao[12].coletado = false;
  blocosColisao[12].sprite = NULL;
  blocosColisao[12].spriteMascara = NULL;
  
  blocosColisao[13].x = 1100;
  blocosColisao[13].y = 130;
  blocosColisao[13].altura = 64;
  blocosColisao[13].largura = 64;
  blocosColisao[13].tipo = 5;
  blocosColisao[13].colidido = false;
  blocosColisao[13].cliqueMouse = false;
  blocosColisao[13].coletado = false;
  blocosColisao[13].sprite = NULL;
  blocosColisao[13].spriteMascara = NULL;
  
  blocosColisao[13].x = 1100;
  blocosColisao[13].y = 130;
  blocosColisao[13].altura = 64;
  blocosColisao[13].largura = 64;
  blocosColisao[13].tipo = 5;
  blocosColisao[13].colidido = false;
  blocosColisao[13].cliqueMouse = false;
  blocosColisao[13].coletado = false;
  blocosColisao[13].sprite = NULL;
  blocosColisao[13].spriteMascara = NULL;
  
  blocosColisao[14].x = 1100;
  blocosColisao[14].y = 130;
  blocosColisao[14].altura = 64;
  blocosColisao[14].largura = 64;
  blocosColisao[14].tipo = 5;
  blocosColisao[14].colidido = false;
  blocosColisao[14].cliqueMouse = false;
  blocosColisao[14].coletado = false;
  blocosColisao[14].sprite = NULL;
  blocosColisao[14].spriteMascara = NULL;
  
  blocosColisao[15].x = 1100;
  blocosColisao[15].y = 130;
  blocosColisao[15].altura = 64;
  blocosColisao[15].largura = 64;
  blocosColisao[15].tipo = 5;
  blocosColisao[15].colidido = false;
  blocosColisao[15].cliqueMouse = false;
  blocosColisao[15].coletado = false;
  blocosColisao[15].sprite = NULL;
  blocosColisao[15].spriteMascara = NULL;
  
  blocosColisao[16].x = 1100;
  blocosColisao[16].y = 130;
  blocosColisao[16].altura = 64;
  blocosColisao[16].largura = 64;
  blocosColisao[16].tipo = 5;
  blocosColisao[16].colidido = false;
  blocosColisao[16].cliqueMouse = false;
  blocosColisao[16].coletado = false;
  blocosColisao[16].sprite = NULL;
  blocosColisao[16].spriteMascara = NULL;
  
  blocosColisao[17].x = 1100;
  blocosColisao[17].y = 130;
  blocosColisao[17].altura = 64;
  blocosColisao[17].largura = 64;
  blocosColisao[17].tipo = 5;
  blocosColisao[17].colidido = false;
  blocosColisao[17].cliqueMouse = false;
  blocosColisao[17].coletado = false;
  blocosColisao[17].sprite = NULL;
  blocosColisao[17].spriteMascara = NULL;
  
  blocosColisao[18].x = 1100;
  blocosColisao[18].y = 130;
  blocosColisao[18].altura = 64;
  blocosColisao[18].largura = 64;
  blocosColisao[18].tipo = 5;
  blocosColisao[18].colidido = false;
  blocosColisao[18].cliqueMouse = false;
  blocosColisao[18].coletado = false;
  blocosColisao[18].sprite = NULL;
  blocosColisao[18].spriteMascara = NULL;
  
  blocosColisao[19].x = 1100;
  blocosColisao[19].y = 130;
  blocosColisao[19].altura = 64;
  blocosColisao[19].largura = 64;
  blocosColisao[19].tipo = 5;
  blocosColisao[19].colidido = false;
  blocosColisao[19].cliqueMouse = false;
  blocosColisao[19].coletado = false;
  blocosColisao[19].sprite = NULL;
  blocosColisao[19].spriteMascara = NULL;
  
  blocosColisao[20].x = 1100;
  blocosColisao[20].y = 130;
  blocosColisao[20].altura = 64;
  blocosColisao[20].largura = 64;
  blocosColisao[20].tipo = 5;
  blocosColisao[20].colidido = false;
  blocosColisao[20].cliqueMouse = false;
  blocosColisao[20].coletado = false;
  blocosColisao[20].sprite = NULL;
  blocosColisao[20].spriteMascara = NULL;
  
  blocosColisao[21].x = 1100;
  blocosColisao[21].y = 130;
  blocosColisao[21].altura = 64;
  blocosColisao[21].largura = 64;
  blocosColisao[21].tipo = 5;
  blocosColisao[21].colidido = false;
  blocosColisao[21].cliqueMouse = false;
  blocosColisao[21].coletado = false;
  blocosColisao[21].sprite = NULL;
  blocosColisao[21].spriteMascara = NULL;
  
  blocosColisao[22].x = 1100;
  blocosColisao[22].y = 130;
  blocosColisao[22].altura = 64;
  blocosColisao[22].largura = 64;
  blocosColisao[22].tipo = 5;
  blocosColisao[22].colidido = false;
  blocosColisao[22].cliqueMouse = false;
  blocosColisao[22].coletado = false;
  blocosColisao[22].sprite = NULL;
  blocosColisao[22].spriteMascara = NULL;
  
  blocosColisao[23].x = 1100;
  blocosColisao[23].y = 130;
  blocosColisao[23].altura = 64;
  blocosColisao[23].largura = 64;
  blocosColisao[23].tipo = 5;
  blocosColisao[23].colidido = false;
  blocosColisao[23].cliqueMouse = false;
  blocosColisao[23].coletado = false;
  blocosColisao[23].sprite = NULL;
  blocosColisao[23].spriteMascara = NULL;
  
  blocosColisao[24].x = 1100;
  blocosColisao[24].y = 130;
  blocosColisao[24].altura = 64;
  blocosColisao[24].largura = 64;
  blocosColisao[24].tipo = 5;
  blocosColisao[24].colidido = false;
  blocosColisao[24].cliqueMouse = false;
  blocosColisao[24].coletado = false;
  blocosColisao[24].sprite = NULL;
  blocosColisao[24].spriteMascara = NULL;
  
  blocosColisao[25].x = 1100;
  blocosColisao[25].y = 130;
  blocosColisao[25].altura = 64;
  blocosColisao[25].largura = 64;
  blocosColisao[25].tipo = 5;
  blocosColisao[25].colidido = false;
  blocosColisao[25].cliqueMouse = false;
  blocosColisao[25].coletado = false;
  blocosColisao[25].sprite = NULL;
  blocosColisao[25].spriteMascara = NULL;
  
  blocosColisao[26].x = 1100;
  blocosColisao[26].y = 130;
  blocosColisao[26].altura = 64;
  blocosColisao[26].largura = 64;
  blocosColisao[26].tipo = 5;
  blocosColisao[26].colidido = false;
  blocosColisao[26].cliqueMouse = false;
  blocosColisao[26].coletado = false;
  blocosColisao[26].sprite = NULL;
  blocosColisao[26].spriteMascara = NULL;
  
  blocosColisao[27].x = 1100;
  blocosColisao[27].y = 130;
  blocosColisao[27].altura = 64;
  blocosColisao[27].largura = 64;
  blocosColisao[27].tipo = 5;
  blocosColisao[27].colidido = false;
  blocosColisao[27].cliqueMouse = false;
  blocosColisao[27].coletado = false;
  blocosColisao[27].sprite = NULL;
  blocosColisao[27].spriteMascara = NULL;
  
  //personagens
  //=>Lontra sem outline
  blocosColisao[28].x = 595;
  blocosColisao[28].y = 255;
  blocosColisao[28].altura = 255;
  blocosColisao[28].largura = 255;
  blocosColisao[28].tipo = 6;
  blocosColisao[28].colidido = false;
  blocosColisao[28].cliqueMouse = false;
  blocosColisao[28].coletado = false;
  blocosColisao[28].sprite = carregarImagem(".//Artes//Personagens//lontra.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  blocosColisao[28].spriteMascara = carregarImagem(".//Artes//Personagens//lontra_masc.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  
  //=>Lontra com outline
  blocosColisao[29].x = 595;
  blocosColisao[29].y = 255;
  blocosColisao[29].altura = 255;
  blocosColisao[29].largura = 255;
  blocosColisao[29].tipo = 6;
  blocosColisao[29].colidido = false;
  blocosColisao[29].cliqueMouse = false;
  blocosColisao[29].coletado = false;
  blocosColisao[29].sprite = carregarImagem(".//Artes//Personagens//lontra_outline.bmp", blocosColisao[29].largura, blocosColisao[29].altura, 0, 0);
  blocosColisao[29].spriteMascara = carregarImagem(".//Artes//Personagens//lontra_outline_masc.bmp", blocosColisao[29].largura, blocosColisao[29].altura, 0, 0);

  blocosColisao[30].x = blocosColisao[29].x;
  blocosColisao[30].y = blocosColisao[29].y;
  blocosColisao[30].altura = blocosColisao[29].altura;
  blocosColisao[30].largura = blocosColisao[29].largura;
  blocosColisao[30].tipo = blocosColisao[29].tipo;
  blocosColisao[30].colidido = false;
  blocosColisao[30].cliqueMouse = false;
  blocosColisao[30].coletado = false;
  blocosColisao[30].sprite = carregarImagem(".//Artes//Personagens//lontra_outline.bmp", blocosColisao[29].largura, blocosColisao[29].altura, 0, 0);
  blocosColisao[30].spriteMascara = carregarImagem(".//Artes//Personagens//lontra_outline_masc.bmp", blocosColisao[29].largura, blocosColisao[29].altura, 0, 0);

  //===============================> Fases <===============================
  int fases = 1;
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
      if(fases == 1)
      {
        LidandoComFases(cenarios[0], fases, false, false, true, false, inventario, 5, 6, 5, 9);
        if(qntItensColetados >= 5)
        {
	      inventario = false;
          fases = 4;
		}
	  }//n�o h� segunda fase pela l�gica que criei de transi��o de fase
	  else if(fases == 3)
	  {
	    LidandoComFases(cenarios[1], fases, false, false, false, true, inventario, 7, 9, 5, 9);
	    if(qntItensColetados >= 5)
	    {
	      inventario = false;
          fases = 4;
		}
	  }
	  else if(fases == 4)
	  {
	    LidandoComFases(cenarios[2], fases, false, false, true, false, inventario, 7, 9, 5, 9);
	    if(!blocosColisao[28].colidido)
	      DesenhandoBotao( blocosColisao[28].x, blocosColisao[28].y, blocosColisao[28].sprite, blocosColisao[28].spriteMascara);
	    else
	      DesenhandoBotao( blocosColisao[29].x, blocosColisao[29].y, blocosColisao[29].sprite, blocosColisao[29].spriteMascara);
	  }
	  else if(fases == 6)
	    LidandoComFases(cenarios[3], fases, true, false, pegouMissao, true, inventario, 7, 9, 5, 9);
	  else if(fases == 7)
	    LidandoComFases(cenarios[4], fases, false, true, true, false, inventario, 7, 9, 5, 9);
	  else if(fases == 8)
	    LidandoComFases(cenarios[6], fases, false, false, true, true, inventario, 7, 9, 5, 9);
	  else if(fases == 9)
	  {
	    LidandoComFases(cenarios[5], fases, false, false, false, true, inventario, 7, 9, 5, 9);//bruxa aqui
	    pegouMissao = true;
	  }
	  else if(fases == 10)
	  {
	    LidandoComFases(cenarios[7], fases, false, false, true, true, inventario, 7, 9, 5, 9);
	    inventario = true;
	  }
	  else if(fases == 12)
	    LidandoComFases(cenarios[8], fases, true, false, true, true, inventario, 7, 9, 5, 9);
	  else if(fases == 13)
	    LidandoComFases(cenarios[9], fases, false, true, true, false, inventario, 7, 9, 5, 9);
	  else if(fases == 14)
	    LidandoComFases(cenarios[11], fases, false, false, false, true, inventario, 7, 9, 5, 9);
	  else if(fases == 15)
	    LidandoComFases(cenarios[10], fases, false, false, false, true, inventario, 7, 9, 5, 9);
	  
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
  for(int i = 0; i<qntDeCenarios; i++)//liberando mem�ria dos cen�rios
  {
    free(cenarios[i]);
  }
  free(cenarios);
  closegraph();
  return 0; 
}

//===============================> �rea para Fazer as Fun��es <===============================
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
  //Criando vari�vel para controlar a localidade de cada item no invent�rio.
  int controleItensInventario = InvAltura/(InvIndexFinal - InvIndexInicial + 1);//Criando vari�vel para controlar a localidade de cada item no invent�rio.
  
  //Desenhando Cen�rio
  putimage(0, 0, cenario, COPY_PUT);
  //===============================> Desenhando os Itens <===============================
  setfillstyle(1,RGB(255, 255, 0));
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo quais itens tem na fase
  {
	if(!blocosColisao[i].coletado)
	  bar(blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].x + blocosColisao[i].largura, blocosColisao[i].y + blocosColisao[i].altura);
  }
  //===============================> Lidando com Som <=========================
  mciSendString("open .\\sons\\my.mp3 type MPEGVideo alias fundo2", NULL, 0, 0); 
  mciSendString("open .\\sons\\led.mp3 type MPEGVideo alias fundo", NULL, 0, 0); 
  mciSendString("play fundo2 repeat", NULL, 0, 0);
  if(blocosColisao[30].cliqueMouse == true)
  {
  	mciSendString("stop fundo2", NULL, 0, 0);
    mciSendString("play fundo", NULL, 0, 0); 
    fase ++;
  }
  //===============================> Lidando com os Blocos <===============================
  setfillstyle(1,RGB(255, 0, 0));
  for(int i = 0; i <= 4; i++)//pegando os bot�es bases para troca de fase e invent�rio
  {
    ChecagemDeColisaoDoBloco(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
    //===============================> Blocos para Mudan�a de Fase <===============================
    if(blocosColisao[i].tipo == 0 && fasePraCima == true)//conferindo para quais lados ser�o as pr�ximas fases / fases anteriores
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);//Desenhando Seta para Cima
      
    if(blocosColisao[i].tipo == 1 && fasePraDireita == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);//Desenhando Seta para Direita
      
    if(blocosColisao[i].tipo == 2 && fasePraEsquerda == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);//Desenhando Seta para Esquerda
      
    if(blocosColisao[i].tipo == 3 && fasePraBaixo == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);//Desenhando Seta para Baixo
      
    if(blocosColisao[i].tipo == 4 && inventario == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);//Desenhando Bot� do Invent�rio

    //==================> A��es dos Bloco de Colis�o que n�o sejam colet�veis (Bot�es de mudan�a de fases e invent�rio) <==================
    if(blocosColisao[i].tipo == 0 && blocosColisao[i].cliqueMouse == true && fasePraCima == true)//bot�o fase Acima
	{
	  fase += 1;
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 1 && blocosColisao[i].cliqueMouse == true && fasePraDireita == true)//bot�o fase para � Direita
	{	
	  fase += 2;
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 2 && blocosColisao[i].cliqueMouse == true && fasePraEsquerda == true)//bot�o fase � Esquerda
	{	
	  fase -= 2;
	  blocosColisao[i].cliqueMouse = false;
	}
    else if(blocosColisao[i].tipo == 3 && blocosColisao[i].cliqueMouse == true && fasePraBaixo == true)//bot�o fase Abaixo
    {
	  fase -= 1;
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

	
	//==================> Criando o Invent�rio <==================
    if(blocosColisao[i].tipo == 4 && blocosColisao[i].coletado == true)
    {
      setfillstyle(1,RGB(50, 125, 255));
      bar(blocosColisao[i].x + blocosColisao[i].largura, 0, 1280, 720);//desenhando o invent�rio
      
      for(int j = InvIndexInicial; j < InvIndexFinal + 1; j++)//pegando quais itens tem pela fase para aparecer no invent�rio
      {
	    if(!blocosColisao[j].coletado)//modificando a imagem com base se o item foi coletado ou n�o
	    {
	      setfillstyle(1,RGB(50, 125, 50));
	      bar(xInv + (InvLargura/2) - (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), xInv + (InvLargura/2) + (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) + (blocosColisao[j].largura/2));
		}
	    else
	    {
	      setfillstyle(1,RGB(0, 0, 0));
	      bar(xInv + (InvLargura/2) - (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), xInv + (InvLargura/2) + (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) + (blocosColisao[j].largura/2));
		}
      }
	}
  }
  
  //===============================> Colis�o do Mouse <===============================
  (GetCursorPos(&P));
  (ScreenToClient(janela, &P));
  ChecagemDeColisaoDoMouse(blocoInicial, blocoFinal);
  
  //===============================> Captura de Inputs <===============================
  CapturaDeInput_e_Acao();
}

void ChecagemDeColisaoDoBloco( int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )//chegagem se o mouse est� � direita e em baixo do v�rtice superior esquerdo
  {
    if( xColisor <= (xColidido + larguraColidido) && yColisor <= (yColidido + alturaColidido) )//chegagem se o mouse est� � esquerda e em cima do v�rtice inferior direito
      colidiu = true;
    else
      colidiu = false;
  }
  else
    colidiu = false;
}

void CapturaDeInput_e_Acao()
{
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

void ChecagemDeColisaoDoMouse(int indexInicial, int indexFinal)
{
  for(int i = indexInicial; i < indexFinal + 1; i++)//conferindo a colis�o para cada Bloco de Colis�o
  {
    ChecagemDeColisaoDoBloco(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);//Coletando a informa��o de colis�o e guardando dentro do item.
  }
  if(colisaoMouse == false)
  {
    for(int i = 0; i < qntBlocos + 1; i++)//checagem dos Itens para ver se h� algum colidido e Qual foi.
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
	for(int i = 0; i < qntBlocos + 1; i++)//checagem se n�o h� mais colis�o.
	{
	  if(blocosColisao[i].colidido == true)//se h�, ent�o ignora a verifica��o
		break;
	  else if(i == qntBlocos - 1)//se n�o h�. ent�o mouse n�o est� colidido
	  {
		if(blocosColisao[i].colidido == false) colisaoMouse = false;
	  }
	}
  }
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
 
