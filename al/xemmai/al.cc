#include "source.h"

#include <xemmai/tuple.h>

namespace al
{

namespace xemmai
{

using ::xemmai::t_tuple;
using ::xemmai::t_symbol;
using ::xemmai::f_define;
using ::xemmai::portable::t_mutex;
using ::xemmai::portable::t_scoped_lock;

t_transfer f_tuple(const t_transfer& a_0, const t_transfer& a_1, const t_transfer& a_2)
{
	t_transfer p = t_tuple::f_instantiate(3);
	t_tuple& tuple = f_as<t_tuple&>(p);
	tuple[0].f_construct(a_0);
	tuple[1].f_construct(a_1);
	tuple[2].f_construct(a_2);
	return p;
}

t_transfer f_tuple(const t_transfer& a_0, const t_transfer& a_1, const t_transfer& a_2, const t_transfer& a_3, const t_transfer& a_4, const t_transfer& a_5)
{
	t_transfer p = t_tuple::f_instantiate(6);
	t_tuple& tuple = f_as<t_tuple&>(p);
	tuple[0].f_construct(a_0);
	tuple[1].f_construct(a_1);
	tuple[2].f_construct(a_2);
	tuple[3].f_construct(a_3);
	tuple[4].f_construct(a_4);
	tuple[5].f_construct(a_5);
	return p;
}

XEMMAI__PORTABLE__THREAD t_session* t_session::v_instance;

namespace
{

t_mutex v_mutex;
bool v_running;

}

t_session::t_session(t_extension* a_extension) : v_extension(a_extension)
{
	{
		t_scoped_lock lock(v_mutex);
		if (v_running) t_throwable::f_throw(L"main already running.");
		v_running = true;
	}
	v_instance = this;
	if (alutInitWithoutContext(NULL, NULL) != AL_TRUE) t_throwable::f_throw(L"alutInitWithoutContext failed.");
}

t_session::~t_session()
{
	while (!v_devices.empty()) f_as<t_device&>(v_devices.begin()->second).f_close();
	if (alutExit() != AL_TRUE) t_throwable::f_throw(L"alutExit failed.");
	v_instance = 0;
	t_scoped_lock lock(v_mutex);
	v_running = false;
}

namespace
{

void f_main(t_extension* a_extension, const t_value& a_callable)
{
	t_session session(a_extension);
	a_callable();
}

}

t_extension::t_extension(t_object* a_module) : ::xemmai::t_extension(a_module)
{
	t_type_of<t_error>::f_define(this);
	t_type_of<t_device>::f_define(this);
	t_type_of<t_context>::f_define(this);
	t_type_of<t_source>::f_define(this);
	t_type_of<t_buffer>::f_define(this);
	f_define<void (*)(t_extension*, const t_value&), f_main>(this, L"main");
	a_module->f_put(t_symbol::f_instantiate(L"SOURCE_RELATIVE"), f_as(AL_SOURCE_RELATIVE));
	a_module->f_put(t_symbol::f_instantiate(L"CONE_INNER_ANGLE"), f_as(AL_CONE_INNER_ANGLE));
	a_module->f_put(t_symbol::f_instantiate(L"CONE_OUTER_ANGLE"), f_as(AL_CONE_OUTER_ANGLE));
	a_module->f_put(t_symbol::f_instantiate(L"PITCH"), f_as(AL_PITCH));
	a_module->f_put(t_symbol::f_instantiate(L"POSITION"), f_as(AL_POSITION));
	a_module->f_put(t_symbol::f_instantiate(L"DIRECTION"), f_as(AL_DIRECTION));
	a_module->f_put(t_symbol::f_instantiate(L"VELOCITY"), f_as(AL_VELOCITY));
	a_module->f_put(t_symbol::f_instantiate(L"LOOPING"), f_as(AL_LOOPING));
	a_module->f_put(t_symbol::f_instantiate(L"BUFFER"), f_as(AL_BUFFER));
	a_module->f_put(t_symbol::f_instantiate(L"GAIN"), f_as(AL_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"MIN_GAIN"), f_as(AL_MIN_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"MAX_GAIN"), f_as(AL_MAX_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"ORIENTATION"), f_as(AL_ORIENTATION));
	a_module->f_put(t_symbol::f_instantiate(L"SOURCE_STATE"), f_as(AL_SOURCE_STATE));
	a_module->f_put(t_symbol::f_instantiate(L"INITIAL"), f_as(AL_INITIAL));
	a_module->f_put(t_symbol::f_instantiate(L"PLAYING"), f_as(AL_PLAYING));
	a_module->f_put(t_symbol::f_instantiate(L"PAUSED"), f_as(AL_PAUSED));
	a_module->f_put(t_symbol::f_instantiate(L"STOPPED"), f_as(AL_STOPPED));
	a_module->f_put(t_symbol::f_instantiate(L"BUFFERS_QUEUED"), f_as(AL_BUFFERS_QUEUED));
	a_module->f_put(t_symbol::f_instantiate(L"BUFFERS_PROCESSED"), f_as(AL_BUFFERS_PROCESSED));
	a_module->f_put(t_symbol::f_instantiate(L"SEC_OFFSET"), f_as(AL_SEC_OFFSET));
	a_module->f_put(t_symbol::f_instantiate(L"SAMPLE_OFFSET"), f_as(AL_SAMPLE_OFFSET));
	a_module->f_put(t_symbol::f_instantiate(L"BYTE_OFFSET"), f_as(AL_BYTE_OFFSET));
	a_module->f_put(t_symbol::f_instantiate(L"SOURCE_TYPE"), f_as(AL_SOURCE_TYPE));
	a_module->f_put(t_symbol::f_instantiate(L"STATIC"), f_as(AL_STATIC));
	a_module->f_put(t_symbol::f_instantiate(L"STREAMING"), f_as(AL_STREAMING));
	a_module->f_put(t_symbol::f_instantiate(L"UNDETERMINED"), f_as(AL_UNDETERMINED));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_MONO8"), f_as(AL_FORMAT_MONO8));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_MONO16"), f_as(AL_FORMAT_MONO16));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_STEREO8"), f_as(AL_FORMAT_STEREO8));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_STEREO16"), f_as(AL_FORMAT_STEREO16));
	a_module->f_put(t_symbol::f_instantiate(L"REFERENCE_DISTANCE"), f_as(AL_REFERENCE_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"ROLLOFF_FACTOR"), f_as(AL_ROLLOFF_FACTOR));
	a_module->f_put(t_symbol::f_instantiate(L"CONE_OUTER_GAIN"), f_as(AL_CONE_OUTER_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"MAX_DISTANCE"), f_as(AL_MAX_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"FREQUENCY"), f_as(AL_FREQUENCY));
	a_module->f_put(t_symbol::f_instantiate(L"BITS"), f_as(AL_BITS));
	a_module->f_put(t_symbol::f_instantiate(L"CHANNELS"), f_as(AL_CHANNELS));
	a_module->f_put(t_symbol::f_instantiate(L"SIZE"), f_as(AL_SIZE));
	a_module->f_put(t_symbol::f_instantiate(L"UNUSED"), f_as(AL_UNUSED));
	a_module->f_put(t_symbol::f_instantiate(L"PENDING"), f_as(AL_PENDING));
	a_module->f_put(t_symbol::f_instantiate(L"PROCESSED"), f_as(AL_PROCESSED));
	a_module->f_put(t_symbol::f_instantiate(L"NO_ERROR"), f_as(AL_NO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_NAME"), f_as(AL_INVALID_NAME));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_ENUM"), f_as(AL_INVALID_ENUM));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_VALUE"), f_as(AL_INVALID_VALUE));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_OPERATION"), f_as(AL_INVALID_OPERATION));
	a_module->f_put(t_symbol::f_instantiate(L"OUT_OF_MEMORY"), f_as(AL_OUT_OF_MEMORY));
	a_module->f_put(t_symbol::f_instantiate(L"VENDOR"), f_as(AL_VENDOR));
	a_module->f_put(t_symbol::f_instantiate(L"VERSION"), f_as(AL_VERSION));
	a_module->f_put(t_symbol::f_instantiate(L"RENDERER"), f_as(AL_RENDERER));
	a_module->f_put(t_symbol::f_instantiate(L"EXTENSIONS"), f_as(AL_EXTENSIONS));
	a_module->f_put(t_symbol::f_instantiate(L"DOPPLER_FACTOR"), f_as(AL_DOPPLER_FACTOR));
	a_module->f_put(t_symbol::f_instantiate(L"DOPPLER_VELOCITY"), f_as(AL_DOPPLER_VELOCITY));
	a_module->f_put(t_symbol::f_instantiate(L"SPEED_OF_SOUND"), f_as(AL_SPEED_OF_SOUND));
	a_module->f_put(t_symbol::f_instantiate(L"DISTANCE_MODEL"), f_as(AL_DISTANCE_MODEL));
	a_module->f_put(t_symbol::f_instantiate(L"INVERSE_DISTANCE"), f_as(AL_INVERSE_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"INVERSE_DISTANCE_CLAMPED"), f_as(AL_INVERSE_DISTANCE_CLAMPED));
	a_module->f_put(t_symbol::f_instantiate(L"LINEAR_DISTANCE"), f_as(AL_LINEAR_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"LINEAR_DISTANCE_CLAMPED"), f_as(AL_LINEAR_DISTANCE_CLAMPED));
	a_module->f_put(t_symbol::f_instantiate(L"EXPONENT_DISTANCE"), f_as(AL_EXPONENT_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"EXPONENT_DISTANCE_CLAMPED"), f_as(AL_EXPONENT_DISTANCE_CLAMPED));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_FREQUENCY"), f_as(ALC_FREQUENCY));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_REFRESH"), f_as(ALC_REFRESH));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_SYNC"), f_as(ALC_SYNC));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_MONO_SOURCES"), f_as(ALC_MONO_SOURCES));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_STEREO_SOURCES"), f_as(ALC_STEREO_SOURCES));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_NO_ERROR"), f_as(ALC_NO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_DEVICE"), f_as(ALC_INVALID_DEVICE));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_CONTEXT"), f_as(ALC_INVALID_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_ENUM"), f_as(ALC_INVALID_ENUM));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_VALUE"), f_as(ALC_INVALID_VALUE));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_OUT_OF_MEMORY"), f_as(ALC_OUT_OF_MEMORY));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_DEFAULT_DEVICE_SPECIFIER"), f_as(ALC_DEFAULT_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_DEVICE_SPECIFIER"), f_as(ALC_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_EXTENSIONS"), f_as(ALC_EXTENSIONS));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_MAJOR_VERSION"), f_as(ALC_MAJOR_VERSION));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_MINOR_VERSION"), f_as(ALC_MINOR_VERSION));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_ATTRIBUTES_SIZE"), f_as(ALC_ATTRIBUTES_SIZE));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_ALL_ATTRIBUTES"), f_as(ALC_ALL_ATTRIBUTES));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_CAPTURE_DEVICE_SPECIFIER"), f_as(ALC_CAPTURE_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER"), f_as(ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_CAPTURE_SAMPLES"), f_as(ALC_CAPTURE_SAMPLES));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_NO_ERROR"), f_as(ALUT_ERROR_NO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_OUT_OF_MEMORY"), f_as(ALUT_ERROR_OUT_OF_MEMORY));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_INVALID_ENUM"), f_as(ALUT_ERROR_INVALID_ENUM));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_INVALID_VALUE"), f_as(ALUT_ERROR_INVALID_VALUE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_INVALID_OPERATION"), f_as(ALUT_ERROR_INVALID_OPERATION));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_NO_CURRENT_CONTEXT"), f_as(ALUT_ERROR_NO_CURRENT_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_AL_ERROR_ON_ENTRY"), f_as(ALUT_ERROR_AL_ERROR_ON_ENTRY));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_ALC_ERROR_ON_ENTRY"), f_as(ALUT_ERROR_ALC_ERROR_ON_ENTRY));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_OPEN_DEVICE"), f_as(ALUT_ERROR_OPEN_DEVICE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_CLOSE_DEVICE"), f_as(ALUT_ERROR_CLOSE_DEVICE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_CREATE_CONTEXT"), f_as(ALUT_ERROR_CREATE_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_MAKE_CONTEXT_CURRENT"), f_as(ALUT_ERROR_MAKE_CONTEXT_CURRENT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_DESTROY_CONTEXT"), f_as(ALUT_ERROR_DESTROY_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_GEN_BUFFERS"), f_as(ALUT_ERROR_GEN_BUFFERS));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_BUFFER_DATA"), f_as(ALUT_ERROR_BUFFER_DATA));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_IO_ERROR"), f_as(ALUT_ERROR_IO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_UNSUPPORTED_FILE_TYPE"), f_as(ALUT_ERROR_UNSUPPORTED_FILE_TYPE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE"), f_as(ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA"), f_as(ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_SINE"), f_as(ALUT_WAVEFORM_SINE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_SQUARE"), f_as(ALUT_WAVEFORM_SQUARE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_SAWTOOTH"), f_as(ALUT_WAVEFORM_SAWTOOTH));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_WHITENOISE"), f_as(ALUT_WAVEFORM_WHITENOISE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_IMPULSE"), f_as(ALUT_WAVEFORM_IMPULSE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_LOADER_BUFFER"), f_as(ALUT_LOADER_BUFFER));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_LOADER_MEMORY"), f_as(ALUT_LOADER_MEMORY));
}

void t_extension::f_scan(t_scan a_scan)
{
	a_scan(v_type_error);
	a_scan(v_type_alc_error);
	a_scan(v_type_alut_error);
	a_scan(v_type_device);
	a_scan(v_type_context);
	a_scan(v_type_source);
	a_scan(v_type_buffer);
}

}

}

#ifdef _MSC_VER
extern "C" __declspec(dllexport) xemmai::t_extension* f_factory(xemmai::t_object* a_module)
#else
extern "C" xemmai::t_extension* f_factory(xemmai::t_object* a_module)
#endif
{
	return new al::xemmai::t_extension(a_module);
}
