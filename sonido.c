#include "sonido.h"
#include <stdio.h>

int inicializarSonido() {
    // Inicializar SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error inicializando SDL_mixer: %s\n", Mix_GetError());
        return -1;
    }
    return 0;
}

int cargarEfectosSonido(EfectosSonido *efectos) {
    // Cargar sonido de selección
    efectos->seleccion = Mix_LoadWAV("snd/seleccion.wav");
    if (!efectos->seleccion) {
        printf("Advertencia: No se pudo cargar seleccion.wav: %s\n", Mix_GetError());
    }
    
    // Cargar sonido de acierto
    efectos->acierto = Mix_LoadWAV("snd/acierto.wav");
    if (!efectos->acierto) {
        printf("Advertencia: No se pudo cargar acierto.wav: %s\n", Mix_GetError());
    }
    
    // Cargar sonido de error
    efectos->error = Mix_LoadWAV("snd/error.wav");
    if (!efectos->error) {
        printf("Advertencia: No se pudo cargar error.wav: %s\n", Mix_GetError());
    }
    
    return 0;
}

void liberarEfectosSonido(EfectosSonido *efectos) {
    if (efectos->seleccion) {
        Mix_FreeChunk(efectos->seleccion);
        efectos->seleccion = NULL;
    }
    if (efectos->acierto) {
        Mix_FreeChunk(efectos->acierto);
        efectos->acierto = NULL;
    }
    if (efectos->error) {
        Mix_FreeChunk(efectos->error);
        efectos->error = NULL;
    }
}

void reproducirSonido(Mix_Chunk *efecto) {
    if (efecto) {
        Mix_PlayChannel(-1, efecto, 0);
    }
}

void cerrarSonido() {
    Mix_CloseAudio();
}
