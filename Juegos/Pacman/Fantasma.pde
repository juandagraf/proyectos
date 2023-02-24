class fantasma { //<>//
  
  PImage fantasma;
  int x, y, LADO = 14, antX, antY, ind = 3, antInd, antDir, vel = 3, colorFantasma;
  boolean enPortal = false;
  fantasma(int _colorFantasma) {
    
    colorFantasma = _colorFantasma;
    fantasma = loadImage("FantasmaRojo/rojoFantasmaDer.png");
    x = 465;
    y = 345;
  }

  void dibujar() {
    
    pushStyle();
    imageMode(CENTER);
    image(fantasma, x, y);
    popStyle();
  }

  void mover() {
    
    antInd=ind;
    antX=x;
    antY=y;
    direccion();
    for (int i=0; i<23; i++) {
      
      for (int j=0; j<37; j++) { 
        
        if (mapa.mapa[i][j]=='X') { 
          
          if ((x-LADO <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+LADO) && (y-LADO <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+LADO))
          {
            
            x=antX;
            y=antY;
            direccion();
            for (i=0; i<23; i++) {
              
              for (j=0; j<37; j++) { 
                
                if (mapa.mapa[i][j]=='X') { 
                  
                  if ((x-LADO <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+LADO) && (y-LADO <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+LADO))  
                  {       
                    
                    x=antX;
                    y=antY;
                    antInd = ind;
                    while (ind==antInd) {
                      ind = (int)random(1, 5);
                    }
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
      
      if (colorFantasma == 1) {
        fantasma = loadImage("FantasmaRojo/rojoFantasmaDer.png");
      }
      if (colorFantasma == 2) {
        fantasma = loadImage("FantasmaAmarillo/amarilloFantasmaDer.png");
      }
      if (colorFantasma == 3) {
        fantasma = loadImage("FantasmaAzul/azulFantasmaDer.png");
      }
      if (colorFantasma == 4) {
        fantasma = loadImage("FantasmaVerde/verdeFantasmaDer.png");
      }
      x+=vel;
    } else if (ind==2) {
      
      if (colorFantasma == 1) {
        fantasma = loadImage("FantasmaRojo/rojoFantasmaArr.png");
      }
      if (colorFantasma == 2) {
        fantasma = loadImage("FantasmaAmarillo/amarilloFantasmaArr.png");
      }
      if (colorFantasma == 3) {
        fantasma = loadImage("FantasmaAzul/azulFantasmaArr.png");
      }
      if (colorFantasma == 4) {
        fantasma = loadImage("FantasmaVerde/verdeFantasmaArr.png");
      }
      y-=vel;
    } else if (ind==3) { 
      
      if (colorFantasma == 1) {
        fantasma = loadImage("FantasmaRojo/rojoFantasmaAba.png");
      }
      if (colorFantasma == 2) {
        fantasma = loadImage("FantasmaAmarillo/amarilloFantasmaAba.png");
      }
      if (colorFantasma == 3) {
        fantasma = loadImage("FantasmaAzul/azulFantasmaAba.png");
      }
      if (colorFantasma == 4) {
        fantasma = loadImage("FantasmaVerde/verdeFantasmaAba.png");
      }
      y+=vel;
    } else if (ind==4) { 
      
      if (colorFantasma == 1) {
        fantasma = loadImage("FantasmaRojo/rojoFantasmaIzq.png");
      }
      if (colorFantasma == 2) {
        fantasma = loadImage("FantasmaAmarillo/amarilloFantasmaIzq.png");
      }
      if (colorFantasma == 3) {
        fantasma = loadImage("FantasmaAzul/azulFantasmaIzq.png");
      }
      if (colorFantasma == 4) {
        fantasma = loadImage("FantasmaVerde/verdeFantasmaIzq.png");
      }
      x-=vel;
    }
  }

  void teletransportar() {
    
    for (int i=0; i<23; i++) {
      
      for (int j=0; j<37; j++) {
        
        if (mapa.mapa[i][j]=='3' && enPortal==false) {
          
          if ((x-LADO <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+LADO) && (y-LADO <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+LADO)) {
            
            x=(29*30)+15;
            y=(1*30)+15;
            enPortal=true;
          }
        } else if (mapa.mapa[i][j]=='1' && enPortal==false) {
          
          if ((x-LADO <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+LADO) && (y-LADO <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+LADO)) {
            
            x=(29*30)+15;
            y=(21*30)+15;
            enPortal=true;
          }
        } else if (mapa.mapa[i][j]=='2' && enPortal==false) {
          
          if ((x-LADO <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+LADO) && (y-LADO <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+LADO)) {
            
            x=(1*30)+15;
            y=(21*30)+15;
            enPortal=true;
          }
        } else    if (mapa.mapa[i][j]=='4' && enPortal==false) {
          
          if ((x-LADO <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+LADO) && (y-LADO <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+LADO)) {
            
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
          
          if ((x-LADO <= j*mapa.LADO+mapa.LADO) && ( j*mapa.LADO<= x+LADO) && (y-LADO <= i*mapa.LADO+mapa.LADO) && (i*mapa.LADO <= y+LADO)) {
            
            enPortal=false;
          }
        }
      }
    }
  }
}