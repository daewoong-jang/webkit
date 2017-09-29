/*
 * Copyright (C) 2017 Daewoong Jang.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "MediaPlayerPrivateAndroid.h"

#include "MediaPlayer.h"
#include "NotImplemented.h"
#include "WebCorePlatformNatives.h"
#include "WebMediaPlayerNatives.h"
#include <java/lang/Natives/String.h>
#include <java/util/Natives/Vector.h>
#include <wtf/NeverDestroyed.h>

namespace WebCore {

MediaPlayerPrivateAndroid::MediaPlayerPrivateAndroid(MediaPlayer* player)
    : m_player(player)
    , m_backend(WebMediaPlayer::create())
{
}

MediaPlayerPrivateAndroid::~MediaPlayerPrivateAndroid()
{
}

static HashSet<String, ASCIICaseInsensitiveHash>& supportedMIMETypes()
{
    static NeverDestroyed<HashSet<String, ASCIICaseInsensitiveHash>> cachedTypes;

    if (cachedTypes.get().size() > 0)
        return cachedTypes;

    JNI::LocalRef<java::util::Natives::Vector> mimeTypes = WebCorePlatform::getSupportedMIMETypes();

    for (int32_t index = 0; index < mimeTypes->size(); ++index) {
        JNI::LocalRef<java::lang::Natives::String> value = mimeTypes->elementAt(index).as<java::lang::Natives::String>();
        String type = String::fromUTF8(reinterpret_cast<const char*>(value->getBytes().data()), value->getBytes().count());
        cachedTypes.get().add(type);
    }

    return cachedTypes;
}

static MediaPlayer::SupportsType supportsType(const MediaEngineSupportParameters& parameters)
{
    if (parameters.type.isEmpty())
        return MediaPlayer::IsNotSupported;

    if (supportedMIMETypes().contains(parameters.type.containerType()))
        return MediaPlayer::IsSupported;

    return MediaPlayer::IsNotSupported;
}

static void getSupportedTypes(HashSet<String, ASCIICaseInsensitiveHash>& types)
{
    types = supportedMIMETypes();
}

void MediaPlayerPrivateAndroid::registerMediaEngine(MediaEngineRegistrar registrar)
{
    registrar([](MediaPlayer* player) { return std::make_unique<MediaPlayerPrivateAndroid>(player); },
        getSupportedTypes, supportsType, 0, 0, 0, 0);
}

void MediaPlayerPrivateAndroid::load(const String& url)
{
    notImplemented();
}

#if ENABLE(MEDIA_SOURCE)
void MediaPlayerPrivateAndroid::load(const String& url, MediaSourcePrivateClient*)
{
    notImplemented();
}
#endif

#if ENABLE(MEDIA_STREAM)
void MediaPlayerPrivateAndroid::load(MediaStreamPrivate&)
{
    notImplemented();
}
#endif

void MediaPlayerPrivateAndroid::cancelLoad()
{
    notImplemented();
}

void MediaPlayerPrivateAndroid::prepareToPlay()
{
    notImplemented();
}

PlatformMedia MediaPlayerPrivateAndroid::platformMedia() const
{
    notImplemented();
    return { PlatformMedia::None, { nullptr } };
}

PlatformLayer* MediaPlayerPrivateAndroid::platformLayer() const
{
    return const_cast<MediaPlayerPrivateAndroid*>(this);
}

void MediaPlayerPrivateAndroid::play()
{
    notImplemented();
}

void MediaPlayerPrivateAndroid::pause()
{
    notImplemented();
}

FloatSize MediaPlayerPrivateAndroid::naturalSize() const
{
    notImplemented();
    return { };
}

bool MediaPlayerPrivateAndroid::hasVideo() const
{
    notImplemented();
    return 0;
}

bool MediaPlayerPrivateAndroid::hasAudio() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::setVisible(bool)
{
    notImplemented();
}

float MediaPlayerPrivateAndroid::duration() const
{
    notImplemented();
    return 0;
}

float MediaPlayerPrivateAndroid::currentTime() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::seek(float)
{
    notImplemented();
}

bool MediaPlayerPrivateAndroid::seeking() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::setRate(float)
{
    notImplemented();
}

double MediaPlayerPrivateAndroid::rate() const
{
    notImplemented();
    return 0;
}

bool MediaPlayerPrivateAndroid::paused() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::setVolume(float)
{
    notImplemented();
}

bool MediaPlayerPrivateAndroid::supportsMuting() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::setMuted(bool)
{
    notImplemented();
}

double MediaPlayerPrivateAndroid::maxFastForwardRate() const
{
    notImplemented();
    return 0;
}

double MediaPlayerPrivateAndroid::minFastReverseRate() const
{
    notImplemented();
    return 0;
}

MediaPlayer::NetworkState MediaPlayerPrivateAndroid::networkState() const
{
    notImplemented();
    return MediaPlayer::NetworkState::Empty;
}

MediaPlayer::ReadyState MediaPlayerPrivateAndroid::readyState() const
{
    notImplemented();
    return MediaPlayer::ReadyState::HaveNothing;
}

float MediaPlayerPrivateAndroid::maxTimeSeekable() const
{
    notImplemented();
    return 0;
}

double MediaPlayerPrivateAndroid::minTimeSeekable() const
{
    notImplemented();
    return 0;
}

std::unique_ptr<PlatformTimeRanges> MediaPlayerPrivateAndroid::buffered() const
{
    notImplemented();
    return std::make_unique<PlatformTimeRanges>(MediaTime::zeroTime(), MediaTime::zeroTime());
}

unsigned long long MediaPlayerPrivateAndroid::totalBytes() const
{
    notImplemented();
    return 0;
}

bool MediaPlayerPrivateAndroid::didLoadingProgress() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::setSize(const IntSize&)
{
    notImplemented();
}

void MediaPlayerPrivateAndroid::paint(GraphicsContext&, const FloatRect&)
{
    notImplemented();
}

void MediaPlayerPrivateAndroid::paintCurrentFrameInContext(GraphicsContext& c, const FloatRect& r)
{
    notImplemented();
}

bool MediaPlayerPrivateAndroid::copyVideoTextureToPlatformTexture(GraphicsContext3D*, Platform3DObject, GC3Denum, GC3Dint, GC3Denum, GC3Denum, GC3Denum, bool, bool)
{
    notImplemented();
    return 0;
}

NativeImagePtr MediaPlayerPrivateAndroid::nativeImageForCurrentTime()
{
    notImplemented();
    return 0;
}

bool MediaPlayerPrivateAndroid::canLoadPoster() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::setPoster(const String&)
{
    notImplemented();
}

bool MediaPlayerPrivateAndroid::supportsAcceleratedRendering() const
{
    return true;
}

void MediaPlayerPrivateAndroid::acceleratedRenderingStateChanged()
{
    notImplemented();
}

bool MediaPlayerPrivateAndroid::shouldMaintainAspectRatio() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::setShouldMaintainAspectRatio(bool)
{
    notImplemented();
}

bool MediaPlayerPrivateAndroid::hasSingleSecurityOrigin() const
{
    notImplemented();
    return 0;
}

bool MediaPlayerPrivateAndroid::didPassCORSAccessCheck() const
{
    notImplemented();
    return 0;
}

MediaPlayer::MovieLoadType MediaPlayerPrivateAndroid::movieLoadType() const
{
    notImplemented();
    return MediaPlayer::MovieLoadType::Unknown;
}

void MediaPlayerPrivateAndroid::prepareForRendering()
{
    notImplemented();
}

void MediaPlayerPrivateAndroid::setPrivateBrowsingMode(bool)
{
    notImplemented();
}

bool MediaPlayerPrivateAndroid::ended() const
{
    notImplemented();
    return 0;
}

void MediaPlayerPrivateAndroid::swapBuffersIfNeeded()
{
    notImplemented();
}

} // namespace WebCore
