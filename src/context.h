#ifndef XEMMAIX__AL__CONTEXT_H
#define XEMMAIX__AL__CONTEXT_H

#include "device.h"

namespace xemmaix::al
{

class t_context
{
	friend class t_device;
	friend class t_source;

	t_device* v_device;
	std::map<ALCcontext*, t_scoped>::iterator v_entry;
	std::map<ALuint, t_scoped> v_sources;

	t_context(t_device* a_device, std::map<ALCcontext*, t_scoped>::iterator a_entry) : v_device(a_device), v_entry(a_entry)
	{
	}
	~t_context();
	void f_make_current() const
	{
		alcMakeContextCurrent(v_entry->first);
	}

public:
	void f_destroy()
	{
		if (v_entry->first == v_device->v_default) xemmai::f_throw(L"default context can not be destroyed."sv);
		alcMakeContextCurrent(v_device->v_default);
		alcDestroyContext(v_entry->first);
		v_device->f_check_error();
		delete this;
	}
	void f_process()
	{
		alcProcessContext(v_entry->first);
		v_device->f_check_error();
	}
	void f_suspend()
	{
		alcSuspendContext(v_entry->first);
		v_device->f_check_error();
	}
	t_scoped f_get_device() const
	{
		return v_device->v_entry->second;
	}
	void f_enable(ALenum a_capability)
	{
		f_make_current();
		alEnable(a_capability);
		t_error::f_check();
	}
	void f_disable(ALenum a_capability)
	{
		f_make_current();
		alDisable(a_capability);
		t_error::f_check();
	}
	bool f_is_enabled(ALenum a_capability) const
	{
		f_make_current();
		ALboolean b = alIsEnabled(a_capability);
		t_error::f_check();
		return b != AL_FALSE;
	}
	std::wstring f_get_string(ALenum a_parameter) const
	{
		f_make_current();
		const ALchar* p = alGetString(a_parameter);
		t_error::f_check();
		return f_convert(p);
	}
	t_scoped f_get_booleanv(ALenum a_parameter) const
	{
		f_make_current();
		ALboolean data[3];
		alGetBooleanv(a_parameter, data);
		t_error::f_check();
		return f_tuple(data[0] != AL_FALSE, data[1] != AL_FALSE, data[2] != AL_FALSE);
	}
	t_scoped f_get_integerv(ALenum a_parameter) const
	{
		f_make_current();
		ALint data[3];
		alGetIntegerv(a_parameter, data);
		t_error::f_check();
		return f_tuple(data[0], data[1], data[2]);
	}
	t_scoped f_get_floatv(ALenum a_parameter) const
	{
		f_make_current();
		ALfloat data[3];
		alGetFloatv(a_parameter, data);
		t_error::f_check();
		return f_tuple(data[0], data[1], data[2]);
	}
	t_scoped f_get_doublev(ALenum a_parameter) const
	{
		f_make_current();
		ALdouble data[3];
		alGetDoublev(a_parameter, data);
		t_error::f_check();
		return f_tuple(data[0], data[1], data[2]);
	}
	bool f_get_boolean(ALenum a_parameter) const
	{
		f_make_current();
		ALboolean b = alGetBoolean(a_parameter);
		t_error::f_check();
		return b != AL_FALSE;
	}
	ALint f_get_integer(ALenum a_parameter) const
	{
		f_make_current();
		ALint i = alGetInteger(a_parameter);
		t_error::f_check();
		return i;
	}
	ALfloat f_get_float(ALenum a_parameter) const
	{
		f_make_current();
		ALfloat f = alGetFloat(a_parameter);
		t_error::f_check();
		return f;
	}
	ALdouble f_get_double(ALenum a_parameter) const
	{
		f_make_current();
		ALdouble d = alGetDouble(a_parameter);
		t_error::f_check();
		return d;
	}
	void f_listenerf(ALenum a_parameter, ALfloat a_value)
	{
		f_make_current();
		alListenerf(a_parameter, a_value);
		t_error::f_check();
	}
	void f_listener3f(ALenum a_parameter, ALfloat a_value1, ALfloat a_value2, ALfloat a_value3)
	{
		f_make_current();
		alListener3f(a_parameter, a_value1, a_value2, a_value3);
		t_error::f_check();
	}
	void f_listener6f(ALenum a_parameter, ALfloat a_value1, ALfloat a_value2, ALfloat a_value3, ALfloat a_value4, ALfloat a_value5, ALfloat a_value6)
	{
		f_make_current();
		ALfloat values[] = {
			a_value1, a_value2, a_value3,
			a_value4, a_value5, a_value6
		};
		alListenerfv(a_parameter, values);
		t_error::f_check();
	}
	void f_listeneri(ALenum a_parameter, ALint a_value)
	{
		f_make_current();
		alListeneri(a_parameter, a_value);
		t_error::f_check();
	}
	void f_listener3i(ALenum a_parameter, ALint a_value1, ALint a_value2, ALint a_value3)
	{
		f_make_current();
		alListener3i(a_parameter, a_value1, a_value2, a_value3);
		t_error::f_check();
	}
	void f_listener6i(ALenum a_parameter, ALint a_value1, ALint a_value2, ALint a_value3, ALint a_value4, ALint a_value5, ALint a_value6)
	{
		f_make_current();
		ALint values[] = {
			a_value1, a_value2, a_value3,
			a_value4, a_value5, a_value6
		};
		alListeneriv(a_parameter, values);
		t_error::f_check();
	}
	ALfloat f_get_listenerf(ALenum a_parameter) const
	{
		f_make_current();
		ALfloat value;
		alGetListenerf(a_parameter, &value);
		t_error::f_check();
		return value;
	}
	t_scoped f_get_listener3f(ALenum a_parameter) const
	{
		f_make_current();
		ALfloat value1;
		ALfloat value2;
		ALfloat value3;
		alGetListener3f(a_parameter, &value1, &value2, &value3);
		t_error::f_check();
		return f_tuple(value1, value2, value3);
	}
	t_scoped f_get_listenerfv(ALenum a_parameter) const
	{
		f_make_current();
		ALfloat values[6];
		alGetListenerfv(a_parameter, values);
		t_error::f_check();
		return f_tuple(values[0], values[1], values[2], values[3], values[4], values[5]);
	}
	ALint f_get_listeneri(ALenum a_parameter) const
	{
		f_make_current();
		ALint value;
		alGetListeneri(a_parameter, &value);
		t_error::f_check();
		return value;
	}
	t_scoped f_get_listener3i(ALenum a_parameter) const
	{
		f_make_current();
		ALint value1;
		ALint value2;
		ALint value3;
		alGetListener3i(a_parameter, &value1, &value2, &value3);
		t_error::f_check();
		return f_tuple(value1, value2, value3);
	}
	t_scoped f_get_listeneriv(ALenum a_parameter) const
	{
		f_make_current();
		ALint values[6];
		alGetListeneriv(a_parameter, values);
		t_error::f_check();
		return f_tuple(values[0], values[1], values[2], values[3], values[4], values[5]);
	}
	t_scoped f_create_source();
	template<void (*A_function)(ALsizei, const ALuint*)>
	static void f_source_do(xemmai::t_extension* a_extension, t_stacked* a_stack, size_t a_n);
	void f_doppler_factor(ALfloat a_value)
	{
		f_make_current();
		alDopplerFactor(a_value);
		t_error::f_check();
	}
	void f_doppler_velocity(ALfloat a_value)
	{
		f_make_current();
		alDopplerVelocity(a_value);
		t_error::f_check();
	}
	void f_speed_of_sound(ALfloat a_value)
	{
		f_make_current();
		alSpeedOfSound(a_value);
		t_error::f_check();
	}
	void f_distance_model(ALenum a_model)
	{
		f_make_current();
		alDistanceModel(a_model);
		t_error::f_check();
	}
};

}

namespace xemmai
{

template<>
struct t_type_of<xemmaix::al::t_context> : t_uninstantiatable<xemmaix::al::t_holds<xemmaix::al::t_context>>
{
	typedef xemmaix::al::t_context t_context;

	static void f_define(t_extension* a_extension);

	using t_base::t_base;
};

}

#endif
