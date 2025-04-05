#ifndef XEMMAIX__AL__AL_H
#define XEMMAIX__AL__AL_H

#include <map>
#include <xemmai/convert.h>
#include <xemmai/bytes.h>
#include <xemmai/tuple.h>
#include <AL/alut.h>

namespace xemmaix::al
{

using namespace xemmai;
using namespace xemmai::portable;

class t_library;
class t_error;
struct t_alc_error;
struct t_alut_error;
class t_base_device;
class t_device;
class t_capture_device;
class t_context;
class t_source;
class t_buffer;

template<typename T_error>
void f_throw(const ALchar* a_message, ALenum a_error);

class t_session
{
	template<typename T_error>
	friend void f_throw(const ALchar* a_message, ALenum a_error);
	friend class t_device;
	friend class t_capture_device;
	friend class t_context;

	static std::atomic_flag v_running;
	static XEMMAI__PORTABLE__THREAD t_session* v_instance;

	t_library* v_library;
	std::map<ALCdevice*, t_root> v_devices;
	std::map<ALCdevice*, t_root> v_capture_devices;

public:
	static t_session* f_instance()
	{
		if (!v_instance) xemmai::f_throw(L"must be inside main."sv);
		return v_instance;
	}

	t_session(t_library* a_library);
	~t_session();
};

class t_library : public xemmai::t_library
{
	t_slot_of<t_type> v_type_error;
	t_slot_of<t_type> v_type_alc_error;
	t_slot_of<t_type> v_type_alut_error;
	t_slot_of<t_type> v_type_base_device;
	t_slot_of<t_type> v_type_device;
	t_slot_of<t_type> v_type_capture_device;
	t_slot_of<t_type> v_type_context;
	t_slot_of<t_type> v_type_source;
	t_slot_of<t_type> v_type_buffer;

public:
	using xemmai::t_library::t_library;
	XEMMAI__LIBRARY__MEMBERS
};

XEMMAI__LIBRARY__BASE(t_library, t_global, f_global())
XEMMAI__LIBRARY__TYPE(t_library, error)
XEMMAI__LIBRARY__TYPE(t_library, alc_error)
XEMMAI__LIBRARY__TYPE(t_library, alut_error)
XEMMAI__LIBRARY__TYPE(t_library, base_device)
XEMMAI__LIBRARY__TYPE(t_library, device)
XEMMAI__LIBRARY__TYPE(t_library, capture_device)
XEMMAI__LIBRARY__TYPE(t_library, context)
XEMMAI__LIBRARY__TYPE(t_library, source)
XEMMAI__LIBRARY__TYPE(t_library, buffer)

template<typename T>
struct t_holds : t_bears<T>
{
	template<typename U>
	static U& f_cast(auto&& a_object)
	{
		xemmaix::al::t_session::f_instance();
		auto& p = static_cast<t_object*>(a_object)->f_as<U>();
		if (p.v_entry == decltype(p.v_entry){}) xemmai::f_throw(L"already destroyed."sv);
		return p;
	}
	template<typename U>
	struct t_cast
	{
		static U f_as(auto&& a_object)
		{
			return f_cast<typename t_fundamental<U>::t_type>(std::forward<decltype(a_object)>(a_object));
		}
		static bool f_is(t_object* a_object)
		{
			return reinterpret_cast<uintptr_t>(a_object) >= c_tag__OBJECT && a_object->f_type()->f_derives<typename t_fundamental<U>::t_type>();
		}
	};
	template<typename U>
	struct t_cast<U*>
	{
		static U* f_as(auto&& a_object)
		{
			return static_cast<t_object*>(a_object) ? &f_cast<U>(std::forward<decltype(a_object)>(a_object)) : nullptr;
		}
		static bool f_is(t_object* a_object)
		{
			return reinterpret_cast<uintptr_t>(a_object) == c_tag__NULL || reinterpret_cast<uintptr_t>(a_object) >= c_tag__OBJECT && a_object->f_type()->f_derives<typename t_fundamental<U>::t_type>();
		}
	};
	using t_library = xemmaix::al::t_library;
	using t_base = t_holds;

	using t_bears<T>::t_bears;
	static void f_do_finalize(t_object* a_this)
	{
		auto& p = a_this->f_as<T>();
		assert(p.v_entry == decltype(p.v_entry){});
		p.~T();
	}
};

}

#endif
