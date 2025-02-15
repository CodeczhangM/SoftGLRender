/*
 * SoftGLRender
 * @author 	: keith@robot9.me
 *
 */


namespace SoftGL {

const char *BASIC_VS = R"(
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_tangent;

layout (std140) uniform UniformsMVP {
    mat4 u_modelMatrix;
    mat4 u_modelViewProjectionMatrix;
    mat3 u_inverseTransposeModelMatrix;
};

void main() {
    gl_Position = u_modelViewProjectionMatrix * vec4(a_position, 1.0);
}
)";

const char *BASIC_FS = R"(
out vec4 FragColor;

layout (std140) uniform UniformsColor {
    vec4 u_baseColor;
};

void main() {
    FragColor = u_baseColor;
}
)";

}
