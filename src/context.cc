#include "source.h"

namespace xemmaix::al
{

void t_context::f_destroy()
{
	if (v_entry->first == v_device->v_default) xemmai::f_throw(L"default context can not be destroyed."sv);
	alcMakeContextCurrent(v_device->v_default);
	alcDestroyContext(v_entry->first);
	v_device->f_check_error();
	while (!v_sources.empty()) v_sources.begin()->second->f_as<t_source>().f_delete();
	v_device->v_contexts.erase(v_entry);
	v_entry = {};
}

t_pvalue t_context::f_create_source()
{
	auto session = t_session::f_instance();
	f_make_current();
	ALuint id;
	alGenSources(1, &id);
	t_error::f_check();
	return f_new<t_source>(session->v_library, this, id);
}

}

namespace xemmai
{

void t_type_of<xemmaix::al::t_context>::f_define(t_library* a_library)
{
	t_define{a_library}
	(L"destroy"sv, t_member<void(t_context::*)(), &t_context::f_destroy>())
	(L"process"sv, t_member<void(t_context::*)(), &t_context::f_process>())
	(L"suspend"sv, t_member<void(t_context::*)(), &t_context::f_suspend>())
	(L"get_device"sv, t_member<t_pvalue(t_context::*)() const, &t_context::f_get_device>())
	(L"enable"sv, t_member<void(t_context::*)(ALenum), &t_context::f_enable>())
	(L"disable"sv, t_member<void(t_context::*)(ALenum), &t_context::f_disable>())
	(L"is_enabled"sv, t_member<bool(t_context::*)(ALenum) const, &t_context::f_is_enabled>())
	(L"get_string"sv, t_member<std::wstring(t_context::*)(ALenum) const, &t_context::f_get_string>())
	(L"get_booleanv"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_booleanv>())
	(L"get_integerv"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_integerv>())
	(L"get_floatv"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_floatv>())
	(L"get_doublev"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_doublev>())
	(L"get_boolean"sv, t_member<bool(t_context::*)(ALenum) const, &t_context::f_get_boolean>())
	(L"get_integer"sv, t_member<ALint(t_context::*)(ALenum) const, &t_context::f_get_integer>())
	(L"get_float"sv, t_member<ALfloat(t_context::*)(ALenum) const, &t_context::f_get_float>())
	(L"get_double"sv, t_member<ALdouble(t_context::*)(ALenum) const, &t_context::f_get_double>())
	(L"listenerf"sv, t_member<void(t_context::*)(ALenum, ALfloat), &t_context::f_listenerf>())
	(L"listener3f"sv, t_member<void(t_context::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_context::f_listener3f>())
	(L"listener6f"sv, t_member<void(t_context::*)(ALenum, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat), &t_context::f_listener6f>())
	(L"listeneri"sv, t_member<void(t_context::*)(ALenum, ALint), &t_context::f_listeneri>())
	(L"listener3i"sv, t_member<void(t_context::*)(ALenum, ALint, ALint, ALint), &t_context::f_listener3i>())
	(L"listener6i"sv, t_member<void(t_context::*)(ALenum, ALint, ALint, ALint, ALint, ALint, ALint), &t_context::f_listener6i>())
	(L"get_listenerf"sv, t_member<ALfloat(t_context::*)(ALenum) const, &t_context::f_get_listenerf>())
	(L"get_listener3f"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_listener3f>())
	(L"get_listenerfv"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_listenerfv>())
	(L"get_listeneri"sv, t_member<ALint(t_context::*)(ALenum) const, &t_context::f_get_listeneri>())
	(L"get_listener3i"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_listener3i>())
	(L"get_listeneriv"sv, t_member<t_pvalue(t_context::*)(ALenum) const, &t_context::f_get_listeneriv>())
	(L"create_source"sv, t_member<t_pvalue(t_context::*)(), &t_context::f_create_source>())
	(L"source_play"sv, t_context::f_source_do<alSourcePlayv>)
	(L"source_stop"sv, t_context::f_source_do<alSourceStopv>)
	(L"source_rewind"sv, t_context::f_source_do<alSourceRewindv>)
	(L"source_pause"sv, t_context::f_source_do<alSourcePausev>)
	(L"doppler_factor"sv, t_member<void(t_context::*)(ALfloat), &t_context::f_doppler_factor>())
	(L"doppler_velocity"sv, t_member<void(t_context::*)(ALfloat), &t_context::f_doppler_velocity>())
	(L"speed_of_sound"sv, t_member<void(t_context::*)(ALfloat), &t_context::f_speed_of_sound>())
	(L"distance_model"sv, t_member<void(t_context::*)(ALenum), &t_context::f_distance_model>())
	.f_derive<t_context, t_object>();
}

}
