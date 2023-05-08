#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
using namespace std;

#define ESC    	27

/*
  Como programar as funções do clique do mouse com o que está colidido:
  
  
*/

//local para os structs
typedef struct _item
{
  int x, y, altura, largura;
  bool colidido, cliqueMouse;
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

bool ChecagemDeColisao( int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )
  {
    if( xColisor <= (xColidido + larguraColidido) && yColisor <= (yColidido + alturaColidido) )
    {
      colidiu = true;
	}
   else
    colidiu = false;
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
  unsigned long long gt1, gt2;//Clocks do Computador
  int fps = 60;
  
  //===============================> Mouse <===============================
  POINT P;//Posição do Mouse
  bool colisaoMouse = false;
  
  //===============================> Itens <===============================
  //Vetor guardando os itens
  Item *Itens;
  int qntItens = 3;
  Itens = NULL;
  Itens = (Item *) malloc(sizeof(Item) * qntItens);
  int indexItemColidido = 0;
  
  //itens
  Itens[0].x = 20;
  Itens[0].y = 20;
  Itens[0].altura = 20;
  Itens[0].largura = 20;
  Itens[0].colidido = false;
  Itens[0].cliqueMouse = false;
  
  Itens[1].x = 500;
  Itens[1].y = 30;
  Itens[1].altura = 20;
  Itens[1].largura = 20;
  Itens[1].colidido = false;
  Itens[1].cliqueMouse = false;
  
  Itens[2].x = 30;
  Itens[2].y = 500;
  Itens[2].altura = 20;
  Itens[2].largura = 20;
  Itens[2].colidido = false;
  Itens[2].cliqueMouse = false;
  
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
	  setfillstyle(1,RGB(255, 0, 0));
	  for(int i = 0; i < qntItens; i++)
	  {	
	    if(!Itens[i].cliqueMouse)
	      bar(Itens[i].x, Itens[i].y, Itens[i].x + Itens[i].largura, Itens[i].y + Itens[i].altura);
	  }
	  
      //===============================> Colisão do Mouse <===============================
      (GetCursorPos(&P));
      (ScreenToClient(janela, &P));
      
      for(int i = 0; i < qntItens; i++)
      {
        ChecagemDeColisao(P.x, P.y, Itens[i].x, Itens[i].y, Itens[i].largura, Itens[i].altura, Itens[i].colidido);//Coletando a informação de colisão e guardando dentro do item.
	  }
	  if(colisaoMouse == false)
	  {
	    for(int i = 0; i < qntItens; i++)//Checagem dos Itens para ver se há algum colidido e Qual foi.
	    {
	      colisaoMouse = Itens[i].colidido;
	      if(colisaoMouse == true)
	      {
	        indexItemColidido = i;
	        break;
		  }
		}
	  }
	  if(colisaoMouse == true)
	  {
	    for(int i = 0; i < qntItens; i++)//Checagem se não há mais colisão.
	    {
		  if(Itens[i].colidido == true)
		    break;
		  else if(i == qntItens - 1)
		  {  
		    if(Itens[i].colidido == false)
		      colisaoMouse = false;
		  }
		} 
	  }
	  
	  //===============================> Captura de Inputs <===============================
      if(GetKeyState(VK_LBUTTON)&0x80 && colisaoMouse == true)//Input do Mouse
      {
        Itens[indexItemColidido].cliqueMouse = true;
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
 
