#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
using namespace std;

#define ESC    	27

//local para os structs
typedef struct _item
{
  int x, y, altura, largura;
}Item;

typedef struct _tiles
{
  int x, y, tipo;
}Tiles;

//Area para funcoes
void* loadimage(const char *sprite, int largura, int altura , int x, int y)
{	
  void *image;
  int aux = imagesize(x, y, largura, altura);
  image = malloc(aux);
  readimagefile(sprite, x, y, largura, altura);
  getimage(x, y, largura, altura, image);
  cleardevice();
  
  return image;		
}

bool ChecagemDeColisaoVertical( int xColisor, int &yColisor, int yColisorAnterior, int larguraColisor, int alturaColisor, int xColidido, int yColidido, int TamanhoColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )
  {
    if( xColisor <= (xColidido + TamanhoColidido) && yColisor <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Superior Esquerdo
    {
      colidiu = true;
      yColisor = yColisorAnterior;
	}
  }
  else if( (xColisor + larguraColisor) >= xColidido && yColisor >= yColidido )
  {
    if( (xColisor + larguraColisor) <= (xColidido + TamanhoColidido) && yColisor <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Superior Direito
    {
      colidiu = true;
      yColisor = yColisorAnterior;
	}
  }
  else if( xColisor >= xColidido && (yColisor + alturaColisor) >= yColidido )
  {
    if(xColisor <= (xColidido + TamanhoColidido) && (yColisor + alturaColisor) <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Inferior Esquerdo
    {
      colidiu = true;
      yColisor = yColisorAnterior;
      printf("\n Inferior Esquerdo %d",colidiu);
	}
  }
  else if( (xColisor + larguraColisor) >= xColidido && (yColisor + alturaColisor) >= yColidido )
  {
    if( (xColisor + larguraColisor) <= (xColidido + TamanhoColidido) && (yColisor + alturaColisor) <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Inferior Direito
    {
      colidiu = true;
      yColisor = yColisorAnterior;
	}
  }
  else
    colidiu = false;
}

bool ChecagemDeColisaoHorizontal( int &xColisor, int yColisor, int xColisorAnterior, int larguraColisor, int alturaColisor, int xColidido, int yColidido, int TamanhoColidido, bool &colidiu)
{
  if( xColisor >= (xColidido + TamanhoColidido - 2) && yColisor >= yColidido )
  {
    if(xColisor <= (xColidido + TamanhoColidido) && yColisor <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Superior Esquerdo do Colisor
    {
      xColisor = xColisorAnterior;
      colidiu = true;
	}
  }
  else if( (xColisor + larguraColisor) >= (xColidido + TamanhoColidido - 2) && yColisor >= yColidido)
  {
    if( (xColisor + larguraColisor) <= (xColidido + TamanhoColidido) && yColisor <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Superior Direito do Colisor
    {
      xColisor = xColisorAnterior;
      colidiu = true;
	}
  }
  else if( (xColisor >= xColidido + TamanhoColidido - 2) && (yColisor + alturaColisor) >= yColidido )
  {
    if( xColisor <= (xColidido + TamanhoColidido) && (yColisor + alturaColisor) <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Inferior Esquerdo do Colisor
    {
      xColisor = xColisorAnterior;
      colidiu = true;
	}
  }
  else if( (xColisor + larguraColisor) >= (xColidido + TamanhoColidido - 2) && (yColisor + alturaColisor) >= yColidido)
  {
    if( (xColisor + larguraColisor) <= (xColidido + TamanhoColidido) && (yColisor + alturaColisor) <= (yColidido + TamanhoColidido))//Verificando colisao vertice Inferior Direito do Colisor
    {
      xColisor = xColisorAnterior;
      colidiu = true;
	}
  }
  else if( (xColisor + (largura/2)) >= (xColidido + TamanhoColidido - 2) )//em processo
  else
    colidiu = false;
  return colidiu;
}

//==================JOGO==================
int main()
{
  int pg = 1;
  char tecla;
  POINT P;
  HWND janela;	
  unsigned long long gt1, gt2;	
  int fps = 60;
  
  //====Fases====
  int fase = 0;
  int cenario = 0;  

  //====Variáveis do Tile====
  int tileComprimento = 64;
  
  Tiles *tiles;
  tiles = NULL;
  
  int qntTilesAltura = 12;
  int qntTilesLargura = 20;
  
  int tamTiles;
  tamTiles = qntTilesAltura * qntTilesLargura;
  tiles = (Tiles *)malloc(sizeof(Tiles)*tamTiles);
  
  int tileMap[12][20] = // Posições dos Tiles na tela:
  {
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 4},
    {3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 4},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };
  
  int contagem = 0;
  for (int i = 0; i < qntTilesAltura; i++)//Tipos de Tiles
  {
    for (int j = 0; j < qntTilesLargura; j++) 
	{
      switch(tileMap[i][j]) 
	  {
        case 0:// Tipo 0 - Sem colisão
          tiles[contagem].tipo = 0;
          break;
        case 1:// Tipo 1 - Bloco de Colisão de Vertical
          tiles[contagem].tipo = 1;
          break;
        case 2:// Tipo 1 - Bloco de Colisão de Horizontal
          tiles[contagem].tipo = 2;
          break;
        case 3:// Tipo 3 - Área para transição Fase Anterior
          tiles[contagem].tipo = 3;
          break;
        case 4:// Tipo 4 - Área para transição Fase Posterior
          tiles[contagem].tipo = 3;
          break;
      }
      tiles[contagem].x = j * tileComprimento;
      tiles[contagem].y = i * tileComprimento;
      contagem++;
    }
  }
  //====Tempo de Espera ====
  int espera = 1000;
  double tempoDecorrido;
  unsigned long long inicio, agora;
  bool contar = false;
  
  //====Player====
  int xPlayer, yPlayer, xAnterior, yAnterior, passoyP, passoxP, altura, largura;
  int limitePulo;
  int gravidade;
  bool jump = false;
  
  bool colisaoVertical = false;
  bool colisaoHorizontal = false;
  
  altura = 128;
  largura = 64;
  xPlayer = 130;
  yPlayer = 600 - altura;
  gravidade = 5;
  passoxP = 4;
  passoyP = gravidade + 8;
  limitePulo = yPlayer - 80;
  
  //=======Lindando com a janela=======
  initwindow(1280, 720, "Projeto Alma");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do laço para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  tecla = 0;
  
  //====Salvando o tick do computador Inicialmente====
  gt1 = GetTickCount();
  inicio = GetTickCount();
  agora = inicio + espera;
  
  while(tecla != ESC)
  {	
    //====Coletando o Tick ao passar do tempo====
    gt2 = GetTickCount();
    
    if(contar == true)
      agora = GetTickCount();
    
	tempoDecorrido = agora - inicio;
	    
    if (gt2-gt1 > 1000/fps)
	{
      gt1 = gt2;
	  if(pg == 1) pg = 2; else pg = 1;
	  setactivepage(pg);
	  cleardevice();
	  
	  //====Lidando com o Player====
	  if(!colisaoVertical)
	    yAnterior = yPlayer;
	
	  if(!colisaoHorizontal)
	    xAnterior = xPlayer;
	    
	  //====LOCAL PARA PROGRAMAR NA TELA===
	  //========Player=======
	  setfillstyle(1, RED);
      setcolor (YELLOW);
      bar(xPlayer, yPlayer, xPlayer + largura, yPlayer + altura); 
      
	  //====Gravidade====
	  if(!colisaoVertical)
	    yPlayer = yPlayer + gravidade;
	  
	  //====Pulo====
	  if(GetKeyState(VK_SPACE)&0x80 && !jump && tempoDecorrido >= espera)
	  {
	    jump = true;
	    limitePulo = yPlayer - 80;
	  	inicio = GetTickCount();
	    contar = true;
	  } 
	  if(yPlayer >= limitePulo && jump == true)
	  {
	    yPlayer = yPlayer - passoyP;
	    if(yPlayer <= limitePulo)
		{
	      jump = false;	
		}
	  }
	  
	  //======Captura de Inputs do teclado(Esquerda e Direta, Respestivamente)======
	  if(GetKeyState(0x41)&0x80)  xPlayer = xPlayer - passoxP;
      if(GetKeyState(0x44)&0x80)  xPlayer = xPlayer + passoxP;
      
      //======Lidando com Colisão dos Tiles======
      for(int i = 0; i < tamTiles; i++)
      {
        if(tiles[i].tipo == 1)
        {
        	setfillstyle(1, RGB(255,255,0));
        	bar(tiles[i].x, tiles[i].y, tiles[i].x+tileComprimento, tiles[i].y+tileComprimento);
			ChecagemDeColisaoVertical(xPlayer, yPlayer, yAnterior, largura, altura, tiles[i].x, tiles[i].y, tileComprimento, colisaoVertical);
			printf("\n%d",colisaoVertical);
		}
		if(tiles[i].tipo == 2)
        {
        	setfillstyle(1, RGB(255,0,255));
        	bar(tiles[i].x, tiles[i].y, tiles[i].x+tileComprimento, tiles[i].y+tileComprimento);
        	ChecagemDeColisaoHorizontal(xPlayer, yPlayer, xAnterior, largura, altura, tiles[i].x, tiles[i].y, tileComprimento,  colisaoHorizontal); 
		}
	  }
	  
      setvisualpage(pg);
      
      //====Ajeitando a Posição do Mouse====
      (GetCursorPos(&P));
      (ScreenToClient(janela, &P));
    }
    
    //====Input para saída do laço====
    fflush(stdin);    
    if (kbhit())
	{
      tecla = getch();
    }
  }
  free(tiles);
  printf("\nFim do Programa");
  closegraph();
  return 0; 
}
/*
strcpy() -> Copia a string para dentro da variável
strcat() -> Junta duas strings
strlen() -> Nos fala o tamanho de uma string
strcmp() -> Compara duas strings
gets()   -> Input para strings, muito melhor que scanf
puts()   -> Print para string, mais fácil que printf
*/
 
