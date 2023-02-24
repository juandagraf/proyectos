class pac { //<>//

  PImage pac;
  int x, y, vel = 3, antX, antY, ind = 0, antInd, fin = 0, vida = 3, antDir, contComida, t;
  int R = 14, XINI = 465, YINI = 465;
  boolean enPortal = false;
  boolean renacer = false;


  pac() {

    x = XINI;
    y = YINI;
    pac = loadImage("Pac/pacDer.png");
    t = millis();
  }

  void dibujar() {

    pushStyle();
    imageMode(CENTER);
    image (pac, x, y);
    popStyle();

    pushStyle();
    textSize(28);
    text ("Vidas "+vida, 940, 300);
    popStyle();

    pushStyle();
    textSize(18);
    fill(0, 100, 170);
    text ("Comida: "+contComida+"/347", 940, 60);
    popStyle();
  }

  void mover() {

    if (x == XINI-90 || x == XINI+90 || y == YINI+60) {
      renacer = false;
    }
    if (x>width) {
      fin=1;
    }

    antInd=ind;
    antX=x;
    antY=y;

    if (keyCode==RIGHT)
    {  
      ind=1;
    } else if (keyCode==UP)
    {
      ind=2;
    } else if (keyCode==DOWN)
    {
      ind=3;
    } else if (keyCode==LEFT)
    {
      ind=4;
    }
    direccion();
    for (int i=0; i<23; i++) {

      for (int j=0; j<37; j++) {

        if (mapa.mapa[i][j]=='X' || mapa.mapa[i][j]=='P') { 

          if ((x-R <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R) && (y-R <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R)) {

            ind=antInd;
            x=antX;
            y=antY;
            direccion();
            for (i=0; i<23; i++) {

              for (j=0; j<37; j++) { 

                if (mapa.mapa[i][j]=='X' || mapa.mapa[i][j]=='P') { 

                  if ((x-R <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R) && (y-R <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R)) {

                    ind=antInd;
                    x=antX;
                    y=antY;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  void direccion()
  {
    if (ind==1) {
      if (renacer) {
        pac = loadImage("Pac/pacMuertoDer.png");
      } 
      else if ((t-millis())%500 < -250) {
          pac = loadImage("Pac/pacDer.png");
        }
        else {
          pac = loadImage("Pac/pacCerDer.png");
        }
      x+=vel;
    } 
    else if (ind==2) {
      if (renacer) {
        pac = loadImage("Pac/pacMuertoArr.png");
      } else if ((t-millis())%500 < -250) {
          pac = loadImage("Pac/pacArr.png");
        }
        else {
          pac = loadImage("Pac/pacCerArr.png");
        }
      y-=vel;

      }

     else if (ind==3) { 
      if (renacer) {
        pac = loadImage("Pac/pacMuertoAba.png");
      } else if ((t-millis())%500 < -250) {
          pac = loadImage("Pac/pacAba.png");
      }
      else {
        pac = loadImage("Pac/pacCerAba.png");
      }
      y+=vel;
     }
     else if (ind==4) { 
      if (renacer) {
        pac = loadImage("Pac/pacMuertoIzq.png");
      } else if ((t-millis())%500 < -250) {
          pac = loadImage("Pac/pacIzq.png");
      }
      else {
        pac = loadImage("Pac/pacCerIzq.png");
      }
      x-=vel;
    }
  }

  void comida() {

    for (int i=0; i<23; i++) {

      for (int j=0; j<37; j++) { 

        if (mapa.mapa[i][j]=='o') { 

          if ((x-R/3 <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R/3) && (y-R/3 <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R/3)) {

            contComida++;
            mapa.mapa[i][j] = 'n';
          }
        }
        if (mapa.mapa[i][j]=='P') {

          if (contComida == 347) {
            mapa.mapa[i][j] = 'n';
          }
        }
      }
    }
  }

  void teletransportar() {

    for (int i=0; i<23; i++) {

      for (int j=0; j<37; j++) {

        if (mapa.mapa[i][j]=='3' && enPortal==false) {

          if ((x-R <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R) && (y-R <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R)) {

            x=(29*30)+15;
            y=(1*30)+15;
            enPortal=true;
          }
        } else if (mapa.mapa[i][j]=='1' && enPortal==false) {

          if ((x-R <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R) && (y-R <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R)) {

            x=(29*30)+15;
            y=(21*30)+15;
            enPortal=true;
          }
        } else if (mapa.mapa[i][j]=='2' && enPortal==false) {

          if ((x-R <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R) && (y-R <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R)) {

            x=(1*30)+15;
            y=(21*30)+15;
            enPortal=true;
          }
        } else    if (mapa.mapa[i][j]=='4' && enPortal==false) {

          if ((x-R <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R) && (y-R <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R)) {

            x=(1*30)+15;
            y=(1*30)+15;
            enPortal=true;
          }
        }
      }
    }
    for (int i=0; i<23; i++) {

      for (int j=0; j<37; j++) {

        if (enPortal==true && (29*mapa.LADO-45 == x || 21*mapa.LADO-45 == y || 1*mapa.LADO+45 == y || 1*mapa.LADO+45 == x)) { 

          if ((x-R <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+R) && (y-R <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+R)) {

            enPortal=false;
          }
        }
      }
    }
  }

  void vida() {

    for (int i = 0; i<fantasma.length; i++) {

      float d = dist(x, y, fantasma[i].x, fantasma[i].y);
      if (d<15 && renacer==false) {
        vida -= 1;
        if (vida == 0) {
          fin = 2;
        }
        renacer();
      }
    }
  }

  void renacer() {

    renacer = true;
    x = XINI;
    y = YINI;
    ind = 0;
    antInd = 0;
    keyCode = ' ';
    pac = loadImage("Pac/pacMuertoDer.png");
  }
}