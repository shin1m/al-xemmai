#include "source.h"

namespace xemmaix::al
{

t_context::~t_context()
{
	while (!v_sources.empty()) delete &f_as<t_source&>(v_sources.begin()->second);
	v_entry->second.f_pointer__(nullptr);
	v_device->v_contexts.erase(v_entry);
}

t_scoped t_context::f_create_source()
{
	t_session* session = t_session::f_instance();
	f_make_current();
	ALuint id;
	alGenSources(1, &id);
	t_error::f_check();
	t_scoped object = t_object::f_allocate(session->v_extension->f_type<t_source>());
	object.f_pointer__(new t_source(this, v_sources.insert(std::make_pair(id, static_cast<t_object*>(object))).first));
	return object;
}

}

namespace xemmai
{

void t_type_of<xemmaix::al::t_context>::f_define(t_extension* a_extension)
{
	t_define<t_context, t_object>(a_extension, L"Context")
		(L"destroy", t_member<void (t_context::*)(), &t_context::f_destroy>())
		(L"process", t_member<void (t_context::*)(), &t_context::f_process>())
		(L"suspend", t_member<void (t_context::*)(), &t_context::f_suspend>())
		(L"get_device", t_member<t_scoped (t_context::*)() const, &t_context::f_get_device>())
		(L"enable", t_member<void (t_context::*)(ALenum), &t_context::f_enable>())
		(L"disable", t_member<void (t_context::*)(ALenum), &t_context::f_disable>())
		(L"is_enabled", t_member<bool (t_context::*)(ALenum) const, &t_context::f_is_enabled>())
		(L"get_string", t_member<std::wstring (t_context::*)(ALenum) const, &t_context::f_get_string>())
		(L"get_booleanv", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_booleanv>())
		(L"get_integerv", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_integerv>())
		(L"get_floatv", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_floatv>())
		(L"get_doublev", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_doublev>())
		(L"get_boolean", t_member<bool (t_context::*)(ALenum) const, &t_context::f_get_boolean>())
		(L"get_integer", t_member<ALint (t_context::*)(ALenum) const, &t_context::f_get_integer>())
		(L"get_float", t_member<ALfloat (t_context::*)(ALenum) const, &t_context::f_get_float>())
		(L"get_double", t_member<ALdouble (t_context::*)(ALenum) const, &t_context::f_get_double>())
		(L"listenerf", t_member<void (t_context::*)(ALenum, ALfloat), &t_context::f_listenerf>())
		(L"listener3f", t_member<void (t_context::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_context::f_listener3f>())
		(L"listener6f", t_member<void (t_context::*)(ALenum, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat), &t_context::f_listener6f>())
		(L"listeneri", t_member<void (t_context::*)(ALenum, ALint), &t_context::f_listeneri>())
		(L"listener3i", t_member<void (t_context::*)(ALenum, ALint, ALint, ALint), &t_context::f_listener3i>())
		(L"listener6i", t_member<void (t_context::*)(ALenum, ALint, ALint, ALint, ALint, ALint, ALint), &t_context::f_listener6i>())
		(L"get_listenerf", t_member<ALfloat (t_context::*)(ALenum) const, &t_context::f_get_listenerf>())
		(L"get_listener3f", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_listener3f>())
		(L"get_listenerfv", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_listenerfv>())
		(L"get_listeneri", t_member<ALint (t_context::*)(ALenum) const, &t_context::f_get_listeneri>())
		(L"get_listener3i", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_listener3i>())
		(L"get_listeneriv", t_member<t_scoped (t_context::*)(ALenum) const, &t_context::f_get_listeneriv>())
		(L"create_source", t_member<t_scoped (t_context::*)(), &t_context::f_create_source>())
		(L"source_play", t_context::f_source_do<alSourcePlayv>)
		(L"source_stop", t_context::f_source_do<alSourceStopv>)
		(L"source_rewind", t_context::f_source_do<alSourceRewindv>)
		(L"source_pause", t_context::f_source_do<alSourcePausev>)
		(L"doppler_factor", t_member<void (t_context::*)(ALfloat), &t_context::f_doppler_factor>())
		(L"doppler_velocity", t_member<void (t_context::*)(ALfloat), &t_context::f_doppler_velocity>())
		(L"speed_of_sound", t_member<void (t_context::*)(ALfloat), &t_context::f_speed_of_sound>())
		(L"distance_model", t_member<void (t_context::*)(ALenum), &t_context::f_distance_model>())
	;
}

}
