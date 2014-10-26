#ifndef MESH_OBJECT
# define MESH_OBJECT

# include "object.hpp"

class mesh_object : public object
{
public:
    virtual ~mesh_object() override;
    // Draw objects in an indexed manner
    virtual void draw() override;
};

#endif // MESH_OBJECT
