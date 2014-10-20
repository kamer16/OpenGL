#ifndef OBJECT_HPP
# define OBJECT_HPP

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

class object
{
public:
    void translate(const glm::vec3& dir);
    void rotate(float degrees, const glm::vec3& dir);
    virtual void draw() = 0;
    void set_model_mat(glm::mat4& model_mat);
    const glm::mat4& get_model_mat();
    virtual ~object();
private:
    glm::mat4 model_mat_;
};

#endif // OBJECT_HPP
