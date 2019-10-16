#ifndef AREAJUEGO_H
#define AREAJUEGO_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include "Pulsador.h"
#include "Animacion.h"

using namespace sf;
using namespace std;

class AreaJuego {

	private:

    class Nota {

    public:
			Texture textura_notaRect;
			Sprite sprite_notaRect;
			Vector2f pos;
			Vector2f tam;
			char tipo; //Tipo de nota (normal o mantenida) y cual de las 4 notas es. d f D F // J K j k
			int32_t posTiempo; //Posicion en el tiempo de la nota en la cancion

      Nota(int32_t &momento, const char &t) {
				if (!textura_notaRect.loadFromFile("Recursos/Area de Juego/nota.png")) {
					cout << "Error al cargar Recursos/Area de Juego/nota.png" << endl;
					exit(1);
				}
				sprite_notaRect.setTexture(textura_notaRect);

				posTiempo = momento;
				tipo = t;
    		tam.x = 60;
    		tam.y = 20;

				//Le asigno posicion en X en la pantalla dependiendo en cual pulsador tenga que ir la nota
				if (tipo == 'd' || tipo == 'D') {
					pos.x = 100;
				}
				else if (tipo == 'f' || tipo == 'F') {
					pos.x = 100 + tam.x;
				}
				else if (tipo == 'j' || tipo == 'J') {
					pos.x = 100 + tam.x*2;
				}
				else if (tipo == 'k' || tipo == 'K') {
					pos.x = 100 + tam.x*3;
				}
    	}
    	~Nota() {}

    	void mostrar(const int32_t &offsetTema, const float velocNotas, RenderWindow &ventana) {
				/*
				 * Nota: Al estar los fps con un cap en 60, el programa solo toma 6 momentos del offset por segundo, es decir
				 * que en la transicion del -200 al -100 solo se capturan 6 valores intermedios, los cuales incluso pueden estar repetidos.
				 * Al incrementar la cantidad de frames por segundo se incrementa la cantidad de muestras, pero en vez de tomar valores intermedios,
				 * las muestras solo repiten muchas veces el mismo valor. La forma de conseguir un movimiento mas fluido es predecir el movimiento de las notas, cambiando
				 * su posicion en pantalla por cada frame, ya que se mueven a velocidad constante (porque la cancion avanza de forma constante) aunque el tiempo de ejecucion
				 * entre cada frame varia (el programa no tarda lo mismo en ejecutarse cada frame), por lo que seguiria teniendo margen de error, pero mas diminuto.
				 */
        //Muestra y actualiza la posicion de la nota en la pantalla
    		pos.y = ( (offsetTema-posTiempo) / velocNotas ) + 621;
				/* El siguiente if, dibuja solamente las notas que se ven en pantalla.
				 * -540 es el espacio desde el area donde se tocan las notas hacia arriba
				 * y se multicplica por la velocidad de las notas, o sino al cambiar el valor, las notas
				 * dejan de dibujarse desde el inicio de la pantalla y se dibujan desde la mitad u otro lado.
				 */
				if (offsetTema-posTiempo >= -540*velocNotas && offsetTema-posTiempo <= 30*velocNotas) {
					sprite_notaRect.setPosition(pos);
	    	  ventana.draw(sprite_notaRect);
				}
    	}
    };

