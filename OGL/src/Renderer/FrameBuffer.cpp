#include "glad/glad.h"
#include "FrameBuffer.h"



namespace Logl
{
	FrameBuffer::FrameBuffer()
		: m_DefaultColorAttachmentId(0), m_DefaultRBOId(0)
	{
		glGenFramebuffers(1, &m_Id);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_Id);
		glDeleteTextures(1, &m_DefaultColorAttachmentId);
		glDeleteRenderbuffers(1, &m_DefaultRBOId);
	}

	void FrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
	}

	void FrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::InitDefaultAttachment(int width, int height)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &m_DefaultColorAttachmentId);
		glBindTexture(GL_TEXTURE_2D, m_DefaultColorAttachmentId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_DefaultColorAttachmentId, 0);

		glGenRenderbuffers(1, &m_DefaultRBOId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DefaultRBOId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DefaultRBOId);
	}

	bool FrameBuffer::IsComplete() const
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
}