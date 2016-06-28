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

#ifndef DTL_PROXY_H_
#define DTL_PROXY_H_

#include "DtlStreamers.h"
#include "SCOProcessorInterface.h"
#include "SCO.h"

#include "distributed-transaction-log/fungilib/Socket.h"
#include "distributed-transaction-log/fungilib/IOBaseStream.h"
#include "distributed-transaction-log/fungilib/Buffer.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_array.hpp>

namespace volumedriver
{

class DtlConfig;
class Volume;

class DtlProxy
{
public:
    DtlProxy(const DtlConfig&,
                       const Namespace&,
                       const LBASize,
                       const ClusterMultiplier,
                       const boost::chrono::seconds);

    DtlProxy(const DtlProxy&) = delete;

    DtlProxy&
    operator=(const DtlProxy&) = delete;

    ~DtlProxy();

    void
    addEntries(std::vector<DtlEntry> entries);

    // returns the SCO size - 0 indicates a problem.
    // Z42: throw instead!
    uint64_t
    getSCOFromFailOver(SCO a,
                       SCOProcessorFun processor);

    void
    getSCORange(SCO& oldest,
                SCO& youngest);

    void
    clear();

    void
    flush();

    void
    removeUpTo(const SCO sconame) throw ();

    void
    setRequestTimeout(const boost::chrono::seconds);

    void
    getEntries(SCOProcessorFun processor);

    void
    delete_failover_dir()
    {
        LOG_INFO("Setting delete_failover_dir_ to true");
        delete_failover_dir_ = true;
    }

    LBASize
    lba_size() const
    {
        return lba_size_;
    }

    ClusterMultiplier
    cluster_multiplier() const
    {
        return cluster_mult_;
    }

private:
    DECLARE_LOGGER("DtlProxy");

    void
    register_();

    void
    unregister_();

    void
    checkStreamOK(const std::string& ex);

    uint64_t
    getObject_(SCOProcessorFun,
               bool cork_per_cluster);

    std::unique_ptr<fungi::Socket> socket_;
    fungi::IOBaseStream stream_;
    const Namespace ns_;
    const LBASize lba_size_;
    const ClusterMultiplier cluster_mult_;
    bool delete_failover_dir_;
};

}

#endif // DTL_PROXY_H_

// Local Variables: **
// mode: c++ **
// End: **