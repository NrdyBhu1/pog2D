#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#ifndef Py_POG_H
#define Py_POG_H
#include "./render.h"

static PyObject* PogInitError;
static bool initialized;

typedef struct {
	PyObject_HEAD
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event ev;
	PyObject *handlers;
} PogContext;

static PogContext *context;

#endif // Py_POG_H
