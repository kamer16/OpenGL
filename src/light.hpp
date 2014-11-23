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

class dir_light : public light
{
public:
    static const bool is_dir = 1;
    static const bool is_pos = 0;
    static const bool is_spot = 0;
};


class pos_light : public light
{
public:
    static const bool is_dir = 0;
    static const bool is_pos = 1;
    static const bool is_spot = 0;
    void set_const_att(float att) { const_att_ = att; }
    float get_const_att() { return const_att_; }
    void set_linear_att(float att) { linear_att_ = att; }
    float get_linear_att() { return linear_att_; }
    void set_quadratic_att(float att) { quadratic_att_ = att; }
    float get_quadratic_att() { return quadratic_att_; }
private:
    float const_att_ = 1.0f;
    float linear_att_ = 0.0001f;
    float quadratic_att_ = 0.0001f;
};

class spot_light : public pos_light
{
public:
    static const bool is_dir = 0;
    static const bool is_pos = 0;
    static const bool is_spot = 1;
    void set_spot_cutoff(float att) { spot_cutoff_ = att; }
    float get_spot_cutoff() { return spot_cutoff_; }
    void set_spot_exponent(float att) { spot_exponent_ = att; }
    float get_spot_exponent() { return spot_exponent_; }
    void set_spot_dir(glm::vec3 dir) { spot_dir_ = dir; }
    glm::vec3 get_spot_dir() { return spot_dir_; }
private:
    // Spot lights lighting direction
    glm::vec3 spot_dir_ = glm::vec3(0, 0, -1);
    // Degree value between 0 and 90
    float spot_cutoff_ = 40;
    // Exponent between 0 and 128
    float spot_exponent_ = 2;
};


dir_light* light_dir_default_new();
pos_light* light_pos_default_new();
spot_light* light_spot_default_new();

#endif // LIGHT_HPP
