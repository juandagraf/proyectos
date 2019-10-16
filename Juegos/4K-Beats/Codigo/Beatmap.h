#ifndef BEATMAP_H
#define BEATMAP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include "Animacion.h"

using namespace sf;
using namespace std;

class Beatmap {

private:
	unsigned int dificultad_seleccionada;
	static const int CANTIDAD_DATOS_CANCION = 3;
	static const int POSICION_METADATA_X = 110;
	static const int POSICION_METADATA_Y = 450;
	string nombre;
	string artista;
  string duracion;
	string dir_archivos;
	vector<string> dificultades;
	vector<string> dificultadesNum;
	Text* texto_metadata[CANTIDAD_DATOS_CANCION];
	vector<Text*> texto_dificultades;
	Music song;
	Texture textura_portada, textura_cuadro_datos, textura_cuadro_dificultad, textura_selector_dificultad;
	Sprite sprite_portada, sprite_cuadro_datos;
	Sprite sprite_imagen_fondo, sprite_selector_dificultad;
	vector<Sprite*> sprites_cuadros_dificultad;
	Animacion* musicaFadeOut = NULL;

public:
	Beatmap(const string _nombre, const string _artista, const string _duracion, const string _dir_archivos, const vector <string> _dificultades, const vector<string> _dificultadesNum) {

		extern int volumen_musica;
		dificultad_seleccionada = 0;
		nombre = _nombre;
		artista = _artista;
		duracion = _duracion;
		dificultades = _dificultades;
		dir_archivos = _dir_archivos;
		dificultadesNum = _dificultadesNum;

		if (!song.openFromFile(dir_archivos + "cancion.ogg")) {
			cout << "Error al cargar " + dir_archivos + "/cancion.ogg" << endl;
			exit(1);
		}
		else {
			setVolumen(volumen_musica);
		}
		crearSprites();
		setTextos();
	}
	~Beatmap() {}

	void crearSprites() {

		if (!textura_portada.loadFromFile(dir_archivos + "background.png")) {
			cout << "Error al cargar la textura " << dir_archivos << "/background.png" << endl;
		}
		else {
			sprite_imagen_fondo.setTexture(textura_portada);
			sprite_imagen_fondo.setColor( Color(255, 255, 255, 150) );
			sprite_portada.setTexture(textura_portada);
			sprite_portada.scale(0.45f, 0.45f);
			sprite_portada.setPosition(100, 80);
		}
		if (!textura_cuadro_datos.loadFromFile("Recursos/Seleccion de canciones/cuadro_cancion.png")) {
			cout << "Error al cargar la textura Recursos/Seleccion de canciones/cuadro_cancion.png" << endl;
		}
		else {
			sprite_cuadro_datos.setTexture(textura_cuadro_datos);
			sprite_cuadro_datos.setPosition(87.5f, 45);
		}
		if (!textura_selector_dificultad.loadFromFile("Recursos/Seleccion de canciones/cuadro_selector.png")) {
			cout << "Error al cargar la textura Recursos/Seleccion de canciones/cuadro_selector.png" << endl;
		}
		else {
			sprite_selector_dificultad.setTexture(textura_selector_dificultad);
			sprite_selector_dificultad.setPosition(760, 310);
		}
		if (!textura_cuadro_dificultad.loadFromFile("Recursos/Seleccion de canciones/cuadro_dificultad.png")) {
			cout << "Error al cargar la textura Recursos/Seleccion de canciones/cuadro_dificultad.png" << endl;
		}
		else {
			for (unsigned int i = 0; i < dificultadesNum.size(); i++) {
				sprites_cuadros_dificultad.push_back(new Sprite);
				sprites_cuadros_dificultad[i]->setTexture(textura_cuadro_dificultad);
				sprites_cuadros_dificultad[i]->setPosition(750, 300 + i*120);
			}
		}
	}

