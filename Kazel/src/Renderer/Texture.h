#pragma once
#include "Core/Core.h"
#include <cstdint>
#include <memory>
#include <string>

namespace Kazel {
class Texture {
 public:
  virtual ~Texture() {}

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  virtual void SetData(const void* dataC, uint32_t size) const = 0;
  virtual void Bind(uint32_t slot = 0) const = 0;
  virtual void UnBind() const = 0;
  virtual bool operator==(const Texture&) const = 0;

 private:
};

class Texture2D : public Texture {
 public:
  using EAxis = int;
  enum : EAxis { EWrap_S, EWrap_T };
  /**
   * 环绕方向
   * 方式              描述
   * GL_REPEAT	对纹理的默认行为。重复纹理图像。
   * GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
   * GL_CLAMP_TO_EDGE
   * 纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
   * GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色
   */
  using EWrapType = int;
  enum : EWrapType {
    EWrap_Repeat,
    EWrap_Mirror_Repeat,
    EWrap_Clap_To_Edge,
    EWrap_Clap_To_Border,
  };

  using EFilterType = int;  //过滤类型
  enum : EFilterType {
    EFilter_Min,  //缩小
    EFilter_Max   //放大
  };

  using EFilterOperator = int;
  enum : EFilterOperator  //过滤操作
  {
    EFilter_Nearest,  //临近过滤
    EFilter_Linear    //线性过滤
  };
  static Ref<Texture2D> Create(const std::string& path);
  static Ref<Texture2D> Create(uint32_t width, uint32_t height,
                               uint32_t channel = 3);

  //设置环绕方向
  virtual void SetWrapping(EAxis axis, EWrapType type,
                           const float* color = nullptr) = 0;
  //设置过滤
  virtual void SetFilter(EFilterType type, EFilterOperator op) = 0;
};
}  // namespace Kazel