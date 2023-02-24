
pac pacman;
mapa mapa;
fantasma[] fantasma;

PImage menuComenzar, menuSalir, menuActual;
String estadoJuego; 

void setup() {
  
  size(1100,690);
  estadoJuego = "INICIO";
  mapa = new mapa();
  pacman = new pac();
  fantasma = new fantasma[4];
  menuComenzar = loadImage ("Menu/menuComenzar.jpg");
  menuActual = menuComenzar;
  menuSalir = loadImage ("Menu/menuSalir.jpg");
  
  for (int i = 0; i < fantasma.length; i ++) { 
    fantasma[i] = new fantasma(i+1);
  }
}

void draw() {
  
  background(0);
  switch (estadoJuego) {
    case "INICIO":
    menu();
    break;
    case "JUGAR":
    jugar();
    break;
    case "FINJUEGO":
    finJuego();
    break;
  }
}

void menu() {
  
  image(menuActual,0,0);
  if (keyCode == UP) {
    menuActual = menuComenzar;
  }
  if (keyCode == DOWN) {
    menuActual = menuSalir;
  }
  if (keyCode == ENTER) {
    if (menuActual == menuComenzar) {
      estadoJuego = "JUGAR";
    } else {
      exit();
    }
  }
}

void jugar() {
  
  mapa.dibujar();
  pacman.dibujar();
  pacman.mover();
  pacman.comida();
  pacman.teletransportar();
  pacman.vida();
  
  for (int i = 0; i < fantasma.length; i ++) { 
    fantasma[i].dibujar();
    fantasma[i].mover();
    fantasma[i].teletransportar();
  }
 if (pacman.fin==1 || pacman.fin==2) {
   estadoJuego = "FINJUEGO";
 }
}

 void finJuego() {
   
   if (pacman.fin == 1) {
     textSize(18);
     text ("Felicidades, terminaste el juego.", 410, height/2);
     text ("Tu tiempo es de: "+mapa.h+":"+mapa.m+":"+mapa.s, 450, height/2+20);
   }
   else if (pacman.fin == 2) {
     textSize(18);
     text ("Mejor suerte la proxima",430, height/2);
   }
 }