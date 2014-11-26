template <class light_t>
void
scene::add_light(light_t* light)
{
    if (light_t::is_dir)
        dir_lights_.push_back(reinterpret_cast<dir_light*>(light));
    if (light_t::is_pos)
        pos_lights_.push_back(reinterpret_cast<pos_light*>(light));
    if (light_t::is_spot)
        spot_lights_.push_back(reinterpret_cast<spot_light*>(light));
}
