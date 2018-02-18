#include "error.h"

namespace xemmaix::al
{

void t_error::f_check()
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR) xemmaix::al::f_throw<t_error>(alGetString(error), error);
}

void t_alut_error::f_throw()
{
	ALenum error = alutGetError();
	xemmaix::al::f_throw<t_alut_error>(alutGetErrorString(error), error);
}

}

namespace xemmai
{

void t_type_of<xemmaix::al::t_error>::f_define(t_extension* a_extension)
{
	using namespace xemmaix::al;
	t_define<t_error, t_throwable>(a_extension, L"Error")
		(L"error", t_member<ALenum(t_error::*)() const, &t_error::f_error>())
	;
	t_define<t_alc_error, t_error>(a_extension, L"ALCError");
	t_define<t_alut_error, t_error>(a_extension, L"ALUTError");
}

t_type* t_type_of<xemmaix::al::t_error>::f_derive(t_object* a_this)
{
	return nullptr;
}

void t_type_of<xemmaix::al::t_error>::f_instantiate(t_object* a_class, t_stacked* a_stack, size_t a_n)
{
	t_throwable::f_throw(a_stack, a_n, L"uninstantiatable.");
}

}
