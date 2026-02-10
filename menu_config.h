#ifndef MENU_CONFIG_H_INCLUDED
#define MENU_CONFIG_H_INCLUDED

// Funciones de menú de configuración
int mostrarMenuConfiguracion(SDL_Renderer *renderer, SDL_Texture *texturaFondo, Configuracion *config);
int solicitarNombresJugadores(SDL_Renderer *renderer, SDL_Texture *texturaFondo, DatosJugadores *jugadores, int cantJugadores);

#endif // MENU_CONFIG_H_INCLUDED
