﻿/*
 * Copyright (c) 2016-present The ZLMediaKit project authors. All Rights Reserved.
 *
 * This file is part of ZLMediaKit(https://github.com/ZLMediaKit/ZLMediaKit).
 *
 * Use of this source code is governed by MIT-like license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#ifndef SRC_DEVICE_PLAYERPROXY_H_
#define SRC_DEVICE_PLAYERPROXY_H_

#include "Common/MultiMediaSourceMuxer.h"
#include "Player/MediaPlayer.h"
#include "Util/TimeTicker.h"
#include <memory>

namespace mediakit {

struct StreamInfo
{
    TrackType codec_type;
    std::string codec_name;
    int bitrate;
    int audio_sample_rate;
    int audio_sample_bit;
    int audio_channel;
    int video_width;
    int video_height;
    float video_fps;

    StreamInfo()
    {
        codec_type = TrackInvalid;
        codec_name = "none";
        bitrate = -1;
        audio_sample_rate = -1;
        audio_channel = -1;
        audio_sample_bit = -1;
        video_height = -1;
        video_width = -1;
        video_fps = -1.0;
    }
};

struct TranslationInfo
{
    std::vector<StreamInfo> stream_info;
    int byte_speed;
    uint64_t start_time_stamp;

    TranslationInfo()
    {
        byte_speed = -1;
        start_time_stamp = 0;
    }
};

class PlayerProxy
    : public MediaPlayer
    , public MediaSourceEvent
    , public std::enable_shared_from_this<PlayerProxy> {
public:
    using Ptr = std::shared_ptr<PlayerProxy>;

    // 如果retry_count<0,则一直重试播放；否则重试retry_count次数  [AUTO-TRANSLATED:5582d53c]
    // If retry_count < 0, then retry playing indefinitely; otherwise, retry retry_count times
    // 默认一直重试  [AUTO-TRANSLATED:779d3c46]
    // Default to retrying indefinitely
    PlayerProxy(const MediaTuple &tuple, const ProtocolOption &option, int retry_count = -1,
        const toolkit::EventPoller::Ptr &poller = nullptr, 
        int reconnect_delay_min = 2, int reconnect_delay_max = 60, int reconnect_delay_step = 3);

    ~PlayerProxy() override;

    /**
     * 设置play结果回调，只触发一次；在play执行之前有效
     * @param cb 回调对象
     * Set a callback for the play result, triggered only once; effective before play execution
     * @param cb Callback object
     
     * [AUTO-TRANSLATED:f34625f6]
     */
    void setPlayCallbackOnce(std::function<void(const toolkit::SockException &ex)> cb);

    /**
     * 设置主动关闭回调
     * @param cb 回调对象
     * Set a callback for active closure
     * @param cb Callback object
     
     * [AUTO-TRANSLATED:83b7700a]
     */
    void setOnClose(std::function<void(const toolkit::SockException &ex)> cb);

    /**
    * Set a callback for failed server connection
    * @param cb 回调对象
     * Set a callback for failed server connection
     * @param cb Callback object
     
     * [AUTO-TRANSLATED:e7f5e7cc]
    */
    void setOnDisconnect(std::function<void()> cb);

    /**
    * Set a callback for a successful connection to the server
    * @param cb 回调对象
     * Set a callback for a successful connection to the server
     * @param cb Callback object
     
     * [AUTO-TRANSLATED:b88e0d4c]
    */
    void setOnConnect(std::function<void(const TranslationInfo&)> cb);

    /**
     * 开始拉流播放
     * @param strUrl
     * Start streaming playback
     * @param strUrl
     
     * [AUTO-TRANSLATED:a2f0e859]
     */
    void play(const std::string &strUrl) override;

    /**
     * 获取观看总人数
     * Get the total number of viewers
     
     * [AUTO-TRANSLATED:6c1b8bf1]
     */
    int totalReaderCount();

    int getStatus();
    uint64_t getLiveSecs();
    uint64_t getRePullCount();

    // Using this only makes sense after a successful connection to the server
    TranslationInfo getTranslationInfo();

    const std::string& getUrl() const { return _pull_url; }
    const MediaTuple& getMediaTuple() const { return _tuple; }
    const ProtocolOption& getOption() const { return _option; }

private:
    // MediaSourceEvent override
    bool close(MediaSource &sender) override;
    int totalReaderCount(MediaSource &sender) override;
    MediaOriginType getOriginType(MediaSource &sender) const override;
    std::string getOriginUrl(MediaSource &sender) const override;
    std::shared_ptr<toolkit::SockInfo> getOriginSock(MediaSource &sender) const override;
    float getLossRate(MediaSource &sender, TrackType type) override;

    void rePlay(const std::string &strUrl, int iFailedCnt);
    void onPlaySuccess();
    void setDirectProxy();
    void setTranslationInfo();

private:
    int _retry_count;
    int _reconnect_delay_min;
    int _reconnect_delay_max;
    int _reconnect_delay_step;
    MediaTuple _tuple;
    ProtocolOption _option;
    std::string _pull_url;
    toolkit::Timer::Ptr _timer;
    std::function<void()> _on_disconnect;
    std::function<void(const TranslationInfo &info)> _on_connect;
    std::function<void(const toolkit::SockException &ex)> _on_close;
    std::function<void(const toolkit::SockException &ex)> _on_play;
    TranslationInfo _transtalion_info;
    MultiMediaSourceMuxer::Ptr _muxer;

    toolkit::Ticker _live_ticker;
    // 0 表示正常 1 表示正在尝试拉流  [AUTO-TRANSLATED:2080bedf]
    // 0 indicates normal, 1 indicates attempting to stream
    std::atomic<int> _live_status;
    std::atomic<uint64_t> _live_secs;

    std::atomic<uint64_t> _repull_count;
};

} /* namespace mediakit */
#endif /* SRC_DEVICE_PLAYERPROXY_H_ */
