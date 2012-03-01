#ifndef AL__XEMMAI__BUFFER_H
#define AL__XEMMAI__BUFFER_H

#include "device.h"

namespace al
{

namespace xemmai
{

class t_buffer
{
	friend class t_device;
	friend class t_source;

	t_device* v_device;
	std::map<ALuint, t_scoped>::iterator v_entry;

	t_buffer(t_device* a_device, std::map<ALuint, t_scoped>::iterator a_entry) : v_device(a_device), v_entry(a_entry)
	{
	}
	~t_buffer()
	{
		v_entry->second.f_pointer__(0);
		v_device->v_buffers.erase(v_entry);
	}

public:
	void f_delete()
	{
		alDeleteBuffers(1, &v_entry->first);
		t_error::f_check();
		delete this;
	}
	void f_data(ALenum a_format, const t_bytes& a_data, ALsizei a_size, ALsizei a_frequency)
	{
		if (a_size > static_cast<ALsizei>(a_data.f_size())) t_throwable::f_throw(L"out of range.");
		alBufferData(v_entry->first, a_format, &a_data[0], a_size, a_frequency);
		t_error::f_check();
	}
	void f_setf(ALenum a_parameter, ALfloat a_value)
	{
		alBufferf(v_entry->first, a_parameter, a_value);
		t_error::f_check();
	}
	void f_set3f(ALenum a_parameter, ALfloat a_value1, ALfloat a_value2, ALfloat a_value3)
	{
		alBuffer3f(v_entry->first, a_parameter, a_value1, a_value2, a_value3);
		t_error::f_check();
	}
	void f_seti(ALenum a_parameter, ALint a_value)
	{
		alBufferi(v_entry->first, a_parameter, a_value);
		t_error::f_check();
	}
	void f_set3i(ALenum a_parameter, ALint a_value1, ALint a_value2, ALint a_value3)
	{
		alBuffer3i(v_entry->first, a_parameter, a_value1, a_value2, a_value3);
		t_error::f_check();
	}
	ALfloat f_getf(ALenum a_parameter) const
	{
		ALfloat value;
		alGetBufferf(v_entry->first, a_parameter, &value);
		t_error::f_check();
		return value;
	}
	t_transfer f_get3f(ALenum a_parameter) const
	{
		ALfloat value1;
		ALfloat value2;
		ALfloat value3;
		alGetBuffer3f(v_entry->first, a_parameter, &value1, &value2, &value3);
		t_error::f_check();
		return f_tuple(t_transfer(value1), t_transfer(value2), t_transfer(value3));
	}
	ALint f_geti(ALenum a_parameter) const
	{
		ALint value;
		alGetBufferi(v_entry->first, a_parameter, &value);
		t_error::f_check();
		return value;
	}
	t_transfer f_get3i(ALenum a_parameter) const
	{
		ALint value1;
		ALint value2;
		ALint value3;
		alGetBuffer3i(v_entry->first, a_parameter, &value1, &value2, &value3);
		t_error::f_check();
		return f_tuple(t_transfer(value1), t_transfer(value2), t_transfer(value3));
	}
};

}

}

namespace xemmai
{

using al::xemmai::t_buffer;

template<>
struct t_type_of<t_buffer> : t_type
{
#include "cast.h"
	typedef al::xemmai::t_extension t_extension;

	static void f_define(t_extension* a_extension);

	t_type_of(const t_transfer& a_module, const t_transfer& a_super) : t_type(a_module, a_super)
	{
	}
	virtual t_type* f_derive(t_object* a_this);
	virtual void f_finalize(t_object* a_this);
	virtual void f_instantiate(t_object* a_class, t_slot* a_stack, size_t a_n);
};

}

#endif
