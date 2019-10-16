#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream> //Manejo de archivos de texto
#include <vector>
#include <iostream>
#include <string>
#include <sstream> //Manejo de streams
#include <windows.h> //Busqueda de archivos
#include "Pulsador.h"
#include "Beatmap.h"
#include "AreaJuego.h"

using namespace std;
using namespace sf; //namespace del sfml

//Enumerador de los estados del juego
enum estadosMenu { MENU = 0, SELECCION_CANCION, OPCIONES, SALIR, JUGANDO, VOLVER_MENU, PUNTAJE, PERDER };
//Estado inicia en el menu por defecto (menu principal) y el selector (de la opcion del menu) en Comenzar
unsigned int estado = 0;
int selector = 1;
int volumen_musica = 70/2;
int volumen_sfx = 50/2;
int escala_velocidad_notas = 4;
float velocidadNotas = 1.6f;
//Variable que controla si se esta presionando una tecla, utilizado para navegacion en menus
bool btnPressed = false;
Font fuente;
Sound sfx_menu;
Sound sfx_menu_volver;
Sound sfx_hit;

//Carga las texturas y crea los sprites del menu
void cargarSprites (Texture &, Texture &, Texture &, Sprite &, Sprite &, Sprite &);
//Configura los textos usados en el menu de opciones.
void setTextos (Text &, Text &, Text &);
//Guarda en un vector la ubicacion de cada cancion
void listarCanciones(ifstream &, vector<string> &);
//Lista los archivos de dificultades y los almacena en un vector
vector<string> getDificultades(string);
//Lee las ubicaciones de las dificultades y los datos de las canciones y crea los beatmaps
void crearBeatmaps(Beatmap* [], const unsigned int, vector<string> &);
//Menu principal del juego, desde donde se puede salir, ir a opciones o ir a seleccion de canciones
void menu(Event &, Sprite &, Sprite &, Beatmap* [], RenderWindow &);
//Modifica el volumen de los efectos de sonido y la musica.
void modificarVolumenSonidos(Beatmap* [], const unsigned int);
//Muestra datos de la cancion, permite seleccionar entre las distintas canciones, y crea el area de juego correspondiente
void seleccionCancion(Event &, Beatmap* [], unsigned int const &, RenderWindow &, AreaJuego*&, Sprite &);
//Controla el menu de opciones, permite cambiar la velocidad de las notas y volumen de los sonidos y musica.
void opciones(Event &, Sprite &, Sprite &, RenderWindow &, Text &, Text &, Text &, Beatmap*[], const unsigned int);
//Ejecutar acciones del area de juego.
void jugar(Event &, RenderWindow &, AreaJuego* &);
//Elimina el area de juego y vuelve al menu
void volverMenu(Beatmap* [], AreaJuego*);
//Muestra el puntaje, al terminar ejecuta la animacion y elimina el area de juego, luego vulve a SELECCION_CANCION
void mostrarPuntaje(AreaJuego*, RenderWindow&, Beatmap*);
//Secuencia de acciones al perder, luego cambia de estado a VOLVER_MENU, eliminando el area de juego
void perder(RenderWindow &, AreaJuego* );

