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
	t_define<t_error, t_throwable>(a_extension, L"Error"sv)
		(L"error"sv, t_member<ALenum(t_error::*)() const, &t_error::f_error>())
	;
	t_define<t_alc_error, t_error>(a_extension, L"ALCError"sv);
	t_define<t_alut_error, t_error>(a_extension, L"ALUTError"sv);
}

}
