/*
 * SoftGLRender
 * @author 	: keith@robot9.me
 *
 */

#pragma once

#include "renderer_base.h"
#include "base/thread_pool.h"
#include "render/renderer.h"
#include "render/soft/vertex_soft.h"
#include "render/soft/framebuffer_soft.h"
#include "render/bounding_box.h"

namespace SoftGL {

class RendererSoft : public Renderer {
 public:
  RendererSoft();

  // config
  bool ReverseZ() const override {
    return false;
  }

  // framebuffer
  std::shared_ptr<FrameBuffer> CreateFrameBuffer() override;

  // texture
  std::shared_ptr<Texture2D> CreateTexture2D() override;
  std::shared_ptr<TextureCube> CreateTextureCube() override;
  std::shared_ptr<TextureDepth> CreateTextureDepth() override;

  // vertex
  std::shared_ptr<VertexArrayObject> CreateVertexArrayObject(const VertexArray &vertex_array) override;

  // shader program
  std::shared_ptr<ShaderProgram> CreateShaderProgram() override;

  // uniform
  std::shared_ptr<UniformBlock> CreateUniformBlock(const std::string &name, int size) override;
  std::shared_ptr<UniformSampler> CreateUniformSampler(const std::string &name, TextureType type) override;

  // pipeline
  void SetFrameBuffer(FrameBuffer &frame_buffer) override;
  void SetViewPort(int x, int y, int width, int height) override;
  void Clear(const ClearState &state) override;
  void SetRenderState(const RenderState &state) override;
  void SetVertexArrayObject(std::shared_ptr<VertexArrayObject> &vao) override;
  void SetShaderProgram(std::shared_ptr<ShaderProgram> &program) override;
  void SetShaderUniforms(std::shared_ptr<ShaderUniforms> &uniforms) override;
  void Draw(PrimitiveType type) override;

 private:
  void ProcessVertexShader();
  void ProcessPrimitiveAssembly();
  void ProcessClipping();
  void ProcessPerspectiveDivide();
  void ProcessViewportTransform();
  void ProcessFaceCulling();
  void ProcessRasterization();
  void ProcessFragmentShader(glm::vec4 &screen_pos, bool front_facing, void *varyings, ShaderProgramSoft *shader);
  void ProcessPerSampleOperations(int x, int y, ShaderBuiltin &builtin);
  bool ProcessDepthTest(int x, int y, float depth);
  void ProcessColorBlending(int x, int y, glm::vec4 &color);

  void ProcessPointAssembly();
  void ProcessLineAssembly();
  void ProcessPolygonAssembly();

  void ClippingPoint(PrimitiveHolder &point);
  void ClippingLine(PrimitiveHolder &line);
  void ClippingTriangle(PrimitiveHolder &triangle);

  void InterpolateVertex(VertexHolder &out, VertexHolder &v0, VertexHolder &v1, float t);
  void InterpolateLinear(float *varyings_out, const float *varyings_in[2], size_t elem_cnt, float t);
  void InterpolateBarycentric(float *varyings_out, const float *varyings_in[3], size_t elem_cnt, glm::aligned_vec4 &bc);

  void RasterizationPoint(VertexHolder *v, float point_size);
  void RasterizationLine(VertexHolder *v0, VertexHolder *v1, float line_width);
  void RasterizationTriangle(VertexHolder *v0, VertexHolder *v1, VertexHolder *v2, bool front_facing);
  void RasterizationPolygons(std::vector<PrimitiveHolder> &primitives);
  void RasterizationPixelQuad(PixelQuadContext &quad);

 private:
  inline glm::u8vec4 GetFrameColor(int x, int y);
  inline void SetFrameColor(int x, int y, const glm::u8vec4 &color);

  void VertexShaderImpl(VertexHolder &vertex);
  int CountFrustumClipMask(glm::vec4 &clip_pos);
  BoundingBox TriangleBoundingBox(glm::vec4 *vert, float width, float height);

  bool Barycentric(glm::aligned_vec4 *vert, glm::aligned_vec4 &v0, glm::aligned_vec4 &p, glm::aligned_vec4 &bc);
  void BarycentricCorrect(PixelQuadContext &quad);

 private:
  Viewport viewport_{};
  PrimitiveType primitive_type_ = Primitive_TRIANGLE;
  FrameBufferSoft *fbo_ = nullptr;
  const RenderState *render_state_ = nullptr;
  VertexArrayObjectSoft *vao_ = nullptr;
  ShaderProgramSoft *shader_program_ = nullptr;

  std::shared_ptr<BufferRGBA> fbo_color_ = nullptr;
  std::shared_ptr<BufferDepth> fbo_depth_ = nullptr;

  std::vector<VertexHolder> vertexes_;
  std::vector<PrimitiveHolder> primitives_;

  std::shared_ptr<float> varyings_ = nullptr;
  size_t varyings_cnt_ = 0;
  size_t varyings_aligned_cnt_ = 0;
  size_t varyings_aligned_size_ = 0;

  int raster_block_size_ = 32;
  ThreadPool thread_pool_;
  std::vector<PixelQuadContext> thread_quad_ctx_;
};

}
