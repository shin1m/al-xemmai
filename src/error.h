#ifndef XEMMAIX__AL__ERROR_H
#define XEMMAIX__AL__ERROR_H

#include "al.h"

namespace xemmaix::al
{

class t_error : public t_throwable
{
	ALenum v_error;

public:
	static void f_check();

	t_error(std::wstring_view a_message, ALenum a_error) : t_throwable(a_message), v_error(a_error)
	{
	}
	ALenum f_error() const
	{
		return v_error;
	}
};

struct t_alc_error : t_error
{
	t_alc_error(std::wstring_view a_message, ALenum a_error) : t_error(a_message, a_error)
	{
	}
};

struct t_alut_error : t_error
{
	static void f_throw();

	t_alut_error(std::wstring_view a_message, ALenum a_error) : t_error(a_message, a_error)
	{
	}
};

template<typename T_error>
inline void f_throw(const ALchar* a_message, ALenum a_error)
{
	throw t_rvalue(f_new<T_error>(t_session::f_instance()->v_library, f_convert(a_message), a_error));
}

}

namespace xemmai
{

template<>
struct t_type_of<xemmaix::al::t_error> : t_uninstantiatable<t_bears<xemmaix::al::t_error, t_type_of<t_throwable>>>
{
	typedef xemmaix::al::t_library t_library;

	static void f_define(t_library* a_library);

	using t_base::t_base;
};

template<>
struct t_type_of<xemmaix::al::t_alc_error> : t_bears<xemmaix::al::t_alc_error, t_type_of<xemmaix::al::t_error>>
{
	using t_base::t_base;
};

template<>
struct t_type_of<xemmaix::al::t_alut_error> : t_bears<xemmaix::al::t_alut_error, t_type_of<xemmaix::al::t_error>>
{
	using t_base::t_base;
};

}

#endif
