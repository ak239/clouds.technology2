#include "stdafx.h"
#include "Object.h"
#include "Logger.h"

/// <summary>
/// Sets the context.
/// Если контекст внутри объекта уже был использован, то ошибка.
/// </summary>
/// <param name="context">The context.</param>
void Object::setContext(GLContext context)
{
	if (m_isUsed)
		LOG_ERROR("Try to change used context in object");
	else
		m_context = context;
}

const GLContext& Object::getContext() const
{
	m_isUsed = true;
	return m_context;
}
