#ifndef XEMMAIX__AL__DEVICE_H
#define XEMMAIX__AL__DEVICE_H

#include "error.h"

namespace xemmaix::al
{

class t_base_device
{
protected:
	std::map<ALCdevice*, t_scoped>::iterator v_entry;

	t_base_device(std::map<ALCdevice*, t_scoped>::iterator a_entry) : v_entry(a_entry)
	{
	}
	void f_check_error() const;

public:
	std::wstring f_get_string(ALCenum a_parameter) const
	{
		const ALCchar* p = alcGetString(v_entry->first, a_parameter);
		f_check_error();
		return f_convert(p);
	}
	ALCint f_get_integer(ALCenum a_parameter) const
	{
		ALCint data;
		alcGetIntegerv(v_entry->first, a_parameter, 1, &data);
		f_check_error();
		return data;
	}
};

class t_device : public t_base_device
{
	friend class t_context;
	friend class t_source;
	friend class t_buffer;

	ALCcontext* v_default;
	std::map<ALCcontext*, t_scoped> v_contexts;
	std::map<ALuint, t_scoped> v_buffers;

	t_device(std::map<ALCdevice*, t_scoped>::iterator a_entry, ALCcontext* a_default);
	~t_device()
	{
		v_entry->second.f_pointer__(nullptr);
		t_session* session = t_session::f_instance();
		session->v_devices.erase(v_entry);
	}
	t_scoped f_create_buffer(ALuint a_id);

public:
	static t_scoped f_construct(t_type* a_class, const t_string* a_name)
	{
		t_session* session = t_session::f_instance();
		ALCdevice* device = alcOpenDevice(a_name ? f_convert(*a_name).c_str() : NULL);
		if (device == NULL) xemmai::f_throw(L"alcOpenDevice failed."sv);
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL) {
			alcCloseDevice(device);
			xemmai::f_throw(L"alcCreateContext failed."sv);
		}
		t_scoped object = t_object::f_allocate(a_class, false);
		object.f_pointer__(new t_device(session->v_devices.insert(std::make_pair(device, static_cast<t_object*>(object))).first, context));
		return object;
	}

	void f_close();
	t_scoped f_default_context() const
	{
		return v_contexts.find(v_default)->second;
	}
	t_scoped f_create_context();
	t_scoped f_create_buffer()
	{
		alcMakeContextCurrent(v_default);
		ALuint id;
		alGenBuffers(1, &id);
		t_error::f_check();
		return f_create_buffer(id);
	}
	t_scoped f_create_buffer_from_file(std::wstring_view a_path)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferFromFile(f_convert(a_path).c_str());
		if (id == AL_NONE) t_alut_error::f_throw();
		return f_create_buffer(id);
	}
	t_scoped f_create_buffer_from_file_image(const t_bytes& a_data)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferFromFileImage(&a_data[0], a_data.f_size());
		if (id == AL_NONE) t_alut_error::f_throw();
		return f_create_buffer(id);
	}
	t_scoped f_create_buffer_hello_world()
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferHelloWorld();
		if (id == AL_NONE) t_alut_error::f_throw();
		return f_create_buffer(id);
	}
	t_scoped f_create_buffer_waveform(ALenum a_shape, ALfloat a_frequency, ALfloat a_phase, ALfloat a_duration)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferWaveform(a_shape, a_frequency, a_phase, a_duration);
		if (id == AL_NONE) t_alut_error::f_throw();
		return f_create_buffer(id);
	}
	std::wstring f_get_mime_types(ALenum a_loader) const
	{
		alcMakeContextCurrent(v_default);
		const char* types = alutGetMIMETypes(a_loader);
		if (types == NULL) t_alut_error::f_throw();
		return f_convert(types);
	}
};

class t_capture_device : public t_base_device
{
	t_capture_device(std::map<ALCdevice*, t_scoped>::iterator a_entry) : t_base_device(a_entry)
	{
	}
	~t_capture_device()
	{
		v_entry->second.f_pointer__(nullptr);
		t_session* session = t_session::f_instance();
		session->v_capture_devices.erase(v_entry);
	}

public:
	static t_scoped f_construct(t_type* a_class, const t_string* a_name, ALCuint a_frequency, ALCenum a_format, ALCsizei a_buffer)
	{
		t_session* session = t_session::f_instance();
		ALCdevice* device = alcCaptureOpenDevice(a_name ? f_convert(*a_name).c_str() : NULL, a_frequency, a_format, a_buffer);
		if (device == NULL) xemmai::f_throw(L"alcCaptureOpenDevice failed."sv);
		t_scoped object = t_object::f_allocate(a_class, false);
		object.f_pointer__(new t_capture_device(session->v_capture_devices.insert(std::make_pair(device, static_cast<t_object*>(object))).first));
		return object;
	}

	void f_close();
	void f_start()
	{
		alcCaptureStart(v_entry->first);
		f_check_error();
	}
	void f_stop()
	{
		alcCaptureStop(v_entry->first);
		f_check_error();
	}
	void f_samples(t_bytes& a_buffer, ALCsizei a_samples)
	{
		if (static_cast<ALCsizei>(a_buffer.f_size()) < a_samples) xemmai::f_throw(L"not enough buffer."sv);
		alcCaptureSamples(v_entry->first, &a_buffer[0], a_samples);
		f_check_error();
	}
};

}

namespace xemmai
{

template<>
struct t_type_of<xemmaix::al::t_base_device> : xemmaix::al::t_holds<xemmaix::al::t_base_device>
{
	static void f_define(t_extension* a_extension);

	using t_base::t_base;
	t_scoped f_do_construct(t_stacked* a_stack, size_t a_n);
	void f_do_instantiate(t_stacked* a_stack, size_t a_n);
};

template<>
struct t_type_of<xemmaix::al::t_device> : t_bears<xemmaix::al::t_device, t_type_of<xemmaix::al::t_base_device>>
{
	static void f_define(t_extension* a_extension);

	using t_base::t_base;
	t_scoped f_do_construct(t_stacked* a_stack, size_t a_n);
};

template<>
struct t_type_of<xemmaix::al::t_capture_device> : t_bears<xemmaix::al::t_capture_device, t_type_of<xemmaix::al::t_base_device>>
{
	static void f_define(t_extension* a_extension);

	using t_base::t_base;
	t_scoped f_do_construct(t_stacked* a_stack, size_t a_n);
};

}

#endif
