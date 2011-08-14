#ifndef AL__XEMMAI__DEVICE_H
#define AL__XEMMAI__DEVICE_H

#include "al.h"

namespace al
{

namespace xemmai
{

class t_device
{
	friend class t_context;
	friend class t_source;
	friend class t_buffer;

	std::map<ALCdevice*, t_scoped>::iterator v_entry;
	ALCcontext* v_default;
	std::map<ALCcontext*, t_scoped> v_contexts;
	std::map<ALuint, t_scoped> v_buffers;

	t_device(std::map<ALCdevice*, t_scoped>::iterator a_entry, ALCcontext* a_default) : v_entry(a_entry), v_default(a_default)
	{
		alcMakeContextCurrent(v_default);
	}
	~t_device()
	{
		v_entry->second.f_pointer__(0);
		t_session* session = t_session::f_instance();
		session->v_devices.erase(v_entry);
	}
	void f_check_error() const;
	t_transfer f_create_buffer(ALuint a_id);

public:
	static t_transfer f_construct(t_object* a_class, const std::wstring* a_name)
	{
		t_session* session = t_session::f_instance();
		ALCdevice* device = alcOpenDevice(a_name ? f_convert(*a_name).c_str() : NULL);
		if (device == NULL) t_throwable::f_throw(L"alcOpenDevice failed.");
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL) {
			alcCloseDevice(device);
			t_throwable::f_throw(L"alcCreateContext failed.");
		}
		t_transfer object = t_object::f_allocate(a_class);
		object.f_pointer__(new t_device(session->v_devices.insert(std::make_pair(device, static_cast<t_object*>(object))).first, context));
		return object;
	}

	void f_close();
	t_transfer f_create_context();
	std::wstring f_get_string(ALCenum a_parameter) const
	{
		const ALCchar* p = alcGetString(v_entry->first, a_parameter);
		f_check_error();
		return f_convert(std::string(p));
	}
	ALCint f_get_integer(ALCenum a_parameter) const
	{
		ALCint data;
		alcGetIntegerv(v_entry->first, a_parameter, 1, &data);
		f_check_error();
		return data;
	}
	t_transfer f_create_buffer()
	{
		alcMakeContextCurrent(v_default);
		ALuint id;
		alGenBuffers(1, &id);
		al::xemmai::f_check_error();
		return f_create_buffer(id);
	}
	t_transfer f_create_buffer_from_file(const std::wstring& a_path)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferFromFile(f_convert(a_path).c_str());
		if (id == AL_NONE) f_alut_throw_error();
		return f_create_buffer(id);
	}
	t_transfer f_create_buffer_from_file_image(const t_bytes& a_data)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferFromFileImage(&a_data[0], a_data.f_size());
		if (id == AL_NONE) f_alut_throw_error();
		return f_create_buffer(id);
	}
	t_transfer f_create_buffer_hello_world()
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferHelloWorld();
		if (id == AL_NONE) f_alut_throw_error();
		return f_create_buffer(id);
	}
	t_transfer f_create_buffer_waveform(ALenum a_shape, ALfloat a_frequency, ALfloat a_phase, ALfloat a_duration)
	{
		alcMakeContextCurrent(v_default);
		ALuint id = alutCreateBufferWaveform(a_shape, a_frequency, a_phase, a_duration);
		if (id == AL_NONE) f_alut_throw_error();
		return f_create_buffer(id);
	}
	std::wstring f_get_mime_types(ALenum a_loader) const
	{
		alcMakeContextCurrent(v_default);
		const char* types = alutGetMIMETypes(a_loader);
		if (types == NULL) f_alut_throw_error();
		return f_convert(std::string(types));
	}
};

}

}

namespace xemmai
{

using al::xemmai::t_device;

template<>
struct t_type_of<t_device> : t_type
{
#include "cast.h"
	typedef al::xemmai::t_extension t_extension;

	static void f_define(t_extension* a_extension);

	t_type_of(const t_transfer& a_module, const t_transfer& a_super) : t_type(a_module, a_super)
	{
	}
	virtual t_type* f_derive(t_object* a_this);
	virtual void f_finalize(t_object* a_this);
	virtual void f_construct(t_object* a_class, t_slot* a_stack, size_t a_n);
	virtual void f_instantiate(t_object* a_class, t_slot* a_stack, size_t a_n);
};

}

#endif
