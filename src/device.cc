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

t_device::t_device(t_session* a_session, ALCdevice* a_device, ALCcontext* a_default) : t_base_device(a_session->v_devices.emplace(a_device, t_object::f_of(this)).first), v_default(a_default)
{
	alcMakeContextCurrent(v_default);
	f_new<t_context>(a_session->v_extension, false, this, v_default);
}

t_pvalue t_device::f_create_buffer(ALuint a_id)
{
	return f_new<t_buffer>(t_session::f_instance()->v_extension, false, this, a_id);
}

void t_device::f_close()
{
	auto& context = v_contexts.find(v_default)->second->f_as<t_context>();
	while (!context.v_sources.empty()) context.v_sources.begin()->second->f_as<t_source>().f_delete();
	v_contexts.erase(v_default);
	context.v_entry = {};
	while (!v_contexts.empty()) v_contexts.begin()->second->f_as<t_context>().f_destroy();
	alcMakeContextCurrent(v_default);
	while (!v_buffers.empty()) v_buffers.begin()->second->f_as<t_buffer>().f_delete();
	alcMakeContextCurrent(NULL);
	f_check_error();
	alcDestroyContext(v_default);
	f_check_error();
	if (alcCloseDevice(v_entry->first) != ALC_TRUE) xemmai::f_throw(L"alcCloseDevice failed."sv);
	t_session::f_instance()->v_devices.erase(v_entry);
	v_entry = {};
}

t_pvalue t_device::f_create_context()
{
	auto session = t_session::f_instance();
	ALCcontext* context = alcCreateContext(v_entry->first, NULL);
	if (context == NULL) f_check_error();
	return f_new<t_context>(session->v_extension, false, this, context);
}

void t_capture_device::f_close()
{
	if (alcCaptureCloseDevice(v_entry->first) != ALC_TRUE) xemmai::f_throw(L"alcCaptureCloseDevice failed."sv);
	t_session::f_instance()->v_capture_devices.erase(v_entry);
	v_entry = {};
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

t_pvalue t_type_of<xemmaix::al::t_base_device>::f_do_construct(t_pvalue* a_stack, size_t a_n)
{
	f_throw(L"uninstantiatable."sv);
}

void t_type_of<xemmaix::al::t_base_device>::f_do_instantiate(t_pvalue* a_stack, size_t a_n)
{
	a_stack[0] = f_construct(a_stack, a_n);
}

void t_type_of<xemmaix::al::t_device>::f_define(t_extension* a_extension)
{
	using namespace xemmaix::al;
	t_define<t_device, t_base_device>(a_extension, L"Device"sv)
		(L"close"sv, t_member<void(t_device::*)(), &t_device::f_close>())
		(L"default_context"sv, t_member<t_pvalue(t_device::*)() const, &t_device::f_default_context>())
		(L"create_context"sv, t_member<t_pvalue(t_device::*)(), &t_device::f_create_context>())
		(L"create_buffer"sv, t_member<t_pvalue(t_device::*)(), &t_device::f_create_buffer>())
		(L"create_buffer_from_file"sv, t_member<t_pvalue(t_device::*)(std::wstring_view), &t_device::f_create_buffer_from_file>())
		(L"create_buffer_from_file_image"sv, t_member<t_pvalue(t_device::*)(const t_bytes&), &t_device::f_create_buffer_from_file_image>())
		(L"create_buffer_hello_world"sv, t_member<t_pvalue(t_device::*)(), &t_device::f_create_buffer_hello_world>())
		(L"create_buffer_waveform"sv, t_member<t_pvalue(t_device::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_device::f_create_buffer_waveform>())
		(L"get_mime_types"sv, t_member<std::wstring(t_device::*)(ALenum) const, &t_device::f_get_mime_types>())
	;
}

t_pvalue t_type_of<xemmaix::al::t_device>::f_do_construct(t_pvalue* a_stack, size_t a_n)
{
	return t_construct_with<t_pvalue(*)(t_type*, const t_string*), xemmaix::al::t_device::f_construct>::t_bind<xemmaix::al::t_device>::f_do(this, a_stack, a_n);
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

t_pvalue t_type_of<xemmaix::al::t_capture_device>::f_do_construct(t_pvalue* a_stack, size_t a_n)
{
	return t_construct_with<t_pvalue(*)(t_type*, const t_string*, ALCuint, ALCenum, ALCsizei), xemmaix::al::t_capture_device::f_construct>::t_bind<xemmaix::al::t_capture_device>::f_do(this, a_stack, a_n);
}

}
