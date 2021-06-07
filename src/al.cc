#include "source.h"

#include <cstring>

namespace xemmaix::al
{

using namespace xemmai;

std::atomic_flag t_session::v_running = ATOMIC_FLAG_INIT;
XEMMAI__PORTABLE__THREAD t_session* t_session::v_instance;

t_session::t_session(t_library* a_library) : v_library(a_library)
{
	if (v_running.test_and_set(std::memory_order_acquire)) xemmai::f_throw(L"main already running."sv);
	if (alutInitWithoutContext(NULL, NULL) != AL_TRUE) xemmai::f_throw(L"alutInitWithoutContext failed."sv);
	v_instance = this;
}

t_session::~t_session()
{
	while (!v_devices.empty()) v_devices.begin()->second->f_as<t_device>().f_close();
	while (!v_capture_devices.empty()) v_capture_devices.begin()->second->f_as<t_capture_device>().f_close();
	if (alutExit() != AL_TRUE) xemmai::f_throw(L"alutExit failed."sv);
	v_instance = nullptr;
	v_running.clear(std::memory_order_release);
}

namespace
{

void f_main(t_library* a_library, const t_pvalue& a_callable)
{
	t_session session(a_library);
	a_callable();
}

t_pvalue f_alc_get_string(ALCenum a_parameter)
{
	const ALCchar* p = alcGetString(NULL, a_parameter);
	return p == NULL ? t_pvalue() : f_global()->f_as(f_convert(p));
}

t_pvalue f_alc_get_strings(ALCenum a_parameter)
{
	const ALCchar* p = alcGetString(NULL, a_parameter);
	if (p == NULL) return {};
	size_t n = 0;
	for (const ALCchar* q = p; *q != '\0'; q += std::strlen(q) + 1) ++n;
	return t_tuple::f_instantiate(n, [&](auto& tuple)
	{
		for (size_t i = 0; i < n; ++i) {
			new(&tuple[i]) t_svalue(f_global()->f_as(f_convert(p)));
			p += std::strlen(p) + 1;
		}
	});
}

}

void t_library::f_scan(t_scan a_scan)
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

std::vector<std::pair<t_root, t_rvalue>> t_library::f_define()
{
	t_type_of<t_error>::f_define(this);
	t_type_of<t_base_device>::f_define(this);
	t_type_of<t_device>::f_define(this);
	t_type_of<t_capture_device>::f_define(this);
	t_type_of<t_context>::f_define(this);
	t_type_of<t_source>::f_define(this);
	t_type_of<t_buffer>::f_define(this);
	return t_define(this)
		(L"Error"sv, t_object::f_of(v_type_error))
		(L"BaseDevice"sv, t_object::f_of(v_type_base_device))
		(L"Device"sv, t_object::f_of(v_type_device))
		(L"CaptureDevice"sv, t_object::f_of(v_type_capture_device))
		(L"Context"sv, t_object::f_of(v_type_context))
		(L"Source"sv, t_object::f_of(v_type_source))
		(L"Buffer"sv, t_object::f_of(v_type_buffer))
		(L"main"sv, t_static<void(*)(t_library*, const t_pvalue&), f_main>())
		(L"alc_get_string"sv, t_static<t_pvalue(*)(ALCenum), f_alc_get_string>())
		(L"alc_get_strings"sv, t_static<t_pvalue(*)(ALCenum), f_alc_get_strings>())
		(L"SOURCE_RELATIVE"sv, AL_SOURCE_RELATIVE)
		(L"CONE_INNER_ANGLE"sv, AL_CONE_INNER_ANGLE)
		(L"CONE_OUTER_ANGLE"sv, AL_CONE_OUTER_ANGLE)
		(L"PITCH"sv, AL_PITCH)
		(L"POSITION"sv, AL_POSITION)
		(L"DIRECTION"sv, AL_DIRECTION)
		(L"VELOCITY"sv, AL_VELOCITY)
		(L"LOOPING"sv, AL_LOOPING)
		(L"BUFFER"sv, AL_BUFFER)
		(L"GAIN"sv, AL_GAIN)
		(L"MIN_GAIN"sv, AL_MIN_GAIN)
		(L"MAX_GAIN"sv, AL_MAX_GAIN)
		(L"ORIENTATION"sv, AL_ORIENTATION)
		(L"SOURCE_STATE"sv, AL_SOURCE_STATE)
		(L"INITIAL"sv, AL_INITIAL)
		(L"PLAYING"sv, AL_PLAYING)
		(L"PAUSED"sv, AL_PAUSED)
		(L"STOPPED"sv, AL_STOPPED)
		(L"BUFFERS_QUEUED"sv, AL_BUFFERS_QUEUED)
		(L"BUFFERS_PROCESSED"sv, AL_BUFFERS_PROCESSED)
		(L"SEC_OFFSET"sv, AL_SEC_OFFSET)
		(L"SAMPLE_OFFSET"sv, AL_SAMPLE_OFFSET)
		(L"BYTE_OFFSET"sv, AL_BYTE_OFFSET)
		(L"SOURCE_TYPE"sv, AL_SOURCE_TYPE)
		(L"STATIC"sv, AL_STATIC)
		(L"STREAMING"sv, AL_STREAMING)
		(L"UNDETERMINED"sv, AL_UNDETERMINED)
		(L"FORMAT_MONO8"sv, AL_FORMAT_MONO8)
		(L"FORMAT_MONO16"sv, AL_FORMAT_MONO16)
		(L"FORMAT_STEREO8"sv, AL_FORMAT_STEREO8)
		(L"FORMAT_STEREO16"sv, AL_FORMAT_STEREO16)
		(L"REFERENCE_DISTANCE"sv, AL_REFERENCE_DISTANCE)
		(L"ROLLOFF_FACTOR"sv, AL_ROLLOFF_FACTOR)
		(L"CONE_OUTER_GAIN"sv, AL_CONE_OUTER_GAIN)
		(L"MAX_DISTANCE"sv, AL_MAX_DISTANCE)
		(L"FREQUENCY"sv, AL_FREQUENCY)
		(L"BITS"sv, AL_BITS)
		(L"CHANNELS"sv, AL_CHANNELS)
		(L"SIZE"sv, AL_SIZE)
		(L"UNUSED"sv, AL_UNUSED)
		(L"PENDING"sv, AL_PENDING)
		(L"PROCESSED"sv, AL_PROCESSED)
		(L"NO_ERROR"sv, AL_NO_ERROR)
		(L"INVALID_NAME"sv, AL_INVALID_NAME)
		(L"INVALID_ENUM"sv, AL_INVALID_ENUM)
		(L"INVALID_VALUE"sv, AL_INVALID_VALUE)
		(L"INVALID_OPERATION"sv, AL_INVALID_OPERATION)
		(L"OUT_OF_MEMORY"sv, AL_OUT_OF_MEMORY)
		(L"VENDOR"sv, AL_VENDOR)
		(L"VERSION"sv, AL_VERSION)
		(L"RENDERER"sv, AL_RENDERER)
		(L"EXTENSIONS"sv, AL_EXTENSIONS)
		(L"DOPPLER_FACTOR"sv, AL_DOPPLER_FACTOR)
		(L"DOPPLER_VELOCITY"sv, AL_DOPPLER_VELOCITY)
		(L"SPEED_OF_SOUND"sv, AL_SPEED_OF_SOUND)
		(L"DISTANCE_MODEL"sv, AL_DISTANCE_MODEL)
		(L"INVERSE_DISTANCE"sv, AL_INVERSE_DISTANCE)
		(L"INVERSE_DISTANCE_CLAMPED"sv, AL_INVERSE_DISTANCE_CLAMPED)
		(L"LINEAR_DISTANCE"sv, AL_LINEAR_DISTANCE)
		(L"LINEAR_DISTANCE_CLAMPED"sv, AL_LINEAR_DISTANCE_CLAMPED)
		(L"EXPONENT_DISTANCE"sv, AL_EXPONENT_DISTANCE)
		(L"EXPONENT_DISTANCE_CLAMPED"sv, AL_EXPONENT_DISTANCE_CLAMPED)
		(L"ALC_FREQUENCY"sv, ALC_FREQUENCY)
		(L"ALC_REFRESH"sv, ALC_REFRESH)
		(L"ALC_SYNC"sv, ALC_SYNC)
		(L"ALC_MONO_SOURCES"sv, ALC_MONO_SOURCES)
		(L"ALC_STEREO_SOURCES"sv, ALC_STEREO_SOURCES)
		(L"ALC_NO_ERROR"sv, ALC_NO_ERROR)
		(L"ALC_INVALID_DEVICE"sv, ALC_INVALID_DEVICE)
		(L"ALC_INVALID_CONTEXT"sv, ALC_INVALID_CONTEXT)
		(L"ALC_INVALID_ENUM"sv, ALC_INVALID_ENUM)
		(L"ALC_INVALID_VALUE"sv, ALC_INVALID_VALUE)
		(L"ALC_OUT_OF_MEMORY"sv, ALC_OUT_OF_MEMORY)
		(L"ALC_DEFAULT_DEVICE_SPECIFIER"sv, ALC_DEFAULT_DEVICE_SPECIFIER)
		(L"ALC_DEVICE_SPECIFIER"sv, ALC_DEVICE_SPECIFIER)
		(L"ALC_EXTENSIONS"sv, ALC_EXTENSIONS)
		(L"ALC_MAJOR_VERSION"sv, ALC_MAJOR_VERSION)
		(L"ALC_MINOR_VERSION"sv, ALC_MINOR_VERSION)
		(L"ALC_ATTRIBUTES_SIZE"sv, ALC_ATTRIBUTES_SIZE)
		(L"ALC_ALL_ATTRIBUTES"sv, ALC_ALL_ATTRIBUTES)
		(L"ALC_CAPTURE_DEVICE_SPECIFIER"sv, ALC_CAPTURE_DEVICE_SPECIFIER)
		(L"ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER"sv, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER)
		(L"ALC_CAPTURE_SAMPLES"sv, ALC_CAPTURE_SAMPLES)
		(L"ALUT_ERROR_NO_ERROR"sv, ALUT_ERROR_NO_ERROR)
		(L"ALUT_ERROR_OUT_OF_MEMORY"sv, ALUT_ERROR_OUT_OF_MEMORY)
		(L"ALUT_ERROR_INVALID_ENUM"sv, ALUT_ERROR_INVALID_ENUM)
		(L"ALUT_ERROR_INVALID_VALUE"sv, ALUT_ERROR_INVALID_VALUE)
		(L"ALUT_ERROR_INVALID_OPERATION"sv, ALUT_ERROR_INVALID_OPERATION)
		(L"ALUT_ERROR_NO_CURRENT_CONTEXT"sv, ALUT_ERROR_NO_CURRENT_CONTEXT)
		(L"ALUT_ERROR_AL_ERROR_ON_ENTRY"sv, ALUT_ERROR_AL_ERROR_ON_ENTRY)
		(L"ALUT_ERROR_ALC_ERROR_ON_ENTRY"sv, ALUT_ERROR_ALC_ERROR_ON_ENTRY)
		(L"ALUT_ERROR_OPEN_DEVICE"sv, ALUT_ERROR_OPEN_DEVICE)
		(L"ALUT_ERROR_CLOSE_DEVICE"sv, ALUT_ERROR_CLOSE_DEVICE)
		(L"ALUT_ERROR_CREATE_CONTEXT"sv, ALUT_ERROR_CREATE_CONTEXT)
		(L"ALUT_ERROR_MAKE_CONTEXT_CURRENT"sv, ALUT_ERROR_MAKE_CONTEXT_CURRENT)
		(L"ALUT_ERROR_DESTROY_CONTEXT"sv, ALUT_ERROR_DESTROY_CONTEXT)
		(L"ALUT_ERROR_GEN_BUFFERS"sv, ALUT_ERROR_GEN_BUFFERS)
		(L"ALUT_ERROR_BUFFER_DATA"sv, ALUT_ERROR_BUFFER_DATA)
		(L"ALUT_ERROR_IO_ERROR"sv, ALUT_ERROR_IO_ERROR)
		(L"ALUT_ERROR_UNSUPPORTED_FILE_TYPE"sv, ALUT_ERROR_UNSUPPORTED_FILE_TYPE)
		(L"ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE"sv, ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE)
		(L"ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA"sv, ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA)
		(L"ALUT_WAVEFORM_SINE"sv, ALUT_WAVEFORM_SINE)
		(L"ALUT_WAVEFORM_SQUARE"sv, ALUT_WAVEFORM_SQUARE)
		(L"ALUT_WAVEFORM_SAWTOOTH"sv, ALUT_WAVEFORM_SAWTOOTH)
		(L"ALUT_WAVEFORM_WHITENOISE"sv, ALUT_WAVEFORM_WHITENOISE)
		(L"ALUT_WAVEFORM_IMPULSE"sv, ALUT_WAVEFORM_IMPULSE)
		(L"ALUT_LOADER_BUFFER"sv, ALUT_LOADER_BUFFER)
		(L"ALUT_LOADER_MEMORY"sv, ALUT_LOADER_MEMORY)
	;
}

}

XEMMAI__MODULE__FACTORY(xemmai::t_library::t_handle* a_handle)
{
	return xemmai::f_new<xemmaix::al::t_library>(a_handle);
}
