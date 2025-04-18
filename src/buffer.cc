#include "buffer.h"

namespace xemmai
{

void t_type_of<xemmaix::al::t_buffer>::f_define(t_library* a_library)
{
	using namespace xemmaix::al;
	t_define{a_library}
	(L"delete"sv, t_member<void(t_buffer::*)(), &t_buffer::f_delete>())
	(L"data"sv, t_member<void(t_buffer::*)(ALenum, const t_bytes&, ALsizei, ALsizei), &t_buffer::f_data>())
	(L"setf"sv, t_member<void(t_buffer::*)(ALenum, ALfloat), &t_buffer::f_setf>())
	(L"set3f"sv, t_member<void(t_buffer::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_buffer::f_set3f>())
	(L"seti"sv, t_member<void(t_buffer::*)(ALenum, ALint), &t_buffer::f_seti>())
	(L"set3i"sv, t_member<void(t_buffer::*)(ALenum, ALint, ALint, ALint), &t_buffer::f_set3i>())
	(L"getf"sv, t_member<ALfloat(t_buffer::*)(ALenum) const, &t_buffer::f_getf>())
	(L"get3f"sv, t_member<t_pvalue(t_buffer::*)(ALenum) const, &t_buffer::f_get3f>())
	(L"geti"sv, t_member<ALint(t_buffer::*)(ALenum) const, &t_buffer::f_geti>())
	(L"get3i"sv, t_member<t_pvalue(t_buffer::*)(ALenum) const, &t_buffer::f_get3i>())
	.f_derive<t_buffer, t_object>();
}

}