		//Tipos de dato de c++
		enum enum_pulsadores { D = 0, F, J, K };
		static const int NUM_COMBO_POS_X = 205; //Posicion en X del numero de combo
		static const int NUM_COMBO_POS_Y = 465; //Posicion en Y del numero de combo
		static const int ESPACIADO_NUM_COMBO = 26; //Ancho del numero de combo
		static const int POS_CONTADORES_NOTAS_X = 143;
		static const int POS_CONTADORES_NOTAS_Y = 510;
		static const int CANTIDAD_PULSADORES = 4;
		static const int CANTIDAD_CONTADORES_NOTAS = 4;
		int score;
		int perfect, great, good, miss; //Contadores de notas
		int combo = 0, comboMax = 0; //Contador de combo
		int selector = 1;
		float vida;
		float velocNotas;
		float multiplicador = 1;
		bool anim_Contador_Notas_En_Curso = false;
		bool anim_Cancion_En_Curso = false;
		bool enPausa = false;
		bool transicionPuntajeSeleccion = false;
		//Tipos de dato de clases
		Animacion* anim_Contador_Notas;
		Animacion* anim_1;
		Animacion* anim_perder;
		vector<Nota*> notas;
		Pulsador* pulsadores[CANTIDAD_PULSADORES];
		//Tipos de dato de librerias SFML
		Music* cancion;
		Texture textura_Numeros[10], textura_score[10];
		Texture textura_perfect, textura_great, textura_good, textura_miss;
		Texture textura_area_juego, textura_black, textura_menu_opciones, textura_selector_opciones, textura_barra_vida;
		Sprite sprite_numeros[10], sprite_score[10];
		Sprite sprite_perfect, sprite_great, sprite_good, sprite_miss;
		Sprite sprite_area_juego, sprite_black, sprite_menu_opciones, sprite_selector_opciones, sprite_background, sprite_barra_vida;
		Text texto_contadores_notas[CANTIDAD_CONTADORES_NOTAS];
		Text texto_score, texto_combo, texto_ayuda;
		Time momentoPausa;

	public:

		AreaJuego (float _velocNotas,  Music* _cancion, string dificultad, Sprite _background) {

		 extern Font fuente;
		 extern int volumen_musica;
		 ifstream archivoNotas;
		 archivoNotas.open(dificultad);
		 for (int i = 0; i < CANTIDAD_CONTADORES_NOTAS; i++) {
 			texto_contadores_notas[i].setFont(fuente);
 		}
 		texto_score.setFont(fuente);
		texto_combo.setFont(fuente);
		texto_ayuda.setFont(fuente);
		anim_perder = NULL;
	 	pulsadores[D] = new Pulsador('D');
	 	pulsadores[F] = new Pulsador('F');
	 	pulsadores[J] = new Pulsador('J');
	 	pulsadores[K] = new Pulsador('K');
	 	cancion = _cancion;
	 	sprite_background = _background;
	 	vida = 100;
	 	score = 0;
	 	velocNotas = _velocNotas;
	 	perfect = 0;
	 	great = 0;
	 	good = 0;
	 	miss = 0;
	 	combo = 0;
	 	cancion->setVolume(volumen_musica);
	 	crearSprites();
	 	crearNotas(archivoNotas);
	}
		~AreaJuego(){} //Destructor

