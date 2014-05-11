#include "Camera.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>

Camera::Camera()
: field_of_view(45.0f),
  aspect_ratio(4.0f/3.0f),
  near(0.1f), far(1000.0f),
  min_zoom(10.0f),
  max_zoom(500.0f),
  target(64,0,64),
  position(0.0f, 64.0f, 0.0f),
  up(0.0f, 1.0f, 0.0f)
{}

glm::mat4 Camera::get_projection()
{
    return glm::perspective<float>(field_of_view, aspect_ratio, near, far);
}

glm::mat4 Camera::get_view()
{
    return glm::lookAt(position, target, up);
}

glm::vec3 Camera::get_position()
{
    return position;
}

glm::vec3 Camera::get_target()
{
    return target;
}

glm::vec3 Camera::get_up()
{
    return up;
}

glm::vec3 Camera::get_forward()
{
    glm::vec3 direction = position - target;
    return glm::normalize(glm::dot(direction, up) * up - direction);
}

glm::vec3 Camera::get_left()
{
    return glm::normalize(glm::cross(position - target, up));
}

void Camera::move(const glm::vec3& displacement)
{
    target += displacement;
    position += displacement;
}

void Camera::zoom(float distance)
{
    glm::vec3 direction = position - target;
    float current_distance = glm::length(direction);
    distance = std::min(current_distance - min_zoom, distance);
    distance = std::max(current_distance - max_zoom, distance);
    glm::vec3 displacement = glm::normalize(direction) * distance;
    position -= displacement;
}

void Camera::orbitate(float angle)
{
    glm::vec3 direction = position - target;
    position = glm::rotate(direction, -angle, up) + target;
}

void Camera::set_aspect_ratio(float aspect)
{
    aspect_ratio = aspect;
}
