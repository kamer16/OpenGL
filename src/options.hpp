#ifndef OPTIONS_CPP
# define OPTIONS_CPP

# include <string>

struct options
{
public:
    void parse_args(int argc, char *argv[]);
    std::string mesh_file = "data/teapot/teapot.obj";
    int window_width = 880;
    int window_height = 520;
    char fullscreen = 0;
    char padding[7];
};

#endif // OPTIONS_CPP
