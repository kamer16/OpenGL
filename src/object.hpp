#ifndef OBJECT_HPP
# define OBJECT_HPP

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtx/transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <GL/glew.h>

# include <unordered_set>
# include <tuple>
# include <vector>

# include "utility.hpp"

class object
{
public:
    using container3 = std::vector<glm::vec3>;
    using container2 = std::vector<glm::vec2>;
    using container_vtn = std::vector<utility::vertex_vtn>;
    using container_vn = std::vector<utility::vertex_vn>;
    using vertices_idx = std::vector<unsigned>;

    object();
    void translate(const glm::vec3& dir);
    void scale(const glm::vec3& vec);
    void rotate(float degrees, const glm::vec3& dir);
    virtual void draw() = 0;
    void set_model_mat(glm::mat4& model_mat);
    const glm::mat4& get_model_mat();
    container_vtn& get_vertices_vtn();
    container_vn& get_vertices_vn();
    vertices_idx& get_indices();
    // Generate all required buffers and vao.
    void bind_indexed_vao(GLuint program_id);
    virtual ~object();
protected:
    GLuint vert_buffer_id_;
    GLuint index_buffer_id_;
    GLuint vao_id_;
    container_vtn vertices_vtn_;
    container_vn vertices_vn_;
    vertices_idx indices_;
private:
    template <typename T>
    void load_vertex_buffer(GLuint program_id, std::vector<T>& vertices,
                            bool has_text_coord);
    template <typename T>
    void load_index_buffer(std::vector<T>& indices);
    glm::mat4 model_mat_;
};

#endif // OBJECT_HPP
