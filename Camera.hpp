#ifndef Camera_hpp
#define Camera_hpp

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();

    glm::mat4 get_projection();
    glm::mat4 get_view();

    glm::vec3 get_position();
    glm::vec3 get_target();
    glm::vec3 get_up();
    glm::vec3 get_forward();
    glm::vec3 get_left();

    // move camera
    void move(const glm::vec3& displacement);

    // move camera towards target
    void zoom(float distance);

    // move camera left/right around target
    void orbitate(float angle);

    void set_aspect_ratio(float aspect);

private:
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    float field_of_view;
    float aspect_ratio;
    float near;
    float far;
    float min_zoom;
    float max_zoom;

    glm::vec3 target;
    glm::vec3 position;
    glm::vec3 up;
};

#endif
