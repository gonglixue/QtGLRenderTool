#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_projection_mat;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;


out vec3 o_normal;
out vec3 o_lightVector;
out vec3 o_viewVector;  //in view space


void main()
{
    vec4 positionWorld = u_model_mat * vec4(position, 1.0);
    vec3 normalWorld = mat3(transpose(inverse(u_model_mat))) * normal;

    o_lightVector = u_lightPos - positionWorld.xyz;
    o_viewVector = -vec3(u_view_mat  * positionWorld);
    o_normal = normalWorld;

    gl_Position = u_projection_mat * u_view_mat * u_model_mat * vec4(position, 1.0f);
}
