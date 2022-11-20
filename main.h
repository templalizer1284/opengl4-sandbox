#ifndef CSHADER_H
#define CSHADER_H

#include "includes.h"
#include "visual.h"
#include "shader.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

/* Counts number of triangles in array */
#define vsize(x) \
	( ( sizeof(x) / sizeof(GLfloat) ) / 3 )

static SDL_Window *window;
static SDL_GLContext gl_context;

static void init(void);
static void destroy(void);

enum VAO_IDs { TriangleVAO, NumVAOs };
enum VBO_IDs { CubeVBO, ColorVBO, TriangleVBO, NumVBOs};
enum Attribute_IDs { vModelPosition = 0, vColor, vTexturePosition };
enum ModelID { Cube, NumModels };

static GLuint VAO[NumVAOs];

void init(){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0){
		printf("SDL_Init error: %s\n", SDL_GetError());
	}

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	if( ( window = SDL_CreateWindow("Test",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					1024,
					768,
					SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)) == NULL ){
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
	}

	if( ( gl_context = SDL_GL_CreateContext(window) ) == NULL ){
		printf("SDL_CreateContext error: %s\n", SDL_GetError());
	}

	/* Enables Z-buffering */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	/* Enables vertical sync */
	if( SDL_GL_SetSwapInterval(1) != 0 ){
		printf("Vertical sync failed. %s\n", SDL_GetError());
	}
	
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	if( IMG_Init(IMG_INIT_JPG) == 0){
		printf("SDL_image failed to load. %s\n", IMG_GetError());
	}
}

void destroy(){
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

#endif
