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

	static std::mutex v_mutex;
	static bool v_running;
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
	t_library* f_library() const
	{
		return v_library;
	}
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
	virtual void f_scan(t_scan a_scan);
	virtual std::vector<std::pair<t_root, t_rvalue>> f_define();
	template<typename T>
	const T* f_library() const
	{
		return f_global();
	}
	template<typename T>
	t_slot_of<t_type>& f_type_slot()
	{
		return f_global()->f_type_slot<T>();
	}
	template<typename T>
	t_type* f_type() const
	{
		return const_cast<t_library*>(this)->f_type_slot<T>();
	}
	template<typename T>
	t_pvalue f_as(T&& a_value) const
	{
		typedef t_type_of<typename t_fundamental<T>::t_type> t;
		return t::f_transfer(f_library<typename t::t_library>(), std::forward<T>(a_value));
	}
};

template<>
inline const t_library* t_library::f_library<t_library>() const
{
	return this;
}

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
	template<typename T0>
	struct t_cast
	{
		template<typename T1>
		static T0& f_call(T1&& a_object)
		{
			xemmaix::al::t_session::f_instance();
			auto& p = t_base::f_object(std::forward<T1>(a_object))->template f_as<T0>();
			if (p.v_entry == decltype(p.v_entry){}) xemmai::f_throw(L"already destroyed."sv);
			return p;
		}
	};
	template<typename T0>
	struct t_as
	{
		template<typename T1>
		static T0 f_call(T1&& a_object)
		{
			return t_cast<typename t_fundamental<T0>::t_type>::f_call(std::forward<T1>(a_object));
		}
	};
	template<typename T0>
	struct t_as<T0*>
	{
		template<typename T1>
		static T0* f_call(T1&& a_object)
		{
			return t_base::f_object(std::forward<T1>(a_object)) ? &t_cast<T0>::f_call(std::forward<T1>(a_object)) : nullptr;
		}
	};
	template<typename T0>
	struct t_is
	{
		template<typename T1>
		static bool f_call(T1&& a_object)
		{
			auto p = t_base::f_object(std::forward<T1>(a_object));
			return reinterpret_cast<uintptr_t>(p) >= e_tag__OBJECT && p->f_type()->template f_derives<typename t_fundamental<T0>::t_type>();
		}
	};
	template<typename T0>
	struct t_is<T0*>
	{
		template<typename T1>
		static bool f_call(T1&& a_object)
		{
			auto p = t_base::f_object(std::forward<T1>(a_object));
			switch (reinterpret_cast<uintptr_t>(p)) {
			case e_tag__NULL:
				return true;
			case e_tag__BOOLEAN:
			case e_tag__INTEGER:
			case e_tag__FLOAT:
				return false;
			default:
				return p->f_type()->template f_derives<typename t_fundamental<T0>::t_type>();
			}
		}
	};
	typedef xemmaix::al::t_library t_library;
	typedef t_holds t_base;

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
