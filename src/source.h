#ifndef XEMMAIX__AL__SOURCE_H
#define XEMMAIX__AL__SOURCE_H

#include "context.h"
#include "buffer.h"

namespace xemmaix
{

namespace al
{

class t_source
{
	friend class t_context;

	t_context* v_context;
	std::map<ALuint, t_scoped>::iterator v_entry;

	t_source(t_context* a_context, std::map<ALuint, t_scoped>::iterator a_entry) : v_context(a_context), v_entry(a_entry)
	{
	}
	~t_source()
	{
		v_entry->second.f_pointer__(nullptr);
		v_context->v_sources.erase(v_entry);
	}

public:
	void f_delete()
	{
		v_context->f_make_current();
		alDeleteSources(1, &v_entry->first);
		t_error::f_check();
		delete this;
	}
	void f_setf(ALenum a_parameter, ALfloat a_value)
	{
		v_context->f_make_current();
		alSourcef(v_entry->first, a_parameter, a_value);
		t_error::f_check();
	}
	void f_set3f(ALenum a_parameter, ALfloat a_value1, ALfloat a_value2, ALfloat a_value3)
	{
		v_context->f_make_current();
		alSource3f(v_entry->first, a_parameter, a_value1, a_value2, a_value3);
		t_error::f_check();
	}
	void f_seti(ALenum a_parameter, ALint a_value)
	{
		v_context->f_make_current();
		alSourcei(v_entry->first, a_parameter, a_value);
		t_error::f_check();
	}
	void f_set3i(ALenum a_parameter, ALint a_value1, ALint a_value2, ALint a_value3)
	{
		v_context->f_make_current();
		alSource3i(v_entry->first, a_parameter, a_value1, a_value2, a_value3);
		t_error::f_check();
	}
	void f_setb(ALenum a_parameter, bool a_value)
	{
		v_context->f_make_current();
		alSourcei(v_entry->first, a_parameter, a_value ? AL_TRUE : AL_FALSE);
		t_error::f_check();
	}
	ALfloat f_getf(ALenum a_parameter) const
	{
		v_context->f_make_current();
		ALfloat value;
		alGetSourcef(v_entry->first, a_parameter, &value);
		t_error::f_check();
		return value;
	}
	t_scoped f_get3f(ALenum a_parameter) const
	{
		v_context->f_make_current();
		ALfloat value1;
		ALfloat value2;
		ALfloat value3;
		alGetSource3f(v_entry->first, a_parameter, &value1, &value2, &value3);
		t_error::f_check();
		return f_tuple(t_scoped(value1), t_scoped(value2), t_scoped(value3));
	}
	ALint f_geti(ALenum a_parameter) const
	{
		v_context->f_make_current();
		ALint value;
		alGetSourcei(v_entry->first, a_parameter, &value);
		t_error::f_check();
		return value;
	}
	t_scoped f_get3i(ALenum a_parameter) const
	{
		v_context->f_make_current();
		ALint value1;
		ALint value2;
		ALint value3;
		alGetSource3i(v_entry->first, a_parameter, &value1, &value2, &value3);
		t_error::f_check();
		return f_tuple(t_scoped(value1), t_scoped(value2), t_scoped(value3));
	}
	bool f_getb(ALenum a_parameter) const
	{
		v_context->f_make_current();
		ALint value;
		alGetSourcei(v_entry->first, a_parameter, &value);
		t_error::f_check();
		return value != AL_FALSE;
	}
	void f_play()
	{
		v_context->f_make_current();
		alSourcePlay(v_entry->first);
		t_error::f_check();
	}
	void f_stop()
	{
		v_context->f_make_current();
		alSourceStop(v_entry->first);
		t_error::f_check();
	}
	void f_rewind()
	{
		v_context->f_make_current();
		alSourceRewind(v_entry->first);
		t_error::f_check();
	}
	void f_pause()
	{
		v_context->f_make_current();
		alSourcePause(v_entry->first);
		t_error::f_check();
	}
	void f_set_buffer(t_buffer* a_buffer)
	{
		v_context->f_make_current();
		alSourcei(v_entry->first, AL_BUFFER, a_buffer ? a_buffer->v_entry->first : AL_NONE);
		t_error::f_check();
	}
	t_scoped f_get_buffer() const
	{
		v_context->f_make_current();
		ALint id;
		alGetSourcei(v_entry->first, AL_BUFFER, &id);
		t_error::f_check();
		return v_context->v_device->v_buffers.find(id)->second;
	}
	void f_queue_buffer(t_buffer& a_buffer)
	{
		v_context->f_make_current();
		alSourceQueueBuffers(v_entry->first, 1, &a_buffer.v_entry->first);
		t_error::f_check();
	}
	t_scoped f_unqueue_buffer()
	{
		v_context->f_make_current();
		ALuint id;
		alSourceUnqueueBuffers(v_entry->first, 1, &id);
		t_error::f_check();
		return v_context->v_device->v_buffers.find(id)->second;
	}
};

}

}

namespace xemmai
{

template<>
struct t_type_of<xemmaix::al::t_source> : t_type
{
#include "cast.h"
	typedef xemmaix::al::t_extension t_extension;

	static void f_define(t_extension* a_extension);

	using t_type::t_type;
	virtual t_type* f_derive(t_object* a_this);
	virtual void f_finalize(t_object* a_this);
	virtual void f_instantiate(t_object* a_class, t_stacked* a_stack, size_t a_n);
};

}

template<void (*A_function)(ALsizei, const ALuint*)>
void xemmaix::al::t_context::f_source_do(t_object* a_module, t_stacked* a_stack, size_t a_n)
{
	t_destruct_n destruct(a_stack, a_n);
	f_as<t_context&>(a_stack[1]).f_make_current();
	std::vector<ALuint> ids(a_n);
	for (size_t i = 0; i < a_n; ++i) ids[i] = f_as<t_source&>(a_stack[i + 2]).v_entry->first;
	A_function(a_n, &ids[0]);
	t_error::f_check();
}

#endif
