#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

#include <GL/glew.h> // GLuint

void set_model_view_matrix(GLuint program_id, float aspect_ration);
void render_arrays(GLuint *vaoID, int nb_elt);
void render_elements(GLuint *vaoID, int nb_elt);

#endif // RENDER_SCENE_CPP
