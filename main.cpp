#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
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

//criando o vetor para blocos de colisão
BlocoDeColisao *blocosColisao;
int qntBlocos = 27;

//checagem de itens coletados
int qntItensColetados = 0;
 
//Area para funcoes
void* carregarImagem(const char *sprite, int largura, int altura , int x, int y);

bool ChecagemDeColisao(int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu);

void LidandoComFases(void *cenario, int &numItensColetados, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal);

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara);

//=====================> Tempo de Espera para Clique do Mouse <=====================
int Espera = 500;
double TempoDecorrido;
long long Inicio, Agora;
					
//===============================> Mouse <===============================
HWND janela;//coletando a janela
POINT P;//posição do Mouse
int indexItemColidido = 0;//coletandoBlocoColidido
bool colisaoMouse = false;//verificando se o mouse colidiu com algum bloco
  
//===============================> Inventário <===============================
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
  int fps = 60;
  
  initwindow(1280, 720, "Kartelles Seele");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do laço para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  tecla = 0;//index do teclado

  //===============================> Blocos de Colisão para o Mouse <===============================
  //Vetor que está guardando os blocos
  blocosColisao = NULL;
  blocosColisao = (BlocoDeColisao *) malloc(sizeof(BlocoDeColisao) * qntBlocos);
  
  //===============================> Botões para troca de fases <===============================
  //botão para cima
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
  
  //botão para direita
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
  
  //botão para esquerda
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
  
  //botão para baixo
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
  
  //inventário
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
  
  //===============================> Fases <===============================
  int fases = 1;
  bool pegouMissao = false;
  bool inventario = true;
  
  //===============================> Cenários <===============================
  void *cenario1;
  cenario1 = carregarImagem(".//Artes//Cenarios//sala_tutas.bmp", 1280, 600, 0, 0);
  
  void *cenario2;
  cenario2 = carregarImagem(".//Artes//Cenarios//quarto_tutas.bmp", 1280, 600, 0, 0);
  
  void *cenario3;
  cenario3 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_1.bmp", 1280, 600, 0, 0);
  
  void *cenario4;
  cenario4 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_2.bmp", 1280, 600, 0, 0);
  
  void *cenario5;
  cenario5 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_3.bmp", 1280, 600, 0, 0);
  
  void *cenario6;
  cenario6 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_4.bmp", 1280, 600, 0, 0);
  
  void *cenario7;
  cenario7 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_5.bmp", 1280, 600, 0, 0);
  
  void *cenario8;
  cenario8 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_6.bmp", 1280, 600, 0, 0);
  
  void *cenario9;
  cenario9 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_7.bmp", 1280, 600, 0, 0);
  
  void *cenario10;
  cenario10 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_8.bmp", 1280, 600, 0, 0);
  
  void *cenario11;
  cenario11 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_9.bmp", 1280, 600, 0, 0);
  
  void *cenario12;
  cenario12 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_10.bmp", 1280, 600, 0, 0);
  
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
	  if(pg == 1) pg = 2; else pg = 1;//técnica de paginação
	  setactivepage(pg);
	  cleardevice();
	  
      //=================> Lidando com a troca de fases <=================
      if(fases == 1)
      {
        LidandoComFases(cenario1, qntItensColetados, fases, false, false, true, false, inventario, 5, 6, 5, 9);
        if(qntItensColetados >= 5)
        {
	      inventario = false;
          fases = 4;
		}
	  }//não há segunda fase pela lógica que criei de transição de fase
	  else if(fases == 3)
	  {
	    LidandoComFases(cenario2, qntItensColetados, fases, false, false, false, true, inventario, 7, 9, 5, 9);
	    if(qntItensColetados >= 5)
	    {
	      inventario = false;
          fases = 4;
		}
	  }
	  else if(fases == 4)
	  {
	    LidandoComFases(cenario3, qntItensColetados, fases, false, false, true, false, inventario, 7, 9, 5, 9);
	    if(!blocosColisao[10].colidido)
	      DesenhandoBotao( blocosColisao[25].x, blocosColisao[25].y, blocosColisao[25].sprite, blocosColisao[25].spriteMascara);
	    else
	      DesenhandoBotao( blocosColisao[26].x, blocosColisao[26].y, blocosColisao[26].sprite, blocosColisao[26].spriteMascara);
	  }
	  else if(fases == 6)
	  {
	    LidandoComFases(cenario4, qntItensColetados, fases, true, false, pegouMissao, true, inventario, 7, 9, 5, 9);
	  }
	  else if(fases == 7)
	  {
	    LidandoComFases(cenario5, qntItensColetados, fases, false, true, true, false, inventario, 7, 9, 5, 9);
	  }
	  else if(fases == 8)
	  {
	    LidandoComFases(cenario7, qntItensColetados, fases, false, false, true, true, inventario, 7, 9, 5, 9);
	  }
	  else if(fases == 9)
	  {
	    LidandoComFases(cenario6, qntItensColetados, fases, false, false, false, true, inventario, 7, 9, 5, 9);
	    pegouMissao = true;
	  }
	  else if(fases == 10)
	  {
	    LidandoComFases(cenario8, qntItensColetados, fases, false, false, true, true, inventario, 7, 9, 5, 9);
	    inventario = true;
	  }
	  else if(fases == 12)
	  {
	    LidandoComFases(cenario9, qntItensColetados, fases, true, false, true, true, inventario, 7, 9, 5, 9);
	  }
	  else if(fases == 13)
	  {
	    LidandoComFases(cenario10, qntItensColetados, fases, false, true, true, false, inventario, 7, 9, 5, 9);
	  }
	  else if(fases == 14)
	  {
	    LidandoComFases(cenario12, qntItensColetados, fases, false, false, false, true, inventario, 7, 9, 5, 9);
	  }
	  else if(fases == 15)
	  {
	    LidandoComFases(cenario11, qntItensColetados, fases, false, false, false, true, inventario, 7, 9, 5, 9);
	  }
	  
      setvisualpage(pg);
    }
    
    //===============================> Input para saída do laço <===============================
    fflush(stdin);
    if (kbhit())
	{
      tecla = getch();
    }
  }
  
  printf("\n\nFim do Programa");
  for(int i = 0; i<qntBlocos; i++)
  {
    free(blocosColisao[i].sprite);
  	free(blocosColisao[i].spriteMascara);
  }
  free(blocosColisao);
  free(cenario1);
  free(cenario2);
  free(cenario3);
  free(cenario4);
  free(cenario5);
  free(cenario6);
  free(cenario7);
  free(cenario8);
  free(cenario9);
  free(cenario10);
  free(cenario11);
  free(cenario12);
  closegraph();
  return 0; 
}

