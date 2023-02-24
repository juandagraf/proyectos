class nave {

  int posX, posY;
  int TAM = 30;
  boolean arriba, abajo, muerto;
  int t;
  PImage nav = loadImage("Nave.png");
  int score;
  int vida;
  boolean poderTeclas = false;

  nave(int x, int y) {
    posX = x;
    posY = y;
    score = 0;
    vida = 3;
    t = 0;
    muerto = false;
  }

  void mostrar() {
    fill(170, 0, 200);
    stroke(170, 0, 200);
    if (muerto && millis()%1000 < 250) {
      //No dibujar
    } else {
      pushStyle();
      imageMode(CENTER);
      image(nav, posX, posY);
      popStyle();
    }
  }

  void mover() {
    if (poderTeclas) {
      if (arriba) {
        posY+=5;
      } else if (abajo) {
        posY -= 5;
      }
    } else {

      if (arriba) {
        posY -= 5;
      }
      if (abajo) {
        posY += 5;
      }
    }
  }

  void choque() {
    if (millis()-t > 3000) {
      muerto = false;
    }
    for (int i = 0; i < columnas.size(); i++) {

      if ((columnas.get(i).posX < 55 && columnas.get(i).posX > 11) && ( posY - columnas.get(i).alto1 < 15 || columnas.get(i).alto1+columnas.get(i).alto2 - posY < 15 )) {
        if (!muerto) {
          posX = 40;
          posY = height/2;
          muerto = true;
          t = millis();
          vida--;
        }
      } else {
        if (columnas.get(i).posX-posX < 2 && !muerto) {
          score += 10;
        }
      }
    }
    if (posY < 15) {
      posY = 15;
    } else if (posY > height-15) {
      posY = height-15;
    }
    if (!muerto) {
      for (int i = 0; i < enemigos.size(); i++) {
        if ( (enemigos.get(i).posX - posX < 15) && (enemigos.get(i).posY - posY > -27 && enemigos.get(i).posY - posY < 15) ) {
          posX = 40;
          posY = height/2;
          muerto = true;
          t = millis();
          vida--;
        }
      }
    }
  }

  void dibujarDisparo() {
    fill(2, 21, 211);
    //noStroke();
    ellipse (posX, posY, 4, 4);
  }

  void moverDisparo() {
    posX += 3;
  }

  boolean colisionDisparo() {
    boolean b = false;
    for (int i = enemigos.size(); i > 0; i--) {
      enemigo e = enemigos.get(i-1);
      if ( (posX+2 >= e.posX && posX-2 <=e.posX+42) && (posY+2 >= e.posY && posY-2 <= e.posY+12) ) {
        enemigos.remove(i-1);
        b = true;
      } else {
        b = false;
      }
    }
    return b;
  }

  boolean direccion (boolean b, int t) {

    switch(t) {
    case 38: 
      return arriba = b;
    case 40: 
      return abajo  = b;
    default:      
      return b;
    }
  }

  void poderDibujar() {
    if (!poderActivo) {
      pushStyle();
      fill(255);
      ellipse(posX, posY, 30, 30);
      popStyle();
    }
  }

  void poderMover() {
    if (!poderActivo) posX -= 3;
  }

  void poderActivar() {
    if ( ((posX-n.posX <= 30 && n.posX-posX <=30) && (posY-n.posY <= 30 && n.posY-posY <= 30)) && !poderActivo) {
      poderActivo = true;
      poder = null;
      timerPoder = millis();
      if (ran == 0) {
        ran =random(0,1.1);
      }
    }
    if (poderActivo) {
      if (ran < 0.2) {
        for (int i = 0; i < enemigos.size(); i++) { //Acelero a los enemigos
          enemigo e = enemigos.get(i);
          e.vel = 8;
        }
      } else if (ran < 0.4) {
        for (int i = 0; i < enemigos.size(); i++) { //Desacelero a los enemigos
          enemigo e = enemigos.get(i);
          e.vel = 0;
        }
      } else if (ran < 0.6) {
          n.poderTeclas = true;
      } else if (ran < 0.8) {
          n.score += 5000;
          poderActivo = false;
      } else if (ran < 1) {
          n.score -= 6000;
          poderActivo = false;

      }
      else {
        n.vida++;
        poderActivo = false;
        poder = null;
      }
    }
  }
  void rango() {
    if (posX < -15) poder = null;
  }
}
