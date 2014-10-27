#ifndef OBJECT_HPP
# define OBJECT_HPP

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtx/transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <GL/glew.h>

# include <unordered_map>
# include <tuple>
# include <vector>

# include "utility.hpp"
# include "material.hpp"

class object
{
public:
    using container3 = std::vector<glm::vec3>;
    using container2 = std::vector<glm::vec2>;
    using container_vtn = std::vector<utility::vertex_vtn>;
    using container_vn = std::vector<utility::vertex_vn>;
    using materials_t = std::vector<material*>;

    object();
    void translate(const glm::vec3& dir);
    void scale(const glm::vec3& vec);
    void rotate(float degrees, const glm::vec3& dir);
    virtual void draw(GLuint program_id);
    void set_model_mat(glm::mat4& model_mat);
    const glm::mat4& get_model_mat();
    // Vertices can be updated by caller
    container_vtn& get_vertices_vtn();
    container_vn& get_vertices_vn();
    void add_material(material* mat);
    // Can be accessed to be sorted
    materials_t& get_materials();
    // Generate all required buffers and vao.
    // TODO object should not know about bindings
    void bind_indexed_vao(GLuint program_id);
    virtual ~object();
protected:
    GLuint vertex_buffer_id_;
    GLuint index_buffer_id_;
    GLuint vao_id_;
    container_vtn vertices_vtn_;
    container_vn vertices_vn_;
    materials_t materials_;
private:
    // TODO should be called by resour/program manager
    template <typename T>
    void load_vertex_buffer(std::vector<T>& vertices, GLuint* id);
    // TODO should be called by resour/program manager
    template <typename T>
    void load_index_buffer(std::vector<T>& indices, GLuint* buff_id);
    void set_vao_attribs(GLuint program_id, bool has_text_coord, size_t stride);
    glm::mat4 model_mat_;
};

#endif // OBJECT_HPP
