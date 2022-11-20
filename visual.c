#include "visual.h"

visualEntity visualEntityInit( char *path ){
	visualEntity ve = {
		.m4Projection = GLM_MAT4_IDENTITY_INIT,
		.m4View = GLM_MAT4_IDENTITY_INIT,
		.m4Model = GLM_MAT4_IDENTITY_INIT,
		.m4MVP = GLM_MAT4_IDENTITY_INIT
	};
	
	ve.Render = &visualEntityRender;
	ve.Set = &visualEntityMVPSet;
	ve.Update = &visualEntityUpdate;
	ve.Destroy = &visualEntityDestroy;

	FILE *fp = fopen(path, "r");
	if(fp == NULL){
		printf("Model load error: %s\n", path);
	}

	char tmpc[100];

	int v_count = 0;
	int vt_count = 0;
	int vn_count = 0;
	int fv_count = 0;
	int fvt_count = 0;
	int fvn_count = 0;

	/* get v,vt and vn data */
	while(1){
		if( fscanf(fp, "%s", tmpc) == EOF ){
			break;
		}

		if(strcmp(tmpc, "v") == 0){
			fscanf(fp, "%f", &ve.v[v_count]);
			v_count++;
			fscanf(fp, "%f", &ve.v[v_count]);
			v_count++;
			fscanf(fp, "%f", &ve.v[v_count]);
			v_count++;
		}

		if(strcmp(tmpc, "vt") == 0){
			fscanf(fp, "%f", &ve.vt[vt_count]);
			vt_count++;
			fscanf(fp, "%f", &ve.vt[vt_count]);
			vt_count++;
			fscanf(fp, "%f", &ve.vt[vt_count]);
			vt_count++;
		}

		if(strcmp(tmpc, "vn") == 0){
			fscanf(fp, "%f", &ve.vn[vn_count]);
			vn_count++;
			fscanf(fp, "%f", &ve.vn[vn_count]);
			vn_count++;
			fscanf(fp, "%f", &ve.vn[vn_count]);
			vn_count++;
		}

		if(strcmp(tmpc, "f") == 0){
			fscanf(fp, "%d/%d/%d", &ve.fv[fv_count], &ve.fvt[fvt_count], &ve.fvn[fvn_count]);
			fv_count++; fvt_count++; fvn_count++;
			fscanf(fp, "%d/%d/%d", &ve.fv[fv_count], &ve.fvt[fvt_count], &ve.fvn[fvn_count]);
			fv_count++; fvt_count++; fvn_count++;
			fscanf(fp, "%d/%d/%d", &ve.fv[fv_count], &ve.fvt[fvt_count], &ve.fvn[fvn_count]);
			fv_count++; fvt_count++; fvn_count++;
		}

	}
	
	glGenBuffers(1, &ve.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, ve.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ve.v), ve.v, GL_STATIC_DRAW);

	glGenBuffers(1, &ve.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ve.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ve.fv), ve.fv, GL_STATIC_DRAW);

	ve.v_size = sizeof(GLfloat) * v_count;
	ve.vt_size = sizeof(GLfloat) * vt_count;
	ve.vn_size = sizeof(GLfloat) * vn_count;
	ve.fv_size = sizeof(GLfloat) * fv_count;
	ve.fvt_size = sizeof(GLfloat) * fvt_count;
	ve.fvn_size = sizeof(GLfloat) * fvn_count;

	// TODO, use ve.x_count variables directly in here, discard local x_count;
	ve.v_count = v_count;
	ve.vt_count = vt_count;
	ve.vn_count = vn_count;
	ve.fv_count = fv_count;
	ve.fvt_count = fvt_count;
	ve.fvn_count = fvn_count;

	return ve;
}

void visualEntityRender(visualEntity *ve, GLuint type){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindBuffer(GL_ARRAY_BUFFER, ve->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ve->EBO);
	
	glEnableVertexAttribArray(ve->shaderID);
	glVertexAttribPointer( ve->shaderID, 3,
			       GL_FLOAT, GL_FALSE,
			       0, (void *)0 );

	switch(type){
	case GL_ARRAY_BUFFER: goto TRIANGLES; break;
	case GL_ELEMENT_ARRAY_BUFFER: goto ELEMENTS; break;
	default: printf("Uknown rendering type.\n"); goto END; break;
	}
	
TRIANGLES:	
	glDrawArrays(GL_TRIANGLES, 0, ve->v_count);
	
	goto END;
ELEMENTS:
	glDrawElements(GL_TRIANGLES, ve->fv_count, GL_UNSIGNED_INT, (void*)0);
END:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(ve->shaderID);
}

void visualEntityUpdate(visualEntity *ve){
	glm_perspective(
		ve->fFOV,
		ve->fAspectRatio,
		ve->fNear,
		ve->fFar,
		ve->m4Projection );

	glm_lookat(
		ve->v3Camera,
		ve->v3Origin,
		ve->v3Up,
		ve->m4View );

	glm_mat4_mulN( (mat4 *[]){&ve->m4Projection, &ve->m4View, &ve->m4Model}, 3 , ve->m4MVP );
}




void visualEntityDestroy(visualEntity *ve){
	glDeleteBuffers(1, &ve->VBO);
	ve->Render = NULL;
	ve->Update = NULL;
	ve->Destroy = NULL;
}

void visualEntityMVPSet(visualEntity *ve, float fov, float aspect_ratio,
			float near, float far,
			vec3 camera, vec3 origin, vec3 up){
	ve->fFOV = fov;
	ve->fAspectRatio = aspect_ratio;
	ve->fNear = near;
	ve->fFar = far;

	for(int i = 0; i < 2; i++){
		ve->v3Camera[i] = camera[i];
	}

	for(int i = 0; i < 2; i++){
		ve->v3Origin[i] = origin[i];
	}

	for(int i = 0; i < 2; i++){
		ve->v3Up[i] = up[i];
	}
}
