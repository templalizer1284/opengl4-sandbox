#include "main.h"

int main(void){

	init();
	
	SDL_Event ev;
	
	glGenVertexArrays(NumVAOs, VAO);
	glBindVertexArray(VAO[TriangleVAO]);

	Shader shader = ShaderInit();
	shader.Load(&shader, "VertexShader.glsl", "FragmentShader.glsl");
	glUseProgram(shader.program);
	
	visualEntity veCube = visualEntityInit( "triangle.obj" );
	veCube.Set(&veCube, glm_rad(45), 4.0f / 3.0f,
		   0.1f, 100.0f, (vec3){4,3,3}, (vec3){0,0,0}, (vec3){0,1,0});
	veCube.Update(&veCube);
	veCube.uniformMVPLocation = glGetUniformLocation(shader.program, "MVP");

	float angle = 0;

	SDL_Surface *surf = IMG_Load("wall.jpg");

	GLfloat texCoords[] = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surf->w, surf->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(surf);

	printf("%d:%d\n", surf->w, surf->h);

	glEnableVertexAttribArray(vTexturePosition);
	glBindTexture(GL_TEXTURE_2D, texture);
	glVertexAttribPointer(vTexturePosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint textLocation = glGetUniformLocation(shader.program, "Texture");
	glUniform1i(textLocation, 0);

	cgltf_options options = {0};
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, "cube.gltf", &data);
	if (result == cgltf_result_success){
		printf("Successfully loaded.\n");
	}

	printf("Meshes number: %lu.\n", data->meshes->primitives->indices);
	
 	while(true){
		if(SDL_PollEvent(&ev) != 0){
			if(ev.key.keysym.sym == SDLK_q){
				break;
			}
			if(ev.key.keysym.sym == SDLK_UP){
				veCube.v3Camera[0] -= 0.1f;
				veCube.Update(&veCube);
			}
			if(ev.key.keysym.sym == SDLK_DOWN){
				veCube.v3Camera[0] += 0.1f;
				veCube.Update(&veCube);
			}
		}
		
		glUniformMatrix4fv(veCube.uniformMVPLocation, 1, GL_FALSE, &veCube.m4MVP[0][0]);
		veCube.shaderID = vModelPosition;
		veCube.Render(&veCube, GL_ARRAY_BUFFER);
		
		SDL_GL_SwapWindow(window);
		SDL_Delay(20);
	}

	glDeleteVertexArrays(NumVAOs, VAO);
	ShaderDestroy(&shader);
	destroy();

	return 0;
}
