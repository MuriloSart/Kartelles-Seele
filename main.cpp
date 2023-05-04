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

bool ChecagemDeColisao( int xColisor, int yColisor, int larguraColisor, int alturaColisor, int xColidido, int yColidido, int TamanhoColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )
  {
    if( xColisor <= (xColidido + TamanhoColidido) && yColisor <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Superior Esquerdo
    {
      colidiu = true;
	}
  }
  else if( (xColisor + larguraColisor) >= xColidido && yColisor >= yColidido )
  {
    if( (xColisor + larguraColisor) <= (xColidido + TamanhoColidido) && yColisor <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Superior Direito
    {
      colidiu = true;
	}
  }
  else if( xColisor >= xColidido && (yColisor + alturaColisor) >= yColidido )
  {
    if(xColisor <= (xColidido + TamanhoColidido) && (yColisor + alturaColisor) <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Inferior Esquerdo
    {
      colidiu = true;
	}
  }
  else if( (xColisor + larguraColisor) >= xColidido && (yColisor + alturaColisor) >= yColidido )
  {
    if( (xColisor + larguraColisor) <= (xColidido + TamanhoColidido) && (yColisor + alturaColisor) <= (yColidido + TamanhoColidido) )//Verificando colisao vertice Inferior Direito
    {
      colidiu = true;
	}
  }
  else
    colidiu = false;
}

//=================================================> JOGO <=================================================
int main()
{
  int pg = 1;
  char tecla;
  HWND janela;	
  unsigned long long gt1, gt2;	
  int fps = 60;
  
  //===============================> Itens <===============================
  POINT P;
  bool colisaoMouse = false;
  
  //===============================> Itens <===============================
  Item seiLa;
  seiLa.x = 50;
  seiLa.y = 50;
  seiLa.altura = 40;
  seiLa.largura = 40;
  
  //===============================> Fases <===============================
  int fase = 0;
  int cenario = 0;  

  //===============================> Tempo de Espera <===============================
  int espera = 2000;
  double tempoDecorrido;
  unsigned long long inicio, agora;
  bool contar = false;
  
  //===============================> Lindando com a janela <===============================
  initwindow(1280, 720, "Projeto Alma");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do laço para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  tecla = 0;
  
  //===============================> Salvando o tick do computador Inicialmente <===============================
  gt1 = GetTickCount();
  inicio = GetTickCount();
  agora = inicio + espera;
  
  while(tecla != ESC)
  {	
    //===============================> Coletando o Tick ao passar do tempo <===============================
    gt2 = GetTickCount();
    agora = GetTickCount();
    
	tempoDecorrido = agora - inicio;
	    
    if (gt2-gt1 > 1000/fps)
	{
      gt1 = gt2;
	  if(pg == 1) pg = 2; else pg = 1;
	  setactivepage(pg);
	  cleardevice();
	    
	  //===============================> LOCAL PARA PROGRAMAR NA TELA <===============================
	  setfillstyle(1,RGB(255, 255, 0));
	  bar(seiLa.x, seiLa.y, seiLa.x + seiLa.largura, seiLa.y + seiLa.altura);
	  
	  //===============================> Captura de Inputs <===============================
      
      //===============================> Ajeitando a Posição do Mouse <===============================
      (ScreenToClient(janela, &P));
      (GetCursorPos(&P));
      ChecagemDeColisao( P.x, P.y, 1, 1, seiLa.x, seiLa.y, seiLa.altura, colisaoMouse);
      printf("%d", colisaoMouse);
      
      setvisualpage(pg);
    }
    
    //===============================> Input para saída do laço <===============================
    fflush(stdin);    
    if (kbhit())
	{
      tecla = getch();
    }
  }
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
 
