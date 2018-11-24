#include "source.h"

#include <cstring>

namespace xemmaix::al
{

using namespace xemmai;

std::mutex t_session::v_mutex;
bool t_session::v_running = false;
XEMMAI__PORTABLE__THREAD t_session* t_session::v_instance;

t_session::t_session(t_extension* a_extension) : v_extension(a_extension)
{
	std::unique_lock<std::mutex> lock(v_mutex);
	if (v_running) xemmai::f_throw(L"main already running."sv);
	if (alutInitWithoutContext(NULL, NULL) != AL_TRUE) xemmai::f_throw(L"alutInitWithoutContext failed."sv);
	v_running = true;
	v_instance = this;
}

t_session::~t_session()
{
	while (!v_devices.empty()) v_devices.begin()->second->f_as<t_device>().f_close();
	while (!v_capture_devices.empty()) v_capture_devices.begin()->second->f_as<t_capture_device>().f_close();
	std::unique_lock<std::mutex> lock(v_mutex);
	v_running = false;
	v_instance = nullptr;
	if (alutExit() != AL_TRUE) xemmai::f_throw(L"alutExit failed."sv);
}

namespace
{

void f_main(t_extension* a_extension, const t_value& a_callable)
{
	t_session session(a_extension);
	a_callable();
}

t_scoped f_alc_get_string(ALCenum a_parameter)
{
	const ALCchar* p = alcGetString(NULL, a_parameter);
	return p == NULL ? t_scoped() : f_global()->f_as(f_convert(p));
}

t_scoped f_alc_get_strings(ALCenum a_parameter)
{
	const ALCchar* p = alcGetString(NULL, a_parameter);
	if (p == NULL) return t_scoped();
	size_t n = 0;
	for (const ALCchar* q = p; *q != '\0'; q += std::strlen(q) + 1) ++n;
	auto q = t_tuple::f_instantiate(n);
	auto& tuple = f_as<t_tuple&>(q);
	for (size_t i = 0; i < n; ++i) {
		tuple[i].f_construct(f_global()->f_as(f_convert(p)));
		p += std::strlen(p) + 1;
	}
	return q;
}

}

t_extension::t_extension(t_object* a_module) : xemmai::t_extension(a_module)
{
	t_type_of<t_error>::f_define(this);
	t_type_of<t_base_device>::f_define(this);
	t_type_of<t_device>::f_define(this);
	t_type_of<t_capture_device>::f_define(this);
	t_type_of<t_context>::f_define(this);
	t_type_of<t_source>::f_define(this);
	t_type_of<t_buffer>::f_define(this);
	f_define<void (*)(t_extension*, const t_value&), f_main>(this, L"main"sv);
	f_define<t_scoped (*)(ALCenum), f_alc_get_string>(this, L"alc_get_string"sv);
	f_define<t_scoped (*)(ALCenum), f_alc_get_strings>(this, L"alc_get_strings"sv);
	a_module->f_put(t_symbol::f_instantiate(L"SOURCE_RELATIVE"sv), f_as(AL_SOURCE_RELATIVE));
	a_module->f_put(t_symbol::f_instantiate(L"CONE_INNER_ANGLE"sv), f_as(AL_CONE_INNER_ANGLE));
	a_module->f_put(t_symbol::f_instantiate(L"CONE_OUTER_ANGLE"sv), f_as(AL_CONE_OUTER_ANGLE));
	a_module->f_put(t_symbol::f_instantiate(L"PITCH"sv), f_as(AL_PITCH));
	a_module->f_put(t_symbol::f_instantiate(L"POSITION"sv), f_as(AL_POSITION));
	a_module->f_put(t_symbol::f_instantiate(L"DIRECTION"sv), f_as(AL_DIRECTION));
	a_module->f_put(t_symbol::f_instantiate(L"VELOCITY"sv), f_as(AL_VELOCITY));
	a_module->f_put(t_symbol::f_instantiate(L"LOOPING"sv), f_as(AL_LOOPING));
	a_module->f_put(t_symbol::f_instantiate(L"BUFFER"sv), f_as(AL_BUFFER));
	a_module->f_put(t_symbol::f_instantiate(L"GAIN"sv), f_as(AL_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"MIN_GAIN"sv), f_as(AL_MIN_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"MAX_GAIN"sv), f_as(AL_MAX_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"ORIENTATION"sv), f_as(AL_ORIENTATION));
	a_module->f_put(t_symbol::f_instantiate(L"SOURCE_STATE"sv), f_as(AL_SOURCE_STATE));
	a_module->f_put(t_symbol::f_instantiate(L"INITIAL"sv), f_as(AL_INITIAL));
	a_module->f_put(t_symbol::f_instantiate(L"PLAYING"sv), f_as(AL_PLAYING));
	a_module->f_put(t_symbol::f_instantiate(L"PAUSED"sv), f_as(AL_PAUSED));
	a_module->f_put(t_symbol::f_instantiate(L"STOPPED"sv), f_as(AL_STOPPED));
	a_module->f_put(t_symbol::f_instantiate(L"BUFFERS_QUEUED"sv), f_as(AL_BUFFERS_QUEUED));
	a_module->f_put(t_symbol::f_instantiate(L"BUFFERS_PROCESSED"sv), f_as(AL_BUFFERS_PROCESSED));
	a_module->f_put(t_symbol::f_instantiate(L"SEC_OFFSET"sv), f_as(AL_SEC_OFFSET));
	a_module->f_put(t_symbol::f_instantiate(L"SAMPLE_OFFSET"sv), f_as(AL_SAMPLE_OFFSET));
	a_module->f_put(t_symbol::f_instantiate(L"BYTE_OFFSET"sv), f_as(AL_BYTE_OFFSET));
	a_module->f_put(t_symbol::f_instantiate(L"SOURCE_TYPE"sv), f_as(AL_SOURCE_TYPE));
	a_module->f_put(t_symbol::f_instantiate(L"STATIC"sv), f_as(AL_STATIC));
	a_module->f_put(t_symbol::f_instantiate(L"STREAMING"sv), f_as(AL_STREAMING));
	a_module->f_put(t_symbol::f_instantiate(L"UNDETERMINED"sv), f_as(AL_UNDETERMINED));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_MONO8"sv), f_as(AL_FORMAT_MONO8));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_MONO16"sv), f_as(AL_FORMAT_MONO16));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_STEREO8"sv), f_as(AL_FORMAT_STEREO8));
	a_module->f_put(t_symbol::f_instantiate(L"FORMAT_STEREO16"sv), f_as(AL_FORMAT_STEREO16));
	a_module->f_put(t_symbol::f_instantiate(L"REFERENCE_DISTANCE"sv), f_as(AL_REFERENCE_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"ROLLOFF_FACTOR"sv), f_as(AL_ROLLOFF_FACTOR));
	a_module->f_put(t_symbol::f_instantiate(L"CONE_OUTER_GAIN"sv), f_as(AL_CONE_OUTER_GAIN));
	a_module->f_put(t_symbol::f_instantiate(L"MAX_DISTANCE"sv), f_as(AL_MAX_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"FREQUENCY"sv), f_as(AL_FREQUENCY));
	a_module->f_put(t_symbol::f_instantiate(L"BITS"sv), f_as(AL_BITS));
	a_module->f_put(t_symbol::f_instantiate(L"CHANNELS"sv), f_as(AL_CHANNELS));
	a_module->f_put(t_symbol::f_instantiate(L"SIZE"sv), f_as(AL_SIZE));
	a_module->f_put(t_symbol::f_instantiate(L"UNUSED"sv), f_as(AL_UNUSED));
	a_module->f_put(t_symbol::f_instantiate(L"PENDING"sv), f_as(AL_PENDING));
	a_module->f_put(t_symbol::f_instantiate(L"PROCESSED"sv), f_as(AL_PROCESSED));
	a_module->f_put(t_symbol::f_instantiate(L"NO_ERROR"sv), f_as(AL_NO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_NAME"sv), f_as(AL_INVALID_NAME));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_ENUM"sv), f_as(AL_INVALID_ENUM));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_VALUE"sv), f_as(AL_INVALID_VALUE));
	a_module->f_put(t_symbol::f_instantiate(L"INVALID_OPERATION"sv), f_as(AL_INVALID_OPERATION));
	a_module->f_put(t_symbol::f_instantiate(L"OUT_OF_MEMORY"sv), f_as(AL_OUT_OF_MEMORY));
	a_module->f_put(t_symbol::f_instantiate(L"VENDOR"sv), f_as(AL_VENDOR));
	a_module->f_put(t_symbol::f_instantiate(L"VERSION"sv), f_as(AL_VERSION));
	a_module->f_put(t_symbol::f_instantiate(L"RENDERER"sv), f_as(AL_RENDERER));
	a_module->f_put(t_symbol::f_instantiate(L"EXTENSIONS"sv), f_as(AL_EXTENSIONS));
	a_module->f_put(t_symbol::f_instantiate(L"DOPPLER_FACTOR"sv), f_as(AL_DOPPLER_FACTOR));
	a_module->f_put(t_symbol::f_instantiate(L"DOPPLER_VELOCITY"sv), f_as(AL_DOPPLER_VELOCITY));
	a_module->f_put(t_symbol::f_instantiate(L"SPEED_OF_SOUND"sv), f_as(AL_SPEED_OF_SOUND));
	a_module->f_put(t_symbol::f_instantiate(L"DISTANCE_MODEL"sv), f_as(AL_DISTANCE_MODEL));
	a_module->f_put(t_symbol::f_instantiate(L"INVERSE_DISTANCE"sv), f_as(AL_INVERSE_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"INVERSE_DISTANCE_CLAMPED"sv), f_as(AL_INVERSE_DISTANCE_CLAMPED));
	a_module->f_put(t_symbol::f_instantiate(L"LINEAR_DISTANCE"sv), f_as(AL_LINEAR_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"LINEAR_DISTANCE_CLAMPED"sv), f_as(AL_LINEAR_DISTANCE_CLAMPED));
	a_module->f_put(t_symbol::f_instantiate(L"EXPONENT_DISTANCE"sv), f_as(AL_EXPONENT_DISTANCE));
	a_module->f_put(t_symbol::f_instantiate(L"EXPONENT_DISTANCE_CLAMPED"sv), f_as(AL_EXPONENT_DISTANCE_CLAMPED));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_FREQUENCY"sv), f_as(ALC_FREQUENCY));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_REFRESH"sv), f_as(ALC_REFRESH));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_SYNC"sv), f_as(ALC_SYNC));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_MONO_SOURCES"sv), f_as(ALC_MONO_SOURCES));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_STEREO_SOURCES"sv), f_as(ALC_STEREO_SOURCES));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_NO_ERROR"sv), f_as(ALC_NO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_DEVICE"sv), f_as(ALC_INVALID_DEVICE));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_CONTEXT"sv), f_as(ALC_INVALID_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_ENUM"sv), f_as(ALC_INVALID_ENUM));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_INVALID_VALUE"sv), f_as(ALC_INVALID_VALUE));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_OUT_OF_MEMORY"sv), f_as(ALC_OUT_OF_MEMORY));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_DEFAULT_DEVICE_SPECIFIER"sv), f_as(ALC_DEFAULT_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_DEVICE_SPECIFIER"sv), f_as(ALC_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_EXTENSIONS"sv), f_as(ALC_EXTENSIONS));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_MAJOR_VERSION"sv), f_as(ALC_MAJOR_VERSION));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_MINOR_VERSION"sv), f_as(ALC_MINOR_VERSION));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_ATTRIBUTES_SIZE"sv), f_as(ALC_ATTRIBUTES_SIZE));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_ALL_ATTRIBUTES"sv), f_as(ALC_ALL_ATTRIBUTES));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_CAPTURE_DEVICE_SPECIFIER"sv), f_as(ALC_CAPTURE_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER"sv), f_as(ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
	a_module->f_put(t_symbol::f_instantiate(L"ALC_CAPTURE_SAMPLES"sv), f_as(ALC_CAPTURE_SAMPLES));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_NO_ERROR"sv), f_as(ALUT_ERROR_NO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_OUT_OF_MEMORY"sv), f_as(ALUT_ERROR_OUT_OF_MEMORY));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_INVALID_ENUM"sv), f_as(ALUT_ERROR_INVALID_ENUM));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_INVALID_VALUE"sv), f_as(ALUT_ERROR_INVALID_VALUE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_INVALID_OPERATION"sv), f_as(ALUT_ERROR_INVALID_OPERATION));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_NO_CURRENT_CONTEXT"sv), f_as(ALUT_ERROR_NO_CURRENT_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_AL_ERROR_ON_ENTRY"sv), f_as(ALUT_ERROR_AL_ERROR_ON_ENTRY));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_ALC_ERROR_ON_ENTRY"sv), f_as(ALUT_ERROR_ALC_ERROR_ON_ENTRY));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_OPEN_DEVICE"sv), f_as(ALUT_ERROR_OPEN_DEVICE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_CLOSE_DEVICE"sv), f_as(ALUT_ERROR_CLOSE_DEVICE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_CREATE_CONTEXT"sv), f_as(ALUT_ERROR_CREATE_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_MAKE_CONTEXT_CURRENT"sv), f_as(ALUT_ERROR_MAKE_CONTEXT_CURRENT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_DESTROY_CONTEXT"sv), f_as(ALUT_ERROR_DESTROY_CONTEXT));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_GEN_BUFFERS"sv), f_as(ALUT_ERROR_GEN_BUFFERS));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_BUFFER_DATA"sv), f_as(ALUT_ERROR_BUFFER_DATA));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_IO_ERROR"sv), f_as(ALUT_ERROR_IO_ERROR));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_UNSUPPORTED_FILE_TYPE"sv), f_as(ALUT_ERROR_UNSUPPORTED_FILE_TYPE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE"sv), f_as(ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA"sv), f_as(ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_SINE"sv), f_as(ALUT_WAVEFORM_SINE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_SQUARE"sv), f_as(ALUT_WAVEFORM_SQUARE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_SAWTOOTH"sv), f_as(ALUT_WAVEFORM_SAWTOOTH));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_WHITENOISE"sv), f_as(ALUT_WAVEFORM_WHITENOISE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_WAVEFORM_IMPULSE"sv), f_as(ALUT_WAVEFORM_IMPULSE));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_LOADER_BUFFER"sv), f_as(ALUT_LOADER_BUFFER));
	a_module->f_put(t_symbol::f_instantiate(L"ALUT_LOADER_MEMORY"sv), f_as(ALUT_LOADER_MEMORY));
}

void t_extension::f_scan(t_scan a_scan)
{
	a_scan(v_type_error);
	a_scan(v_type_alc_error);
	a_scan(v_type_alut_error);
	a_scan(v_type_base_device);
	a_scan(v_type_device);
	a_scan(v_type_capture_device);
	a_scan(v_type_context);
	a_scan(v_type_source);
	a_scan(v_type_buffer);
}

}

XEMMAI__MODULE__FACTORY(xemmai::t_object* a_module)
{
	return new xemmaix::al::t_extension(a_module);
}
