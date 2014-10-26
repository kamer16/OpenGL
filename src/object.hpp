#ifndef OBJECT_HPP
# define OBJECT_HPP

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtx/transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <GL/glew.h>

# include <memory>
# include <vector>

class object
{
public:
    using container3 = std::vector<glm::vec3>;
    using container2 = std::vector<glm::vec2>;

    object();
    void translate(const glm::vec3& dir);
    void scale(const glm::vec3& vec);
    void rotate(float degrees, const glm::vec3& dir);
    virtual void draw() = 0;
    void set_model_mat(glm::mat4& model_mat);
    const glm::mat4& get_model_mat();
    container3& get_vertices();
    container3& get_normals();
    container2& get_text_coord();
    // Generate all required buffers and vao.
    void bind_vao(GLuint program_id);
    virtual ~object();
protected:
    GLuint vert_buffer_id_;
    GLuint text_buffer_id_;
    GLuint norm_buffer_id_;
    GLuint vao_id_;
    container3 vertices_;
    container3 normals_;
    container2 text_coords_;
private:
    template <typename T>
    void load_data(GLuint program_id, std::vector<T> &data, const char *name,
                   GLuint* buffer_id);
    glm::mat4 model_mat_;
};

#endif // OBJECT_HPP
