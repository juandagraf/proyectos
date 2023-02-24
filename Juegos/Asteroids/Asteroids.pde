/** Asteroids **/

movil[] asteroides;
movil nave;

String estado = "FIN";
int t, h, m, s, ind = 0;
PVector[] disparos = new PVector[20];
int x, y, aux, tam, vel, v;
float dir;
PVector pos;
PImage ms, fondo;

void setup() {
  size(1100, 800);
  ms = loadImage ("index.jpg");
  fondo = loadImage ("fondo.jpg");
  //fullScreen();
  creacion();
}

void draw() {
  if (nave.fin != 0) {
    estado = "FIN";
  }
  switch (estado) {
  case "INI":
    ini();
    break;
  case "JUEGO":
    juego();
    break;
  case "FIN":
    fin();
    break;
  }
}

void ini() {

  image(ms, 0, 0);
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
    ellipse (330, 385, 25, 25);
  } else if (ind == 1) {
    ellipse (395, 500, 25, 25);
  } 
  //else {
  //  ellipse (435, 648, 25, 25);
  //}
  if (keyCode == ENTER || keyCode == BACKSPACE) {
    if (ind == 0) {
      t = millis();
      estado = "JUEGO";
    } else {
      exit();
    }
  }
}

void juego() {

  background(fondo);
  //Asteroides
  for (int i = 0; i<asteroides.length; i++) {
    if (asteroides[i] != null) {
      asteroides[i].mostrarAst();
      asteroides[i].moverAst();
      asteroides[i].bordes();
      asteroides[i].vidaAst();
    } else { 
      crearAsteroide(i);
    }
  }
  //Disparos de la nave
  for (int i = 0; i<disparos.length; i++) {
    if (disparos[i]!=null) {
      pushStyle();
      strokeWeight(2);
      stroke(0, 100, 180);
      ellipse (disparos[i].x, disparos[i].y, 5, 5);
      popStyle();

      PVector velDis = PVector.fromAngle(disparos[i].z-PI/2);
      velDis.mult(10);
      disparos[i].add(velDis);

      if (disparos[i].x < 0 || disparos[i].x > width || disparos[i].y > height || disparos[i].y < 0) {
        disparos[i] = null;
      }
    }
  }
  //Nave
  nave.naveMovimiento();
  nave.mostrar();
  nave.bordes();
  nave.vida();

  //Tiempo de juego
  s = (millis() - t)/1000;

  if (s == 60) {
    t = t*60;
    m++;
    s = 0;
  } else if (m == 60) {
    m = 0;
    h++;
  }
  textSize(18);
  text("Tiempo: "+h+" : "+m+": "+s, (width/4)*3, 20);
  //Vidas
  text("Vidas: "+nave.vida, (width/3)*2, 20);
}

void fin() {
  background(0);
  if (nave.fin == 1) {
    pushStyle();
    textSize (72);
    text ("Juego terminado", width/3-100, (height/8)*3);
    textSize(52);
    text ("Tu tiempo: "+h+" : "+m+": "+s, width/3 -55, (height/8)*4);
    textSize(32);
    text ("Presiona enter para volver al menu principal", 200, 600);
    popStyle();

    if (keyCode == ENTER) {
      keyCode = ' ';
      creacion();
      estado = "INI";
    }
  } else {
    estado = "INI";
  }
}

void crearAsteroide(int _i) {
  x = (int)random(width);
  y = (int)random(height);
  pos = new PVector (x, y);
  aux = (int)random(50);
  tam = constrain(aux, 20, 50);
  vel = (int)random(1, 5);
  dir = random(5);
  v = floor( map(tam, 20, 50, 1, 4) );
  asteroides[_i] = new movil(pos, tam, vel, dir, v);
}

void creacion() {
  pos = new PVector (width/2, height/2);
  nave = new movil (pos, 15, 2, 0, 3);
  asteroides = new movil[25];
  //Crear asteroides
  for (int i = 0; i<asteroides.length; i++) {
    crearAsteroide(i);
  }
}

void keyPressed() {
  //Movimiento nave
  nave.direccionNave(keyCode, true);
  //Crear disparos
  if (keyCode == 74) {

    for (int i = 0; i<disparos.length; i++) {
      if (disparos[i] == null) {
        disparos[i] = new PVector (nave.pos.x, nave.pos.y, nave.ang);
        break;
      }
    }
  }
}
void keyReleased() {
  nave.direccionNave(keyCode, false);
}