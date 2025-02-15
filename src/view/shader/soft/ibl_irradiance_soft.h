/*
 * SoftGLRender
 * @author 	: keith@robot9.me
 *
 */

#pragma once

#include "render/soft/shader_program_soft.h"

namespace SoftGL {
namespace ShaderIBLIrradiance {

struct ShaderDefines {
};

struct ShaderAttributes {
  glm::vec3 a_position;
  glm::vec2 a_texCoord;
  glm::vec3 a_normal;
  glm::vec3 a_tangent;
};

struct ShaderUniforms {
  // UniformsMVP
  glm::mat4 u_modelMatrix;
  glm::mat4 u_modelViewProjectionMatrix;
  glm::mat3 u_inverseTransposeModelMatrix;

  // Samplers
  SamplerCubeSoft *u_cubeMap;
};

struct ShaderVaryings {
  glm::vec3 v_worldPos;
};

class ShaderIBLIrradiance : public ShaderSoft {
 public:
  CREATE_SHADER_OVERRIDE

  std::vector<std::string> &GetDefines() override {
    static std::vector<std::string> defines;
    return defines;
  }

  std::vector<UniformDesc> &GetUniformsDesc() override {
    static std::vector<UniformDesc> desc = {
        {"UniformsMVP", offsetof(ShaderUniforms, u_modelMatrix)},
        {"u_cubeMap", offsetof(ShaderUniforms, u_cubeMap)},
    };
    return desc;
  };
};

class VS : public ShaderIBLIrradiance {
 public:
  CREATE_SHADER_CLONE(VS)

  void ShaderMain() override {
    glm::vec4 pos = u->u_modelViewProjectionMatrix * glm::vec4(a->a_position, 1.0);
    gl->Position = pos;
    gl->Position.z = pos.w;
    v->v_worldPos = a->a_position;
  }
};

class FS : public ShaderIBLIrradiance {
 public:
  CREATE_SHADER_CLONE(FS)

  void ShaderMain() override {
    glm::vec3 N = normalize(v->v_worldPos);

    glm::vec3 irradiance = glm::vec3(0.0f);

    // tangent space calculation from origin point
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(up, N));
    up = glm::normalize(glm::cross(N, right));

    float sampleDelta = 0.025f;
    float nrSamples = 0.0f;
    for (float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta) {
      for (float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta) {
        // spherical to cartesian (in tangent space)
        glm::vec3 tangentSample = glm::vec3(glm::sin(theta) * glm::cos(phi),
                                            glm::sin(theta) * glm::sin(phi), glm::cos(theta));
        // tangent space to world
        glm::vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

        irradiance += glm::vec3(texture(u->u_cubeMap, sampleVec)) * glm::cos(theta) * glm::sin(theta);
        nrSamples++;
      }
    }
    irradiance = PI * irradiance * (1.0f / float(nrSamples));

    gl->FragColor = glm::vec4(irradiance, 1.0f);
  }
};

}
}
