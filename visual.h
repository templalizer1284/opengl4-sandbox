#ifndef VISUAL_H
#define VISUAL_H

#include "includes.h"
#include "cglm/cglm.h"

typedef struct visualEntity visualEntity;

// TODO Check variables inside visualEntity struct and make them all initialize and "destruct" per standard procedure
struct visualEntity {
	GLuint VBO, EBO;
	int shaderID;
	GLuint uniformMVPLocation;
	GLfloat v[65500], vt[65000], vn[65000]; // OBJ Wavefront model data.
	GLuint fv[65000], fvt[65000], fvn[65000];
	size_t v_size, vt_size, vn_size, fv_size, fvt_size, fvn_size; // Size in bytes
	int v_count, vt_count, vn_count, fv_count, fvt_count, fvn_count; // Vertex number
	int size; /* Use it with vsize() macro, model.size = vsize(model.data) */
	mat4 m4Projection, m4View, m4Model, m4MVP;
	float fFOV, fAspectRatio, fNear, fFar;
	vec3 v3Origin, v3Camera, v3Up;
	void (*Render)(visualEntity *, GLuint);
	void (*Set)(visualEntity *, float, float, float, float, vec3 , vec3 , vec3 );
	void (*Update)(visualEntity *);
	void (*Destroy)(visualEntity *);
};

extern visualEntity visualEntityInit(char *);
extern void visualEntityRender(visualEntity *, GLuint);
extern void visualEntityUpdate(visualEntity *);
extern GLfloat *visualEntityLoadModel(visualEntity *, char *);
extern void visualEntityDestroy(visualEntity *);
extern void visualEntityMVPSet(visualEntity *, float, float, float, float, vec3 , vec3 , vec3 );

#endif
