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

//area para funcoes
  //Imagens
void* loadimage(const char *sprite, int largura, int altura , int x, int y){	
  void *image;
  int aux = imagesize(x, y, largura, altura);
  image = malloc(aux);
  readimagefile(sprite, x, y, largura, altura);
  getimage(x, y, largura, altura, image);
  cleardevice();
  
  return image;		
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
  
 //====Tempo de Espera ====
  int Espera = 5000;
  double TempoDecorrido;
  
//====Player====
  int xPlayer, yPlayer, passoyP, passoxP, altura, largura;
  int limitePulo;
  int gravidade;
  bool jump = false;
  
  altura = 64;
  largura = 32;
  xPlayer = 10;
  yPlayer = 720 - altura;
  gravidade = 5;
  passoxP = 4;
  passoyP = gravidade + 8;
  limitePulo = yPlayer - 80;
  
  tecla = 0;
//=======Lindando com a janela=======
  initwindow(1280, 720, "Projeto Alma");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do laço para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  
//====Salvando o tick do computador Inicialmente====
  gt1 = GetTickCount();
  
  while(tecla != ESC)
  {	
    //====Coletando o Tick ao passar do tempo====
    gt2 = GetTickCount();
    
	//TempoDecorrido = Agora - Inicio;
	    
    if (gt2-gt1 > 1000/fps)
	{
      gt1 = gt2;
	  if(pg == 1) pg = 2; else pg = 1;
	  setactivepage(pg);
	  cleardevice();  
	  
	 //====LOCAL PARA PROGRAMAR NA TELA===
	 //========Player=======
	  setfillstyle(1, YELLOW);
      setcolor (YELLOW);
      bar(xPlayer, yPlayer, xPlayer + largura, yPlayer + altura); 
	  
	 //====Gravidade====
	  if(yPlayer < 719 - altura)
	    yPlayer = yPlayer + gravidade;
	  
	 //====Pulo====
	  if(GetKeyState(VK_SPACE)&0x80 && !jump && yPlayer >= 719 - altura)
	  {
	    jump = true;
	    limitePulo = yPlayer - 80;
	  }
	  if(yPlayer >= limitePulo && jump == true)
	  {
	    yPlayer = yPlayer - passoyP;
	    if(yPlayer <= limitePulo)
		{
	      jump = false;	
		}
	  }
	  
	  setvisualpage(pg);
	  
	 //======Captura de Inputs do teclado(Esquerda e Direta, Respestivamente)======
	  if(GetKeyState(0x41)&0x80)  xPlayer = xPlayer - passoxP;
      if(GetKeyState(0x44)&0x80)  xPlayer = xPlayer + passoxP;
      
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
  printf("Fim do Programa");
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
 
