#pragma once

namespace Logl
{
	enum class AttachType
	{
		ColorTexture,
		DepthTexture,
		StencilTexture,
		Depth_Stencil
	};

	struct ColorTextureAttachment
	{

	};

	struct RenderBufferAttachment
	{
		GLenum internalformat;
	};

	struct FramebufferSpecification
	{
		int width;
		int height;

		// attachments
	};


	class FrameBuffer
	{
	public:
		FrameBuffer();
		FrameBuffer(const FrameBuffer& fbo) = delete;
		~FrameBuffer();

		FrameBuffer& operator=(const FrameBuffer& fbo) = delete;

		void Bind() const;
		void Unbind() const;

		void InitDefaultAttachment(int width, int height);
		unsigned int GetDefalutColorAttachmentId() const { return m_DefaultColorAttachmentId; }

		bool IsComplete() const;

	private:
		unsigned int m_Id;
		unsigned int m_DefaultColorAttachmentId;
		unsigned int m_DefaultRBOId;
	};
}