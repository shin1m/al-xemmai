#include "source.h"

#include <cassert>

namespace xemmai
{

void t_type_of<t_source>::f_define(t_extension* a_extension)
{
	t_define<t_source, t_object>(a_extension, L"Source")
		(L"delete", t_member<void (t_source::*)(), &t_source::f_delete>())
		(L"setf", t_member<void (t_source::*)(ALenum, ALfloat), &t_source::f_setf>())
		(L"set3f", t_member<void (t_source::*)(ALenum, ALfloat, ALfloat, ALfloat), &t_source::f_set3f>())
		(L"seti", t_member<void (t_source::*)(ALenum, ALint), &t_source::f_seti>())
		(L"set3i", t_member<void (t_source::*)(ALenum, ALint, ALint, ALint), &t_source::f_set3i>())
		(L"setb", t_member<void (t_source::*)(ALenum, bool), &t_source::f_setb>())
		(L"getf", t_member<ALfloat (t_source::*)(ALenum) const, &t_source::f_getf>())
		(L"get3f", t_member<t_transfer (t_source::*)(ALenum) const, &t_source::f_get3f>())
		(L"geti", t_member<ALint (t_source::*)(ALenum) const, &t_source::f_geti>())
		(L"get3i", t_member<t_transfer (t_source::*)(ALenum) const, &t_source::f_get3i>())
		(L"getb", t_member<bool (t_source::*)(ALenum) const, &t_source::f_getb>())
		(L"play", t_member<void (t_source::*)(), &t_source::f_play>())
		(L"stop", t_member<void (t_source::*)(), &t_source::f_stop>())
		(L"rewind", t_member<void (t_source::*)(), &t_source::f_rewind>())
		(L"pause", t_member<void (t_source::*)(), &t_source::f_pause>())
		(L"set_buffer", t_member<void (t_source::*)(t_buffer*), &t_source::f_set_buffer>())
		(L"get_buffer", t_member<t_transfer (t_source::*)() const, &t_source::f_get_buffer>())
		(L"queue_buffer", t_member<void (t_source::*)(t_buffer&), &t_source::f_queue_buffer>())
		(L"unqueue_buffer", t_member<t_transfer (t_source::*)(), &t_source::f_unqueue_buffer>())
	;
}

t_type* t_type_of<t_source>::f_derive(t_object* a_this)
{
	return 0;
}

void t_type_of<t_source>::f_finalize(t_object* a_this)
{
	t_source* p = static_cast<t_source*>(a_this->f_pointer());
	assert(!p);
}

void t_type_of<t_source>::f_instantiate(t_object* a_class, t_slot* a_stack, size_t a_n)
{
	t_throwable::f_throw(L"uninstantiatable.");
}

}
