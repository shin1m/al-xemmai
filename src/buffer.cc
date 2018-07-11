#include "buffer.h"

namespace xemmai
{

void t_type_of<xemmaix::al::t_buffer>::f_define(t_extension* a_extension)
{
	using namespace xemmaix::al;
	t_define<t_buffer, t_object>(a_extension, L"Buffer")
		(L"delete", t_member<void(t_buffer::*)(), &t_buffer::f_delete>())
		(L"data", t_member<void(t_buffer::*)(ALenum, const t_bytes&, ALsizei, ALsizei), &t_buffer::f_data>())
		(L"setf", t_member<void(t_buffer::*)(ALenum, ALfloat), &t_buffer::f_setf>())
		(L"set3f", t_member<void(t_buffer::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_buffer::f_set3f>())
		(L"seti", t_member<void(t_buffer::*)(ALenum, ALint), &t_buffer::f_seti>())
		(L"set3i", t_member<void(t_buffer::*)(ALenum, ALint, ALint, ALint), &t_buffer::f_set3i>())
		(L"getf", t_member<ALfloat(t_buffer::*)(ALenum) const, &t_buffer::f_getf>())
		(L"get3f", t_member<t_scoped(t_buffer::*)(ALenum) const, &t_buffer::f_get3f>())
		(L"geti", t_member<ALint(t_buffer::*)(ALenum) const, &t_buffer::f_geti>())
		(L"get3i", t_member<t_scoped(t_buffer::*)(ALenum) const, &t_buffer::f_get3i>())
	;
}

}