	void setTextos() {
		extern Font fuente;
		for (unsigned int i = 0; i < dificultadesNum.size(); i++) {
			texto_dificultades.push_back(new Text);
			texto_dificultades[i]->setFont(fuente);
			texto_dificultades[i]->setPosition(930, 340 + i*120);
			texto_dificultades[i]->setCharacterSize(24);
			texto_dificultades[i]->setFillColor(Color::Black);
			texto_dificultades[i]->setString("Dificultad " + dificultadesNum[i]);
		}
		for (unsigned int i = 0; i < CANTIDAD_DATOS_CANCION; i++) {
			texto_metadata[i] = new Text;
			texto_metadata[i]->setFont(fuente);
			texto_metadata[i]->setPosition(POSICION_METADATA_X, POSICION_METADATA_Y + i*35);
			texto_metadata[i]->setCharacterSize(24);
			texto_metadata[i]->setFillColor(Color::Black);
			if (i == 0) {
				texto_metadata[i]->setString("Nombre: " + nombre);
			}
			else if (i == 1) {
				texto_metadata[i]->setString("Artista: " + artista);
			}
			else {
				texto_metadata[i]->setString("Duracion: " + duracion);
			}
		}
	}

	void setVolumen(int &volumen_musica) {
		song.setVolume(volumen_musica);
	}

	void tocarPreview() {
    //Suena una parte de la cancion.
    Time preview = milliseconds(30000);
    song.setPlayingOffset(preview);
    song.play();
	}

	void detenerPreview() {
		song.stop();
	}

	void mostrarSeleccionCanciones(RenderWindow &ventana) {
		ventana.draw(sprite_imagen_fondo);
		this->mostrarMetadata(ventana);
		this->mostrarDificultades(ventana);
	}

	void mostrarMetadata(RenderWindow &ventana) {
		ventana.draw(sprite_cuadro_datos);
		ventana.draw(sprite_portada);
		for (int i = 0; i < CANTIDAD_DATOS_CANCION; i++) {
			ventana.draw(*texto_metadata[i]);
		}
	}

	void mostrarDificultades(RenderWindow &ventana) {
		for (unsigned int i = 0; i < texto_dificultades.size(); i++) {
			ventana.draw(*sprites_cuadros_dificultad[i]);
			ventana.draw(*texto_dificultades[i]);
		}
		ventana.draw(sprite_selector_dificultad);
	}

	void animarPreview() {
		extern int volumen_musica;
		Time t = song.getPlayingOffset();
		int32_t offsetActual = t.asMilliseconds();
		if (offsetActual-45000 >= 0) {
			if (musicaFadeOut == NULL) {
				musicaFadeOut = new Animacion(volumen_musica, 3);
			}
			else {
				float vol = musicaFadeOut->fadeOutMusica();
				if (vol != -1) {
					song.setVolume(vol);
				}
				else {
					delete musicaFadeOut;
					musicaFadeOut = NULL;
					this->tocarPreview();
					song.setVolume(volumen_musica);
				}
			}
		}
	}

	void seleccionarDificultad(int tecla) {
		if (tecla == Keyboard::Up) {
			if (dificultad_seleccionada > 0) {
				dificultad_seleccionada--;
				for (unsigned int i = 0; i < sprites_cuadros_dificultad.size(); i++) {
					sprites_cuadros_dificultad[i]->setPosition(750, 300 - (dificultad_seleccionada*120) + i*120);
					texto_dificultades[i]->setPosition(930, 340  - (dificultad_seleccionada*120) + i*120);
				}
			}
		}
		else if (tecla == Keyboard::Down) {
			if (dificultad_seleccionada < dificultadesNum.size()-1) {
				dificultad_seleccionada++;
				for (unsigned int i = 0; i < sprites_cuadros_dificultad.size(); i++) {
					sprites_cuadros_dificultad[i]->setPosition(750, 300 - (dificultad_seleccionada*120) + i*120 );
					texto_dificultades[i]->setPosition(930, 340 - (dificultad_seleccionada*120) + i*120);
				}
			}
		}
	}

	Sprite getBackground() {
		return sprite_imagen_fondo;
	}

	Music* getCancion() {
		return &song;
	}

	string getDirDificultad() {
		return dificultades[dificultad_seleccionada];
	}
};

#endif
