#ifndef OBJECT_HPP
# define OBJECT_HPP

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtx/transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <memory>
# include <vector>

# include "material.hpp"

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
    void set_material(std::shared_ptr<const material> material);
    container3& get_vertices();
    container3& get_normals();
    container2& get_text_coord();
    virtual ~object();
private:
    glm::mat4 model_mat_;
    std::shared_ptr<const material> material_;
    container3 vertices_;
    container3 normals_;
    container2 text_coords_;
};

#endif // OBJECT_HPP
