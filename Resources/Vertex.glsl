#version 150

uniform mat4 projection; //this is the new variable
uniform mat4 camera;

in vec3 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord;
    
    // Apply camera and projection transformations to the vertex
    gl_Position = projection * camera * vec4(vert, 1);
}