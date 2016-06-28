// Copyright (C) 2016 iNuron NV
//
// This file is part of Open vStorage Open Source Edition (OSE),
// as available from
//
//      http://www.openvstorage.org and
//      http://www.openvstorage.com.
//
// This file is free software; you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License v3 (GNU AGPLv3)
// as published by the Free Software Foundation, in version 3 as it comes in
// the LICENSE.txt file of the Open vStorage OSE distribution.
// Open vStorage is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY of any kind.

#ifndef VD_DTL_SYNC_BRIDGE_H_
#define VD_DTL_SYNC_BRIDGE_H_

#include "DtlBridgeCommon.h"
#include "DtlStreamers.h"
#include "DtlProxy.h"
#include "DtlClientInterface.h"
#include "SCO.h"

#include <youtils/FileDescriptor.h>
#include <youtils/IOException.h>

namespace volumedriver
{

class DtlTest;

class DtlSyncBridge
    : public DtlClientInterface
{
    friend class DtlTest;

public:
    explicit DtlSyncBridge(const size_t max_entries);

    DtlSyncBridge(const DtlSyncBridge&) = delete;

    DtlSyncBridge&
    operator=(const DtlSyncBridge&) = delete;

    ~DtlSyncBridge() = default;

    virtual void
    initialize(DegradedFun) override;

    virtual const char*
    getName() const override;

    virtual void
    destroy(SyncFailOverToBackend) override;

    virtual bool
    addEntries(const std::vector<ClusterLocation>& locs,
               size_t num_locs,
               uint64_t start_address,
               const uint8_t* data) override;

    virtual bool
    backup() override;

    virtual void
    newCache(std::unique_ptr<DtlProxy>) override;

    virtual void
    setRequestTimeout(const boost::chrono::seconds) override;

    virtual void
    removeUpTo(const SCO& sconame) override;

    virtual uint64_t
    getSCOFromFailOver(SCO sconame,
                       SCOProcessorFun processor) override;

    virtual void
    Flush() override;

    virtual void
    Clear() override;

    virtual DtlMode
    mode() const override;

private:
    DECLARE_LOGGER("DtlSyncBridge");

    boost::mutex mutex_;
    std::unique_ptr<DtlProxy> cache_;
    DegradedFun degraded_fun_;

    void
    handleException(std::exception& e,
                    const char* where);
};

}

#endif // VD_DTL_SYNC_BRIDGE_H_

// Local Variables: **
// mode: c++ **
// End: **
