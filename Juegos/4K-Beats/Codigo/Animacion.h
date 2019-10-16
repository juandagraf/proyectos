#ifndef ANIMACION_H
#define ANIMACION_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class Animacion {

private:
  Clock reloj;
  float transcurrido; //Tiempo transcurrido desde que comenzo la animacion
  float duracion; //Duracion de la animacion
  float comienzo; //Comienzo de la animacion (0)
  float posX, posY;
  float valMax;
  Sprite sprite;
  bool finalizada = false;

public:
  //Utilizar para animar contadores de notas
  Animacion (Sprite &sprite, float posX, float posY, float duracion) {
    this->sprite = sprite;
    this->posX = posX;
    this->posY = posY;
    this->duracion = duracion;
    this->comenzarReloj();
  }
  //Utilizar para fade out musica
  Animacion (float _valMax, float _duracion) {
    valMax = _valMax; //El volumen original de la musica.
    duracion = _duracion;
    comenzarReloj();
  }
  //Utilizar para desvanecer u oscurecer sprite
  Animacion (Sprite &sprite, float duracion) {
    this->sprite = sprite;
    this->duracion = duracion;
    comenzarReloj();
  }
  ~Animacion (){}

  //Animacion de contadores de notas (perfect, miss, etc.)
  void animarContadorNotas(RenderWindow &ventana) {
    transcurrido = tiempoTranscurrido();
    int val = ( transcurrido * 100 ) / 0.05;
    sprite.setColor(Color (255, 255, 255, 250 - val));

    if (transcurrido < 0.1) {
      sprite.setPosition(posX, posY + val * 0.1);
		}
		else if (transcurrido < duracion) {
      finalizada = true;
		}
    ventana.draw(sprite);
  }
  //Desvanece un sprite modificando el alpha en el tiempo
  void desvanecer(RenderWindow &ventana) {
    transcurrido = tiempoTranscurrido();
    int transparencia = 250 - ( transcurrido * 100 ) / (duracion / 2.5); //Dividido 2.5 porque el alpha es 250.
    sprite.setColor(Color(255, 255, 255, transparencia));
    if (transcurrido < duracion) {
      ventana.draw(sprite);
    }
    else {
      finalizada = true;
    }
  }
  //Oscurece un sprite modificando el alpha en el tiempo
  void oscurecer(RenderWindow &ventana) {
    transcurrido = tiempoTranscurrido();
    int transparencia = (250 - ( transcurrido * 100 ) / (duracion / 2.5) ) * -1; //Dividido 2.5 porque el alpha es 250.
    sprite.setColor(Color(255, 255, 255, transparencia));
    if (transcurrido < duracion) {
      ventana.draw(sprite);
    }
    else {
      finalizada = true;
    }
  }
  //Desvanecer el sonido de la musica en seleccion de cancion
  float fadeOutMusica() {
    float nuevoVolumen;
    transcurrido = tiempoTranscurrido();
    if (transcurrido >= duracion+1) {
      //finalizada = true;
      return -1;
    }
    else {
      nuevoVolumen = valMax - ( (transcurrido * 100) / duracion ) * (valMax/100);
      if (nuevoVolumen < 0) {
        nuevoVolumen = 0;
      }
      return nuevoVolumen;
    }
  }
  //Comienza el reloj
  void comenzarReloj() {
    comienzo = reloj.getElapsedTime().asSeconds();
  }
  //Devulve el tiempo transcurrido
  float tiempoTranscurrido() {
    return reloj.getElapsedTime().asSeconds() - comienzo;
  }
  //Devuelve si la animacion esta finalizada o no
  bool finalizado() {
    return finalizada;
  }
};

#endif
