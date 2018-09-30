#include "context.h"
#include "source.h"
#include "buffer.h"

namespace xemmaix::al
{

void t_base_device::f_check_error() const
{
	ALCenum error = alcGetError(v_entry->first);
	if (error != ALC_NO_ERROR) f_throw<t_alc_error>(alcGetString(v_entry->first, error), error);
}

t_device::t_device(std::map<ALCdevice*, t_scoped>::iterator a_entry, ALCcontext* a_default) : t_base_device(a_entry), v_default(a_default)
{
	t_session* session = t_session::f_instance();
	alcMakeContextCurrent(v_default);
	t_scoped object = t_object::f_allocate(session->v_extension->f_type<t_context>(), false);
	object.f_pointer__(new t_context(this, v_contexts.insert(std::make_pair(v_default, static_cast<t_object*>(object))).first));
}

t_scoped t_device::f_create_buffer(ALuint a_id)
{
	t_session* session = t_session::f_instance();
	t_scoped object = t_object::f_allocate(session->v_extension->f_type<t_buffer>(), false);
	object.f_pointer__(new t_buffer(this, v_buffers.insert(std::make_pair(a_id, static_cast<t_object*>(object))).first));
	return object;
}

void t_device::f_close()
{
	t_context& context = f_as<t_context&>(v_contexts.find(v_default)->second);
	while (!context.v_sources.empty()) f_as<t_source&>(context.v_sources.begin()->second).f_delete();
	delete &context;
	while (!v_contexts.empty()) f_as<t_context&>(v_contexts.begin()->second).f_destroy();
	alcMakeContextCurrent(v_default);
	while (!v_buffers.empty()) f_as<t_buffer&>(v_buffers.begin()->second).f_delete();
	alcMakeContextCurrent(NULL);
	f_check_error();
	alcDestroyContext(v_default);
	f_check_error();
	if (alcCloseDevice(v_entry->first) != ALC_TRUE) xemmai::f_throw(L"alcCloseDevice failed."sv);
	delete this;
}

t_scoped t_device::f_create_context()
{
	t_session* session = t_session::f_instance();
	ALCcontext* context = alcCreateContext(v_entry->first, NULL);
	if (context == NULL) f_check_error();
	t_scoped object = t_object::f_allocate(session->v_extension->f_type<t_context>(), false);
	object.f_pointer__(new t_context(this, v_contexts.insert(std::make_pair(context, static_cast<t_object*>(object))).first));
	return object;
}

void t_capture_device::f_close()
{
	if (alcCaptureCloseDevice(v_entry->first) != ALC_TRUE) xemmai::f_throw(L"alcCaptureCloseDevice failed."sv);
	delete this;
}

}

namespace xemmai
{

void t_type_of<xemmaix::al::t_base_device>::f_define(t_extension* a_extension)
{
	using namespace xemmaix::al;
	t_define<t_base_device, t_object>(a_extension, L"BaseDevice"sv)
		(L"get_string"sv, t_member<std::wstring(t_base_device::*)(ALenum) const, &t_base_device::f_get_string>())
		(L"get_integer"sv, t_member<ALint(t_base_device::*)(ALenum) const, &t_base_device::f_get_integer>())
	;
}

t_scoped t_type_of<xemmaix::al::t_base_device>::f_do_construct(t_stacked* a_stack, size_t a_n)
{
	f_throw(L"uninstantiatable."sv);
}

void t_type_of<xemmaix::al::t_base_device>::f_do_instantiate(t_stacked* a_stack, size_t a_n)
{
	t_destruct_n destruct(a_stack, a_n);
	a_stack[0].f_construct(f_construct(a_stack, a_n));
}

void t_type_of<xemmaix::al::t_device>::f_define(t_extension* a_extension)
{
	using namespace xemmaix::al;
	t_define<t_device, t_base_device>(a_extension, L"Device"sv)
		(L"close"sv, t_member<void(t_device::*)(), &t_device::f_close>())
		(L"default_context"sv, t_member<t_scoped(t_device::*)() const, &t_device::f_default_context>())
		(L"create_context"sv, t_member<t_scoped(t_device::*)(), &t_device::f_create_context>())
		(L"create_buffer"sv, t_member<t_scoped(t_device::*)(), &t_device::f_create_buffer>())
		(L"create_buffer_from_file"sv, t_member<t_scoped(t_device::*)(std::wstring_view), &t_device::f_create_buffer_from_file>())
		(L"create_buffer_from_file_image"sv, t_member<t_scoped(t_device::*)(const t_bytes&), &t_device::f_create_buffer_from_file_image>())
		(L"create_buffer_hello_world"sv, t_member<t_scoped(t_device::*)(), &t_device::f_create_buffer_hello_world>())
		(L"create_buffer_waveform"sv, t_member<t_scoped(t_device::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_device::f_create_buffer_waveform>())
		(L"get_mime_types"sv, t_member<std::wstring(t_device::*)(ALenum) const, &t_device::f_get_mime_types>())
	;
}

t_scoped t_type_of<xemmaix::al::t_device>::f_do_construct(t_stacked* a_stack, size_t a_n)
{
	return t_construct_with<t_scoped(*)(t_type*, const t_string*), xemmaix::al::t_device::f_construct>::t_bind<xemmaix::al::t_device>::f_do(this, a_stack, a_n);
}

void t_type_of<xemmaix::al::t_capture_device>::f_define(t_extension* a_extension)
{
	using namespace xemmaix::al;
	t_define<t_capture_device, t_base_device>(a_extension, L"CaptureDevice"sv)
		(L"close"sv, t_member<void(t_capture_device::*)(), &t_capture_device::f_close>())
		(L"start"sv, t_member<void(t_capture_device::*)(), &t_capture_device::f_start>())
		(L"stop"sv, t_member<void(t_capture_device::*)(), &t_capture_device::f_stop>())
		(L"samples"sv, t_member<void(t_capture_device::*)(t_bytes&, ALCsizei), &t_capture_device::f_samples>())
	;
}

t_scoped t_type_of<xemmaix::al::t_capture_device>::f_do_construct(t_stacked* a_stack, size_t a_n)
{
	return t_construct_with<t_scoped(*)(t_type*, const t_string*, ALCuint, ALCenum, ALCsizei), xemmaix::al::t_capture_device::f_construct>::t_bind<xemmaix::al::t_capture_device>::f_do(this, a_stack, a_n);
}

}