int main() {
  RenderWindow window(VideoMode(1280, 720), "4K Beats");
	window.setFramerateLimit(144);
	window.setMouseCursorVisible(false);

	//Declaro un archivo de donde leer mi lista de canciones.
	ifstream listaCanciones;
	listaCanciones.open("Song_db.dat");

  //Leo de la lista de canciones sus respectivas ubicaciones y las guardo en "ubicaciones".
  vector<string> ubicaciones;
  listarCanciones(listaCanciones, ubicaciones);

  //Declaro las texturas y spirtes para poder crear el menu y los cargo.
  Texture texturaMenu, texturaSelectorMenu, texturaOpciones;
  Sprite spriteMenu, spriteSelectorMenu, spriteOpciones;
	cargarSprites(texturaMenu, texturaSelectorMenu, texturaOpciones, spriteMenu, spriteSelectorMenu, spriteOpciones);

  //Cargo la fuente
  if (!fuente.loadFromFile("Recursos/Fuentes/Fuente.ttc")) {
    cout << "Error al cargar la Recursos/Fuentes/Fuente.ttc" << endl;
  }

  //Creo los textos del menu de opciones.
  Text texto_velocidad_notas, texto_volumen_musica, texto_volumen_sfx;
  setTextos(texto_velocidad_notas, texto_volumen_musica, texto_volumen_sfx);

  //Declaro el area de juego.
  AreaJuego *areaJuego;

	//Creo los beatmaps.
	const unsigned int cantidadBeatmaps = ubicaciones.size(); //Cantidad de beatmaps que tengo.
	Beatmap* beatmaps[cantidadBeatmaps];
	crearBeatmaps(beatmaps, cantidadBeatmaps, ubicaciones);

  //Creo los sonidos del menu y de las notas
  SoundBuffer b_sfx_menu, b_sfx_menu_volver, b_hit;
  if (!b_sfx_menu.loadFromFile("Recursos/SFX/menu.wav")) {
    cout << "Error al cargar Recursos/SFX/menu.wav" << endl;
  }
  else {
    sfx_menu.setBuffer(b_sfx_menu);
    sfx_menu.setVolume(volumen_sfx);
  }
  if (!b_sfx_menu_volver.loadFromFile("Recursos/SFX/menu_volver.wav")) {
    cout << "Error al cargar Recursos/SFX/menu_volver.wav" << endl;
  }
  else {
    sfx_menu_volver.setBuffer(b_sfx_menu_volver);
    sfx_menu_volver.setVolume(volumen_sfx);
  }
  if (!b_hit.loadFromFile("Recursos/SFX/hit_nota.wav")) {
    cout << "Error al cargar Recursos/SFX/hit_nota.wav" << endl;
  }
  else {
    sfx_hit.setBuffer(b_hit);
    sfx_hit.setVolume(volumen_sfx);
  }

  //Loop principal
  while (window.isOpen()) {
    Event evento;
    while (window.pollEvent(evento)) {
      if (evento.type == sf::Event::Closed)
        window.close();
    	}
			//Switch que maneja los estados del juego.
			switch(estado) {
		    case MENU:
					menu(evento, spriteMenu, spriteSelectorMenu, beatmaps, window);
		    break;

		    case SELECCION_CANCION:
				 seleccionCancion(evento, beatmaps, cantidadBeatmaps, window, areaJuego, spriteSelectorMenu);
		    break;

		    case OPCIONES:
					opciones(evento, spriteOpciones, spriteSelectorMenu, window, texto_velocidad_notas, texto_volumen_musica, texto_volumen_sfx, beatmaps, cantidadBeatmaps);
		    break;

		    case SALIR:
		      exit(0);
		    break;

				case JUGANDO:
					jugar(evento, window, areaJuego);
				break;

				case VOLVER_MENU:
          volverMenu(beatmaps, areaJuego);
				break;

				case PUNTAJE:
          mostrarPuntaje(areaJuego, window, beatmaps[selector]);
				break;

        case PERDER:
          perder(window, areaJuego);
        break;

		    default:
          estado = MENU;
		    break;
		  }
		  window.display();
			//Se limpia la pantalla si no esta en pausa.
			if (estado == JUGANDO) {
				if (!areaJuego->estadoEnPausa()) {
					window.clear();
				}
			}
			else {
				window.clear();
			}
  }
  return 0;
}