void* carregarImagem(const char *sprite, int largura, int altura , int x, int y)
{	
  void *image;//vetor para bits brutos
  int aux = imagesize(x, y, largura, altura);//tamanho da imagem
  image = malloc(aux);//abrindo espaço de memória para o vetor com base no tamanho da imagem
  readimagefile(sprite, x, y, largura, altura);//lendo a imagem que queremos
  getimage(x, y, largura, altura, image);
  
  return image;
}

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara)
{
  putimage(xImagem, yImagem, spritesMascara, AND_PUT);	
  putimage(xImagem, yImagem, sprites, OR_PUT);
}

void LidandoComFases(void *cenario, int &numItensColetados, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal)
{
  int controleItensInventario = InvAltura/(InvIndexFinal - InvIndexInicial + 1);//Criando variável para controlar a localidade de cada item no inventário.
  int salvandoPosicaoInicial;
  
  //Desenhando Cenário
  putimage(0, 0, cenario, COPY_PUT);
  
  //===============================> Desenhando os Itens <===============================
  setfillstyle(1,RGB(255, 255, 0));
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo quais itens tem na fase
  {
	if(!blocosColisao[i].coletado)
	  bar(blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].x + blocosColisao[i].largura, blocosColisao[i].y + blocosColisao[i].altura);
  }
  
  //===============================> Lidando com os Blocos <===============================
  setfillstyle(1,RGB(255, 0, 0));
  for(int i = 0; i <= 4; i++)//pegando os botões bases para troca de fase e inventário
  {
    //===============================> Blocos para Mudança de Fase <===============================
    if(blocosColisao[i].tipo == 0 && fasePraCima == true)//conferindo para quais lados serão as próximas fases / fases anteriores
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
      
    if(blocosColisao[i].tipo == 1 && fasePraDireita == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
      
    if(blocosColisao[i].tipo == 2 && fasePraEsquerda == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
      
    if(blocosColisao[i].tipo == 3 && fasePraBaixo == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
      
      
    //===============================> Botão do Inventário <===============================
    if(i == 4 && inventario == true)
    {
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}

    //==================> Ações dos Bloco de Colisão que não sejam coletáveis (Botões de mudança de fases e inventário) <==================
    if(blocosColisao[i].tipo == 0 && blocosColisao[i].cliqueMouse == true && fasePraCima == true)//botão fase Acima
	{
	  fase += 1;
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 1 && blocosColisao[i].cliqueMouse == true && fasePraDireita == true)//botão fase para à Direita
	{	
	  fase += 2;
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 2 && blocosColisao[i].cliqueMouse == true && fasePraEsquerda == true)//botão fase à Esquerda
	{	
	  fase -= 2;
	  blocosColisao[i].cliqueMouse = false;
	}
    else if(blocosColisao[i].tipo == 3 && blocosColisao[i].cliqueMouse == true && fasePraBaixo == true)//botão fase Abaixo
    {
	  fase -= 1;
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 4 && inventario == true && blocosColisao[i].cliqueMouse == true)//botão para inventário
    {
      if(!blocosColisao[i].coletado)//abrindo o inventário
      {
        blocosColisao[i].x = xInv - blocosColisao[i].largura;
        blocosColisao[i].coletado = true;
	  }
	  else//fechando o inventário
	  {
	    blocosColisao[i].x = 1280 - blocosColisao[i].largura;
	    blocosColisao[i].coletado = false;
	  }
	  blocosColisao[i].cliqueMouse = false;
	}

	
	//==================> Criando o Inventário <==================
    if(blocosColisao[i].tipo == 4 && blocosColisao[i].coletado == true)
    {
      setfillstyle(1,RGB(50, 125, 255));
      bar(blocosColisao[i].x + blocosColisao[i].largura, 0, 1280, 720);//desenhando o inventário
      
      for(int j = InvIndexInicial; j < InvIndexFinal + 1; j++)
      {
	    if(!blocosColisao[j].coletado)//modificando a imagem c base se o item foi coletado ou não
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
  
  //===============================> Colisão do Mouse <===============================
  (GetCursorPos(&P));
  (ScreenToClient(janela, &P));
      
  for(int i = 0; i < qntBlocos; i++)
  {
    ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);//Coletando a informação de colisão e guardando dentro do item.
  }
  if(colisaoMouse == false)
  {
    for(int i = 0; i < qntBlocos; i++)//checagem dos Itens para ver se há algum colidido e Qual foi.
	{
	  colisaoMouse = blocosColisao[i].colidido;
	  if(colisaoMouse == true)//se há, coleta qual item
	  {
	    indexItemColidido = i;
	    break;
	  }
	}
  }
  if(colisaoMouse == true)
  {
	for(int i = 0; i < qntBlocos; i++)//checagem se não há mais colisão.
	{
	  if(blocosColisao[i].colidido == true)//se há, então ignora a verificação
		break;
	  else if(i == qntBlocos - 1)//se não há. então mouse não está colidido
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
      if(blocosColisao[indexItemColidido].tipo == 5 && blocosColisao[indexItemColidido].coletado == false)//Captura de Coletáveis
      {	
        numItensColetados++;//registrando itens coletados
        blocosColisao[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
	  }
	}
  }
  
}

bool ChecagemDeColisao( int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )//chegagem se o mouse está à direita e em baixo do vértice superior esquerdo
  {
    if( xColisor <= (xColidido + larguraColidido) && yColisor <= (yColidido + alturaColidido) )//chegagem se o mouse está à esquerda e em cima do vértice inferior direito
      colidiu = true;
    else
      colidiu = false;
  }
  else
    colidiu = false;
}

/*
largura de uma letra na tela = 4 pixeis;
strcpy() -> Copia a string para dentro da variável
strcat() -> Junta duas strings
strlen() -> Nos fala o tamanho de uma string
strcmp() -> Compara duas strings
gets()   -> Input para strings, muito melhor que scanf
puts()   -> Print para string, mais fácil que printf
*/
 
