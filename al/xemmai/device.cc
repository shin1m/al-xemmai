#include "context.h"
#include "buffer.h"

#include <cassert>

namespace al
{

namespace xemmai
{

void t_device::f_check_error() const
{
	ALCenum error = alcGetError(v_entry->first);
	if (error != ALC_NO_ERROR) f_throw<t_alc_error>(alcGetString(v_entry->first, error), error);
}

t_transfer t_device::f_create_buffer(ALuint a_id)
{
	t_session* session = t_session::f_instance();
	t_transfer object = t_object::f_allocate(session->v_extension->f_type<t_buffer>());
	object.f_pointer__(new t_buffer(this, v_buffers.insert(std::make_pair(a_id, static_cast<t_object*>(object))).first));
	return object;
}

void t_device::f_close()
{
	alcMakeContextCurrent(v_default);
	while (!v_contexts.empty()) f_as<t_context&>(v_contexts.begin()->second).f_destroy();
	while (!v_buffers.empty()) f_as<t_buffer&>(v_buffers.begin()->second).f_delete();
	alcMakeContextCurrent(NULL);
	f_check_error();
	alcDestroyContext(v_default);
	f_check_error();
	if (alcCloseDevice(v_entry->first) != ALC_TRUE) t_throwable::f_throw(L"alcCloseDevice failed.");
	delete this;
}

t_transfer t_device::f_create_context()
{
	t_session* session = t_session::f_instance();
	ALCcontext* context = alcCreateContext(v_entry->first, NULL);
	if (context == NULL) t_throwable::f_throw(L"alcCreateContext failed.");
	t_transfer object = t_object::f_allocate(session->v_extension->f_type<t_context>());
	object.f_pointer__(new t_context(this, v_contexts.insert(std::make_pair(context, static_cast<t_object*>(object))).first));
	return object;
}

}

}

namespace xemmai
{

void t_type_of<t_device>::f_define(t_extension* a_extension)
{
	t_define<t_device, t_object>(a_extension, L"Device")
		(L"close", t_member<void (t_device::*)(), &t_device::f_close>())
		(L"create_context", t_member<t_transfer (t_device::*)(), &t_device::f_create_context>())
		(L"get_string", t_member<std::wstring (t_device::*)(ALenum) const, &t_device::f_get_string>())
		(L"get_integer", t_member<ALint (t_device::*)(ALenum) const, &t_device::f_get_integer>())
		(L"create_buffer", t_member<t_transfer (t_device::*)(), &t_device::f_create_buffer>())
		(L"create_buffer_from_file", t_member<t_transfer (t_device::*)(const std::wstring&), &t_device::f_create_buffer_from_file>())
		(L"create_buffer_from_file_image", t_member<t_transfer (t_device::*)(const t_bytes&), &t_device::f_create_buffer_from_file_image>())
		(L"create_buffer_hello_world", t_member<t_transfer (t_device::*)(), &t_device::f_create_buffer_hello_world>())
		(L"create_buffer_waveform", t_member<t_transfer (t_device::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_device::f_create_buffer_waveform>())
		(L"get_mime_types", t_member<std::wstring (t_device::*)(ALenum) const, &t_device::f_get_mime_types>())
	;
}

t_type* t_type_of<t_device>::f_derive(t_object* a_this)
{
	return 0;
}

void t_type_of<t_device>::f_finalize(t_object* a_this)
{
	t_device* p = static_cast<t_device*>(a_this->f_pointer());
	assert(!p);
}

void t_type_of<t_device>::f_construct(t_object* a_class, t_slot* a_stack, size_t a_n)
{
	t_construct_with<t_transfer (*)(t_object*, const std::wstring*), t_device::f_construct>::f_call(a_class, a_stack, a_n);
}

void t_type_of<t_device>::f_instantiate(t_object* a_class, t_slot* a_stack, size_t a_n)
{
	f_construct(a_class, a_stack, a_n);
}

}