void cargarSprites (Texture &texturaMenu, Texture &texturaSelectorMenu, Texture &texturaOpciones, Sprite &spriteMenu, Sprite &spriteSelectorMenu, Sprite &spriteOpciones) {
	//Cargo las texturas Y creo los sprites
	if (!texturaMenu.loadFromFile("Recursos/Menu/menu.png")) {
		cout<<"Error al cargar textura Recursos/Menu/menu.png" << endl;
	}
  else {
    spriteMenu.setTexture(texturaMenu);
  }
	if (!texturaSelectorMenu.loadFromFile("Recursos/Menu/selector.png")) {
		cout<<"Error al cargar textura Recursos/Menu/selector.png" << endl;
	}
  else {
    spriteSelectorMenu.setTexture(texturaSelectorMenu);
  	spriteSelectorMenu.setPosition(435, 290);
  }
	if (!texturaOpciones.loadFromFile("Recursos/Menu/opciones.png")) {
		cout<<"Error al cargar la textura Recursos/Menu/opciones.png." << endl;
	}
  else {
    spriteOpciones.setTexture(texturaOpciones);
  }
}
void setTextos (Text &texto_velocidad_notas, Text &texto_volumen_musica, Text &texto_volumen_sfx) {
  //Se configuran los textos usados en el menu de opciones.
  texto_velocidad_notas.setFont(fuente);
  texto_volumen_musica.setFont(fuente);
  texto_volumen_sfx.setFont(fuente);
  texto_velocidad_notas.setPosition(1100, 195);
  texto_volumen_musica.setPosition(1100, 325);
  texto_volumen_sfx.setPosition(1100, 455);
  texto_velocidad_notas.setCharacterSize(32);
  texto_volumen_musica.setCharacterSize(32);
  texto_volumen_sfx.setCharacterSize(32);
  texto_velocidad_notas.setString( to_string(escala_velocidad_notas) );
  texto_volumen_musica.setString( to_string(volumen_musica*2) );
  texto_volumen_sfx.setString( to_string(volumen_sfx*2) );
}
void listarCanciones(ifstream &listaCanciones, vector<string> &ubicaciones) {
  //Guarda en un vector la ubicacion de cada cancion

  //Mientras que no sea el fin del archivo listaCanciones
	while (!listaCanciones.eof()) {
		string linea;
		listaCanciones >> linea;
    //Guardo mi linea en un stringstream para poder leerla despues.
		stringstream ss(linea);
		string ubicacion;

		//Mientras no haya terminado de leer de mi stringstream repetir.
		while (ss.good() ) {
			getline (ss, ubicacion);
			ubicacion = "Canciones/" + ubicacion;
			ubicaciones.push_back(ubicacion);
		}
	}
  //Cerrar el archivo al finalizar
	listaCanciones.close();
}
vector<string> getDificultades(string carpeta) {
  //Lista los archivos de texto encontrados en "canciones/Dificultades" y los almacena en un vector "listaDificultades"
  vector<string> listaDificultades;
  string rutaBusqueda = carpeta + "/*.txt";
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(rutaBusqueda.c_str(), &fd);
  if(hFind != INVALID_HANDLE_VALUE) {
		do {
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
			   listaDificultades.push_back(carpeta + "/" + fd.cFileName);
			}
		} while(::FindNextFile(hFind, &fd));
	::FindClose(hFind);
}
return listaDificultades;
}
void crearBeatmaps(Beatmap* beatmaps[], const unsigned int cantidadBeatmaps, vector<string> &ubicaciones) {
  //Lee las ubicaciones de las dificultades y los datos de las canciones y crea los beatmaps
  for (unsigned int i = 0; i < cantidadBeatmaps; i++) {
		//Listar dificultades
		string carpeta = ubicaciones[i] + "/Dificultades";
		vector <string> dificultades = getDificultades(carpeta); //Guarda el nombre de los archivos de dificultades
    vector <string> dificultadesNum; //Guarda el numero de dificultad de las dificultades, que esta almacenado en data.dat

    //Se guardan los datos para luego crear el beatmap
		string artista;
		string nombre;
		string duracion;

    //Se abre el archivo que contiene los datos de la cancion
		ifstream metadata;
		metadata.open(ubicaciones[i] + "/data.dat");
    unsigned int dif = 0;

    //Se lee la metadata de las canciones y la guardo
		while (!metadata.eof()) {

			string linea;
			metadata >> linea;

      if (linea == "Artista:") {
        while (linea != "Nombre:") {
          artista = linea;
          metadata >> linea;
        }
      }
      if (linea == "Nombre:") {
        metadata >> linea;
        while (linea != "Duracion:") {
          nombre += linea + " ";
          metadata >> linea;
        }
      }
      if (linea == "Duracion:") {
        metadata >> linea;
        duracion = linea;
        dif = 1;
      }
      /*
       * Se leen tantas lineas como dificultades tenga el mapa segun el vector "Dificultades".
       * Estas lineas son las consiguientes a "Duracion: "
       */
      if (dif == 1) {
        dif = 0;
        while(dif < dificultades.size()) {
          metadata >> linea;
          dificultadesNum.push_back(linea);
          dif++;
        }
      }
		}
    //Se crea el beatmap
		beatmaps[i] = new Beatmap(nombre, artista, duracion, ubicaciones[i]+"/", dificultades, dificultadesNum);
    //Se cierra el archivo
    metadata.close();
	}
}
void menu(Event &evento, Sprite &spriteMenu, Sprite &spriteSelectorMenu, Beatmap* beatmaps[], RenderWindow &ventana) {
//Menu principal del juego, desde donde se puede salir, ir a opciones o ir a seleccion de canciones
extern Sound sfx_menu;
	if (evento.type == Event::KeyPressed) {
		if (evento.key.code == Keyboard::Down && !btnPressed) {
			if (selector == SELECCION_CANCION) {
				selector = OPCIONES;
				spriteSelectorMenu.setPosition(430, 450);
			}
			else if (selector == OPCIONES) {
				selector = SALIR;
				spriteSelectorMenu.setPosition(430, 630);
			}
			else if (selector == SALIR) {
				selector = SELECCION_CANCION;
				spriteSelectorMenu.setPosition(430, 290);
			}
			btnPressed = true;
      sfx_menu.play();
		}
		else if (evento.key.code == Keyboard::Up && !btnPressed) {
			if (selector == SELECCION_CANCION) {
				selector = SALIR;
				spriteSelectorMenu.setPosition(430, 630);
			}
			else if (selector == SALIR) {
				selector = OPCIONES;
				spriteSelectorMenu.setPosition(430, 450);
			}
			else if (selector == OPCIONES) {
				selector = SELECCION_CANCION;
				spriteSelectorMenu.setPosition(430, 290);
			}
			btnPressed = true;
      sfx_menu.play();
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::Enter) && !btnPressed) {

		if (selector == SELECCION_CANCION) {
			estado = SELECCION_CANCION;
			selector = 0;
      sfx_menu.play();
			beatmaps[selector]->tocarPreview();
		}
		else if (selector == OPCIONES) {
			estado = OPCIONES;
      selector = 1;
			spriteSelectorMenu.setPosition(550, 240);
      sfx_menu.play();
		}
	else if (selector == SALIR) {
		estado = SALIR;
	}
		btnPressed = true;
	}
	else if (evento.type == Event::KeyReleased) {
		btnPressed = false;
	}
	//Dibujo el menu
	ventana.draw(spriteMenu);
	ventana.draw(spriteSelectorMenu);
}
void modificarVolumenSonidos(Beatmap* beatmaps[], const unsigned int cantidadBeatmaps) {
  //Modifica el volumen de los efectos de sonido y la musica.
  for (unsigned int i = 0; i < cantidadBeatmaps; i++) {
    beatmaps[i]->setVolumen(volumen_musica);
  }
  sfx_menu.setVolume(volumen_sfx);
  sfx_menu_volver.setVolume(volumen_sfx);
  sfx_hit.setVolume(volumen_sfx);
}
void seleccionCancion(Event &evento, Beatmap* beatmaps[], unsigned int const &cantidadBeatmaps, RenderWindow &ventana, AreaJuego* &area, Sprite &spriteSelectorMenu) {
  //Muestra datos de la cancion, permite seleccionar entre las distintas canciones, y crea el area de juego correspondiente
  beatmaps[selector]->mostrarSeleccionCanciones(ventana);
  beatmaps[selector]->animarPreview();
  extern Sound sfx_menu;
  extern Sound sfx_menu_volver;
	if (evento.type == Event::KeyPressed) {
		if (evento.key.code == Keyboard::Right && !btnPressed) {
      sfx_menu.play();
			btnPressed = true;
			if (selector+1 < cantidadBeatmaps) {
				beatmaps[selector]->detenerPreview();
				selector++;
				beatmaps[selector]->tocarPreview();
			}
			else {
				beatmaps[selector]->detenerPreview();
				selector = 0;
				beatmaps[selector]->tocarPreview();
			}
		}
		else if (evento.key.code == Keyboard::Left && !btnPressed) {
      sfx_menu.play();
			btnPressed = true;
			if (selector-1 >= 0) {
				beatmaps[selector]->detenerPreview();
				selector--;
				beatmaps[selector]->tocarPreview();
			}
			else {
				beatmaps[selector]->detenerPreview();
				selector = cantidadBeatmaps-1;
				beatmaps[selector]->tocarPreview();
			}
		}
    else if ( (evento.key.code == Keyboard::Up || evento.key.code == Keyboard::Down) && !btnPressed) {
      sfx_menu.play();
      btnPressed = true;
      beatmaps[selector]->seleccionarDificultad(evento.key.code);
    }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Escape) && !btnPressed) {
		beatmaps[selector]->detenerPreview();
		estado = MENU;
		selector = SELECCION_CANCION;
		btnPressed = true;
		spriteSelectorMenu.setPosition(435, 290);
    sfx_menu_volver.play();
	}
	else if (Keyboard::isKeyPressed(Keyboard::Enter) && !btnPressed) {
    beatmaps[selector]->detenerPreview();
    //El area de juego la deberia crear beatmap.
		area = new AreaJuego(velocidadNotas, beatmaps[selector]->getCancion(), beatmaps[selector]->getDirDificultad(), beatmaps[selector]->getBackground());
		area->comenzar();
		estado = JUGANDO;
	}
	else if (evento.type == Event::KeyReleased) {
		btnPressed = false;
	}

}
void opciones(Event &evento, Sprite &spriteOpciones, Sprite &spriteSelectorMenu, RenderWindow &ventana, Text &texto_velocidad_notas, Text &texto_volumen_musica, Text &texto_volumen_sfx, Beatmap* beatmaps[], const unsigned int cantidadBeatmaps) {
  //Controla el menu de opciones, permite cambiar la velocidad de las notas y volumen de los sonidos y musica.
  extern Sound sfx_menu;
  if (evento.type == Event::KeyPressed) {
    if (evento.key.code == Keyboard::Down && !btnPressed) {
			if (selector == 1) {
				selector = 2;
				spriteSelectorMenu.setPosition(550, 370);
			}
			else if (selector == 2) {
				selector = 3;
				spriteSelectorMenu.setPosition(550, 500);
			}
			else if (selector == 3) {
				selector = 4;
				spriteSelectorMenu.setPosition(-55, 665);
			}
      else if (selector == 4) {
        selector = 1;
        spriteSelectorMenu.setPosition(550, 240);
      }
			btnPressed = true;
      sfx_menu.play();
		}
		else if (evento.key.code == Keyboard::Up && !btnPressed) {
			if (selector == 1) {
				selector = 4;
				spriteSelectorMenu.setPosition(-55, 665);
			}
			else if (selector == 2) {
				selector = 1;
				spriteSelectorMenu.setPosition(550, 240);
			}
			else if (selector == 3) {
				selector = 2;
				spriteSelectorMenu.setPosition(550, 370);
			}
      else if (selector == 4) {
				selector = 3;
				spriteSelectorMenu.setPosition(550, 500);
			}
			btnPressed = true;
      sfx_menu.play();
		}
    else if ( (Keyboard::isKeyPressed(Keyboard::Left) && !btnPressed) || (Keyboard::isKeyPressed(Keyboard::Right) && !btnPressed) ) {
      btnPressed = true;
      if (selector == 1 && Keyboard::isKeyPressed(Keyboard::Right)) {
        if (escala_velocidad_notas <= 5) {
          escala_velocidad_notas++;
          texto_velocidad_notas.setString( to_string(escala_velocidad_notas) );
        }
      }
      else if ( selector == 1 && Keyboard::isKeyPressed(Keyboard::Left)) {
        if (escala_velocidad_notas >= 2) {
          escala_velocidad_notas--;
          texto_velocidad_notas.setString( to_string(escala_velocidad_notas) );
        }
      }
      if (selector == 2 && Keyboard::isKeyPressed(Keyboard::Right)) {
        if (volumen_musica <= 45) {
          volumen_musica+=5;
          texto_volumen_musica.setString( to_string(volumen_musica*2) );
        }
      }
      else if ( selector == 2 && Keyboard::isKeyPressed(Keyboard::Left)) {
        if (volumen_musica >= 5) {
          volumen_musica-=5;
          texto_volumen_musica.setString( to_string(volumen_musica*2) );
        }
      }
      if (selector == 3 && Keyboard::isKeyPressed(Keyboard::Right)) {
        if (volumen_sfx <= 45) {
          volumen_sfx+=5;
          texto_volumen_sfx.setString( to_string(volumen_sfx*2) );
        }
      }
      else if ( selector == 3 && Keyboard::isKeyPressed(Keyboard::Left)) {
        if (volumen_sfx >= 5) {
          volumen_sfx-=5;
          texto_volumen_sfx.setString( to_string(volumen_sfx*2) );
        }
      }
      sfx_menu.play();
    }
	}
	else if ( (Keyboard::isKeyPressed(Keyboard::Enter) && !btnPressed) || (Keyboard::isKeyPressed(Keyboard::Escape) && !btnPressed) ) {
    if (selector == 4 || (Keyboard::isKeyPressed(Keyboard::Escape) && !btnPressed) ) {
      extern Sound sfx_menu_volver;
      estado = MENU; //Volver atras
  		btnPressed = true;
  		spriteSelectorMenu.setPosition(430, 290);
      selector = SELECCION_CANCION;
      sfx_menu_volver.play();
    }
	}
	else if (evento.type == Event::KeyReleased) {
		btnPressed = false;
	}
  //Modifico el volumen de los sonidos/musica
  modificarVolumenSonidos(beatmaps, cantidadBeatmaps);
  //Le modifico la velocidad a velocidadNotas
  if (escala_velocidad_notas == 6) {
    velocidadNotas = 0.85;
  }
  else if (escala_velocidad_notas == 5) {
    velocidadNotas = 1;
  }
  else if (escala_velocidad_notas == 4) {
    velocidadNotas = 1.3;
  }
  else if (escala_velocidad_notas == 3) {
    velocidadNotas = 1.6;
  }
  else if (escala_velocidad_notas == 2) {
    velocidadNotas = 1.9;
  }
  else {
    velocidadNotas = 2.3;
  }
	//Dibujo el menu
  ventana.draw(spriteOpciones);
  ventana.draw(spriteSelectorMenu);
  ventana.draw(texto_velocidad_notas);
  ventana.draw(texto_volumen_musica);
  ventana.draw(texto_volumen_sfx);
}
void jugar(Event &evento, RenderWindow &ventana, AreaJuego* &area) {
//Ejecutar acciones del area de juego.
  //Si no esta en pausa, ejecutar
	if (!area->estadoEnPausa()) {
		area->mostrarAreaJuego(ventana);
    area->controlarNotas(evento, ventana);
    area->controlarAnimaciones(ventana);
		if (Keyboard::isKeyPressed(Keyboard::Escape) && !btnPressed) {
			area->pausa(ventana, evento);
			btnPressed = true;
		}
		else if ( (evento.type == Event::KeyReleased) && btnPressed) {
			btnPressed = false;
		}
  }
	else {
		area->pausa(ventana, evento);
	}
}
void volverMenu(Beatmap* beatmaps[], AreaJuego* areaJuego) {
  //Elimina el area de juego y vuelve al menu
  extern int selector;
  estado = SELECCION_CANCION;
  beatmaps[selector]->tocarPreview();
  delete areaJuego;
}
void mostrarPuntaje(AreaJuego* areaJuego, RenderWindow &ventana, Beatmap* beatmap) {
  //Muestra el puntaje, al terminar ejecuta la animacion y elimina el area de juego, luego vulve a SELECCION_CANCION
  areaJuego->pantallaPuntaje(ventana);
  areaJuego->controlarAnimaciones(ventana);
  if (estado == SELECCION_CANCION) {
    delete areaJuego;
    beatmap->tocarPreview();
  }
}
void perder(RenderWindow &ventana, AreaJuego* area) {
  //Secuencia de acciones al perder, luego cambia de estado a VOLVER_MENU, eliminando el area de juego
  area->mostrarAreaJuego(ventana);
  area->perder(ventana);
}