		void crearSprites() {
		for (int i = 0; i < 10; i++) {
			string nroTextura;
			nroTextura = to_string(i);
			if (!textura_Numeros[i].loadFromFile("Recursos/Area de Juego/nro" + nroTextura + ".png")) {
				cout << "Error al cargar Recursos/Area de Juego/nro" + nroTextura + ".png" << endl;
			}//;
			else {
				sprite_numeros[i].setTexture(textura_Numeros[i]);
			}
		}
		for (int i = 0; i < 10; i++) {
			string nroTextura;
			nroTextura = to_string(i);
			if (!textura_score[i].loadFromFile("Recursos/Area de Juego/score_" + nroTextura + ".png")) {
				cout << "Error al cargar Recursos/Area de Juego/score_" + nroTextura + ".png" << endl;
			}
			else {
				sprite_score[i].setTexture(textura_score[i]);
			}
		}

		if (!textura_selector_opciones.loadFromFile("Recursos/Area de Juego/selector.png")) {
			cout << "Error al cargar Recursos/Area de Juego/selector.png" << endl;
		}
		sprite_selector_opciones.setTexture(textura_selector_opciones);

		if (!textura_area_juego.loadFromFile("Recursos/Area de Juego/AreaJuego.png")) {
			cout<<"Error al cargar Recursos/Area de Juego/AreaJuego.png" << endl;
		}
		textura_area_juego.setSmooth(true);
		sprite_area_juego.setTexture(textura_area_juego);
		sprite_area_juego.setPosition(0, 5);

		if (!textura_menu_opciones.loadFromFile("Recursos/Area de Juego/menu.png")) {
			cout << "Error al cargar Recursos/Area de Juego/menu.png" << endl;
		}
		sprite_menu_opciones.setTexture(textura_menu_opciones);
		sprite_menu_opciones.setPosition(440, 185);

		if (!textura_perfect.loadFromFile("Recursos/Area de Juego/contador_perfect.png")) {
			cout << "Error al cargar Recursos/Area de Juego/contador_perfect.png" << endl;
		}
		sprite_perfect.setTexture(textura_perfect);

		if (!textura_great.loadFromFile("Recursos/Area de Juego/contador_great.png")) {
			cout << "Error al cargar Recursos/Area de Juego/contador_great.png" << endl;
		}
		sprite_great.setTexture(textura_great);

		if (!textura_good.loadFromFile("Recursos/Area de Juego/contador_good.png")) {
			cout << "Error al cargar contador_good.png" << endl;
		}
		sprite_good.setTexture(textura_good);

		if (!textura_miss.loadFromFile("Recursos/Area de Juego/contador_miss.png")) {
			cout << "Error al cargar Recursos/Area de Juego/contador_miss.png" << endl;
		}
		sprite_miss.setTexture(textura_miss);

		if (!textura_black.loadFromFile("Recursos/black.png")) {
			cout << "Error al cargar Recursos/black.png";
		}
		sprite_black.setTexture(textura_black);

		if (!textura_barra_vida.loadFromFile("Recursos/Area de Juego/vida.png")) {
			cout << "Error al cargar Recursos/Area de Juego/vida.png" << endl;
		}
		else {
			sprite_barra_vida.setTexture(textura_barra_vida);
			sprite_barra_vida.rotate(180);
			sprite_barra_vida.setPosition(390, 635);
		}
		sprite_background.setColor(Color(255, 255, 255, 90));
	}

		void crearNotas(ifstream &archivoDeOffsets) {
		/*
		 * Mientras que no sea el fin del archivo de notas leo una
		 * linea e instancio una nueva nota.
		 */
		while (!archivoDeOffsets.eof()) {
			//Defino los datos que quiero guardar del archivo (offsetNota y tipo)
			int offsetNota;
			/*
			 * Creo una variable string y le asigno la primera linea de mi archivo.
			 * Despues creo una variable stringstream y le asigno mi string "a".
			 * De esta forma se puede usar getline() y stringstream.good()
			 */
			string a;
			archivoDeOffsets >> a;
			stringstream ss(a);
			/*
			 * El contador me ayuda a dividir la linea en 2 y guardar cada parte
			 * en la variable correspondiente.
			 */
			int cont = 0;
			//Mientras no haya terminado de leer de mi stringstream repetir.
			while (ss.good() ) {
				string linea; // Variable auxiliar donde se guarda la linea del archivo dividida hasta la coma
				getline (ss, linea, ',');
				//La primera division se guarda en "offsetNota"  y la segunda en "tipo"
				if (cont == 0) {
					offsetNota = stoi(linea)+55;
					cont++;
				}
				else {
					const char* tipo = linea.c_str();
					Nota* n = new Nota(offsetNota, *tipo);
					notas.push_back(n);
				}
			}
		}
		archivoDeOffsets.close();
	}

		void comenzar() {
		if (!anim_Cancion_En_Curso) {
			anim_1 = new Animacion(sprite_black, 3);
			anim_Cancion_En_Curso = true;
		}
		else {
			cancion->play();
		}
	}

		void mostrarAreaJuego(RenderWindow &ventana) {
		ventana.draw(sprite_background);
		ventana.draw(sprite_barra_vida);
		ventana.draw(sprite_area_juego);
		this->mostrarCombo(ventana);
		this->mostrarScore(ventana);
	}

		const void mostrarCombo(RenderWindow &ventana) {
		if (combo > comboMax) {
			comboMax = combo;
		}
		if (combo >= 1000) {
			//Posicionar los numeros que componen el combo.
			int num1 = combo/1000;
			sprite_numeros[num1].setPosition(NUM_COMBO_POS_X - 1.5*ESPACIADO_NUM_COMBO, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num1]);

			int num2 = combo/100 - (num1*10); //Centena
			sprite_numeros[num2].setPosition(NUM_COMBO_POS_X - ESPACIADO_NUM_COMBO/2, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num2]);

