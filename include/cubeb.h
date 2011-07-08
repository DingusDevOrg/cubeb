/*
 * Copyright © 2011 Mozilla Foundation
 *
 * This program is made available under an ISC-style license.  See the
 * accompanying file LICENSE for details.
 */
#ifndef   CUBEB_c2f983e9_c96f_e71c_72c3_bbf62992a382
#define   CUBEB_c2f983e9_c96f_e71c_72c3_bbf62992a382

#include <cubeb/cubeb-stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @mainpage

    @section intro Introduction

    This is the documentation for the <tt>libcubeb</tt> C API.
    <tt>libcubeb</tt> is a callback-based audio API library allowing the
    authoring of portable multiplatform audio playback.

    @section example Example code

    @code
    cubeb * app_ctx;
    cubeb_init(&app_ctx, "Example Application");

    cubeb_stream_params params;
    params.format = CUBEB_SAMPLE_S16LE;
    params.rate = 48000;
    params.channels = 2;

    latency = params.rate / 4;

    cubeb_stream * stm;
    cubeb_stream_init(app_ctx, &stm, "Example Stream 1", params,
                      latency, data_cb, state_cb, NULL);

    cubeb_start(stm);
    for (;;) {
      cubeb_get_time(stm, &ts);
      printf("time=%lu\n", ts);
      sleep(1);
    }
    cubeb_stop(stm);

    cubeb_stream_destroy(stm);
    cubeb_destroy(app_ctx);
    @endcode

    @code
    int data_cb(cubeb_stream * stm, void * user, void * buffer, size_t nframes)
    {
      short * buf = buffer;
      for (i = 0; i < nframes; ++i) {
        for (c = 0; c < params.channels; ++c) {
          buf[i][c] = 0;
        }
      }
      return CUBEB_OK;
    }
    @endcode

    @code
    int state_cb(cubeb_stream * stm, void * user, cubeb_state state)
    {
      printf("state=%d\n", state);
      return CUBEB_OK;
    }
    @endcode
*/


/** @file
    The <tt>libcubeb</tt> C API. */

typedef struct cubeb cubeb;               /**< Opaque handle referencing the application state. */
typedef struct cubeb_stream cubeb_stream; /**< Opaque handle referencing the stream state. */

/** Sample format enumeration. */
typedef enum {
  CUBEB_SAMPLE_U8,       /**< 8-bit unsigned PCM. */
  CUBEB_SAMPLE_S16LE,    /**< Little endian 16-bit signed PCM. */
  CUBEB_SAMPLE_FLOAT32LE /**< Little endian 32-bit IEEE floating point PCM. */
} cubeb_sample_format;

/** Stream format initialization parameters. */
typedef struct {
  cubeb_sample_format format; /**< Requested sample format.  One of
                                   #CUBEB_SAMPLE_U8, #CUBEB_SAMPLE_S16LE, or
                                   #CUBEB_SAMPLE_FLOAT32LE. */
  unsigned int rate;          /**< Requested sample rate.  Valid range is [X, Y] (XXX: fix). */
  unsigned int channels;      /**< Requested channel count.  Valid range is
                                   [X, Y]. (XXX: fix and deal with channel
                                   mapping). */
} cubeb_stream_params;

/** Stream states signaled via state_callback. */
typedef enum {
  CUBEB_STATE_STARTED, /**< Stream started. */
  CUBEB_STATE_STOPPED, /**< Stream stopped. */
  CUBEB_STATE_DRAINED /**< Stream drained. */
} cubeb_state;

/** Result code enumeration. */
enum {
  CUBEB_OK = 0,              /**< Success. */
  CUBEB_ERROR = -1,          /**< Unclassified error. */
  CUBEB_ERROR_INVALID_FORMAT /**< Unsupported #cubeb_stream_params requested. */
};

/** User supplied data callback.
    @param stream
    @param user_ptr
    @param buffer
    @param nframes
    @retval Number of frames written to buffer, which must equal nframes except at end of stream.
    @retval CUBEB_ERROR on error, in which case the data callback will stop
            and the stream will enter a shutdown state. */
typedef long (* cubeb_data_callback)(cubeb_stream * stream,
                                     void * user_ptr,
                                     void * buffer,
                                     long nframes);

/** User supplied state callback.
    @param stream
    @param user_ptr
    @param state
    @retval CUBEB_OK
    @retval CUBEB_ERROR */
typedef int (* cubeb_state_callback)(cubeb_stream * stream,
                                     void * user_ptr,
                                     cubeb_state state);

/** Initialize an application context.  This will perform any library or
    application scoped initialization.
    @param context
    @param context_name
    @retval CUBEB_OK
    @retval CUBEB_ERROR */
int cubeb_init(cubeb ** context, char const * context_name);

/** Destroy an application context.
    @param context */
void cubeb_destroy(cubeb * context);

/** Initialize a stream associated with the supplied application context.
    @param context
    @param stream
    @param stream_name
    @param stream_params
    @param latency Approximate stream latency in frames.
    @param data_callback Will be called to preroll data before playback is
                          started by cubeb_stream_start.
    @param state_callback
    @param user_ptr
    @retval CUBEB_OK
    @retval CUBEB_ERROR
    @retval CUBEB_ERROR_INVALID_FORMAT */
int cubeb_stream_init(cubeb * context, cubeb_stream ** stream, char const * stream_name,
                      cubeb_stream_params stream_params, unsigned int latency,
                      cubeb_data_callback data_callback,
                      cubeb_state_callback state_callback,
                      void * user_ptr);

/** Destroy a stream.
    @param stream */
void cubeb_stream_destroy(cubeb_stream * stream);

/** Start playback.
    @param stream
    @retval CUBEB_OK
    @retval CUBEB_ERROR */
int cubeb_stream_start(cubeb_stream * stream);

/** Stop playback.
    @param stream
    @retval CUBEB_OK
    @retval CUBEB_ERROR */
int cubeb_stream_stop(cubeb_stream * stream);

/** Get the current stream playback position.
    @param stream
    @param position Playback position in frames.
    @retval CUBEB_OK
    @retval CUBEB_ERROR */
int cubeb_stream_get_position(cubeb_stream * stream, uint64_t * position);

/** Set the stream volume.
    @param stream
    @param volume Range [0.0, 1.0].
    @retval CUBEB_OK
    @retval CUBEB_ERROR */
int cubeb_stream_set_volume(cubeb_stream * stream, float volume);

#ifdef __cplusplus
}
#endif

#endif /* CUBEB_c2f983e9_c96f_e71c_72c3_bbf62992a382 */
