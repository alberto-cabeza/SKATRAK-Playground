#include "../include/SKATRAK_PLAYGROUND.hpp"

/**
 * @brief Crea la clase sistema e inicializa las propiedades de la pantalla. Tambi�n inicializa SDL.
 * @param scr_w Anchura de la pantalla.
 * @param scr_h Altura de la pantalla.
 * @param depth Profundidad de bits de la pantalla.
 * @note En el caso de que la pantalla no se pudiera iniciar con las opciones especificadas, imprimir� un mensaje de error.
 */
system_t::system_t(int scr_w, int scr_h, int depth): screen(NULL), wIcon(NULL), screenWidth(scr_w), screenHeight(scr_h), bpp(depth), fullscr(false) {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		fprintf(stderr, "No se ha podido inicializar SDL.\n");
	screen = SDL_SetVideoMode(screenWidth, screenHeight, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(screen == NULL)
		fprintf(stderr, "No se ha podido iniciar el modo de video %dx%d a %d bits.\n", screenWidth, screenHeight, bpp);
	SDL_WM_SetCaption("SKATRAK Playground", NULL);
}

/**
 * @brief Destructor. Libera la memoria ocupada por la pantalla y cierra SDL.
 */
system_t::~system_t(){
	if(screen != NULL)
		SDL_FreeSurface(screen);
	SDL_Quit();
}

/**
 * @brief Intercambia entre pantalla completa y ventana.
 */
void system_t::toggleFullscreen(){
	fullscr? fullscr = false : fullscr = true;
	if(screen != NULL){
		SDL_FreeSurface(screen);
		screen = NULL;
	}
	if(fullscr){
		screen = SDL_SetVideoMode(screenWidth, screenHeight, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
		if(screen == NULL)
			fprintf(stderr, "No se ha podido iniciar el modo de video %dx%d a %d bits con pantalla completa.\n", screenWidth, screenHeight, bpp);
	} else {
		screen = SDL_SetVideoMode(screenWidth, screenHeight, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
		if(screen == NULL)
			fprintf(stderr, "No se ha podido iniciar el modo de video %dx%d a %d bits sin pantalla completa.\n", screenWidth, screenHeight, bpp);
	}
}

/**
 * @brief Le da un icono a la ventana del programa.
 * @param iconpath Ruta del icono.
 * @note Este icono no ser� visible en pantalla completa.
 */
void system_t::setIcon(string iconpath){
	string compPath = IMG_PATH;
	compPath += iconpath;

	if(wIcon != NULL){
		SDL_FreeSurface(wIcon);
		wIcon = NULL;
	}
	wIcon = IMG_Load(compPath.c_str());
	if(wIcon == NULL)
		fprintf(stderr, "No se ha podido cargar el icono del programa.\n");
	else
		SDL_WM_SetIcon(wIcon, NULL);
}

/**
 * @brief Actualiza la pantalla.
 * @note Es necesario llamar esta funci�n para que se intercambien los buffers de v�deo.
 */
void system_t::update(){
	if(screen != NULL)
		SDL_Flip(screen);
	else
		fprintf(stderr, "No se ha podido mostrar la pantalla.\n");
}
