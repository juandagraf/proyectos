class movil {

  int tam, velMax, vida, fin, renacerT;
  PVector vel, acel, pos, iniPos = new PVector (550, 400);
  boolean arriba, abajo, izquierda, derecha;
  float ang, astDir;
  boolean renacer;
  PImage ast = loadImage("asteroide.png");

  movil(PVector _pos, int _tam, int _velMax, float direccion, int _vida) {
    fin = 0;
    renacer = false;
    vida = _vida;
    vel = new PVector (0, 0);
    acel = new PVector (0, 0);
    pos = _pos;
    tam = _tam;
    velMax = _velMax;
    astDir = direccion;
    ast.resize(tam, tam);
  }

  void naveMovimiento() {
    if (arriba) {
      acel = PVector.fromAngle(ang-PI/2);
      vel.add(acel);
    }
    if (abajo) {
      vel.mult(0);
    }
    if (derecha) {
      ang+=0.1;
    }
    if (izquierda) {
      ang-=0.1;
    }

    vel.limit(velMax);
    pos.add(vel);
  }

  void mostrarAst() {
    pushStyle();
    imageMode(CENTER);
    image(ast, pos.x, pos.y);
    popStyle();
  }

  void moverAst() {

    astDir = constrain(astDir, 0.1, 4);
    acel = PVector.fromAngle(ang-PI/astDir);
    vel.add(acel);
    vel.limit(velMax);
    pos.add(vel);
  }

  void mostrar() {
    pushMatrix();
    translate (pos.x, pos.y);
    noFill();
    stroke(255);
    rotate(ang);
    pushStyle();
    if (millis()-renacerT >2000 && renacer) {
      renacer = false;
    }
    if (millis()%500<250 && renacer) {
      //Titilar
    } else {
      triangle(tam, tam, -tam, +tam, 0, -tam);
      ellipse(0, 0, 12, 12);
      stroke(155, 0, 0);
      strokeWeight(3);
      line(0, -tam, tam/2, 0);
      line(0, -tam, -tam/2, 0);
    }
    popStyle();
    popMatrix();
  }

  boolean direccionNave(int tecla, boolean b) {
    //println("tecla= "+tecla); //Probar tecla presionada.
    switch (tecla) {
    case 87: //UP (valores anteriores)
      return arriba = b;

    case 83: //DOWN
      return abajo = b;

    case 65: //LEFT
      return izquierda = b;

    case 68: //RIGHT
      return derecha = b;

    default:
      return b;
    }
  }
  void bordes() {
    if (pos.x<= 0)        pos.x = width-15;   //IZQUIERDA 
    if (pos.x>= width+1)  pos.x = 15;         //DERECHA
    if (pos.y<= 0)        pos.y = height-15;  //ARRIBA 
    if (pos.y>= height+1) pos.y = 15;         //ABAJO
  }

  void vidaAst() {

    for (int i = 0; i<asteroides.length; i++) {
      for (int j = 0; j<disparos.length; j++) {
        if (asteroides[i] != null && disparos[j] != null) {
          float d = dist(asteroides[i].pos.x, asteroides[i].pos.y, disparos[j].x, disparos[j].y);
          if (d<asteroides[i].tam/2) {
            asteroides[i].vida--;
            disparos[j] = null;
          }
          if (asteroides[i].vida==0) {
            asteroides[i] = null;
          }
        }
      }
    }
  }

  void vida() {
    if (vida == 0) {
      fin = 1;
    }
    for (int i = 0; i<asteroides.length; i++) {
      if (asteroides[i] != null) {
        float d = dist (pos.x, pos.y, asteroides[i].pos.x, asteroides[i].pos.y);
        if (d<asteroides[i].tam && renacer == false) {
          vida--;
          nave.acel = new PVector (0, 0);
          nave.vel = new PVector(0, 0);
          renacer = true;
          renacerT = millis();
        }
      }
    }
  }
}