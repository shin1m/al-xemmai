#ifndef XEMMAIX__AL__DEVICE_H
#define XEMMAIX__AL__DEVICE_H

#include "error.h"

namespace xemmaix::al
{

class t_base_device
{
	friend class t_holds<t_base_device>;

protected:
	std::map<ALCdevice*, t_root>::iterator v_entry;

	t_base_device(std::map<ALCdevice*, t_root>::iterator a_entry) : v_entry(a_entry)
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
	friend class t_type_of<t_object>;

	ALCcontext* v_default;
	std::map<ALCcontext*, t_root> v_contexts;
	std::map<ALuint, t_root> v_buffers;

	t_device(t_session* a_session, ALCdevice* a_device, ALCcontext* a_default);
	~t_device() = default;
	t_pvalue f_create_buffer(ALuint a_id);

public:
	static t_pvalue f_construct(t_type* a_class, const t_string* a_name)
	{
		auto session = t_session::f_instance();
		ALCdevice* device = alcOpenDevice(a_name ? f_convert(*a_name).c_str() : NULL);
		if (device == NULL) xemmai::f_throw(L"alcOpenDevice failed."sv);
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL) {
			alcCloseDevice(device);
			xemmai::f_throw(L"alcCreateContext failed."sv);
		}
		return a_class->f_new<t_device>(false, session, device, context);
	}

	void f_close();
	t_pvalue f_default_context() const
	{
		return static_cast<t_object*>(v_contexts.find(v_default)->second);
	}
	t_pvalue f_create_context();
	t_pvalue f_create_buffer()
	{
		alcMakeContextCurrent(v_default);
		ALuint id;
		alGenBuffers(1, &id);
		t_error::f_check();
		return f_create_buffer(id);
	}
	t_pvalue f_create_buffer_from_file(std::wstring_view a_path)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferFromFile(f_convert(a_path).c_str());
		if (id == AL_NONE) t_alut_error::f_throw();
		return f_create_buffer(id);
	}
	t_pvalue f_create_buffer_from_file_image(const t_bytes& a_data)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferFromFileImage(&a_data[0], a_data.f_size());
		if (id == AL_NONE) t_alut_error::f_throw();
		return f_create_buffer(id);
	}
	t_pvalue f_create_buffer_hello_world()
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferHelloWorld();
		if (id == AL_NONE) t_alut_error::f_throw();
		return f_create_buffer(id);
	}
	t_pvalue f_create_buffer_waveform(ALenum a_shape, ALfloat a_frequency, ALfloat a_phase, ALfloat a_duration)
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
	friend class t_type_of<t_object>;

	t_capture_device(t_session* a_session, ALCdevice* a_device) : t_base_device(a_session->v_capture_devices.emplace(a_device, t_object::f_of(this)).first)
	{
	}
	~t_capture_device() = default;

public:
	static t_pvalue f_construct(t_type* a_class, const t_string* a_name, ALCuint a_frequency, ALCenum a_format, ALCsizei a_buffer)
	{
		auto session = t_session::f_instance();
		ALCdevice* device = alcCaptureOpenDevice(a_name ? f_convert(*a_name).c_str() : NULL, a_frequency, a_format, a_buffer);
		if (device == NULL) xemmai::f_throw(L"alcCaptureOpenDevice failed."sv);
		return a_class->f_new<t_capture_device>(false, session, device);
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
	t_pvalue f_do_construct(t_pvalue* a_stack, size_t a_n);
	void f_do_instantiate(t_pvalue* a_stack, size_t a_n);
};

template<>
struct t_type_of<xemmaix::al::t_device> : t_bears<xemmaix::al::t_device, t_type_of<xemmaix::al::t_base_device>>
{
	static void f_define(t_extension* a_extension);

	using t_base::t_base;
	t_pvalue f_do_construct(t_pvalue* a_stack, size_t a_n);
};

template<>
struct t_type_of<xemmaix::al::t_capture_device> : t_bears<xemmaix::al::t_capture_device, t_type_of<xemmaix::al::t_base_device>>
{
	static void f_define(t_extension* a_extension);

	using t_base::t_base;
	t_pvalue f_do_construct(t_pvalue* a_stack, size_t a_n);
};

}

#endif
