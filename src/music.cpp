#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "../include/music.hpp"

// Prototipo de la funi�n de callback (La que en realidad deber�a ser parte de la clase)
void nextTrack();

/**
 * @brief Inicializa la clase con los valores por defecto e inicia SDL_mixer.
 *
 * Tambi�n reserva espacio para AUDIO_CHANNELS canales de audio para reproducir efectos de sonido.
 * @see sfx_t
 */
music_t::music_t(): music(NULL), music_names(NULL), n_tracks(0), volume(128), current(0), running(false), playing(false), paused(false) {
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096)){
		fprintf(stderr, "No se ha podido inicializar el sistema de audio.\n");
	}
	else {
		Mix_AllocateChannels(AUDIO_CHANNELS);
		running = true;
		Mix_HookMusicFinished(nextTrack);
	}
}

/**
 * @brief Inicializa la clase con los valores por defecto, inicia SDL_mixer y reserva espacio para un n�mero de canciones.
 * @param nT N�mero de canciones que deseas poder reproducir c�clicamente.
 *
 * Tambi�n reserva espacio para AUDIO_CHANNELS canales de audio para reproducir efectos de sonido.
 * @see sfx_t
 */
music_t::music_t(int nT): music(NULL), music_names(NULL), volume(128), current(0), running(false), playing(false), paused(false), n_tracks(nT) {
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096)){
		fprintf(stderr, "No se ha podido inicializar el sistema de audio.\n");
	}
	else {
		Mix_AllocateChannels(AUDIO_CHANNELS);
		running = true;
		setTracks(n_tracks);
		Mix_HookMusicFinished(nextTrack);
	}
}

/**
 * @brief Destructor. Libera la memoria ocupada por todas las canciones.
 */
music_t::~music_t(){
	if(playing)
		halt();
	if(music != NULL){
		for(int i = 0; i < n_tracks; i++)
			Mix_FreeMusic(music[i]);
		delete [] music;
		music = NULL;
		Mix_HookMusicFinished(NULL);
	}
	if(music_names != NULL)
		delete [] music_names;
	n_tracks = 0;
	volume = 0;
	running = false;
	Mix_CloseAudio();
}

/**
 * @brief Reserva espacio para un n�mero determinado de canciones. Borra la informaci�n la lista de reproducci�n actual
 * aunque se especifique el mismo n�mero.
 * @param n N�mero de canciones que se van a reservar.
 * @note Para evitar errores en tiempo de ejecuci�n, hay que llenar este array completamente con music_t::setTrack.
 */
void music_t::setTracks(int n){
	if(playing)
		halt();
	if(music != NULL){
		for(int i = 0; i < n_tracks; i++){
			if(music[i] != NULL){
				Mix_FreeMusic(music[i]);
				music[i] = NULL;
			}
		}
		delete [] music;
		music = NULL;
	}
	if(music_names != NULL){
		delete [] music_names;
		music_names = NULL;
	}
	n_tracks = n;
	music = new Mix_Music*[n_tracks];
	if(music == NULL)
		fprintf(stderr, "Error en la reserva de memoria para la lista de reproducci�n.\n");
	else {
		for(int i = 0; i < n_tracks; i++)
			music[i] = NULL;
	}
	music_names = new string[n_tracks];
	if(music_names == NULL)
		fprintf(stderr, "Error en la reserva de memoria para los nombres de las canciones de la lista de reproducci�n.\n");
}

/**
 * @brief Carga una canci�n en un �ndice del array de la lista de reproducci�n.
 * @param index Posici�n en la lista de reproducci�n (Entre 0 y n-1).
 * @param name Nombre del fichero donde est� guardada la canci�n.
 * @note Para llamar a esta funci�n hay que llamar previamente a music_t::setTracks para reservar la memoria.
 */
void music_t::setTrack(int index, string name){
	if(playing)
		halt();
	if(index < 0 || index >= n_tracks){
		fprintf(stderr, "Se ha intentado cargar una canci�n en una zona de memoria no reservada. Se sobreescribir� la primera canci�n.\n");
		index = 0;
	}
	if(music != NULL && music_names != NULL){
		music_names[index] = name;
		if(music[index] != NULL){
			Mix_FreeMusic(music[index]);
			music[index] = NULL;
		}
		music[index] = Mix_LoadMUS(name.c_str());
		if(!music[index]){
			fprintf(stderr, "No se ha podido cargar el fichero \"%s\".\n", name.c_str());
			music[index] = NULL;
		}
	}
	else
		fprintf(stderr, "No se puede cargar ninguna canci�n en memoria porque no se ha reservado previamente.\n");
}

/**
 * @brief Si el sistema de audio est� iniciado correctamente y la m�sica no se est� reproduciendo, se comienza a reproducir.
 * Si la m�sica est� pausada, se reanuda.
 */
void music_t::play(){
	if(running){
		if(!playing){
			if(!paused){
				Mix_FadeInMusic(music[current], 0, FADEIN_DELAY);
				playing = true;
			}
			else {
				Mix_ResumeMusic();
				paused = false;
			}
		}
	}
	else
		fprintf(stderr, "No se puede reproducir la m�sica porque no se ha inicializado correctamente.\n");
}

/**
 * @brief La m�sica se pausa si se est� reproduciendo.
 */
void music_t::pause(){
	if(running && playing && !paused){
		Mix_PauseMusic();
		paused = true;
	}
}

/**
 * @brief La pista actual se para de reproducir (No pasa autom�ticamente a la siguiente pista).
 * @see nextTrack
 */
void music_t::halt(){
	if(running && playing){
		Mix_HaltMusic();
		playing = false;
		paused = false;
	}
}

/**
 * @brief Le asigna un volumen a la m�sica que se est� reproduciendo.
 * @param vol Valor del volumen que se le va a asignar, perteneciente al intervalo (0, 128).
 * 0 se corresponde con silencioso y 128 es el volumen m�ximo.
 */
void music_t::setVol(int vol){
	if(vol != volume && vol >= 0 && vol <= MAX_VOLUME)
		Mix_VolumeMusic(vol);
}
