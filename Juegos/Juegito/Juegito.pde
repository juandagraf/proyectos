nave n;
nave poder;
int tiempo, tiempoEnemigo;
ArrayList<columna> columnas = new ArrayList<columna>();
ArrayList<enemigo> enemigos = new ArrayList<enemigo>();
ArrayList<nave> disparos = new ArrayList<nave>();
String estadoJuego = "MENU";
int ind;
int contColumna = 0;
float ran;
int timerPoder;
boolean poderActivo = false;

void setup() {
  size(720,600);
  n = new nave(40, height/2);
  ind = 0;
}

void draw() {
  background(30);
  switch(estadoJuego) {
    
    case "MENU":
    menu();
    break;
    case "JUGAR":
    jugar();
    break;
    case "FIN":
    fin();
    break;
    default:
    break;
  }
  
}

void keyPressed() {
  n.direccion(true, keyCode);
}

void keyReleased() {
  n.direccion(false, keyCode);
  if (key == 'f' && estadoJuego == "JUGAR") {
    disparos.add(new nave (n.posX, n.posY));
  }
}

void menu() {
  

  textSize(42);
  text ("Juego nuevo", 240, 320);
  text ("Salir", 240, 380);
  textSize (64);
  text ("NAVE", 280, 200);
  pushStyle();
  noFill();
  strokeWeight(2);
  stroke(#50459B);
  rect (250, 130, 230, 90);
  popStyle();
  noStroke();
  fill(230);
  if (keyPressed && keyCode == DOWN) {
    if (ind == 0) {
      ind = 1;
    } else if (ind == 1) {
      ind = 0;
    }
    keyCode = ' ';
  } else if (keyPressed && keyCode == UP) {
    if (ind == 0) {
      ind = 1;
    } else if (ind == 1) {
      ind = 0;
    }
    keyCode = ' ';
  }
  if (ind == 0) {
    ellipse (210, 305, 25, 25);
  } else if (ind == 1) {
    ellipse (210, 365, 25, 25);
  } 

  if (keyCode == ENTER || keyCode == BACKSPACE) {
    if (ind == 0) {
      iniciar();
      estadoJuego = "JUGAR";
    } else {
      exit();
    }
  }
}

void jugar() {
  
  if (n.vida != 0) {
    n.mostrar();
    n.mover();
    if (tiempo > 100) { 
      columnas.add(new columna(width+30, 0));
      tiempo = 0;
      contColumna++;
    if (contColumna == 3) {
      if (poder == null) {
        columna c = columnas.get(columnas.size()-1);
        poder = new nave(c.posX+12, c.alto1+c.alto2/2);
        println("crear poder");
      }
      contColumna = 0;
    }
  }
  if (tiempoEnemigo > 55) { 
    enemigos.add(new enemigo(width, (int)random(0,height-12)));
    tiempoEnemigo = 0;
  }
    for (int i = columnas.size(); i > 0; i--) {
      columna c = columnas.get(i-1);
      c.mostrar();
      c.mover();
      if (c.borde()) { 
        columnas.remove(i-1);
      }
    }
    for (int i = enemigos.size(); i > 0; i--) {
      enemigo e = enemigos.get(i-1);
      e.mostrar();
      e.mover();
      
        if (!poderActivo) {
          e.vel = 5;
        }
      
      if (e.borde()) {
        enemigos.remove(i-1);
      }
    }
    for (int i = disparos.size(); i > 0; i--) {
       nave disp = disparos.get(i-1);
       disp.dibujarDisparo();
       disp.moverDisparo();
       if (disp.colisionDisparo()) {
         disparos.remove(i-1);
       }
    }
  if (poder != null) {
    if (!poderActivo) {
    poder.poderDibujar();
    poder.poderMover();
    }
    else {
      if (millis()-timerPoder > 4000 || ran >= 0.8) {
        poderActivo = false;
        n.poderTeclas = false;
        ran = 0;
        contColumna = 0;
        poder = null;
        println("desactivar");
      }
    }
    if (poder != null) poder.poderActivar();
    if (poder != null) poder.rango();

  }
  n.choque();
  tiempo++;
  tiempoEnemigo++;
  textSize(18);
  text("Score: "+n.score+"  Vidas:"+n.vida, width/2-70, height-20);
  }
  else {
    
    if (columnas.size()!=0) {
    for (int i = columnas.size(); i > 0; i--) { 
        columnas.remove(i-1);
    } 
  }
  if (enemigos.size()!=0) {
    for (int i = enemigos.size(); i > 0; i--) {
        enemigos.remove(i-1);
    }
  }
  if (poder != null) poder = null;
  poderActivo = false;
  n.poderTeclas = false;
  if (disparos.size()!=0) {
    for (int i = disparos.size(); i > 0; i--) {
        disparos.remove(i-1);
    }
  }
    estadoJuego = "FIN";
  }
}

void fin() {
  
  textSize (32);
  text("El puntaje obtenido es de: "+n.score,130, 300);
  text("Pulse espacio para volver al menu.", 90, 340);
  if (keyPressed && key == ' ') {
    estadoJuego = "MENU";
  }
}

void iniciar() {
  ind = 0;
  n = new nave(40, height/2);
  columnas.add(new columna(width+30, 0));
  tiempo = 0;
  tiempoEnemigo = 0;
}