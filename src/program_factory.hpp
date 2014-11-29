#ifndef PROGRAM_FACTORY_HPP
# define PROGRAM_FACTORY_HPP

# include "program.hpp"

class program_factory
{
public:
    program* generate_deferred_spot_light();
    program* generate_deferred_dir_light();
};

#endif // PROGRAM_FACTORY_HPP
