int[][] grilla;
boolean[][] revelado;
boolean[][] minas;
boolean[][] banderas;
int cantMinas;
int ancho, alto;
int mCol, mFil;
int contMinas = 0;
int minasRestantes;
int contadorGanar;
int contadorBanderas;
int tamX, tamY;
String estado = "menu";

void setup() {
  size (600, 400);
}

void draw() {

  switch (estado) {

  case "menu":
    background(0);
    pushStyle();
    noFill();
    stroke(219, 64, 64);
    strokeWeight(3);
    rect (100, 50, 400, 300);
    fill(255, 0, 0);
    textSize(28);
    fill(255);
    textAlign(CENTER);
    text ("Seleccionar dificultad", width/2, 85);
    textAlign(CORNER);
    text ("1: Facil (30 Minas)", 110, 150); //97 49
    text ("2: Media (80 Minas)", 110, 200); //98 50
    text ("3: Dificil (120 Minas)", 110, 250); //99 51
    textAlign(CENTER);
    textSize(22);
    text ("Presione un numero para continuar", width/2, 320);
    popStyle();

    if (keyCode == 97 || keyCode == 49) {
      tamX = 600;
      tamY = 350;
      surface.setSize(tamX, tamY);
      cantMinas = 30;
      estado = "iniciar";
    } else if (keyCode == 98 || keyCode == 50) {
      tamX = 750;
      tamY = 500;
      cantMinas = 80;
      surface.setSize(tamX, tamY);
      estado = "iniciar";
    } else if (keyCode == 99 || keyCode == 51) {
      tamX = 900;
      tamY = 650;
      cantMinas = 120;
      surface.setSize(tamX, tamY);
      estado = "iniciar";
    }
    break;

  case "jugar":
    background(255);
    for (int col = 0; col < alto; col++) {
      for (int fil = 0; fil < ancho; fil++) {
        if (!revelado[col][fil]) {
          if (banderas[col][fil]) {
            fill(0, 0, 220);
          } else {
            fill(130);
          }
        } else if (revelado[col][fil] && minas[col][fil]) {
          fill(200, 0, 0);
        } else {
          noFill();
        }
        if (grilla[col][fil] != 0) { 
          pushStyle();
          fill(0);
          textSize(12);
          text(grilla[col][fil], (fil-1)*30+13, (col-1)*30+17);
          popStyle();
        }
        if (minas[col][fil] && revelado[col][fil]) {
          estado = "perder";
        }
        rect((fil-1)*30, (col-1)*30, 30, 30);
      }
    }
    if (contadorGanar == (((alto-2) * (ancho-2)) - cantMinas) ) {
      estado = "ganar";
    }

    pushStyle();
    fill(51);
    rect (0, height-50, width, height);
    fill(255);
    textSize(22);
    text ("Minas restantes: "+(cantMinas-contadorBanderas), 20, height-15); 
    popStyle();
    mFil = (int)mouseX/30+1;
    mFil = constrain(mFil, 0, tamX/30); //Para que no crashee el programa si saco el mouse de la ventana
    mCol = (int)mouseY/30+1;
    mCol = constrain(mCol, 0, (tamY-50)/30);
    break;

  case "perder":
    pushStyle();
    textSize(16);
    fill(255); 
    text("Presiona espacio para volver al menu.", 250, height-15);
    textSize(12);
    popStyle();
    break;

  case "iniciar":
    alto = (height-50)/30+2;
    ancho = width/30+2;
    contadorGanar = 0;
    contadorBanderas = 0;
    grilla = new int [alto][ancho];
    revelado = new boolean [alto][ancho];
    minas = new boolean [alto][ancho];
    banderas = new boolean [alto][ancho];
    minasRestantes = cantMinas;
    do {
      ponerMinas();
    } while (minasRestantes > 0);
    crearGrilla();
    estado = "jugar";
    break;

  case "ganar":
    textSize(28);
    fill(0);
    textAlign(CENTER);
    text("Felicitaciones, terminaste el juego.", width/2, height/2);
    text("Presiona espacio para volver al menu.", width/2, height/2+35);
    textAlign(CORNER);
    textSize(12);

  default:
    break;
  }
}