			int num3 = combo/10 - (num1*100 + num2*10); //Decena
			sprite_numeros[num3].setPosition(NUM_COMBO_POS_X + ESPACIADO_NUM_COMBO/2, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num3]);

			int num4 = combo - (num1*1000 + num2*100 + num3*10);
			sprite_numeros[num4].setPosition(NUM_COMBO_POS_X + 1.5*ESPACIADO_NUM_COMBO, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num4]);
		}
		else if (combo >= 100) {
			int num1 = combo/100;
			sprite_numeros[num1].setPosition(NUM_COMBO_POS_X - ESPACIADO_NUM_COMBO, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num1]);

			int num2 = combo/10 - (num1*10);
			sprite_numeros[num2].setPosition(NUM_COMBO_POS_X, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num2]);

			int num3 = combo - (num1*100+num2*10);
			sprite_numeros[num3].setPosition(NUM_COMBO_POS_X + ESPACIADO_NUM_COMBO, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num3]);
		}
		else if (combo >= 10) {
			int num1 = combo/10;
			sprite_numeros[num1].setPosition(NUM_COMBO_POS_X - ESPACIADO_NUM_COMBO/2, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num1]);

			int num2 = combo - (num1*10);
			sprite_numeros[num2].setPosition(NUM_COMBO_POS_X + ESPACIADO_NUM_COMBO/2, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[num2]);
		}
		else if (combo >= 3) {
			sprite_numeros[combo].setPosition(NUM_COMBO_POS_X, NUM_COMBO_POS_Y);
			ventana.draw(sprite_numeros[combo]);
		}
	}

		void mostrarScore(RenderWindow &ventana) {
		//Se convierte el score a un string
		string t_puntaje = to_string(score);
		int largo = t_puntaje.length();
		//Para que los 0 delante del score se muestren constantemente, se agregan
		while (largo < 7) {
			t_puntaje = "0" + t_puntaje;
			largo = t_puntaje.length();
		}
		//Se recorre el string del score y se dibujan los numeros
		for (int i = largo-1; i > -1; i--) {
      int numero;
      numero = t_puntaje.at(i) - '0';
    	sprite_score[numero].setPosition(30 + 55 * i, 20);
			ventana.draw(sprite_score[numero]);
		}
	}

		void controlarNotas(Event &evento, RenderWindow &ventana) {
		extern int estado;
		extern Sound hit;
		int32_t offsetTema = this->getMusicaOffset(); //Momento en el que esta la cancion

		if (notas.size() != 0) {
			checkPulsadores(evento, ventana);
			//Si el pulsador D, F, J o K se presiona, entonces controlar timing si la nota correspende al pulsador.
			if (pulsadores[D]->estaPulsado() && !pulsadores[D]->checked()) {
				if (notas[0]->tipo == 'd' || notas[0]->tipo == 'D') {
					pulsadores[D]->cambiarEstado(true);
					controlarTiming(offsetTema, false, ventana);
				}
			}
			if (pulsadores[F]->estaPulsado() && !pulsadores[F]->checked()) {
				if (notas[0]->tipo == 'f' || notas[0]->tipo == 'F') {
					pulsadores[F]->cambiarEstado(true);
					controlarTiming(offsetTema, false, ventana);
				}
			}
			if (pulsadores[J]->estaPulsado() && !pulsadores[J]->checked()) {
				if (notas[0]->tipo == 'j' || notas[0]->tipo == 'J') {
					pulsadores[J]->cambiarEstado(true);
					controlarTiming(offsetTema, false, ventana);
				}
			}
			if (pulsadores[K]->estaPulsado() && !pulsadores[K]->checked()) {
				if (notas[0]->tipo == 'k' || notas[0]->tipo == 'K') {
					pulsadores[K]->cambiarEstado(true);
					controlarTiming(offsetTema, false, ventana);
				}
			}
		/*
		 * Recorre el vector de notas y las dibuja en pantalla.
		 * Si una nota pasa el area donde deben tocarse, la elimina y la cuenta como "Miss".
		 */
    	for (unsigned int i = 0; i < notas.size(); i++) {
    		notas[i]->mostrar(offsetTema, velocNotas, ventana);
				if ((offsetTema - notas[i]->posTiempo) > 52) {
					delete notas[0];
					notas.erase(notas.begin());
					controlarTiming(offsetTema, true, ventana);
				}
			}
		}
		else {
			/*
			 * Si ya no tengo mas notas, espero a terminar la cancion
			 * despues preparo la pantalla de puntaje e inicio una animacion
			 * de transicion, luego cambio de estado a PUNTAJE.
			 */
			Time duracion = cancion->getDuration();
			Time offset = cancion->getPlayingOffset();
			int32_t d = duracion.asMilliseconds();
			int32_t o = offset.asMilliseconds();
			extern bool btnPressed;
			if ( d - o <= 100 || (Keyboard::isKeyPressed(Keyboard::Enter) && !btnPressed) ) {
				btnPressed = true;
				estado = 6; //PUNTAJE
				cancion->stop();
				setTextosPuntaje();
				}
		}
	}

		int32_t getMusicaOffset() {
	    Time off = cancion->getPlayingOffset();
	    int32_t offset = off.asMilliseconds();
	    return offset;
		}

		void checkPulsadores(Event &evento, RenderWindow &ventana) {
		for (int i = 0; i < CANTIDAD_PULSADORES; i++) {
			pulsadores[i]->checkPulsador(evento);
			pulsadores[i]->animarPulsador(ventana);
		}
	}

		void controlarTiming(const int32_t offsetTema, bool miss, RenderWindow &ventana) {
	  string estadoNota = " ";
		int timing = offsetTema-notas[0]->posTiempo;
		//Suponiendo que 18 es mi 0
		if (!miss) {
			if (timing <= 44 && timing >=-88) {
				if (timing >= -36 && timing <= 24) {//12
					estadoNota = "perfect";
					//cout << "Perfect: " << timing << endl;
				}
				else if (timing >=-68 && timing <= 40) {//28
					estadoNota = "great";
					//cout << "great: " << timing << endl;
				}
				else if (timing >= -88 && timing <= 52) {//44
					estadoNota = "good";
					//cout << "good: " << timing << endl;
				}
				delete notas[0];
				notas.erase(notas.begin());
			}
		}
		else {
			estadoNota = "miss";
		}
		this->contarNotas(estadoNota, ventana);
	}

		void contarNotas(const string _estadoNotas, RenderWindow &ventana) {
		/*
		 * Recibe un string con el estado de la ultima nota y suma su contador correspondiente.
		 */
		if (_estadoNotas == "perfect") {
			perfect++;
		}
		else if (_estadoNotas == "great") {
			great++;
		}
		else if (_estadoNotas == "good") {
			good++;
		}
		else if (_estadoNotas == "miss") {
			miss++;
		}
		this->mostrarContadoresNotas(_estadoNotas, ventana);
		this->incrementarScore(_estadoNotas);
		this->modificarVida(_estadoNotas);
	}

		void setTextosPuntaje () {
		sprite_perfect.setPosition(170, 200);
		sprite_great.setPosition(170, 280);
		sprite_good.setPosition(170, 360);
		sprite_miss.setPosition(170, 440);
		for (int i = 0; i < 4; i++) {
			int posicionY;
			if (i == 0) {
				posicionY = 200;
			}
			else if (i == 1) {
				posicionY = 280;
			}
			else if (i == 2) {
				posicionY = 360;
			}
			else {
				posicionY = 440;
			}
				texto_contadores_notas[i].setPosition(420, posicionY);
		}
			texto_score.setPosition(100, 75);
			texto_score.setCharacterSize(28);
			texto_score.setString("SCORE   " + to_string(score));
			texto_combo.setPosition(100, 550);
			texto_combo.setCharacterSize(28);
			texto_combo.setString("COMBO MAX   " + to_string(comboMax));
			texto_ayuda.setPosition(600, 600);
			texto_ayuda.setCharacterSize(36);
			texto_ayuda.setFillColor(Color::White);
			texto_ayuda.setString("Presione Esc o Enter para volver.");
	}

		const void mostrarContadoresNotas(const string _estadoNota, RenderWindow &ventana) {
		if (_estadoNota == "perfect") {
			anim_Contador_Notas = new Animacion(sprite_perfect, POS_CONTADORES_NOTAS_X, POS_CONTADORES_NOTAS_Y, 1);
			anim_Contador_Notas_En_Curso = true;
		}
		else if (_estadoNota == "great") {
			anim_Contador_Notas = new Animacion(sprite_great, POS_CONTADORES_NOTAS_X, POS_CONTADORES_NOTAS_Y, 1);
			anim_Contador_Notas_En_Curso = true;
		}
		else if (_estadoNota == "good") {
			anim_Contador_Notas = new Animacion(sprite_good, POS_CONTADORES_NOTAS_X, POS_CONTADORES_NOTAS_Y, 1);
			anim_Contador_Notas_En_Curso = true;
		}
		else if (_estadoNota == "miss") {
			anim_Contador_Notas = new Animacion(sprite_miss, POS_CONTADORES_NOTAS_X + 33, POS_CONTADORES_NOTAS_Y, 1);
			anim_Contador_Notas_En_Curso = true;
		}
	}

		void incrementarScore(const string _estadoNota) {
		/* * * * * * * * * * * * * * * * * * * * * *
		 * Tabla score                             *
		 * ----------------------------------------*
		 * Perfect 200                             *
		 * Great 100                               *
		 * Good 50                                 *
		 * Miss -                                  *
		 * Cuando el combo llega a 10, se agrega   *
		 * multiplicador de 0.2, y cada nota des-  *
		 * pues de los 10 de combo, le agrega un   *
		 * 0.04 al multiplicador.                  *
		 * * * * * * * * * * * * * * * * * * * * * *
		 */
		//Controlo si el combo continua.
		if (_estadoNota != "miss") {
			if (_estadoNota == " ") {
				// lo ignoro
			}
			else {
				combo++;
			}
		}
		else {
			combo = 0;
		}

		if (_estadoNota == "perfect") {
			this->calcularMultiplicador();
			score += 200*multiplicador;
		}
		else if (_estadoNota == "great") {
			this->calcularMultiplicador();
			score += 100*multiplicador;
		}
		else if (_estadoNota == "good") {
			this->calcularMultiplicador();
			score += 50*multiplicador;
		}
		cout << multiplicador << endl;
	}

	void calcularMultiplicador() {
		//Calculo si hay multiplicador, luego incremento el score.
		if (combo == 10) {
		  multiplicador += 1;
		}
		else if (combo > 10) {
			multiplicador += 0.02;
		}
		else if (combo <= 10) {
			multiplicador = 1;
		}
	}

		void modificarVida(const string _estadoNota) {
		/* * * * * * * * * * * * * *
		 * Tabla de vida           *
		 * ------------------------*
		 * Perfect +2              *
		 * Great +1                *
		 * Miss -8                 *
		 * * * * * * * * * * * * * *
		 */
		 extern int estado;
		if ( (vida != 100 || _estadoNota == "miss") && vida > 1) {
			if (_estadoNota == "miss") {
				vida -= 8;
			}
			else if (_estadoNota == "great") {
				vida += 1;
			}
			else if (_estadoNota == "perfect") {
				vida += 2;
			}
			if (vida > 100) {
				vida = 100;
			}
			// Escalar al 100% y despues bajar a la escala de la vida.
			Vector2f escala = sprite_barra_vida.getScale();
			sprite_barra_vida.scale(1, 1 + (1 - escala.y) );
			sprite_barra_vida.scale(1, ((vida*100)/500)/20);
			sprite_barra_vida.setPosition(390, 635 - (1 * (vida/100)) );
		}
		else if (vida < 1) {
			vida = 0;
			estado = 7;
			cancion->stop();
		}
	}

		void controlarAnimaciones(RenderWindow &ventana) {
		if (anim_Contador_Notas_En_Curso) {
		if (anim_Contador_Notas->finalizado()) {
			delete anim_Contador_Notas;
			anim_Contador_Notas_En_Curso = false;
		}
		else {
			anim_Contador_Notas->animarContadorNotas(ventana);
		}
		}
		else if (anim_Cancion_En_Curso) {
			if (anim_1->finalizado()) {
				delete anim_1;
				this->comenzar();
				anim_Cancion_En_Curso = false;
			}
			else {
				anim_1->desvanecer(ventana);
			}
		}
		else if (transicionPuntajeSeleccion) {
			if (anim_1->finalizado()) {
				extern int estado;
				delete anim_1;
				transicionPuntajeSeleccion = false;
				estado = 1;
			}
			else {
				anim_1->oscurecer(ventana);
			}
		}
	}

		void pausa(RenderWindow &ventana, Event &evento) {
		extern int estado;
		extern bool btnPressed;
		extern Sound sfx_menu;
		extern Sound sfx_menu_volver;

		if (!enPausa) {
			momentoPausa = cancion->getPlayingOffset();
			cancion->pause();
			enPausa = true;
			selector = 1;
		}
		else {
			if (evento.type == Event::KeyPressed) {
				if ( (evento.key.code == Keyboard::Up && !btnPressed) || (evento.key.code == Keyboard::Down && !btnPressed) ) {
					sfx_menu.play();
					btnPressed = true;
					if (selector == 1) {
						selector = 2;
					}
					else {
						selector = 1;
					}
				}
			}
			else if (Keyboard::isKeyPressed(Keyboard::Enter) && !btnPressed) {
				sfx_menu.play();
				btnPressed = true;
				if (selector == 1) {
					cancion->setPlayingOffset(momentoPausa);
					cancion->play();
				}
				else {
					sfx_menu_volver.play();
					estado = 5; //VOLVER_MENU
				}
				enPausa = false;
			}
			else if ( (evento.type == Event::KeyReleased) && btnPressed) {
				btnPressed = false;
			}

			if (selector == 1) {
				sprite_selector_opciones.setPosition(490, 332);
			}
			else {
				sprite_selector_opciones.setPosition(515, 424);
			}
			ventana.draw(sprite_black);
			ventana.draw(sprite_menu_opciones);
			ventana.draw(sprite_selector_opciones);
		}
	}

		bool estadoEnPausa() {
		if (enPausa) {
			return true;
		}
		else {
			return false;
		}
	}

		void pantallaPuntaje(RenderWindow &ventana) {
		extern bool btnPressed;
		extern Sound sfx_menu_volver;

		ventana.draw(sprite_background);
		ventana.draw(sprite_perfect);
		ventana.draw(sprite_great);
		ventana.draw(sprite_good);
		ventana.draw(sprite_miss);
		ventana.draw(texto_score);
		ventana.draw(texto_combo);
		ventana.draw(texto_ayuda);
		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				texto_contadores_notas[i].setString(to_string(perfect));
			}
			else if (i == 1) {
				texto_contadores_notas[i].setString(to_string(great));
			}
			else if (i == 2) {
				texto_contadores_notas[i].setString(to_string(good));
			}
			else {
				texto_contadores_notas[i].setString(to_string(miss));
			}
			ventana.draw(texto_contadores_notas[i]);
		}
		if ( (Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Enter) ) && !transicionPuntajeSeleccion) {
			anim_1 = new Animacion(sprite_black, 1);
			sfx_menu_volver.play();
			btnPressed = true;
			transicionPuntajeSeleccion = true;
		}
	}

		void perder(RenderWindow &ventana) {
		extern int estado;
		if (anim_perder == NULL) {
			anim_perder = new Animacion(sprite_black, 1.5);
		}
		else if (anim_perder->finalizado()){
			estado = 5;
		}
		else {
			anim_perder->oscurecer(ventana);
		}
	}
};

#endif
