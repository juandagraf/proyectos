class columna {
  
  int posX, posY;
  int alto1, alto2;
  int TAM = 24;
  PImage pared;
  
  columna(int x, int y) {
    posX = x;
    posY = y;
    alto1 = (int)random(60,541);
    alto2 = (int)random(61, 201);
    pared = loadImage("pared.png");
  }
  
  void mostrar() {
    stroke(255);
    image(pared,posX, posY, TAM, alto1);
    if (alto1+60!=height) {
      image(pared,posX, alto1+alto2, TAM, height);
    }
  }
  
  void mover() {
    posX -= 3;
  }
  
  boolean borde() {
    if (posX <= -30) {
      return true;
    }
    else { return false; }
  }
  
}