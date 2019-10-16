#ifndef PULSADOR_H
#define PULSADOR_H
#include <SFML/Graphics.hpp>
#include "Animacion.h"

using namespace sf;
using namespace std;

class Pulsador {

private:
	char tecla; //Tecla del pulsador
	Animacion* anim_luz_pulsador; //Animacion de la luz del pulsador
	Texture textura_luz_pulsador; //Textura de la luz del pulsador
	Sprite sprite_luz_pulsador; //Imagen de la luz del pulsador
	bool pulsado; //Estado que almacena si esta pulsado o no
	bool check; //Almacena si ya se chequeo o no el timing del pulsador

public:
	Pulsador(char c) {
		pulsado = false;
		tecla = c;
		anim_luz_pulsador = NULL;
		if (!textura_luz_pulsador.loadFromFile("Recursos/Area de Juego/luz_pulsador.png")) {
			cout << "Error al cargar Recursos/Area de Juego/luz_pulsador.png" << endl;
		}
		else {
			sprite_luz_pulsador.setTexture(textura_luz_pulsador);
			if (c == 'D') {
				sprite_luz_pulsador.setPosition(101, 108);
			}
			else if (c == 'F') {
				sprite_luz_pulsador.setPosition(161, 108);
			}
			else if (c == 'J') {
				sprite_luz_pulsador.setPosition(221, 108);
			}
			else {
				sprite_luz_pulsador.setPosition(281, 108);
			}
		}
	}
	~Pulsador(){}
	/*
	 * Revisa si algun pulsador esta siendo pulsado o no (teclas d, f, j o k), y cambia su estado.
	 * Al cambiar el estado pulsado a true tambien toca el sonido de la tecla "hit", y al soltarlo
	 * cambia el estado de pulsado a falase y check a false.
	*/
	void checkPulsador(Event &evento) {
		if (tecla == 'D') {
			if (Keyboard::isKeyPressed(Keyboard::D) && !pulsado) {
	      pulsado = true;
				playHit();
			}
	    else if ( (evento.type == Event::KeyReleased && pulsado) && !Keyboard::isKeyPressed(Keyboard::D)) {
	      pulsado = false;
				check = false;
	    }
		}
		else if (tecla == 'F') {
			if (Keyboard::isKeyPressed(Keyboard::F) && !pulsado) {
	      pulsado = true;
				playHit();

			}
	    else if ( (evento.type == Event::KeyReleased && pulsado) && !Keyboard::isKeyPressed(Keyboard::F)) {
	      pulsado = false;
				check = false;
	    }
		}
		else if (tecla == 'J') {
			if (Keyboard::isKeyPressed(Keyboard::J) && !pulsado) {
	      pulsado = true;
				playHit();
			}
	    else if ( (evento.type == Event::KeyReleased && pulsado) && !Keyboard::isKeyPressed(Keyboard::J)) {
	      pulsado = false;
				check = false;
	    }
		}
		else if (tecla == 'K') {
			if (Keyboard::isKeyPressed(Keyboard::K) && !pulsado) {
	      pulsado = true;
				playHit();
			}
	    else if ( (evento.type == Event::KeyReleased && pulsado) && !Keyboard::isKeyPressed(Keyboard::K)) {
	      pulsado = false;
				check = false;
	    }
		}
	}

	void playHit() {
		extern Sound sfx_hit;
		sfx_hit.play();
	}

	void animarPulsador(RenderWindow &ventana) {
		if (pulsado) {
			if (anim_luz_pulsador != NULL) {
				delete anim_luz_pulsador;
				anim_luz_pulsador = new Animacion(sprite_luz_pulsador, 0.3);
			}
			else {
				anim_luz_pulsador = new Animacion(sprite_luz_pulsador, 0.3);
			}
	}
	  if (anim_luz_pulsador != NULL) {
			if (!anim_luz_pulsador->finalizado()) {
				anim_luz_pulsador->desvanecer(ventana);
			}
			else {
				delete anim_luz_pulsador;
				anim_luz_pulsador = NULL;
			}
		}
	}
	//Devuelve verdadero si esta siendo pulsado.
	bool estaPulsado() {
		return pulsado;
	}
	//Devuelve el estado de check
	bool checked() {
		return check;
	}
	//Cambia el estado de check
	void cambiarEstado(bool b) {
		check = b;
	}
};

#endif
