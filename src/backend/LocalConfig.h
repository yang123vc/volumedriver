// Copyright 2015 iNuron NV
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BACKEND_LOCAL_CONFIG_H_
#define BACKEND_LOCAL_CONFIG_H_

#include "BackendConfig.h"

#include <youtils/Logging.h>
#include <youtils/System.h>

namespace backend
{

class LocalConfig
    : public BackendConfig
{
    friend class boost::serialization::access;
public:
    LocalConfig(const std::string& path,
                const long int i_tv_sec = youtils::System::get_env_with_default("LOCAL_BACKEND_DELAY_SEC", 0),
                const long int i_tv_nsec = youtils::System::get_env_with_default("LOCAL_BACKEND_DELAY_NSEC", 0),
                const bool enable_partial_read = true,
                const bool sync_object_after_write = true)
        : BackendConfig(BackendType::LOCAL)
        , local_connection_path(path)
        , local_connection_tv_sec(i_tv_sec)
        , local_connection_tv_nsec(i_tv_nsec)
        , local_connection_enable_partial_read(enable_partial_read)
        , local_connection_sync_object_after_write(sync_object_after_write)
        , timespec_()
    {
        fill_timespec();
    }

    LocalConfig(const boost::property_tree::ptree& pt)
        : BackendConfig(BackendType::LOCAL)
        , local_connection_path(pt)
        , local_connection_tv_sec(pt)
        , local_connection_tv_nsec(pt)
        , local_connection_enable_partial_read(pt)
        , local_connection_sync_object_after_write(pt)
        , timespec_()
    {
        fill_timespec();
    }

    LocalConfig() = delete;

    LocalConfig(const LocalConfig&) = delete;

    LocalConfig&
    operator=(const LocalConfig&) = delete;

    virtual StrictConsistency
    strict_consistency() const override final
    {
        return StrictConsistency::T;
    }

    virtual const char*
    name() const override final
    {
        static const char* n = "LOCAL";
        return n;
    }

    virtual std::unique_ptr<BackendConfig>
    clone() const override final
    {
        std::unique_ptr<BackendConfig>
            bc(new LocalConfig(local_connection_path.value(),
                               local_connection_tv_sec.value(),
                               local_connection_tv_nsec.value(),
                               local_connection_enable_partial_read.value(),
                               local_connection_sync_object_after_write.value()));
        return bc;
    }

    virtual void
    persist_internal(boost::property_tree::ptree& pt,
                     const ReportDefault reportDefault) const override final
    {
       putBackendType(pt,
                       reportDefault);
        local_connection_path.persist(pt,
                                      reportDefault);
        local_connection_tv_sec.persist(pt,
                                        reportDefault);
        local_connection_tv_nsec.persist(pt,
                                         reportDefault);
        local_connection_enable_partial_read.persist(pt,
                                                     reportDefault);
        local_connection_sync_object_after_write.persist(pt,
                                                         reportDefault);
    }

    virtual void
    update_internal(const boost::property_tree::ptree&,
                    youtils::UpdateReport&) override final
    {}

    virtual bool
    operator==(const BackendConfig& other) const override final
    {
        return other.backend_type.value() == BackendType::LOCAL
            and static_cast<const LocalConfig&>(other).local_connection_path.value() == local_connection_path.value()
            and static_cast<const LocalConfig&>(other).local_connection_tv_nsec.value() == local_connection_tv_nsec.value()
            and static_cast<const LocalConfig&>(other).local_connection_tv_sec.value() == local_connection_tv_sec.value()
            and static_cast<const LocalConfig&>(other).local_connection_sync_object_after_write.value() == local_connection_sync_object_after_write.value();
    }

    DECLARE_PARAMETER(local_connection_path);
    DECLARE_PARAMETER(local_connection_tv_sec);
    DECLARE_PARAMETER(local_connection_tv_nsec);
    DECLARE_PARAMETER(local_connection_enable_partial_read);
    DECLARE_PARAMETER(local_connection_sync_object_after_write);

    const struct timespec timespec_;

    static const std::string backend_type;

    virtual bool
    checkConfig_internal(const boost::property_tree::ptree&,
                         youtils::ConfigurationReport&) const override final
    {
        return true;
    }

private:
    void
    fill_timespec()
    {
        const_cast<timespec&>(timespec_).tv_sec = local_connection_tv_sec.value();
        const_cast<timespec&>(timespec_).tv_nsec = local_connection_tv_nsec.value();
    }
};

}

#endif // !BACKEND_LOCAL_CONFIG_H_
