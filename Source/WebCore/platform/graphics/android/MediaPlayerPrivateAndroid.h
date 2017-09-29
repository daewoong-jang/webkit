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

#ifndef MediaPlayerPrivateAndroid_h
#define MediaPlayerPrivateAndroid_h

#include "MediaPlayerPrivate.h"
#include "TextureMapperPlatformLayerProxy.h"
#include "TextureMapperPlatformLayerProxyProvider.h"
#include <androidjni/GlobalRef.h>

namespace org {
namespace webkit {
namespace Natives {
class WebMediaPlayer;
}
}
}

namespace WebCore {

using WebMediaPlayer = org::webkit::Natives::WebMediaPlayer;

class MediaPlayerPrivateAndroid final : public MediaPlayerPrivateInterface, public PlatformLayer {
    WTF_MAKE_NONCOPYABLE(MediaPlayerPrivateAndroid);
public:
    explicit MediaPlayerPrivateAndroid(MediaPlayer*);
    ~MediaPlayerPrivateAndroid();
    static void registerMediaEngine(MediaEngineRegistrar);

    void load(const String& url) override;
#if ENABLE(MEDIA_SOURCE)
    void load(const String& url, MediaSourcePrivateClient*) override;
#endif
#if ENABLE(MEDIA_STREAM)
    void load(MediaStreamPrivate&) override;
#endif
    void cancelLoad() override;
    
    void prepareToPlay() override;
    PlatformMedia platformMedia() const override;
    PlatformLayer* platformLayer() const override;

    void play() override;
    void pause() override;

    FloatSize naturalSize() const override;

    bool hasVideo() const override;
    bool hasAudio() const override;

    void setVisible(bool) override;

    float duration() const override;

    float currentTime() const override;

    void seek(float) override;

    bool seeking() const override;

    void setRate(float) override;
    double rate() const override;

    bool paused() const override;

    void setVolume(float) override;

    bool supportsMuting() const override;
    void setMuted(bool) override;

    double maxFastForwardRate() const override;
    double minFastReverseRate() const override;

    MediaPlayer::NetworkState networkState() const override;
    MediaPlayer::ReadyState readyState() const override;

    float maxTimeSeekable() const override;
    double minTimeSeekable() const override;
    std::unique_ptr<PlatformTimeRanges> buffered() const override;

    unsigned long long totalBytes() const override;
    bool didLoadingProgress() const override;

    void setSize(const IntSize&) override;

    void paint(GraphicsContext&, const FloatRect&) override;

    void paintCurrentFrameInContext(GraphicsContext& c, const FloatRect& r) override;
    bool copyVideoTextureToPlatformTexture(GraphicsContext3D*, Platform3DObject, GC3Denum, GC3Dint, GC3Denum, GC3Denum, GC3Denum, bool, bool) override;
    NativeImagePtr nativeImageForCurrentTime() override;

    bool canLoadPoster() const override;
    void setPoster(const String&) override;

    bool supportsAcceleratedRendering() const override;
    void acceleratedRenderingStateChanged() override;

    bool shouldMaintainAspectRatio() const override;
    void setShouldMaintainAspectRatio(bool) override;

    bool hasSingleSecurityOrigin() const override;

    bool didPassCORSAccessCheck() const override;

    MediaPlayer::MovieLoadType movieLoadType() const override;

    void prepareForRendering() override;

    void setPrivateBrowsingMode(bool) override;

    bool ended() const override;

    // TextureMapperPlatformLayerProxyProvider
    RefPtr<TextureMapperPlatformLayerProxy> proxy() const override { return m_platformLayerProxy.copyRef(); }
    void swapBuffersIfNeeded() override;

private:
    MediaPlayer* m_player;
    JNI::GlobalRef<WebMediaPlayer> m_backend;
    RefPtr<TextureMapperPlatformLayerProxy> m_platformLayerProxy;
};

} // namespace WebCore

#endif // MediaPlayerPrivateAndroid_h
