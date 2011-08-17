#ifndef AL__XEMMAI__AL_H
#define AL__XEMMAI__AL_H

#include <map>
#include <xemmai/convert.h>
#include <xemmai/bytes.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

namespace al
{

namespace xemmai
{

using ::xemmai::t_object;
using ::xemmai::t_scan;
using ::xemmai::t_value;
using ::xemmai::t_transfer;
using ::xemmai::t_scoped;
using ::xemmai::t_slot;
using ::xemmai::t_fundamental;
using ::xemmai::t_type_of;
using ::xemmai::f_check;
using ::xemmai::f_as;
using ::xemmai::t_define;
using ::xemmai::f_global;
using ::xemmai::t_throwable;
using ::xemmai::t_bytes;
using ::xemmai::portable::f_convert;

class t_extension;
class t_error;
struct t_alc_error;
struct t_alut_error;
class t_device;
class t_context;
class t_source;
class t_buffer;

template<typename T_error>
void f_throw(const ALchar* a_message, ALenum a_error);

t_transfer f_tuple(const t_transfer& a_0, const t_transfer& a_1, const t_transfer& a_2);
t_transfer f_tuple(const t_transfer& a_0, const t_transfer& a_1, const t_transfer& a_2, const t_transfer& a_3, const t_transfer& a_4, const t_transfer& a_5);

class t_session
{
	template<typename T_error>
	friend void f_throw(const ALchar* a_message, ALenum a_error);
	friend class t_device;
	friend class t_context;

	static XEMMAI__PORTABLE__THREAD t_session* v_instance;

	t_extension* v_extension;
	std::map<ALCdevice*, t_scoped> v_devices;

public:
	static t_session* f_instance()
	{
		if (!v_instance) t_throwable::f_throw(L"must be inside main.");
		return v_instance;
	}

	t_session(t_extension* a_extension);
	~t_session();
	t_extension* f_extension() const
	{
		return v_extension;
	}
};

class t_extension : public ::xemmai::t_extension
{
	template<typename T, typename T_super> friend class t_define;

	t_slot v_type_error;
	t_slot v_type_alc_error;
	t_slot v_type_alut_error;
	t_slot v_type_device;
	t_slot v_type_context;
	t_slot v_type_source;
	t_slot v_type_buffer;

	template<typename T>
	void f_type__(const t_transfer& a_type);

public:
	t_extension(t_object* a_module);
	virtual void f_scan(t_scan a_scan);
	template<typename T>
	const T* f_extension() const
	{
		return f_global();
	}
	template<typename T>
	t_object* f_type() const
	{
		return f_global()->f_type<T>();
	}
	template<typename T>
	t_transfer f_as(const T& a_value) const
	{
		typedef t_type_of<typename t_fundamental<T>::t_type> t;
		return t::f_transfer(f_extension<typename t::t_extension>(), a_value);
	}
};

template<>
inline void t_extension::f_type__<t_error>(const t_transfer& a_type)
{
	v_type_error = a_type;
}

template<>
inline void t_extension::f_type__<t_alc_error>(const t_transfer& a_type)
{
	v_type_alc_error = a_type;
}

template<>
inline void t_extension::f_type__<t_alut_error>(const t_transfer& a_type)
{
	v_type_alut_error = a_type;
}

template<>
inline void t_extension::f_type__<t_device>(const t_transfer& a_type)
{
	v_type_device = a_type;
}

template<>
inline void t_extension::f_type__<t_context>(const t_transfer& a_type)
{
	v_type_context = a_type;
}

template<>
inline void t_extension::f_type__<t_source>(const t_transfer& a_type)
{
	v_type_source = a_type;
}

template<>
inline void t_extension::f_type__<t_buffer>(const t_transfer& a_type)
{
	v_type_buffer = a_type;
}

template<>
inline const t_extension* t_extension::f_extension<t_extension>() const
{
	return this;
}

template<>
inline t_object* t_extension::f_type<t_error>() const
{
	return v_type_error;
}

template<>
inline t_object* t_extension::f_type<t_alc_error>() const
{
	return v_type_alc_error;
}

template<>
inline t_object* t_extension::f_type<t_alut_error>() const
{
	return v_type_alut_error;
}

template<>
inline t_object* t_extension::f_type<t_device>() const
{
	return v_type_device;
}

template<>
inline t_object* t_extension::f_type<t_context>() const
{
	return v_type_context;
}

template<>
inline t_object* t_extension::f_type<t_source>() const
{
	return v_type_source;
}

template<>
inline t_object* t_extension::f_type<t_buffer>() const
{
	return v_type_buffer;
}

}

}

#endif
