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
   * ���Ʒ���
   * ��ʽ              ����
   * GL_REPEAT	�������Ĭ����Ϊ���ظ�����ͼ��
   * GL_MIRRORED_REPEAT	��GL_REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
   * GL_CLAMP_TO_EDGE
   * ��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
   * GL_CLAMP_TO_BORDER	����������Ϊ�û�ָ���ı�Ե��ɫ
   */
  using EWrapType = int;
  enum : EWrapType {
    EWrap_Repeat,
    EWrap_Mirror_Repeat,
    EWrap_Clap_To_Edge,
    EWrap_Clap_To_Border,
  };

  using EFilterType = int;  //��������
  enum : EFilterType {
    EFilter_Min,  //��С
    EFilter_Max   //�Ŵ�
  };

  using EFilterOperator = int;
  enum : EFilterOperator  //���˲���
  {
    EFilter_Nearest,  //�ٽ�����
    EFilter_Linear    //���Թ���
  };
  static Ref<Texture2D> Create(const std::string& path);
  static Ref<Texture2D> Create(uint32_t width, uint32_t height,
                               uint32_t channel = 3);

  //���û��Ʒ���
  virtual void SetWrapping(EAxis axis, EWrapType type,
                           const float* color = nullptr) = 0;
  //���ù���
  virtual void SetFilter(EFilterType type, EFilterOperator op) = 0;
};
}  // namespace Kazel