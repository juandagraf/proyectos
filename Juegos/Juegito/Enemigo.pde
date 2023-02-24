class enemigo {
  
  int posX, posY;
  PImage ene = loadImage("Enemigo.png");
  int vel = 5;
  
  enemigo(int eX, int eY) {
    posX = eX;
    posY = eY ;
  }
  
  void mostrar() {
      image(ene, posX, posY);
  }
  void mover() { 
    posX -= vel;
  }
  
  boolean borde() {
    if (posX < -42) {
      return true;
    }
    else { 
      return false;
    }
  }
  
}