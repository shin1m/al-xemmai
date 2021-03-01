#include "source.h"

namespace xemmai
{

void t_type_of<xemmaix::al::t_source>::f_define(t_extension* a_extension)
{
	using namespace xemmaix::al;
	t_define<t_source, t_object>(a_extension, L"Source"sv)
		(L"delete"sv, t_member<void(t_source::*)(), &t_source::f_delete>())
		(L"setf"sv, t_member<void(t_source::*)(ALenum, ALfloat), &t_source::f_setf>())
		(L"set3f"sv, t_member<void(t_source::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_source::f_set3f>())
		(L"seti"sv, t_member<void(t_source::*)(ALenum, ALint), &t_source::f_seti>())
		(L"set3i"sv, t_member<void(t_source::*)(ALenum, ALint, ALint, ALint), &t_source::f_set3i>())
		(L"setb"sv, t_member<void(t_source::*)(ALenum, bool), &t_source::f_setb>())
		(L"getf"sv, t_member<ALfloat(t_source::*)(ALenum) const, &t_source::f_getf>())
		(L"get3f"sv, t_member<t_pvalue(t_source::*)(ALenum) const, &t_source::f_get3f>())
		(L"geti"sv, t_member<ALint(t_source::*)(ALenum) const, &t_source::f_geti>())
		(L"get3i"sv, t_member<t_pvalue(t_source::*)(ALenum) const, &t_source::f_get3i>())
		(L"getb"sv, t_member<bool(t_source::*)(ALenum) const, &t_source::f_getb>())
		(L"play"sv, t_member<void(t_source::*)(), &t_source::f_play>())
		(L"stop"sv, t_member<void(t_source::*)(), &t_source::f_stop>())
		(L"rewind"sv, t_member<void(t_source::*)(), &t_source::f_rewind>())
		(L"pause"sv, t_member<void(t_source::*)(), &t_source::f_pause>())
		(L"set_buffer"sv, t_member<void(t_source::*)(t_buffer*), &t_source::f_set_buffer>())
		(L"get_buffer"sv, t_member<t_pvalue(t_source::*)() const, &t_source::f_get_buffer>())
		(L"queue_buffer"sv, t_member<void(t_source::*)(t_buffer&), &t_source::f_queue_buffer>())
		(L"unqueue_buffer"sv, t_member<t_pvalue(t_source::*)(), &t_source::f_unqueue_buffer>())
	;
}

}
