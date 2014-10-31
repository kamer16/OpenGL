#ifndef LIGHT_HPP
# define LIGHT_HPP

# include <glm/glm.hpp>

class light
{
public:
    const glm::vec4& get_ambient() { return ambient_; }
    const glm::vec4& get_diffuse() { return diffuse_; }
    const glm::vec4& get_specular() { return specular_; }
    const glm::vec4& get_position() { return position_; }
    void set_ambient(glm::vec4&& ambient) { ambient_ = ambient; }
    void set_diffuse(glm::vec4&& diffuse) { diffuse_ = diffuse; }
    void set_specular(glm::vec4&& specular) { specular_ = specular; }
    void set_position(glm::vec4&& position) { position_ = position; }
    void set_ambient(glm::vec4& ambient) { ambient_ = ambient; }
    void set_diffuse(glm::vec4& diffuse) { diffuse_ = diffuse; }
    void set_specular(glm::vec4& specular) { specular_ = specular; }
    void set_position(glm::vec4& position) { position_ = position; }
private:
    glm::vec4 ambient_;
    glm::vec4 diffuse_;
    glm::vec4 specular_;
    glm::vec4 position_;
};

light* default_dir_light_new();
light* default_pos_light_new();

#endif // LIGHT_HPP