void mouseReleased() {
  if (mouseButton == LEFT && estado == "jugar") {
    revelar(mCol, mFil);
  }
  if (mouseButton == RIGHT && estado == "jugar") {
    if (!banderas[mCol][mFil] && !revelado[mCol][mFil]) {
      banderas[mCol][mFil] = true;
      contadorBanderas++;
    } else if (banderas[mCol][mFil]) {
      banderas[mCol][mFil] = false;
      contadorBanderas--;
    }
  }
}

void keyReleased() {
  if ( (estado == "perder" || estado == "ganar") && keyCode == ' ') {
    surface.setSize(600, 400);
    estado = "menu";
  }
}

void ponerMinas() {

  int divisionX = floor(alto/2);
  int divisionY = floor(ancho/2);
  float r;

  if (minasRestantes > floor((cantMinas/4)*3) ) { 
    for (int col = 1; col <  divisionX; col++) {
      for (int fil = 1; fil < divisionY; fil++) {
        if (minasRestantes <= floor((cantMinas/4)*3)) { 
          break;
        }
        r = random(0, 1);
        if (r > 0.8 && !minas[col][fil]) {
          minas[col][fil] = true;
          minasRestantes--;
        }
      }
    }
  } else if (minasRestantes > floor((cantMinas/4)*2) ) {
    for (int col = divisionX; col <  alto-1; col++) {
      for (int fil = 1; fil < divisionY; fil++) {
        if (minasRestantes <= floor((cantMinas/4)*2)) { 
          break;
        }
        r = random(0, 1);
        if (r > 0.8 && !minas[col][fil]) {
          minas[col][fil] = true;
          minasRestantes--;
        }
      }
    }
  } else if (minasRestantes > floor(cantMinas/4) ) {
    for (int col = 1; col <  divisionX; col++) {
      for (int fil = divisionY; fil < ancho-1; fil++) {
        if (minasRestantes <= floor(cantMinas/4)) { 
          break;
        }
        r = random(0, 1);
        if (r > 0.8 && !minas[col][fil]) {
          minas[col][fil] = true;
          minasRestantes--;
        }
      }
    }
  } else if (minasRestantes > 0) {
    for (int col = divisionX; col < alto-1; col++) {
      for (int fil = divisionY; fil < ancho-1; fil++) {
        if (minasRestantes == 0) { 
          break;
        }
        r = random(0, 1);
        if (r > 0.8 && !minas[col][fil]) {
          minas[col][fil] = true;
          minasRestantes--;
        }
      }
    }
  }
}

void crearGrilla() {
  for (int col = 0; col < alto; col++) {
    for (int fil = 0; fil < ancho; fil++) {

      if ( (col > 0 && fil > 0) && (col < alto-1 && fil < ancho-1) && !minas[col][fil]) {
        for (int x = col-1; x < col+2; x++) {
          for (int y = fil-1; y < fil+2; y++) {
            if (minas[x][y]) {
              grilla[x][y] = 11;
              contMinas++;
            }
          }
        }
        grilla[col][fil] = contMinas;
        contMinas = 0;
      }
      if ( (col == 0 && fil > 0 ) || (col == alto-1 && fil > 0) || (fil == 0 && col > 0) || (fil == ancho-1 && col > 0) || (col == 0 && fil == 0) || (col == alto-1 && fil == ancho-1) ) {
        grilla[col][fil] = 12;
      }
    }
  }
}

void revelar(int xPos, int yPos) {

  if (!revelado[xPos][yPos] && !minas[xPos][yPos] && !banderas[xPos][yPos]) {
    if (grilla[xPos][yPos] != 12) {
      revelado[xPos][yPos] = true;
    }
    contadorGanar++;
    if (grilla[xPos][yPos] == 0) {
      if ( (xPos > 0 && yPos > 0) && (xPos < alto-1 && yPos < ancho-1) ) {
        for (int x = xPos-1; x < xPos+2; x++) {
          for (int y = yPos-1; y < yPos+2; y++) {
            if (grilla[x][y] != 12) {
              revelar (x, y);
            }
          }
        }
      }
    }
  } else if (minas[xPos][yPos] && !banderas[xPos][yPos]) {
    for (int col = 0; col < alto; col++) {
      for (int fil = 0; fil < ancho; fil++) {
        if (minas[col][fil]) {
          revelado[col][fil] = true;
        }
      }
    }
  }
}