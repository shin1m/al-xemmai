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

	t_error(const std::wstring& a_message, ALenum a_error) : t_throwable(a_message), v_error(a_error)
	{
	}
	ALenum f_error() const
	{
		return v_error;
	}
};

struct t_alc_error : t_error
{
	t_alc_error(const std::wstring& a_message, ALenum a_error) : t_error(a_message, a_error)
	{
	}
};

struct t_alut_error : t_error
{
	static void f_throw();

	t_alut_error(const std::wstring& a_message, ALenum a_error) : t_error(a_message, a_error)
	{
	}
};

template<typename T_error>
inline void f_throw(const ALchar* a_message, ALenum a_error)
{
	t_session* session = t_session::f_instance();
	std::wstring message = f_convert(std::string(a_message));
	t_scoped object = t_object::f_allocate(session->v_extension->f_type<T_error>());
	object.f_pointer__(new T_error(message, a_error));
	throw object;
}

}

namespace xemmai
{

template<>
struct t_type_of<xemmaix::al::t_error> : t_uninstantiatable<t_underivable<t_bears<xemmaix::al::t_error, t_type_of<t_throwable>>>>
{
	typedef xemmaix::al::t_extension t_extension;

	static void f_define(t_extension* a_extension);

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
