#include "buffer.h"

#include <cassert>

namespace xemmai
{

void t_type_of<t_buffer>::f_define(t_extension* a_extension)
{
	t_define<t_buffer, t_object>(a_extension, L"Buffer")
		(L"delete", t_member<void (t_buffer::*)(), &t_buffer::f_delete>())
		(L"data", t_member<void (t_buffer::*)(ALenum, const t_bytes&, ALsizei, ALsizei), &t_buffer::f_data>())
		(L"setf", t_member<void (t_buffer::*)(ALenum, ALfloat), &t_buffer::f_setf>())
		(L"set3f", t_member<void (t_buffer::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_buffer::f_set3f>())
		(L"seti", t_member<void (t_buffer::*)(ALenum, ALint), &t_buffer::f_seti>())
		(L"set3i", t_member<void (t_buffer::*)(ALenum, ALint, ALint, ALint), &t_buffer::f_set3i>())
		(L"getf", t_member<ALfloat (t_buffer::*)(ALenum) const, &t_buffer::f_getf>())
		(L"get3f", t_member<t_scoped (t_buffer::*)(ALenum) const, &t_buffer::f_get3f>())
		(L"geti", t_member<ALint (t_buffer::*)(ALenum) const, &t_buffer::f_geti>())
		(L"get3i", t_member<t_scoped (t_buffer::*)(ALenum) const, &t_buffer::f_get3i>())
	;
}

t_type* t_type_of<t_buffer>::f_derive(t_object* a_this)
{
	return nullptr;
}

void t_type_of<t_buffer>::f_finalize(t_object* a_this)
{
	t_buffer* p = static_cast<t_buffer*>(a_this->f_pointer());
	assert(!p);
}

void t_type_of<t_buffer>::f_instantiate(t_object* a_class, t_scoped* a_stack, size_t a_n)
{
	t_throwable::f_throw(L"uninstantiatable.");
}

}
