#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

#include <GL/glew.h> // GLuint

void set_model_view_matrix(GLuint program_id, float aspect_ration);
void renderScene(GLuint *vaoID);

#endif // RENDER_SCENE_CPP
